/* Plotting.cpp : Defines the entry point for the console application.
		Peter Dobbs
		BIEN 3200 - Section 401
		8 November 2016
		Lab 5

		Program Description:
			This program reads in data file and plots the data in MatLab
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
#include "mex.h"

int main(void) {
	int numPoints;
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
	}
	fclose(inputFile);

	system("PAUSE");
	//////////////====MATLAB====//////////////////
	Engine *ep;
	ep = engOpen(NULL);

	mxArray* matX1 = mxCreateNumericMatrix(numPoints, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty1 = mxCreateNumericMatrix(numPoints, 1, mxSINGLE_CLASS, mxREAL);

	memcpy((void*)mxGetPr(matX1), (void*)tData, sizeof(float) * numPoints);
	memcpy((void*)mxGetPr(maty1), (void*)aData, sizeof(float) * numPoints);

	engPutVariable(ep, "t", matX1);
	engPutVariable(ep, "a", maty1);

	engEvalString(ep, "figure; plot(t,a);"
		"title ('Plot of EEG_05');"
		"xlabel('Time (sec)');"
		"ylabel('Signal Magnitude');");

	system("pause"); //Pauses figure
	engClose(ep);
	//////////////======C======///////////////////

	free(aData);
	free(tData);

	system("PAUSE");
	return 0; //end program
}

