/* SimpleCProgram.c : Defines the entry point for the console application.
	Peter Dobbs
	BIEN 3200 - Section 401
	13 September 2016
	Lab 2

	Program Description:
		This program calculates and displays the mean arterial pressure
		and the pulse pressure for a patient whose systolic and diastolic
		pressures are inputs.

*/

#include "stdafx.h"
#include <stdio.h>

float CalcMeanArterialPressure(float systolic, float diastolic) {
	//Calculate the mean arterial pressure (MAP)
	float MAP = diastolic + (systolic - diastolic) / 3;
	return MAP;
}

float CalcPulsePressure(float systolic, float diastolic) {
	//Calculate the pulse pressure (PP)
	float PP = systolic - diastolic;
	return PP;
}

int main(void) {
	float systolic, diastolic;

	//User Input
	//	blood pressure
	printf("Enter the systemic arterial systolic and diastolic pressures (mmHg)\n");
	printf("Systolic: ");
	scanf_s("%g", &systolic);
	printf("Diastolic: ");
	scanf_s("%g", &diastolic);

	//Calculate mean arterial pressure (MAP)
	float MAP = CalcMeanArterialPressure(systolic, diastolic);

	//Display systolic pressure, diastolic pressure, and MAP
	printf("\nSystolic Pressure:\t %g mmHg", systolic);
	printf("\nDiastolic Pressure:\t %g mmHg", diastolic);
	printf("\nMAP in mmHg:\t\t %g", MAP);

	//Calculate and display pulse pressure (PP)
	float PP = CalcPulsePressure(systolic, diastolic);
	printf("\nPP in mmHg:\t\t %g\n", PP);

	system("PAUSE"); //pauses command window for user to view results
	return 0;
}