/* 1DDigitalFiltering.cpp : Defines the entry point for the console application.
Peter Dobbs
BIEN 3200
Lab #6
22 November 2016
*/

#include "stdafx.h"
#include "fftfilter.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void fft(float *data, int n, int invrs); // fast fourier transform function
int highpass(float *xdata, int n); // high pass filter function
int lowpass(float *xdata, int n); // low pass filter function

int getImpulseResponse() {
	printf("\nStart of: getImpulseResponse()\n");
	int N = 512;
	static float impulse[512];
	impulse[0] = 1.0;

	if (lowpass(impulse, N) != 0) {
		return 1;
	} // impulse[] contains lowpass filter's impulse response function
	//OR	(must comment out one or the other)
	/*if (highpass(impulse, N) != 0) {
		return 1;
	}*/ // impulse[] contains highpass filter's impulse response function

	// output the impulse response function to a data file
	char fileout[200];
	FILE *output;
	printf("Enter output file name:\n");
	scanf("%s", &fileout);
	output = fopen(fileout, "w");
	for (int i = 0; i < N; ++i) {
		fprintf(output, "%f\n", impulse[i]);
	}
	fclose(output);

	system("PAUSE");
	return 0;
}

int getFrequencyAmplitudeSpectrum() {
	printf("\nStart of: getFrequencyAmplitudeSpectrum()\n");
	// DECLARE VARIABLES
	int fs; // sampling frequency
	int N; // number of points in file
	float t, ampl; // scrap variables for determining N
	float *time, *data; // float pointers for holding data from file
	FILE *fpIn, *fpOut; // file pointers for input and output files
	char inputFile[100], outputFile[100]; // stores input/output file names

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

	// calculate discrete fourier transform (dft)
	fft(data - 1, N2, 1); //returns first n/2 coefficients

	float *amplSpec; // allocate memory
	amplSpec = (float *)calloc(sizeof(float), (N2 / 2));
	float *kValues;
	kValues = (float *)calloc(sizeof(float), (N2 / 2));

	for (int k = 0, i = 2; k < (N2 / 2); k++) {
		// i is regular index skip
		amplSpec[k] = sqrt(pow(data[i], 2) + pow(data[i + 1], 2)) * 2.0 / (float)N;
		kValues[k] = ((float)k*fs) / (float)N2;
		i += 2;
	}

	// output data
	printf("Enter output filename (.txt)\n");
	scanf("%s", &outputFile);
	fpOut = fopen(outputFile, "w");
	for (int k = 0; k < (N2 / 2); k++) {
		fprintf(fpOut, "%f %f\n", kValues[k], amplSpec[k]);
	}

	free(amplSpec);
	free(kValues);
	fclose(fpOut);

	system("PAUSE");
	return 0;
}

int main() {
	printf("Start of: main - 1DDigitalFiltering.cpp\n");

	/*Determine the Impulse Response of the Filter*/
	//getImpulseResponse();

	/*Determine the Filter's Frequency Amplitude Spectrum*/
	//getFrequencyAmplitudeSpectrum();

	/*Filter a Given Signal*/
	printf("\nStart of: filtering input signal\n");
	// declare variables
	char unfilterdata[200], filterdata[200]; // store names of files with unfiltered and filtered data
	FILE *datap, *dataf; // file pointers for unfiltered and filtered data
	float *time, *data; // float pointers for DMA to store the values of the unfiltered signal
	float t, d; // scrap variables used in determining number of points in the file
	int N; // number of points in the file
	int fs; // sampling frequency

	// input data to filter
	printf("Enter input file name (.txt)\n");
	scanf("%s", &unfilterdata);
	datap = fopen(unfilterdata, "r");
	for (N = 0; fscanf(datap, "%f %f", &t, &d) != EOF; ++N);
	fs = N / t;
	printf("Sampling Frequency: %f\n", fs);
	rewind(datap);

	// check that signal length is power of 2
	int exp = 1;
	while (pow(2, exp) < N) {
		exp++;
	}
	int N2 = pow(2, exp);

	time = (float *)calloc(sizeof(float), N2); // allocate memory
	data = (float *)calloc(sizeof(float), N2);

	for (int i = 0; i < N; ++i) {
		fscanf(datap, "%f %f", &time[i], &data[i]);
	}
	fclose(datap);

	// Filter the Input Signal
	/*Note: Before call, 'data' stores unfiltered signal.
			After call, 'data' stores the filtered signal.*/
	if (lowpass(data, N) != 0) {
		return 1;
	} // impulse[] contains lowpass filter's impulse response function

	//OR	(must comment out one or the other)

	/*if (highpass(data, N) != 0) {
		return 1;
	}*/ // impulse[] contains highpass filter's impulse response function

	// Write the Resulting Input Signal to an Output File
	printf("Enter filtered data output file name (.txt)\n");
	scanf("%s", &filterdata);
	dataf = fopen(filterdata, "w");
	for (int i = 0; i < N; ++i) {
		fprintf(dataf, "%f\t%f\n", time[i], data[i]);
	}
	fclose(dataf);
	free(time);
	free(data);

	// FIND THE AMPLITUDE FREQUENCY SPECTRUM FOR THE FILTERED DATA
	// calculate discrete fourier transform (dft)
	fft(data - 1, N2, 1); //returns first n/2 coefficients

	float *amplSpec; // allocate memory
	amplSpec = (float *)calloc(sizeof(float), (N2 / 2));
	float *kValues;
	kValues = (float *)calloc(sizeof(float), (N2 / 2));

	for (int k = 0, i = 2; k < (N2 / 2); k++) {
		amplSpec[k] = sqrt(pow(data[i], 2) + pow(data[i + 1], 2)) * 2.0 / (float)N;
		kValues[k] = ((float)k*fs) / (float)N2;
		i += 2; // i is index skip
	}

	// output data
	FILE *fpOut; // file pointer for output data set
	char outputFile[200]; // stores output filename
	printf("Enter DFT of filtered data output filename (.txt)\n");
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