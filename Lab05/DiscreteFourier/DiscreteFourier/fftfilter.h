#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define PI 4.0*atan(1.0)
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void four1(float *data, int nn, int isign) {
	int n, mmax, m, j, istep, i;
	float wtemp, wr, wpr, wpi, wi, theta;
	float tempr, tempi;

	n = nn << 1;
	j = 1;
	for (i = 1; i < n; i += 2) {
		if (j > i) {
			SWAP(data[j], data[i]);
			SWAP(data[j + 1], data[i + 1]);
		}
		m = n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax = 2;
	while (n > mmax) {
		istep = 2 * mmax;
		theta = (float)6.283185 / (isign*mmax);
		wtemp = (float)sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = (float)sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j = i + mmax;
				tempr = wr*data[j] - wi*data[j + 1];
				tempi = wr*data[j + 1] + wi*data[j];
				data[j] = data[i] - tempr;
				data[j + 1] = data[i + 1] - tempi;
				data[i] += tempr;
				data[i + 1] += tempi;
			}
			wr = (wtemp = wr)*wpr - wi*wpi + wr;
			wi = wi*wpr + wtemp*wpi + wi;
		}
		mmax = istep;
	}
}

void realft(float *data, int n, int isign) {
	int i, i1, i2, i3, i4, n2p3;
	float c1 = 0.5, c2, h1r, h1i, h2r, h2i;
	float wr, wi, wpr, wpi, wtemp, theta;

	theta = (float)3.141593 / (float)n;
	if (isign == 1) {
		c2 = -0.5;
		four1(data, n, 1);
	} else {
		c2 = 0.5;
		theta = -theta;
	}
	wtemp = (float)sin(0.5*theta);
	wpr = (float)-2.0*wtemp*wtemp;
	wpi = (float)sin(theta);
	wr = (float)1.0 + wpr;
	wi = wpi;
	n2p3 = 2 * n + 3;
	for (i = 2; i <= (n / 2); i++) {
		i4 = 1 + (i3 = n2p3 - (i2 = 1 + (i1 = i + i - 1)));
		h1r = c1*(data[i1] + data[i3]);
		h1i = c1*(data[i2] - data[i4]);
		h2r = -c2*(data[i2] + data[i4]);
		h2i = c2*(data[i1] - data[i3]);
		data[i1] = h1r + wr*h2r - wi*h2i;
		data[i2] = h1i + wr*h2i + wi*h2r;
		data[i3] = h1r - wr*h2r + wi*h2i;
		data[i4] = -h1i + wr*h2i + wi*h2r;
		wr = (wtemp = wr)*wpr - wi*wpi + wr;
		wi = wi*wpr + wtemp*wpi + wi;
	}
	if (isign == 1) {
		data[1] = (h1r = data[1]) + data[2];
		data[2] = h1r - data[2];
	} else {
		data[1] = c1*((h1r = data[1]) + data[2]);
		data[2] = c1*(h1r - data[2]);
		four1(data, n, -1);
	}
}

void fft(float *data, int n, int invrs) {
	int newn;
	//    float temp;

	newn = n / 2;
	realft(data, newn, invrs);
}

int highpass(float *xdata, int n) {
	FILE *firfile;
	float h[128], sum;
	static float ydata[2000];
	unsigned int nend, k;
	int i, j, status;

	status = 0;
	firfile = fopen("C:\\mydocuments\\bien_185\\project#2\\highpass.dat", "r");
	if (firfile == NULL) {
		status = 1;
		return(status);
	}

	for (i = 0; i < 2000; ++i)ydata[i] = 0.0;
	for (k = 0; fscanf(firfile, "%e", &h[k]) != EOF; ++k);
	fclose(firfile);

	nend = n + k - 1;
	for (i = 0; i < nend; ++i) {
		sum = 0.0;
		for (j = 0; j < k; ++j) {
			if ((i - j) >= 0 && (i - j) < n) {
				sum += h[j] * xdata[i - j];
			}
		}
		ydata[i] = sum;
	}

	for (i = 0; i < n; ++i) {
		xdata[i] = ydata[i];
	}
	return(status);
}

int lowpass(float *xdata, int n) {
	FILE *firfile;
	float h[128], sum;
	static float ydata[2000];
	unsigned int nend, k;
	int i, j, status;

	status = 0;
	firfile = fopen("c:\\mydocuments\\bien_185\\project#2\\lowpass.dat", "r");
	if (firfile == NULL) {
		status = 1;
		return(status);
	}

	for (i = 0; i < 2000; ++i)ydata[i] = 0.0;
	for (k = 0; fscanf(firfile, "%e", &h[k]) != EOF; ++k);
	fclose(firfile);

	printf("%e\n", h[2]);
	nend = n + k - 1;
	for (i = 0; i < nend; ++i) {
		sum = 0.0;
		for (j = 0; j < k; ++j) {
			if ((i - j) >= 0 && (i - j) < n) {
				sum += h[j] * xdata[i - j];
			}
		}
		ydata[i] = sum;
	}

	for (i = 0; i < n; ++i) {
		xdata[i] = ydata[i];
	}
	return(status);
}

