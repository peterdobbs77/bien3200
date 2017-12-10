#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "engine.h"
//#include "mex.h"

int main(void) {
	float x1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	float y1[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
	
	//open the MatLab engine
	Engine *ep; 
	ep = engOpen(NULL); //creates a link to MatLab

	//Create a pointer of the type mxArray (MatLab array) with the right dimension and type, allocate space
	mxArray* matx1 = mxCreateNumericMatrix(10, 1, mxSINGLE_CLASS, mxREAL);
	mxArray* maty1 = mxCreateNumericMatrix(10, 1, mxSINGLE_CLASS, mxREAL);

	/* Copies values of x1 and y1 into the allocated space: copies 10 *(# of bytes for a float) bytes 
	from x1 to matx1 or from y1 to maty1 */
	memcpy((void *)mxGetPr(matx1), (void *)x1, sizeof(float)*10);
	memcpy((void *)mxGetPr(maty1), (void *)y1, sizeof(float)*10);

	/* Send variable to MatLab through A and F: Write mxArray matx1 or maty1 to the egnine "ep" 
	and gives it the variable name A or B */

	engPutVariable(ep, "A", matx1);
	engPutVariable(ep, "F", maty1);

	//use Matlab commands
	engEvalString(ep, "figure; plot(A,F);");
	engEvalString(ep, "title ('x vs Y');");
	engEvalString(ep, "xlabel ('x values');");
	engEvalString(ep, "ylabel ('y values');");

	//Pauses figure
	system("pause");

	//close the engine 
	
	engClose(ep); 

	return 0;
}