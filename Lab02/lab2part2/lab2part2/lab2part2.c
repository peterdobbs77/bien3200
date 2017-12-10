/* lab2part2.cpp : Defines the entry point for the console application.
	Peter Dobbs
	BIEN 3200 - Section 401
	13 September 2016
	Lab 2

	Program Description:
		This program takes in a column of float data values and performs
		statistic calculations (mean, variance, skew) on it.
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "engine.h"
//#include "mex.h"
#include <string.h>

/*
	data : float array that holds data set
	n : int representing size of array

	Calculates the mean of the input set of data
*/
float fmean(float data[], int n) {
	float mean = 0;
	for (int i = 0; i < n; i++)
		mean = mean + data[i];
	mean = mean / n;
	return mean;
}

/*
	data : float array that holds data set
	n : int representing size of array

	Calculates the variance of the input data set
*/
float fvariance(float data[], int n) {
	float variance = 0;
	float mean = fmean(data, n);
	for (int i = 0; i < n; i++)
		variance = variance + pow(data[i] - mean, 2);
	variance = 1 / (variance - 1);
	return variance;
}

/*
	data : float array that holds data set
	n : int representing size of array

	Calculates the skew of the input data set
*/
float fskewn(float data[], int n) {
	float skew = 0;
	float mean = fmean(data, n);
	float variance = fvariance(data, n);
	for (int i = 0; i < n; i++)
		skew = skew + pow(data[n] - mean, 3);
	skew = skew / ((n - 1)*pow(variance, 3 / 2));
	return skew;
}

int main() {
	printf("Lab 2, Part 2");
	float xdata[100];
	static float mean, var, skewN;
	static int n;
	FILE *inputfile, *outputfile;
	char inputName[100];
	char outputName[100];

	//prompt user for name
	printf("\nInput file name: ");
	scanf("%s", &inputName);

	// read in data
	inputfile = fopen(inputName, "r");
	if (inputfile == NULL) { //file not found
		printf("Input file not found!\n");
		system("PAUSE");
		return 1; //end program
	}
	for (n = 0; fscanf(inputfile, "%g", &xdata[n]) != EOF; n++); /*EOF: End Of File*/
	fclose(inputfile);

	//calculate mean data
	mean = fmean(xdata, n);

	//calculate variance
	var = fvariance(xdata, n);

	//calculate skew of n
	skewN = fskewn(xdata, n);

	//prompt user for output file name
	printf("\nOutput file name: ");
	scanf("%s", &outputName);

	//write to the file
	outputfile = fopen(outputName, "w");
	fprintf(outputfile, "mean:\t\t%3.3g\n", mean);
	fprintf(outputfile, "variance:\t%0.3g\n", var);
	fprintf(outputfile, "skew:\t\t%0.3g\n", skewN);

	system("PAUSE");

	////use MATLAB to create histogram
	//Engine *ep;
	//if ((ep = engOpen(NULL)) == NULL) return 1;
	//mxArray* matx1 = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);
	//memcpy((void *)mxGetPr(matx1), (void *)xdata, sizeof(float) * 100);
	//engPutVariable(ep, "xdata", matx1);
	//engEvalString(ep, "fig = figure; histogram(xdata)"
	//	"xlabel('Bins'); ylabel('Frequency');"
	//	"print(fig,'histogram','-dpng')");

	//system("PAUSE");

	//engClose(ep);

	////system("PAUSE");
	return 0;
}

