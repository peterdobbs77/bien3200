/*
Peter Dobbs
BIEN 3200
8 November 2016
Lab 5: Fourier Analysis, Data Aquisition, A/D conversion

Program Description:
	Generates a data set containing the sum of five sine waves with varying
	frequencies and amplitudes
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*
Function to generate sine wave with given
length (seconds), fundamental frequency (Hz),
amplitude (mV), and sampling frequency (Hz)
*/
void sine(float *wave, float length, float funFrequency, float amplitude, float samplingFrequency) {
	for (int i = 0; i < length; ++i) {
		wave[i] = amplitude * sin(i * funFrequency * (2 * 3.14159) / samplingFrequency);
	}
}

void main() {
	printf("SineGeneration\n");
	// declare/initialize variables
	float samplingFrequency = 800;
	float length = samplingFrequency * 2;

	// generate sine waves
	float *sin_5_1 = (float*)calloc(sizeof(float), length);
	sine(sin_5_1, length, 5, 1, samplingFrequency);
	float *sin_50_2 = (float*)calloc(sizeof(float), length);
	sine(sin_50_2, length, 50, 2, samplingFrequency);
	float *sin_100_2 = (float*)calloc(sizeof(float), length);
	sine(sin_100_2, length, 100, 2, samplingFrequency);
	float *sin_150_1 = (float*)calloc(sizeof(float), length);
	sine(sin_150_1, length, 150, 1, samplingFrequency);
	float *sin_200_1 = (float*)calloc(sizeof(float), length);
	sine(sin_200_1, length, 200, 1, samplingFrequency);

	// sum of sine waves
	float *sum = (float*)calloc(sizeof(float), length);
	for (int i = 0; i < length; ++i) {
		sum[i] = sin_5_1[i] + sin_50_2[i] + sin_100_2[i] + sin_150_1[i] + sin_200_1[i];
	}

	// write resulting waveform to file
	FILE *fp = fopen("out800.txt", "w");
	for (int i = 0; i < length; ++i) {
		float time = i / samplingFrequency;
		fprintf(fp, "%f\t%f\n", time, sum[i]);
	}
	fclose(fp);
	free(sum);
	free(sin_5_1);
	free(sin_50_2);
	free(sin_100_2);
	free(sin_150_1);
	free(sin_200_1);

	system("PAUSE");
}