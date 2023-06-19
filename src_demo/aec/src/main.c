// /***************************************************************************
//   * 
//   * Homework for chapter 5 -- Acoustic Echo Cancellation
//   *
//   * In the main function, we have finished data preparation for you.
//   * Input signal and reference signal, input.pcm & ref.pcm, are read,
//   * frame by frame. Then AEC is applied.
//   *
//   * The format of input/output audio is pcm ".raw". You can use Audition 
//   * or Cooledit to see the waveform or spectrogram of pcm audio files.
//   * 
//   **************************************************************************/

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
// #include <time.h>
// #include "dios_ssp_api.h"

// const int array_frm_len = 128;

// int main(int argc, char** argv)
// {
//     int input_idx = -1;
//     int ref_idx = -1;
//     int output_idx = -1;
//     int ii, jj;
//     for(ii = 0; ii < argc; ii++)
//     {
//         if(strcmp(argv[ii], "-i") == 0)
//         {
//             input_idx = ii;
//         }
//         if(strcmp(argv[ii], "-r") == 0)
//         {
//             ref_idx = ii;
//         }
//         if(strcmp(argv[ii], "-o") == 0)
//         {
//             output_idx = ii;
//         }
//     }
//     if((input_idx == -1) || (output_idx == -1) || (ref_idx == -1))
//     {
//         printf("usage: %s -i input1.pcm [input2.pcm] [-r ref.pcm] -o output.pcm\r\n", argv[0]);
//         return -1;
//     }
//     int channel_num;
//     if(ref_idx == -1)
//     {
//         channel_num = output_idx - input_idx - 1;
//     }
//     else
//     {
//         channel_num = ref_idx - input_idx - 1;
//     }
//     printf("channel_num %d\n", channel_num);
    
//     FILE **fpin = (FILE**)calloc(channel_num, sizeof(FILE*));;
//     FILE *fpref = NULL;
//     FILE *fpout = NULL;
//     short *ptr_input_data = (short*)calloc(channel_num * array_frm_len, sizeof(short));
//     short *ptr_output_data = (short*)calloc(channel_num * array_frm_len, sizeof(short));
//     short *ptr_temp = (short*)calloc(array_frm_len, sizeof(short));
//     short *ptr_ref_data = (short*)calloc(array_frm_len, sizeof(short));
   
//     fpin[0] = fopen(argv[input_idx + 1], "rb");
//     fseek(fpin[0], 0, SEEK_END);
//     long file_len = ftell(fpin[0]);
//     file_len /= 2;
//     rewind(fpin[0]);
//     fclose(fpin[0]);
//     long frame_num = (file_len - array_frm_len * 128) / array_frm_len;
//     int sample_res = (file_len - array_frm_len * 128) % array_frm_len;
//     printf("file_len:%d framenum: %ld and %d samples.\r\n", file_len, frame_num, sample_res);
//     // fpref = fopen(argv[2], "rb");
//     // fpout = fopen(argv[3], "wb");
//     for(ii = 0; ii < channel_num; ii ++) {
//         fpin[ii] = fopen(argv[input_idx + 1 + ii], "rb");
//         if(fpin[ii] == NULL) {
//             printf("can not find file\n");
//             exit(0);
//         }
//         printf("Open input file %s success.\r\n", argv[input_idx + 1 + ii]);

//     }
//     fpout = fopen(argv[output_idx + 1], "wb");
//     if(fpout == NULL) {
//         printf("can not find out file\n");
//         exit(0);
//     }
//     fpref = fopen(argv[ref_idx + 1], "rb");
//     if(fpref == NULL) {
//         printf("can not open ref file\n");
//         exit(0);
//     }


//     // N = 128 也就是128帧，每帧128个采样点 
//     // TDE Begin
//     int delay = 0;
//     short* ptr_input_for_tde = (short*)calloc(array_frm_len * 128, sizeof(short));
//     short* ptr_ref_for_tde = (short*)calloc(array_frm_len * 128, sizeof(short));
//     fread(ptr_input_for_tde, sizeof(short), array_frm_len * 128, fpin[0]);
//     fread(ptr_ref_for_tde, sizeof(short), array_frm_len * 128, fpref);
//     delay = dios_ssp_tde(ptr_input_for_tde, ptr_ref_for_tde, array_frm_len * 128);
//     // TDE End
//     printf("time delay is %d\n", delay);
//     // AEC Begin
//     void* st;
//     st = dios_ssp_init_api(delay);
//     dios_ssp_reset_api(st);
//     for(ii = 0; ii < frame_num; ii++)
//     {
//         for(jj = 0; jj < channel_num; jj ++) {
//             // prepare input signal
//             fread(ptr_temp, sizeof(short), array_frm_len, fpin[jj]);
//             memcpy(&ptr_input_data[jj * array_frm_len], ptr_temp, sizeof(short) * array_frm_len);
//         }
//         // prepare ref signal
//         fread(ptr_temp, sizeof(short), array_frm_len, fpref);
//         memcpy(ptr_ref_data, ptr_temp, sizeof(short) * array_frm_len);
//         // AEC
//         dios_ssp_process_api(st, ptr_input_data, ptr_ref_data, ptr_output_data);
//         // save output file
//         fwrite(ptr_output_data, sizeof(short), array_frm_len * 2, fpout);
//     }
//     fwrite(ptr_output_data, sizeof(short), sample_res, fpout);
//     dios_ssp_uninit_api(st);
//     // AEC End
//     for(ii = 0; ii < channel_num; ii++)
//     {
//         fclose(fpin[ii]);
//     }
//     free(fpin);
//     fclose(fpref);
//     fclose(fpout);
//     free(ptr_temp);
//     free(ptr_input_data);
//     free(ptr_output_data);
//     free(ptr_ref_data);
//     free(ptr_input_for_tde);
//     free(ptr_ref_for_tde);
//     printf("process finished.\r\n");
//     return 0;
// }

