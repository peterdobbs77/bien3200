/* DiscreteFourier.cpp : Defines the entry point for the console application.

Peter Dobbs
BIEN 3200
Lab 5
8 November 2016

Program Description:
	Determines the discrete fourier transform (dft) of an input signal
*/

#include "stdafx.h"
#include <math.h>
#include "fftfilter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void fft(float *data, int n, int invrs); // fast fourier transform function

int main() {
	printf("DiscreteFourier Program\n");

	// DECLARE VARIABLES
	int fs; // sampling frequency
	int N; // number of points in file
	float t, ampl; // scrap variables for determining N
	float *time, *data; // float pointers for holding data from file
	FILE *fpIn, *fpOut; // file pointers for input and output files
	char inputFile[100], outputFile[100]; // stores input/output file names

	// READ INPUT DATA
	printf("Enter input file name (.txt)\n");
	scanf("%s", &inputFile);
	fpIn = fopen(inputFile, "r");

	if (!fpIn) {
		printf("file does not exist in expected location\n");
		return 1;
	}

	for (N = 0; (fscanf(fpIn, "%f %f", &t, &ampl)) != EOF; ++N);
	fs = N / t;
	printf("Sampling Frequency: %f\n", fs);
	rewind(fpIn);

	int exp = 1;
	while (pow(2, exp) < N) {
		exp++;
	}
	int N2 = pow(2, exp);
	printf("Updated Number of Points: %i \n", N2);

	time = (float *)calloc(sizeof(float), N2);// allocate memory
	data = (float *)calloc(sizeof(float), N2);

	for (int i = 0; i < N; ++i) {
		fscanf(fpIn, "%f %f", &time[i], &data[i]);
	}
	fclose(fpIn);
	free(time);
	free(data);

	// CALCULATE DISCRETE FOURIER TRANSFORM (DFT)
	fft(data - 1, N2, 1); //returns first n/2 coefficients

	float *amplSpec; // allocate memory
	amplSpec = (float *)calloc(sizeof(float), (N2 / 2));
	float *kValues;
	kValues = (float *)calloc(sizeof(float), (N2 / 2));

	// DETERMINE THE FREQUENCY AMPLITUDE SPECTRUM
	for (int k = 0, i = 2; k < (N2 / 2); k++) {
		amplSpec[k] = sqrt(pow(data[i], 2) + pow(data[i + 1], 2)) * 2.0 / (float)N;
		kValues[k] = ((float)k*fs) / (float)N2;
		i += 2;// i is regular index skip
	}

	// OUTPUT DATA TO FILE
	printf("Enter output filename (.txt)\n");
	scanf("%s", &outputFile);
	fpOut = fopen(outputFile, "w");
	for (int k = 0; k < (N2 / 2); k++) {
		fprintf(fpOut, "%f %f\n", kValues[k], amplSpec[k]);
	}

	free(amplSpec);
	free(kValues);
	fclose(fpOut);

	printf("\nDONE\n");
	system("PAUSE");
	return 0;
}

