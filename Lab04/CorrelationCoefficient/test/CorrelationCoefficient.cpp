/* CorrelationCoefficient.cpp : Defines the entry point for the console application.
		Peter Dobbs
		BIEN 3200 - Section 401
		18 October 2016
		Lab 4

		Program Description:
			This program determines the correlation between two input datasets
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
#include "mex.h"

float getArithMean(float *data, int length) {
	float sum = 0;
	for (int i = 0; i < length; ++i)
		sum += data[i];
	return (sum / length);
}

float getVariance(float *data, int length) {
	float mean = getArithMean(data, length);
	float var = 0;
	for (int i = 0; i < length; ++i) {
		var += pow(data[i] - mean, 2);
	}
	return var;
}

float getCorrCoeff(float *X, float *Y, int n) {
	float varX = getVariance(X, n);
	float varY = getVariance(Y, n);
	float meanX = getArithMean(X, n);
	float meanY = getArithMean(Y, n);
	float numer = 0;
	for (int i = 0; i < n; ++i) {
		numer += ((X[i] - meanX)*(Y[i] - meanY));
	}
	return (numer / sqrt(varX*varY));
}

int main(void) {
	int numPoints1 = 0, numPoints2 = 0;
	float *t1, *t2, *X, *Y, t, a;
	char in_file[100];
	FILE *inputFile1, *inputFile2;

	printf("Enter 1st input file name\n");	// open first file to be read
	scanf("%s", in_file);
	inputFile1 = fopen(in_file, "r");
	printf("Enter 2nd input file name\n");	// open 2nd file to be read
	scanf("%s", in_file);
	inputFile2 = fopen(in_file, "r");

	if (!(inputFile1) || !(inputFile2)) {
		printf("One of the files does not exist.\n");
		exit(EXIT_FAILURE);
	}

	// determine the number of points in the opened file
	for (numPoints1 = 0; fscanf(inputFile1, "%f %f", &t, &a) != EOF; ++numPoints1);
	printf("number of data points in file = %i\n", numPoints1);
	int samplingFrequency1 = numPoints1 / t;
	for (numPoints2 = 0; fscanf(inputFile2, "%f %f", &t, &a) != EOF; ++numPoints2);
	printf("number of data points in file = %i\n", numPoints2);
	int samplingFrequency2 = numPoints2 / t;

	rewind(inputFile1); // move cursor to beginning of indicated file
	rewind(inputFile2);

	t1 = (float *)calloc(sizeof(float), numPoints1); //allocate memory
	X = (float *)calloc(sizeof(float), numPoints1);
	t2 = (float *)calloc(sizeof(float), numPoints2);
	Y = (float *)calloc(sizeof(float), numPoints2);

	for (int i = 0; i < numPoints1; ++i)
		fscanf(inputFile1, "%f\t%f", &t1[i], &X[i]);
	fclose(inputFile1);
	for (int i = 0; i < numPoints2; ++i)
		fscanf(inputFile2, "%f\t%f", &t2[i], &Y[i]);
	fclose(inputFile2);

	// Correlation Coefficient
	float r_xy = getCorrCoeff(X, Y, numPoints1);
	printf("Correlation Coefficient: %f\n", r_xy);

	//char out_file[100];
	//FILE *outFile;

	//printf("Enter ouput file name\n"); //write to file
	//scanf("%s", out_file);
	//outFile = fopen(out_file, "w");

	// Correlation Traces
	float *trace = (float *)calloc(sizeof(float), numPoints1);
	float varX = getVariance(X, numPoints1);
	float varY = getVariance(Y, numPoints1);
	float meanX = getArithMean(X, numPoints1);
	float meanY = getArithMean(Y, numPoints1);
	/*for (int i = 0; i < numPoints1; ++i) {
		trace[i] = ((X[i] - meanX)*(Y[i] - meanY)) / sqrt(varX*varY);
	}*/
	for (int i = 0; i < numPoints1 - 1; ++i) {
		trace[i] = ((X[i] - meanX)*(X[i + 1] - meanX)) / varX;
	}

	system("PAUSE");
	////////====MATLAB====//////////////////
	//Open MATLAB engine
	Engine *ep;
	ep = engOpen(NULL);

	mxArray* matT1 = mxCreateNumericMatrix(numPoints1, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* matx1 = mxCreateNumericMatrix(numPoints1, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty1 = mxCreateNumericMatrix(numPoints2, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* matTrace = mxCreateNumericMatrix(numPoints1, 1, mxSINGLE_CLASS, mxREAL);

	memcpy((void*)mxGetPr(matT1), (void*)t1, sizeof(float) * numPoints1);
	memcpy((void*)mxGetPr(matx1), (void*)X, sizeof(float) * numPoints1);
	memcpy((void*)mxGetPr(maty1), (void*)Y, sizeof(float) * numPoints2);
	memcpy((void*)mxGetPr(matTrace), (void*)trace, sizeof(float) * numPoints1);

	engPutVariable(ep, "t", matT1);
	engPutVariable(ep, "X", matx1);
	engPutVariable(ep, "Y", maty1);
	engPutVariable(ep, "trace", matTrace);

	engEvalString(ep, "figure; scatter(X,Y);"
		"title('Correlation of Two Signals');"
		"xlabel('Amplitude of Signal X');"
		"ylabel('Amplitude of Signal Y');"
		"figure; plot(t,trace);"
		"title('Correlation Trace of X');"
		"xlabel('Time (sec)');"
		"ylabel('Correlation of X[i] and X[i+1]');");

	system("pause");

	engClose(ep); // close the engine
	////////////======C======//////////////////

	free(t1);
	free(X);
	free(t2);
	free(Y);
	free(trace);

	system("PAUSE");
	//end program
	return 0;
}

