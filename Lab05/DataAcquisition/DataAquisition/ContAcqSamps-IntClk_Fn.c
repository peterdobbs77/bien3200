#include "stdafx.h"
#include "NIDAQmx.h"
#include "ContAcqSamps-IntClk_Fn.h"

// Configures the task.
// Recommended parameters:
//   chan           = "Dev1/ai0"
//   min            = -10.0
//   max            = 10.0
//   rate           = 100.0
int32 Configure_ContAcqSampsIntClk(const char chan[], float64 min, float64 max, float64 rate, TaskHandle *taskHandle, uInt32* numChannels) {
	int32	error = 0;

	/*********************************************************************
	*    1. Create a task.
	*    2. Create an analog input voltage channel.
	*    3. Set the rate for the sample clock. Additionally, define the
	*       sample mode to be continous.
	*********************************************************************/
	DAQmxErrChk(DAQmxCreateTask("", taskHandle));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(*taskHandle, chan, "", DAQmx_Val_Cfg_Default, min, max, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCfgSampClkTiming(*taskHandle, "", rate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000));
	if (numChannels)
		DAQmxErrChk(DAQmxGetTaskAttribute(*taskHandle, DAQmx_Task_NumChans, numChannels));
Error:
	return error;
}

// Starts the task.
int32 Start_ContAcqSampsIntClk(TaskHandle taskHandle) {
	return DAQmxStartTask(taskHandle);
}

// Reads data into user allocated buffer.
// Recommended parameters:
//   bufferSize     = 10
int32 Read_ContAcqSampsIntClk(TaskHandle taskHandle, uInt32 sampsPerChan, float64 data[], uInt32 bufferSize, int32 *read) {
	return DAQmxReadAnalogF64(taskHandle, sampsPerChan, 10.0, DAQmx_Val_GroupByScanNumber, data, bufferSize, read, NULL);
	//10.0 is the max number of seconds that you can record data, change it if longer times are needed
}

// Stops the task.
int32 Stop_ContAcqSampsIntClk(TaskHandle taskHandle) {
	int32	error = 0;

	error = DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	return error;
}
