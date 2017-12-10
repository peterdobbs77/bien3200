#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }

int32 Configure_ContAcqSampsIntClk(const char chan[], float64 min, float64 max, float64 rate, TaskHandle *taskHandle, uInt32 *numChannels);
int32 Start_ContAcqSampsIntClk(TaskHandle taskHandle);
int32 Read_ContAcqSampsIntClk(TaskHandle taskHandle, uInt32 sampsPerChan, float64 data[], uInt32 bufferSize, int32 *read);
int32 Stop_ContAcqSampsIntClk(TaskHandle taskHandle);
