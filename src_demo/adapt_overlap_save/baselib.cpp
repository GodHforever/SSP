/***************************************************************************
  * 
  * Homework for chapter 3 -- "overlap-save method"
  *
  * Here is the realization of FFT function.
  * The input/output data are stored in fftdata. FFT order should also be 
  * assigned. It equals to log2(fft_length)
  * 
  **************************************************************************/

#include <math.h>
#include <stddef.h>
#include "typedefs.h"
#include "baselib.h"

void fft(COMPLEX *fftdata, int fftorder) {

    int n, i, nv2, j, k, le, l, le1, ip, nm1;
    COMPLEX t, u, w;

    n = 1;
    for(i = 0; i < (int)fftorder; i++) {
        n = n * 2;
    }

    nv2 = n / 2;
    nm1 = n - 1;
    j = 1;

    for (i = 1; i <= nm1; i ++) {
        if (i < j) {
            t.real = fftdata[i - 1].real;
            t.image = fftdata[i - 1].image;
            fftdata[i - 1].real = fftdata[j - 1].real;
            fftdata[i - 1].image = fftdata[j - 1].image;
            fftdata[j - 1].real = t.real;
            fftdata[j - 1].image = t.image;
        }
        k = nv2;
        while (k < j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }

    le = 1;

    for (l= 1; l <= (int)fftorder; l++) {
        le *= 2;
		le1 = le / 2;
		u.real = 1.0f;
		u.image = 0.0f;
		w.real = (float) cos(PI / le1);
		w.image =(float) -sin(PI / le1);

		for (j = 1; j <= le1; j++) {
            for (i = j; i <= n; i += le) {
                ip = i + le1;
				t.real = fftdata[ip - 1].real * u.real - fftdata[ip - 1].image * u.image;
				t.image = fftdata[ip - 1].real * u.image + fftdata[ip - 1].image * u.real;
				fftdata[ip - 1].real = fftdata[i - 1].real - t.real;
				fftdata[ip - 1].image = fftdata[i - 1].image - t.image;
				fftdata[i - 1].real = t.real + fftdata[i - 1].real;
				fftdata[i - 1].image = t.image + fftdata[i - 1].image;
            }

            t.real = u.real * w.real - u.image * w.image;
            t.image = u.image * w.real + u.real * w.image;
            u.real = t.real;
            u.image = t.image;
        }
    }
}


