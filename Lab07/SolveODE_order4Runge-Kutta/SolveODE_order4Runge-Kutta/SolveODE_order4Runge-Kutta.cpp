/* SolveODE_order4Runge-Kutta.cpp : Defines the entry point for the console application.
	Peter Dobbs
	BIEN 3200
	Lab 7
	29 November 2016

	Program Description:
		Solves the model ODEs for the blood glucose and insulin concentrations
		in a '''normal subject''' using the 4th order Runge-Kutta method.
*/

#include "stdafx.h"
#include <math.h>
#include <stdlib.h>

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
		if (t >= 2 && t < 5) {
			u1[(int)(t / stepsize)] = (B)*exp(-2 * (t - 2));
		} else if (t >= 8 && t < 11) {
			u1[(int)(t / stepsize)] = (B)*exp(-2 * (t - 8));
		} else if (t >= 16 && t < 19) {
			u1[(int)(t / stepsize)] = (B)*exp(-2 * (t - 16));
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
		if (t >= 2 && t < 5) {
			u2[(int)(t / stepsize)] = (A)*exp(-2 * (t - 2));
		} else if (t >= 8 && t < 11) {
			u2[(int)(t / stepsize)] = (A)*exp(-2 * (t - 8));
		} else if (t >= 16 && t < 19) {
			u2[(int)(t / stepsize)] = (A)*exp(-2 * (t - 16));
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
	float* G; //Blood glucose concentration (mg/dl)
	float* S; //Insulin glucose concentration (mg/dl)
	float *u1; //Input rate of exogenous glucose (mg/(dl*hr))
	float *u2; //Input rate of exogenous insulin (mg/(dl*hr))

	float t0 = 0, tf, ti, stepsize;
	float A, B; //coefficients on functions for exogenous insulin/glucose
	float k11, k12, k13, k14, q11, q12, q13, q14; //slopes

	int N;
	char sim_file[100];
	FILE *out_file;

	/*Model Parameters Under Physiologic Conditions (normal subject)*/
	a1 = 0.05;
	a2 = 1.0;
	a3 = 0.5;
	a4 = 2.0;
	G0 = 100;
	Gb = 10;
	A = 0;
	B = 205;

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

	printf("Enter the name of the output file.\n");
	scanf("%s", &sim_file);
	out_file = fopen(sim_file, "w");
	fprintf(out_file, "%f \t %f \t \n", 0.0, G[0], S[0]);

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
		fprintf(out_file, "%f \t %f \t %f \t \n", ti, G[i + 1], S[i + 1]);
	}
	fclose(out_file);

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

