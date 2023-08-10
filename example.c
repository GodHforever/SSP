// /**
//  * @file example.c
//  * @author Liang Shui (Gao Hao)
//  * @brief 
//  * @date 2023-02-10
//  * 
//  * Copyright (C) 2022 Gao Hao. All rights reserved.
//  * 
//  */
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <string.h>
// #include <math.h>
// #include "mini_log/m_log.h"
// #include "dios_ssp_api.h"

// const int array_frm_len = 128;

// int main(int argc, char **argv)
// {
//     void* ptr = NULL;
// 	ptr = (void*)malloc(sizeof(objSSP_Param));
// 	objSSP_Param* SSP_PARAM = (objSSP_Param*)ptr;
//     SSP_PARAM->AEC_KEY = 1;
//     SSP_PARAM->NS_KEY = 0;
//     SSP_PARAM->AGC_KEY = 0;
//     SSP_PARAM->HPF_KEY = 0;
//     SSP_PARAM->BF_KEY = 0;
//     SSP_PARAM->DOA_KEY = 0;
//     SSP_PARAM->mic_num = 2;
//     SSP_PARAM->ref_num = 1;
//     SSP_PARAM->loc_phi = 90.0;
//     float mic_coord[3] = {0.05, 0.0, 0.0 };
//     int n = 3;

//     if (SSP_PARAM->AEC_KEY == 1)
//     {
//         if (SSP_PARAM->ref_num == 0)
//         {
//             printf("AEC is turned on, ref_num must be greater than 0.  \n");
//             exit(-1);
//         }
//     }

//     if(SSP_PARAM->BF_KEY != 0 || SSP_PARAM->DOA_KEY == 1)
//     {
//         if (SSP_PARAM->mic_num <= 1)
//         {
//             printf("MVDR is turned on, mic_num must be greater than 1. \n");
//             exit(-1);
//         }

//         if (n != 3 * (unsigned int)SSP_PARAM->mic_num)
//         {
//             printf("The number of coordinate points must match the number of microphones. \n");
//             exit(-1);
//         }

//         int i;
//         for (i = 0; i < SSP_PARAM->mic_num; i++)
//         {
//             SSP_PARAM->mic_coord[i].x = mic_coord[3 * i];
//             SSP_PARAM->mic_coord[i].y = mic_coord[3 * i + 1];
//             SSP_PARAM->mic_coord[i].z = mic_coord[3 * i + 2];
//         }
//     }
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
//     if((input_idx == -1) || (output_idx == -1))
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

//     FILE **fp = (FILE**)calloc(channel_num, sizeof(FILE*));;
//     FILE *fpref = NULL;
//     FILE *fpout = NULL;
//     short *ptr_input_data = (short*)calloc(channel_num * array_frm_len, sizeof(short));
//     short *ptr_output_data = (short*)calloc(array_frm_len, sizeof(short));
//     short *ptr_temp = (short*)calloc(array_frm_len, sizeof(short));
//     short *ptr_ref_data = (short*)calloc(array_frm_len, sizeof(short));

//     void* st;
//     st = dios_ssp_init_api(SSP_PARAM);
//     dios_ssp_reset_api(st, SSP_PARAM);

//     /* read data from outline files */
//     char sz_file_name[1024];
//     strcpy(sz_file_name, argv[input_idx + 1]);
//     fp[0] = fopen(sz_file_name, "rb");
//     fseek(fp[0], 0, SEEK_END);
//     long file_len = ftell(fp[0]);
//     file_len /= 2;
//     rewind(fp[0]);
//     fclose(fp[0]);
//     long frame_num = file_len / array_frm_len;
//     int sample_res = file_len % array_frm_len;

//     for(ii = 0; ii < channel_num; ii++)
//     {
//         strcpy(sz_file_name, argv[input_idx + 1 + ii]);
//         fp[ii] = fopen(sz_file_name, "rb");
//         if(fp[ii] == NULL)
//         {
//             printf("Open input file %s error.\r\n", sz_file_name);
//             exit(0);
//         }
//         printf("Open input file %s success.\r\n", sz_file_name);
//     }
//     fpout = fopen(argv[output_idx + 1], "wb");
//     if(fpout == NULL)
//     {
//         printf("Open out file %s error.\r\n", argv[output_idx + 1]);
//         exit(0);
//     }
//     if(ref_idx > 0)
//     {
//         fpref = fopen(argv[ref_idx + 1], "rb");
//         if(fpref == NULL)
//         {
//             printf("Open ref file %s error.\r\n", argv[ref_idx + 1]);
//             exit(0);
//         }
//     }

//     /* if you encounter such error: Matrix is singular!
//      * try to use these codes to add random disturbances to Rnn matrix
//      */
//     /*int kk;
//     srand((unsigned)time(NULL));*/

//     /* signal processing here */
//     for(ii = 0; ii < frame_num; ii++)
//     {
//         /* prepare input signals */
//         for(jj = 0; jj < channel_num; jj++)
//         {
//             fread(ptr_temp, sizeof(short), array_frm_len, fp[jj]);
//             memcpy(&ptr_input_data[jj * array_frm_len], ptr_temp, sizeof(short) * array_frm_len);
//             /* if you encounter such error: Matrix is singular!
//              * try to use these codes to add random disturbances to Rnn matrix
//              */
//             /*for (kk = 0; kk < array_frm_len; kk++)
//             {
//                 ptr_input_data[jj * array_frm_len + kk] += rand() % 10;
//             }*/
//         }

//         /* prepare ref signal */
//         if(ref_idx > 0)
//         {
//             fread(ptr_temp, sizeof(short), array_frm_len, fpref);
//             memcpy(ptr_ref_data, ptr_temp, sizeof(short) * array_frm_len);
//         }
//         else
//         {
//             memset(ptr_ref_data, 0, sizeof(short) * array_frm_len);
//         }

//         /* dios ssp processing here */
//         dios_ssp_process_api(st, ptr_input_data, ptr_ref_data, ptr_output_data, SSP_PARAM);

//         /* save output file */
//         fwrite(ptr_output_data, sizeof(short), array_frm_len, fpout);
//     }
//     fwrite(ptr_output_data, sizeof(short), sample_res, fpout);

//     if(ref_idx > 0)
//     {
//         fclose(fpref);
//     }
//     fclose(fpout);
//     for(ii = 0; ii < channel_num; ii++)
//     {
//         fclose(fp[ii]);
//     }
//     dios_ssp_uninit_api(st, SSP_PARAM);
//     free(fp);
//     free(ptr_temp);
//     free(ptr_input_data);
//     free(ptr_output_data);
//     free(ptr_ref_data);
//     free(SSP_PARAM);
//     printf("pro ok\n");
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "dios_ssp_api.h"
#include "dios_ssp_tde.h"

const int array_frm_len = 128;
const int tde_chunk = 256;

int main(int argc, char** argv)
{
    int input_idx = -1;
    int ref_idx = -1;
    int output_idx = -1;
    int ii, jj;
    for(ii = 0; ii < argc; ii++)
    {
        if(strcmp(argv[ii], "-i") == 0)
        {
            input_idx = ii;
        }
        if(strcmp(argv[ii], "-r") == 0)
        {
            ref_idx = ii;
        }
        if(strcmp(argv[ii], "-o") == 0)
        {
            output_idx = ii;
        }
    }
    if((input_idx == -1) || (output_idx == -1) || (ref_idx == -1))
    {
        printf("usage: %s -i input1.pcm [input2.pcm] [-r ref.pcm] -o output.pcm\r\n", argv[0]);
        return -1;
    }
    int channel_num;
    if(ref_idx == -1)
    {
        channel_num = output_idx - input_idx - 1;
    }
    else
    {
        channel_num = ref_idx - input_idx - 1;
    }
    printf("channel_num %d\n", channel_num);
    
    FILE **fpin = (FILE**)calloc(channel_num, sizeof(FILE*));;
    FILE *fpref = NULL;
    FILE *fpout = NULL;
    short *ptr_input_data = (short*)calloc(channel_num * array_frm_len, sizeof(short));
    short *ptr_output_data = (short*)calloc(channel_num * array_frm_len, sizeof(short));
    short *ptr_temp = (short*)calloc(array_frm_len, sizeof(short));
    short *ptr_ref_data = (short*)calloc(array_frm_len, sizeof(short));
   
    fpin[0] = fopen(argv[input_idx + 1], "rb");
    fseek(fpin[0], 0, SEEK_END);
    long file_len = ftell(fpin[0]);
    file_len /= 2;
    rewind(fpin[0]);
    fclose(fpin[0]);
    long frame_num = (file_len - array_frm_len * tde_chunk) / array_frm_len;
    int sample_res = (file_len - array_frm_len * tde_chunk) % array_frm_len;
    printf("file_len:%d framenum: %ld and %d samples.\r\n", file_len, frame_num, sample_res);
    // fpref = fopen(argv[2], "rb");
    // fpout = fopen(argv[3], "wb");
    for(ii = 0; ii < channel_num; ii ++) {
        fpin[ii] = fopen(argv[input_idx + 1 + ii], "rb");
        if(fpin[ii] == NULL) {
            printf("can not find file\n");
            exit(0);
        }
        printf("Open input file %s success.\r\n", argv[input_idx + 1 + ii]);

    }
    fpout = fopen(argv[output_idx + 1], "wb");
    if(fpout == NULL) {
        printf("can not find out file\n");
        exit(0);
    }
    fpref = fopen(argv[ref_idx + 1], "rb");
    if(fpref == NULL) {
        printf("can not open ref file\n");
        exit(0);
    }


    // N = 128 也就是128帧，每帧128个采样点 
    // TDE Begin
    int delay = 0;
    short* ptr_input_for_tde = (short*)calloc(array_frm_len * tde_chunk, sizeof(short));
    short* ptr_ref_for_tde = (short*)calloc(array_frm_len * tde_chunk, sizeof(short));
    fread(ptr_input_for_tde, sizeof(short), array_frm_len * tde_chunk, fpin[0]);
    fread(ptr_ref_for_tde, sizeof(short), array_frm_len * tde_chunk, fpref);
    delay = dios_ssp_tde(ptr_input_for_tde, ptr_ref_for_tde, array_frm_len * tde_chunk);
    fread(ptr_input_for_tde, sizeof(short), array_frm_len * tde_chunk, fpin[1]);  // 仅用于同步

    // TDE End
    printf("time delay is %d\n", delay);
    // AEC Begin
    void* st;
    st = dios_ssp_init_api(delay);
    dios_ssp_reset_api(st);
    for(ii = 0; ii < frame_num; ii++)
    {
        for(jj = 0; jj < channel_num; jj ++) {
            // prepare input signal
            fread(ptr_temp, sizeof(short), array_frm_len, fpin[jj]);
            memcpy(&ptr_input_data[jj * array_frm_len], ptr_temp, sizeof(short) * array_frm_len);
        }
        // prepare ref signal
        fread(ptr_temp, sizeof(short), array_frm_len, fpref);
        memcpy(ptr_ref_data, ptr_temp, sizeof(short) * array_frm_len);
        // AEC
        dios_ssp_process_api(st, ptr_input_data, ptr_ref_data, ptr_output_data);
        // save output file
        fwrite(ptr_output_data, sizeof(short), array_frm_len * 2, fpout);
    }
    fwrite(ptr_output_data, sizeof(short), sample_res, fpout);
    dios_ssp_uninit_api(st);
    // AEC End
    for(ii = 0; ii < channel_num; ii++)
    {
        fclose(fpin[ii]);
    }
    free(fpin);
    fclose(fpref);
    fclose(fpout);
    free(ptr_temp);
    free(ptr_input_data);
    free(ptr_output_data);
    free(ptr_ref_data);
    free(ptr_input_for_tde);
    free(ptr_ref_for_tde);
    printf("process finished.\r\n");
    return 0;
}

