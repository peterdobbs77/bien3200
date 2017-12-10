/* lab2part3.cpp : Defines the entry point for the console application.
	Peter Dobbs
	BIEN 3200 - Section 401
	13 September 2016
	Lab 2

	Program Description:
		This program reads clinical sinus rhythm (normal) ECG data from
		a text file (sinus2.txt) and uses a connection with MATLAB to plot
		the data on a graph
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>
#include "engine.h"
#include "mex.h"

int main(void) {
	printf("Lab 2, Part 3");
	int length;
	static float xdata[2500], ydata[2500];
	FILE *fp;

	//read from file
	fp = fopen("sinus2.txt", "r");
	if (fp == NULL) { //file not found
		printf("Input file not found!\n");
		system("PAUSE");
		return 1; //end program
	}
	for (length = 0; fscanf(fp, "%g %g", &xdata[length], &ydata[length]) != EOF; length++); /*EOF: End Of File*/
	fclose(fp);

	//interface with Matlab for plotting
	Engine *ep;
	ep = engOpen(NULL);

	mxArray* matx1 = mxCreateNumericMatrix(length, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty1 = mxCreateNumericMatrix(length, 1, mxSINGLE_CLASS, mxREAL);

	/* Copies values of x1 and y1 into the allocated space: copies 10 *(# of bytes for a float) bytes
	from x1 to matx1 or from y1 to maty1 */
	memcpy((void *)mxGetPr(matx1), (void *)xdata, sizeof(float) * length);
	memcpy((void *)mxGetPr(maty1), (void *)ydata, sizeof(float) * length);

	engPutVariable(ep, "X", matx1);
	engPutVariable(ep, "Y", maty1);

	engEvalString(ep, "figure; plot(X,Y);"
		"title ('Amplitude vs Time');"
		"xlabel ('Time (sec)');"
		"ylabel ('Amplitude (mV)');");

	system("pause");
	engClose(ep);
	return 0;
}

