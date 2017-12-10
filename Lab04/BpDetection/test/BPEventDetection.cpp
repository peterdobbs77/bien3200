/* BPEventDetection.cpp : Defines the entry point for the console application.
		Peter Dobbs
		BIEN 3200 - Section 401
		18 October 2016
		Lab 4

		Program Description:
			This program implements a basic event detection algorithm to determine
			the event of systolic pressure and diastolic pressure
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
#include "mex.h"

int main(void) {
	printf("Blood Pressure Event Detection\n");
	int n = 0;
	float *tData, *aData, t, y;
	FILE *inputFile = fopen("BPirreg.txt", "r");

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

	float *d1 = (float *)calloc(sizeof(float), n);
	for (int i = 1; i < n - 1; ++i) // first derivative
		d1[i] = aData[i + 1] - aData[i - 1];

	//Event Detection Algorithm
	float *locationSys, *locationDia, *systolic, *diastolic;

	locationSys = (float *)calloc(sizeof(float), n);
	locationDia = (float *)calloc(sizeof(float), n);
	systolic = (float *)calloc(sizeof(float), n);
	diastolic = (float *)calloc(sizeof(float), n);
	int countSys = 0, countDia = 0;

	for (int i = 1; i < n - 2; ++i) {
		if (d1[i - 1] > 0 && d1[i + 1] < 0) { // the point is a local maximum
			if (aData[i - 1] >= 75) { // it is the systolic pressure
				systolic[countSys] = aData[i - 1];
				locationSys[countSys] = tData[i - 1];
				++countSys;
			} else if (aData[i - 1] >= 60) { // it is the diastolic pressure
				diastolic[countDia] = aData[i - 1];
				locationDia[countDia] = tData[i - 1];
				++countDia;
			}
			i += ((n / t)*0.01); // blanking period
		}
	}

	// ouput algorithm results to file
	FILE *outFile;
	outFile = fopen("out.txt", "w");
	for (int i = 0; i < countSys; ++i) {
		fprintf(outFile, "%f\t%f\n", locationSys[i], systolic[i]);
		fprintf(outFile, "%f\t%f\n", locationDia[i], diastolic[i]);
	}
	fclose(outFile);
	printf("Data written to file: 'out.txt'\n");


	system("PAUSE");
	printf("Plotting In MatLab:\n");
	////////====MATLAB====//////////////////
	Engine *ep;
	ep = engOpen(NULL);

	mxArray* matx1 = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty1 = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* matD1 = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);

	memcpy((void*)mxGetPr(matx1), (void*)tData, sizeof(float) * n);
	memcpy((void*)mxGetPr(maty1), (void*)aData, sizeof(float) * n);
	memcpy((void*)mxGetPr(matD1), (void*)d1, sizeof(float) * n);

	engPutVariable(ep, "time", matx1);
	engPutVariable(ep, "ampl", maty1);
	engPutVariable(ep, "d1", matD1);

	engEvalString(ep, "figure; plot(time,ampl);"
		"title ('Arterial Pressure (irregular)');"
		"xlabel('Time (sec)');"
		"ylabel('Blood Pressure (mm Hg)');"
		);

	system("pause"); // Pauses figure
	engClose(ep); // close the engine
	////////////======C======//////////////////


	free(locationSys);
	free(locationDia);
	free(systolic);
	free(diastolic);
	free(tData);
	free(aData);
	free(d1);

	system("PAUSE");
	return 0; //end program
}

