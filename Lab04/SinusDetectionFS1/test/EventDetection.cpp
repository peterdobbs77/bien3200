/* FS1EventDetection.cpp : Defines the entry point for the console application.
		Peter Dobbs
		BIEN 3200 - Section 401
		18 October 2016
		Lab 4

		Program Description:
			This program implements an event detection algorithm
			that uses the first and second derivatives of a signal
			to determine the location of a threshold crossing in
			order to find the event of a QRS complex.
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
#include "mex.h"

int numPoints;

float getEcgMax(float *data) {
	float max = 0;
	for (int i = 0; i < numPoints; ++i) { //find maxECG from input data
		if (data[i] > max) max = data[i];
	}
	return max;
}

int main(void) {
	float time, amplitude;
	char in_file[100];
	FILE *inputFile;

	printf("Enter input file name\n");	// open file to be read
	scanf("%s", in_file);
	inputFile = fopen(in_file, "r");

	if (!(inputFile)) {
		printf("file does not exist.\n");
		exit(EXIT_FAILURE);
	}

	// determine the number of points in the opened file
	for (numPoints = 0; fscanf(inputFile, "%f %f", &time, &amplitude) != EOF; ++numPoints);
	printf("number of data points in file = %i\n", numPoints);
	float samplingFrequency = numPoints / time;

	rewind(inputFile);

	float *tData = (float *)calloc(sizeof(float), numPoints); //allocate memory
	float *aData = (float *)calloc(sizeof(float), numPoints);

	for (int i = 0; i < numPoints; ++i) {
		fscanf(inputFile, "%f\t%f", &tData[i], &aData[i]);
		aData[i] = aData[i] / samplingFrequency; // modification
	}
	fclose(inputFile);

	float *filteredSignal = (float *)calloc(sizeof(float), numPoints);
	for (int i = 2; i < numPoints; ++i) {//3-point moving average filter (Hanning filter)
		filteredSignal[i - 2] = (aData[i] + 2 * aData[i - 1] + aData[i - 2]) / 4;
	}

	float *y0 = (float *)calloc(sizeof(float), numPoints);
	float *y1 = (float *)calloc(sizeof(float), numPoints);
	float *y2 = (float *)calloc(sizeof(float), numPoints);
	for (int i = 1; i < numPoints - 3; ++i)
		y0[i] = fabsf(filteredSignal[i + 1] - filteredSignal[i - 1]);
	for (int i = 2; i < numPoints - 4; ++i)
		y1[i] = fabsf(filteredSignal[i + 2] - 2 * filteredSignal[i] - filteredSignal[i - 2]);
	for (int i = 0; i < numPoints - 2; ++i)
		y2[i] = 1.3*y0[i] + 1.1*y1[i];

	float threshold = 0.6 * getEcgMax(filteredSignal);
	float blank = (0.2) * (samplingFrequency); //blanking period for the dataset
	int numCrossings = 0;
	for (int i = 0; i < numPoints; ++i) {
		if (filteredSignal[i] > threshold) {
			++numCrossings;
			i += blank;
		}
	}

	system("PAUSE");
	//////////////====MATLAB====//////////////////
	Engine *ep;
	ep = engOpen(NULL);

	mxArray* matX1 = mxCreateNumericMatrix(numPoints, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty1 = mxCreateNumericMatrix(numPoints, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty2 = mxCreateNumericMatrix(numPoints, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* thres = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);

	memcpy((void*)mxGetPr(matX1), (void*)tData, sizeof(float) * numPoints);
	memcpy((void*)mxGetPr(maty1), (void*)filteredSignal, sizeof(float) * numPoints);
	memcpy((void*)mxGetPr(maty2), (void*)y0, sizeof(float) * numPoints);
	memcpy((void*)mxGetPr(thres), (void*)&threshold, sizeof(float) * 1);

	engPutVariable(ep, "t", matX1);
	engPutVariable(ep, "filt", maty1);
	engPutVariable(ep, "d", maty2);
	engPutVariable(ep, "T", thres);

	engEvalString(ep, "figure; plot(t,filt,t,d);"
		"title ('Sinus Rhythm');"
		"xlabel('Time (sec)');"
		"ylabel('ECG (mV)');"
		"legend('Signal','First Derivative');"
		"hline = refline(0,T); hline.Color = 'k';");

	system("pause"); //Pauses figure
	engClose(ep);
	//////////////======C======///////////////////

	float *locations = (float *)calloc(sizeof(float), numPoints); //allocate memory
	float *amplitudes = (float *)calloc(sizeof(float), numPoints);
	int atLeastSix;
	for (int i = 2, j = 0; i < numPoints - 2; ++i) {
		if (y2[i] >= 1.0) {
			atLeastSix = 0;
			for (int n = i + 1; n <= 9 + i; ++n)
				if (y2[n] >= 1.0)++atLeastSix;
			if (atLeastSix >= 6) {
				locations[j] = tData[i + 2];
				amplitudes[j] = aData[i + 2];
				++j;
				i += blank;
			}
		}
	}

	//Calculate average heart rate
	float avgHR = numCrossings / (time / 60);
	printf("Average Heart Rate (beats/minute) = %f\n", avgHR);

	// output results to file
	char out_file[100];
	FILE *outFile;
	printf("Enter ouput file name\n");
	scanf("%s", out_file);
	outFile = fopen(out_file, "w");
	for (int i = 0; i < numCrossings; ++i) {
		fprintf(outFile, "%f\t%f\n", locations[i], amplitudes[i]);
	}
	fprintf(outFile, "Heart Rate: %f", avgHR);
	fclose(outFile);

	free(aData);
	free(filteredSignal);
	free(y0);
	free(y1);
	free(locations);
	free(amplitudes);
	free(tData);
	free(y2);

	system("PAUSE");
	return 0; //end program
}

