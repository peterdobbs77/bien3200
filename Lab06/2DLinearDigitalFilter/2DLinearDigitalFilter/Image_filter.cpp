/* 2-D filtering of a CT image */
//Image_filter.cpp
#pragma warning (disable: 4996)
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {

	int i, j, k, l, m, n;
	float *tmp, *out;
	float sum;
	char in_file[100], out_file[100];
	FILE *inputfile, *outfile;
	//Edge detection kernel	
	float filt[3][3] = {
		{0,-1,0},
		{-1,5,-1},
		{0,-1,0}
	};


	// low pass filter
	/*float wt = 1.0 / 25.0;
	float filt[5][5] = {
		{wt,wt,wt,wt,wt},
		{wt,wt,wt,wt,wt},
		{wt,wt,wt,wt,wt},
		{wt,wt,wt,wt,wt},
		{wt,wt,wt,wt,wt}
	};*/

	// Open input file (unfiltered data)
	printf("Enter input file name\n");
	scanf("%s", in_file);
	inputfile = fopen(in_file, "r");
	// Check for file existance 
	if (!(inputfile)) {
		printf("File does not exist.\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Enter the number of rows:\n");
		scanf("%i", &m);
		printf("Enter the number of columns:\n");
		scanf("%i", &n);

		//dynamic memory allocation to store data
		tmp = (float *)calloc(sizeof(float), m*n);
		out = (float *)calloc(sizeof(float), m*n);

		// read in data from file
		for (i = 0; i < m*n; i++) {
			fscanf(inputfile, "%f", (tmp + i));
			*(out + i) = *(tmp + i);
		}
		fclose(inputfile);

		//convolution with filter kernel

		for (i = 1; i < n - 1; i++) {
			for (j = 1; j < m - 1; j++) {
				sum = 0.0;
				for (k = 0; k < 3; k++) {
					for (l = 0; l < 3; l++) {
						sum = sum + filt[k][l] * (*(tmp + (i - 1 + k)*m + (j - 1 + l)));
					}
				}
				//thresholding
				/*if (sum >= 0.03) {
					sum = 1.0;
				} else {
					sum = 0.0;
				}*/

				*(out + i*m + j) = sum;
			}

		}

		//Save filtered data to a .txt file

		printf("Enter output file name\n");
		scanf("%s", out_file);
		outfile = fopen(out_file, "w");

		//Proper formating
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				fprintf(outfile, "%g ", *(out + i*m + j));
			}
			fprintf(outfile, "\n");
		}

		fclose(outfile);

		free(tmp);
		free(out);
	}
	system("PAUSE");
	return 0;
}
