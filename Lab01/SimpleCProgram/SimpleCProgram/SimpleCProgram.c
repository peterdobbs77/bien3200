/* SimpleCProgram.c : Defines the entry point for the console application.
	@author Peter Dobbs
	@date September 6, 2016
	This simple C program was developed as a solution to the final portion
		of lab exercise one for BIEN 3200 401.
*/

#include "stdafx.h"
#include <stdio.h>


int main(void) {
	int systolic, diastolic, bodyTemperatureC;

	//User Input
	//	blood pressure
	printf("Enter the systemic arterial systolic and diastolic pressures (mmHg)\n");
	printf("Systolic: ");
	scanf_s("%i", &systolic);
	printf("Diastolic: ");
	scanf_s("%i", &diastolic);
	//	body temp
	printf("Enter the Body Temperature in degrees Celcius\n");
	printf("Body Temp: ");
	scanf_s("%i", &bodyTemperatureC);

	//Calculate the mean arterial pressure (MAP)
	int MAP = diastolic + (systolic - diastolic) / 3;

	//Calculate the arithmetic mean pressure (AAP)
	int AAP = (systolic + diastolic) / 2;

	//Converts Body Temp to Fahrenheit
	int bodyTemperatureF = 32 + bodyTemperatureC * 9 / 5;

	//Display the results
	printf("\nMAP in mmHg: %i", MAP);
	printf("\nAAP in mmHg: %i", AAP);
	printf("\nBody Temperature in degrees Fahrenheit: %i", bodyTemperatureF);
	printf("\n");

	system("PAUSE"); //pauses command window for user to view results
	return 0;
}