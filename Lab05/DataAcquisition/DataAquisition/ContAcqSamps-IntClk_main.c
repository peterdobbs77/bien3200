/*********************************************************************
*
* ANSI C Example program:
*    ContAcqSamps-IntClk_main.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to continuously acquire data using
*    the device's internal timing (rate is governed by an internally
*    generated pulse train).
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device. Set to channel # 0 for this project.
*    2. Enter the minimum and maximum voltage ranges.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the sample rate of the acquisiton.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Set the rate for the sample clock. Additionally, define the
*       sample mode to be continous.
*    4. Call the Start function to start acquiring samples.
*    5. Read the waveform data in a loop until the user hits the stop
*       button or an error occurs.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control.
*
* Recommended Use:
*    1. Call Configure and Start functions.
*    2. Call Read function in a timed loop.
*    3. Call Stop function at the end.
*
*********************************************************************/
/* NOTE: this program collect up to 10 sec. If longer sampling time
needed than change the time in the ContAcqSamps-IntClk_Fn.c file from 10 to whatever sampling time needed */
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "NIDAQmx.h"
#include "ContAcqSamps-IntClk_Fn.h"

#define LOOPTIME	250


int main(int argc, char *argv[]) {
	printf("Start of Program: ContAcqSamps-IntClk_main\n");
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	clock_t		startLoop = 0;
	clock_t		startTime = 0;
	int32		read, totalRead = 0;
	float64		data[10000];  // output array
	char        errBuff[2048] = { '\0' };
	char		ch;
	int buffer_size = 10000;  //buffer size

	/* Declare parameters that control the A/D converter
		  * Sampling rate (samp_rate) in samples/sec or Hz
		  * Range of signal (minimum and maximum values), Vmin and Vmax
		  * run time (in seconds), run_time of program */
	float samp_rate = 0; // sampling rate (Hz)
	float Vmin = 0, Vmax = 0; // Range of signal, min and max values
	float run_time = 0; // runtime of the program (seconds)

	/* Declare the output file for storing sampled data */
	char outputFile[100];
	FILE *fp;

	/* prompt user to enter values for the parameters that control the A/D converter */
	printf("Enter the sampling rate (Hz)\n");
	scanf("%f", &samp_rate);
	printf("Enter the minimum signal value (Vmin)\n");
	scanf("%f", &Vmin);
	printf("Enter the maximum signal value (Vmax)\n");
	scanf("%f", &Vmax);
	printf("Enter the runtime of the program (seconds)\n");
	scanf("%f", &run_time);

	/*Calculate number of samples (num_samp = run_time* samp_rate) and check to make sure that it does not
	  exceeds buffer size */
	float num_samp = run_time * samp_rate;
	if (num_samp > buffer_size) {
		return 1;
	}

	DAQmxErrChk(Configure_ContAcqSampsIntClk("Dev1/ai0", Vmin, Vmax, samp_rate, &taskHandle, NULL));
	// Note: ai0 sets ADC channel # to zero
	DAQmxErrChk(Start_ContAcqSampsIntClk(taskHandle));
	startTime = clock();
	printf("Acquiring samples continuously.  Press any key to interrupt\n");
	while (!_kbhit()) {
		while (startLoop && clock() < startLoop + LOOPTIME)
			Sleep(10);
		startLoop = clock();
		DAQmxErrChk(Read_ContAcqSampsIntClk(taskHandle, num_samp, data, buffer_size, &read));
		if (read > 0) {
			totalRead += read;
			printf("Acquired %d samples. Total %d\r", read, totalRead);
		}
	}
	printf("\nAcquired %d total samples.\n", totalRead);
	ch = _getch();

	/* prompt user to enter the output file name to save the sampled amplitude values stored in the
	   array "data" along with the corresponding time points */
	printf("Enter output file name\n");
	scanf("%s", &outputFile);
	fp = fopen(outputFile, "w");

	for (int i = 0; i < num_samp; ++i) {
		fprintf(fp, "%f\t%f\n", i / samp_rate, data[i]);
	}

	fclose(fp);
	return 0;

Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0)
		Stop_ContAcqSampsIntClk(taskHandle);
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
	printf("End of program, press any key to quit\n");
	while (!_kbhit()) {}
	ch = _getch();
}
