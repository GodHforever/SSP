/***************************************************************************
  * 
  * Homework for chapter 4 -- Adaptive filter using LMS & RLS
  *
  * Here is the realization of adapt_filtering function.
  * 
  **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adapt_filt.h"

/**
 * @brief time-domain adaptive filter algorithm
 *
 * @param input          input audio sample of time index(n)
 * @param adapt_filter   adaptive filter buffer
 * @param filter_length  adaptive filter length, 128 by default
 * @param err            error output of time index(n)
 * @return 
 *     @retval 0         successfully
 */
int adapt_filtering(short input, double* adapt_filter, int filter_length, short* err) {
    int i;
    double filter_output = 0.0;
    double error = 0.0;
    double mu = 0.2;

    // update inputdata buffer. Each sample is normalized to [-1,1)
    for(i = filter_length - 1; i > 0; i--) {
        inputdata[i] = inputdata[i - 1];
    }
    inputdata[0] = double(input / 32768.0);

    // finish adaptive filtering algorithm here, using LMS and RLS, respectively
    // 1. calculate filter output, as well as necessary parameters



    // 2. update filter: adapt_filter, for future iterations
    
    

    // end adaptive filtering algorithm

    // output error
    err[0] = short(error * 32768.0);

    return 0;
}

