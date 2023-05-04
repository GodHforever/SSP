 /***************************************************************************
  * 
  * Homework for chapter 3 -- "overlap-save method"
  *
  * In the main function, we have finished data preparation for you. The
  * add-reverb function "conv" is called here. All you have to do is to 
  * finish the add-reverb function in conv.cpp. Run the project to check 
  * if the processed audio is right or not.
  *
  * The format of input/output audio is pcm ".raw". You can use Audition 
  * or Cooledit to see the waveform or spectrogram of pcm audio files.
  * 
  **************************************************************************/
 
#include <stdio.h>
#include "conv.h"
#include "rir.h"

int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("Usage: %s src.raw dst.raw\n", argv[0]);
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
    long rir_length = 4096;
    long inputdata_length = ftell(fpin);
    inputdata_length /= 2;
    rewind(fpin);
    short* inputdata = new short[inputdata_length];
    short* outputdata = new short[inputdata_length];
    fread(inputdata, sizeof(short), inputdata_length, fpin);

    // add rir
    conv(inputdata, inputdata_length, rir, rir_length, outputdata);

    // save output 
    fwrite(outputdata, sizeof(short), inputdata_length, fpout);

    printf("Add rir to %s, OK.\n", argv[1]);
    delete [] inputdata;
    delete [] outputdata;
    fclose(fpin);
    fclose(fpout);
    return 0;
}

