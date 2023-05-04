/***************************************************************************
  * 
  * Homework for chapter 3 -- "overlap-save method"
  *
  * Here is the realization of add rir function.
  * You have to finish "conv" function by yourself. It is called in main 
  * function. Also, you may want to use FFT, which is ready for you too.
  *
  * NOTICE: There is no IFFT function. So you have to realize IFFT using FFT.
  * Take some time to think about how to do this please.
  * 
  **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "conv.h"
#include "baselib.h"

/**
 * @brief add rir using overlap-save method.
 *
 * @param inputdata         input clean audio data buffer
 * @param inputdata_length  length of inputdata(samples)
 * @param rir               room impulse response buffer
 * @param rir_length        length of rir, 4096 by default
 * @param outputdata        processed data, same length as inputdata
 * @return 
 *     @retval 0            successfully
 */
int conv(short* inputdata, long inputdata_length, double* rir, long rir_length, short* outputdata)
{
    // finish overlap-save method here

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
