/* EventDetection.cpp : Defines the entry point for the console application.
		Peter Dobbs
		BIEN 3200 - Section 401
		4 October 2016
		Lab 3

		Program Description:
			This program implements a basic event detection algorithm to determine
			the event of a QRS complex. Based on the number of events in a given
			span of time for the data set, the average heart rate is calculated.
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
#include "mex.h"

float threshold, maxECG, blank;
float numCrossings = 0;

void setEcgMax(float *data, int n) {
	maxECG = 0;
	for (int i = 0; i < n; ++i) { //find maxECG from input data
		if (data[i] > maxECG) maxECG = data[i];
	}
}

int main(void) {
	int n = 0;
	float *tData, *aData, t, y;
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
	for (n = 0; fscanf(inputFile, "%f %f", &t, &y) != EOF; ++n);
	printf("number of data points in file = %i\n", n);

	// move cursor to beginning of indicated file
	rewind(inputFile);

	tData = (float *)calloc(sizeof(float), n); //allocate memory
	aData = (float *)calloc(sizeof(float), n);

	for (int i = 0; i < n; ++i) {
		fscanf(inputFile, "%f\t%f", &tData[i], &aData[i]);
	}
	fclose(inputFile);

	//Event Detection Algorithm
	char out_file[100];
	FILE *outFile;
	float percentage, *locations, *amplitudes;
	int delT = tData[n - 1] - tData[0]; // (delT :: delta T) variable for determining sample time

	setEcgMax(aData, n);
	printf("ECGmax = %f\n", maxECG);

	printf("Enter a threshold percentage of ECGmax\n");
	scanf("%f", &percentage);
	threshold = maxECG * percentage; //amplitude threshold for detecting QRS complex

	printf("Enter a blanking period between 0.1 and 0.3 seconds\n");
	scanf("%f", &blank);
	blank = blank * (n / delT); //specifying the blanking period for the input dataset

	for (int i = 0; i < n; ++i) {
		if (aData[i] > threshold) {
			++numCrossings;
			i = i + blank;
		}
	}
	locations = (float *)calloc(sizeof(float), numCrossings); //allocate memory
	amplitudes = (float *)calloc(sizeof(float), numCrossings);

	for (int i = 0, j = 0; i < n; ++i) { //detect QRS complexes
		if (aData[i] > threshold) {
			locations[j] = tData[i];
			amplitudes[j] = aData[i];
			++j;
			i = i + blank;
		}
	}

	printf("Enter ouput file name\n"); //write to file
	scanf("%s", out_file);
	outFile = fopen(out_file, "w");
	for (int i = 0; i < numCrossings; ++i) {
		fprintf(outFile, "%f\t%f\n", locations[i], amplitudes[i]);
	}
	fclose(outFile);
	free(locations);
	free(amplitudes);

	float avgHR = numCrossings / (tData[n - 1] / 60);
	printf("Average Heart Rate (beats/minute) = %f\n", avgHR);

	//system("PAUSE");
	//////////====MATLAB====//////////////////
	////Open MATLAB engine
	//Engine *ep;
	//ep = engOpen(NULL);

	////create pointer of type mxAray (MATLAB array) with the right dimension and type, allocate space
	//mxArray* matx1 = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);
	//mxArray* maty1 = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);
	//mxArray* thresh = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);

	///*copies values of x1 and y1 into the allocated space:
	//	copies 10 *(#of bytes for a float) bytes from x1 to matx1 or from y1 to maty1*/
	//memcpy((void*)mxGetPr(matx1), (void*)tData, sizeof(float) * n);
	//memcpy((void*)mxGetPr(maty1), (void*)aData, sizeof(float) * n);
	//memcpy((void*)mxGetPr(thresh), (void*)&threshold, sizeof(float) * 1);

	///* Send variable to MatLabthrough A and F:
	//	Write mxArraymatx1 or maty1 to the egnine"ep"and
	//	gives it the variable name A or B */
	//engPutVariable(ep, "A", matx1);
	//engPutVariable(ep, "F", maty1);
	//engPutVariable(ep, "T", thresh);

	////use Matlabcommands
	//engEvalString(ep, "figure; plot(A,F);"
	//	"title ('Sinus Rhythm');"
	//	"xlabel('Time (sec)');"
	//	"ylabel('ECG (mV)');"
	//	"refline(0,T);");

	////Pauses figure
	//system("pause");

	////close the engine 
	//engClose(ep);
	////////////======C======//////////////////

	free(tData);
	free(aData);

	system("PAUSE");
	//end program
	return 0;
}

