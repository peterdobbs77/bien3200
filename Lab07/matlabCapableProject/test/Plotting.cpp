/* Plotting.cpp : Defines the entry point for the console application.
		Peter Dobbs
		BIEN 3200 - Section 401
		7 December 2016
		Lab 7

		Program Description:
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
#include "mex.h"

float a1, a2, a3, a4; //Rate constant for endogenous glucose/insulin production or degradation/utilization
float G0; //Reference blood glucose concentration (mg/dl)
float Gb; // Rate of insulin-independent glucose utilization (mg/(dl*hr))

/*
@params
float* u1 - array for storing values of exogenous insulin
float stepsize - constant value for step size
float B - (205 mg/(dl*hr)) is proportional to injected amount of glucose
*/
void calcExogenousGlucose(float* u1, float stepsize, float B) {
	for (float t = 0; t <= 25.0; t += stepsize) {
		if (t >= 2.0 && t < 5.0) {
			u1[(int)(t / stepsize)] = (B)*exp(-2.0 * (t - 2));
		} else if (t >= 8.0 && t < 11.0) {
			u1[(int)(t / stepsize)] = (B)*exp(-2.0 * (t - 8));
		} else if (t >= 16.0 && t < 19.0) {
			u1[(int)(t / stepsize)] = (B)*exp(-2.0 * (t - 16));
		} else {
			u1[(int)(t / stepsize)] = 0;
		}
	}
}

/*
@params
float* u2 - array for storing values of exogenous insulin
float stepsize - constant value for step size
float A - parameter (in mg/(dl*hr)) is proportional to injected dose of insulin
*/
void calcExogenousInsulin(float* u2, float stepsize, float A) {
	for (float t = 0; t <= 25.0; t += stepsize) {
		if (t >= 2.0 && t < 5.0) {
			u2[(int)(t / stepsize)] = (A)*pow(t - 2, 2.0)*exp(-2.0 * (t - 2) / 3.0);
		} else if (t >= 8.0 && t < 11.0) {
			u2[(int)(t / stepsize)] = (A)*pow(t - 8, 2.0)*exp(-2.0 * (t - 8) / 3.0);
		} else if (t >= 16.0 && t < 19.0) {
			u2[(int)(t / stepsize)] = (A)*pow(t - 16, 2.0)*exp(-2.0 * (t - 16) / 3.0);
		} else {
			u2[(int)(t / stepsize)] = 0;
		}
	}
}

/*
For G<=G0
dG/dt = u1 - a1*G*S + a2*(G0 - G) - Gb
*/
float slope1(float u1, float G, float S) {
	return (u1 - (a1*G*S) + a2*(G0 - G) - Gb);
}

/*
For G<=G0
dS/dt = u2 - a4*S
*/
float slope2(float u2, float S) {
	return (u2 - a4*S);
}

/*
For G>G0
dG/dt = u1 - a1*G*S - Gb
*/
float slope3(float u1, float G, float S) {
	return (u1 - (a1*G*S) - Gb);
}

/*
For G>G0
dS/dt = u2 - a4*S + a3*(G-G0)
*/
float slope4(float u2, float G, float S) {
	return (u2 - a4*S + a3*(G - G0));
}

int main() {
	printf("Start of main program: Lab 7 ODE_Solver\n");
	float* G; //Blood glucose concentration (mg/dl)
	float* S; //Insulin glucose concentration (mg/dl)
	float *u1; //Input rate of exogenous glucose (mg/(dl*hr))
	float *u2; //Input rate of exogenous insulin (mg/(dl*hr))

	float t0, tf, ti, stepsize;
	float A, B; //coefficients on functions for exogenous insulin/glucose
	float k11, k12, k13, k14, q11, q12, q13, q14; //slopes

	int N;

	/*Model Parameters Under Physiologic Conditions (normal subject)*/
	a1 = 0.05;	// normal: 0.05
	a2 = 1.0;	// normal: 1.0
	a3 = 0.5;	// normal: 0.5
	a4 = 2.0;	// normal: 2.0
	G0 = 85;	// normal: 100
	Gb = 10;	// normal: 10
	A = 0;		// normal: 0
	B = 205;	// normal: 205

	/*Setup*/
	t0 = 0;
	tf = 25.0;
	stepsize = 0.005;
	N = (int)((tf - t0) / stepsize);

	/*Allocate memory to arrays*/
	G = (float*)calloc(sizeof(float), N);
	S = (float*)calloc(sizeof(float), N);
	u1 = (float*)calloc(sizeof(float), N);
	u2 = (float*)calloc(sizeof(float), N);

	/*Set Initial Conditions*/
	G[0] = 85;
	S[0] = 0;

	/*Determine values for u1 and u2*/
	calcExogenousGlucose(u1, stepsize, B);
	calcExogenousInsulin(u2, stepsize, A);

	/*char sim_file[100];
	FILE *out_file;
	printf("Enter the name of the output file.\n");
	scanf("%s", &sim_file);
	out_file = fopen(sim_file, "w");
	fprintf(out_file, "%f \t %f \t \n", 0.0, G[0], S[0]);*/

	for (int i = 0; i < N - 1; ++i) {
		ti = (i + 1)*stepsize;

		if (G[i] <= G0) {
			k11 = stepsize*slope1(u1[i], G[i], S[i]);
			q11 = stepsize*slope2(u2[i], S[i]);

			k12 = stepsize*slope1(ti + stepsize / 2, G[i] + k11 / 2, S[i] + q11 / 2);
			q12 = stepsize*slope2(ti + stepsize / 2, S[i] + q11 / 2);

			k13 = stepsize*slope1(ti + stepsize / 2, G[i] + k12 / 2, S[i] + q12 / 2);
			q13 = stepsize*slope2(ti + stepsize / 2, S[i] + q12 / 2);

			k14 = stepsize*slope1(ti + stepsize, G[i] + k13, S[i] + q13);
			q14 = stepsize*slope2(ti + stepsize, S[i] + q13);
		} else {
			k11 = stepsize*slope3(u1[i], G[i], S[i]);
			q11 = stepsize*slope4(u2[i], G[i], S[i]);

			k12 = stepsize*slope3(u1[i] + stepsize / 2, G[i] + k11 / 2, S[i] + q11 / 2);
			q12 = stepsize*slope4(u2[i] + stepsize / 2, G[i] + k11 / 2, S[i] + q11 / 2);

			k13 = stepsize*slope3(u1[i] + stepsize / 2, G[i] + k12 / 2, S[i] + q12 / 2);
			q13 = stepsize*slope4(u2[i] + stepsize / 2, G[i] + k12 / 2, S[i] + q12 / 2);

			k14 = stepsize*slope3(u1[i] + stepsize, G[i] + k13, S[i] + q13);
			q14 = stepsize*slope4(u2[i] + stepsize, G[i] + k13, S[i] + q13);
		}
		G[i + 1] = G[i] + (1.0 / 6)*(k11 + 2 * k12 + 2 * k13 + k14);
		S[i + 1] = S[i] + (1.0 / 6)*(q11 + 2 * q12 + 2 * q13 + q14);

		// send values to output file
		//fprintf(out_file, "%f \t %f \t %f \t \n", ti, G[i + 1], S[i + 1]);
	}
	//fclose(out_file);

	/*Plotting in MatLab*/
	printf("Start of Plotting\n");
	//////////////====MATLAB====//////////////////
	Engine *ep;
	ep = engOpen(NULL);

	mxArray* matG = mxCreateNumericMatrix(N, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* matS = mxCreateNumericMatrix(N, 1, mxSINGLE_CLASS, mxREAL);

	memcpy((void*)mxGetPr(matG), (void*)G, sizeof(float) * N);
	memcpy((void*)mxGetPr(matS), (void*)S, sizeof(float) * N);

	engPutVariable(ep, "G", matG);
	engPutVariable(ep, "S", matS);

	engEvalString(ep, "t=0:0.005:24.995;"
		"figure; subplot(2,1,1),plot(t,G);"
		"title ('Blood Glucose Concentration');"
		"xlabel('Time (hours)');"
		"ylabel('Concentration (mg/dL)');"
		"subplot(2,1,2),plot(t,S);"
		"title ('Blood Insulin Concentration');"
		"xlabel('Time (hours)');"
		"ylabel('Concentration (mg/dL)');");

	system("pause"); //Pauses figure
	engClose(ep);
	//////////////======C======///////////////////
	printf("End of plotting\n");

	/*Free the allocated memory*/
	free(G);
	free(S);
	free(u1);
	free(u2);

	/*End Of Program*/
	printf("That's all folks!\n");
	system("PAUSE");
	return 0;
}