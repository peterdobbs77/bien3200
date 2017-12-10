/* Lab 1.cpp : Defines the entry point for the console application.
// author: Peter Dobbs
// section: BIEN 3200 401
// description: These functions reflect the responsibilities called for
		by the lab outline in Part 2.
*/

#include "stdafx.h"
#include <stdio.h>

/*
Copied code from lab report
	Make sure to comment out the other main
	before running
*/
//int main(void) {
//	float sum, product, mean, a, b, c;
//
//	printf("Enter the values of a, b, and c\n");
//
//	scanf_s("%f %f %f", &a, &b, &c);
//
//	sum = a + b + c;
//	product = a*b*c;
//	mean = (a + b + c) / 3;
//
//	printf("Sum = a + b + c = %3.2f\n", sum);
//	printf("Product = a x b x c = %3.1f\n", product);
//	printf("Mean = (a + b + c)/3 = %3.1f\n", mean);
//
//	system("PAUSE");
//	return 0;
//}

/*
Corrected Source Code
	Make sure to comment out the other main
	before running
*/
float main(void) {
	int sum;
	//compute result
	sum = 40 + 36 - 100 * 2;
	//display result
	printf("The answer is %i \n", sum);

	system("PAUSE");
	return sum;
}
