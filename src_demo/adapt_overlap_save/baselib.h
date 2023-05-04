/***************************************************************************
  * 
  * Homework for chapter 3 -- "overlap-save method"
  *
  * Here is the declaration of FFT function.
  * The input/output data are stored in fftdata. FFT order should also be 
  * assigned. It equals to log2(fft_length)
  * 
  **************************************************************************/

#ifndef  __BASELIB_H_
#define  __BASELIB_H_

#include "typedefs.h"

/** @brief FFT */
void fft(COMPLEX *fftdata, int fftorder);

#endif  //__BASELIB_H_

