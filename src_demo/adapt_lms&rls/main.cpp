 /***************************************************************************
  * 
  * Homework for chapter 4 -- Adaptive filter using LMS & RLS
  *
  * In the main function, we have finished data preparation for you. 
  * For simplicity, we use whitenoise (audio.raw) as both input signal x(n) 
  * and desired signal d(n), that is x(n) = d(n). In this situation, the 
  * optimal filter w_opt should be Delta function. So you are about to 
  * implement such adaptive algorithms as LMS and RLS, to see how fast
  * the filter will converge to Delta function from a all-zero vector.
  *
  * The format of input/output audio is pcm ".raw". You can use Audition 
  * or Cooledit to see the waveform or spectrogram of pcm audio files.
  * 
  **************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include "adapt_filt.h"

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: %s src.raw err.raw\n", argv[0]);
        return -1;
    }

    // open input and output file
    FILE *fpin, *fpout;
    fpin = fopen(argv[1], "rb");
    if(NULL == fpin) {
        printf("open file %s error.\n", argv[1]);
        return -1;
    }
    fpout = fopen(argv[2], "wb");
    if(NULL == fpout) {
        printf("open file %s error.\n", argv[2]);
        fclose(fpin);
        return -1;
    }

    // get date length of input audio
    fseek(fpin, 0, SEEK_END);
    long inputdata_length = ftell(fpin);
    inputdata_length /= 2;
    rewind(fpin);
    short* inputdata = new short[inputdata_length];
    short* outputdata = new short[inputdata_length];
    fread(inputdata, sizeof(short), inputdata_length, fpin);

    // initialize adaptive filter coefficients, by zeros
    double w[filter_length];
    for(int i = 0; i < filter_length; i++) {
        w[i] = 0.0;
    }

    // adaptive filtering sample by sample
    for(int i = 0; i < inputdata_length; i++) {
        adapt_filtering(inputdata[i], w, filter_length, &outputdata[i]);
    }

    // save output i.e. error signal
    fwrite(outputdata, sizeof(short), inputdata_length, fpout);

    printf("%s done.\n", argv[1]);
    delete [] inputdata;
    delete [] outputdata;
    fclose(fpin);
    fclose(fpout);
    return 0;
}

