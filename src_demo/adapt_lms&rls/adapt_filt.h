/***************************************************************************
  * 
  * Homework for chapter 4 -- Adaptive filter using LMS & RLS
  *
  * Here is the declaration of adapt_filtering function.
  * 
  **************************************************************************/

#ifndef _ADAPT_FILT_H_
#define _ADAPT_FILT_H_

const int filter_length = 128;
static double inputdata[filter_length];

int adapt_filtering(short input, double* adapt_filter, int filter_length, short* err);

#endif

