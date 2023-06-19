/***************************************************************************
  * 
  * Homework for chapter 5 -- Acoustic Echo Cancellation
  *
  * Here is the realization of Time Delay Estimation api. 
  *
  **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dios_ssp_tde.h"
#include "dios_ssp_share_rfft.h"

/**
 * @brief tde process
 *
 * You are going to finish this function of TDE, dios_ssp_tde().
 * Obviously the time delay is within [0, inputdata_length).
 * Since we don't know the ground truth delay, we usually need 
 * a longer buffer for TDE. In this lab we use about 1s buffer
 * for TDE. You can apply GCC method to estimate the time delay
 * between input signal and reference signal. 
 *
 * @param inputdata          input data buffer.
 * @param refdata            reference data buffer.
 * @param inputdata_length   input data length (samples), also the reference data length.
 * @return                   estimated time delay (samples) between input signal and reference signal.
 */
int dios_ssp_tde(short* inputdata, short* refdata, long int inputdata_length)
{
    int delay = 0;
    
    // please finish your code here...
    
    // end TDE
    float *inbuffer = (float *)calloc(inputdata_length, sizeof(float));
    float *inbuffer_out = (float *)calloc(inputdata_length, sizeof(float));
    float *refbuffer = (float *)calloc(inputdata_length, sizeof(float));
    float *refbuffer_out = (float *)calloc(inputdata_length, sizeof(float));
    
    for (int i = 0; i < inputdata_length; i ++) {
      inbuffer[i] = (float)inputdata[i];
      refbuffer[i] = (float)refdata[i];
    }
    void *rfft_handle = dios_ssp_share_rfft_init(inputdata_length);
    dios_ssp_share_rfft_process(rfft_handle, inbuffer, inbuffer_out);
    dios_ssp_share_rfft_process(rfft_handle, refbuffer, refbuffer_out);

    // 对每个buffer输出的数据进行拼接，得到复频域的序列。
    // rfft的输出长度是inputdata_length，但因为实部虚部，因此只包含了一半序列，需要根据共轭对称性质转换
    xcomplex *infreq = (xcomplex *)calloc(inputdata_length, sizeof(xcomplex));
    xcomplex *reffreq = (xcomplex *)calloc(inputdata_length, sizeof(xcomplex));
    infreq[0].r = inbuffer_out[0];
    infreq[0].i = 0;
    infreq[inputdata_length / 2].r = inbuffer_out[inputdata_length / 2];
    infreq[inputdata_length / 2].i = 0;
    // 注意这里的还原顺序、正负号，以及共轭对称的特性
    for (int i = 1; i < inputdata_length / 2; i ++) {
      infreq[i].r = inbuffer_out[i];
      infreq[i].i = - inbuffer_out[inputdata_length - i];
      infreq[inputdata_length - i].r = inbuffer_out[i];
      infreq[inputdata_length - i].i = inbuffer_out[inputdata_length - i];
    }
    reffreq[0].r = refbuffer_out[0];
    reffreq[0].i = 0;
    reffreq[inputdata_length / 2].r = refbuffer_out[inputdata_length / 2];
    reffreq[inputdata_length / 2].i = 0;
    // 注意这里的还原顺序、正负号，以及共轭对称的特性
    for (int i = 1; i < inputdata_length / 2; i ++) {
      reffreq[i].r = refbuffer_out[i];
      reffreq[i].i = - refbuffer_out[inputdata_length - i];
      reffreq[inputdata_length - i].r = refbuffer_out[i];
      reffreq[inputdata_length - i].i = refbuffer_out[inputdata_length - i];
    }

    // 取共轭，频域相乘，gcc-phat
    xcomplex *p_yx = (xcomplex *)calloc(inputdata_length, sizeof(xcomplex));
    xcomplex *phat_p_yx = (xcomplex *)calloc(inputdata_length, sizeof(xcomplex));

    for (int i = 0; i < inputdata_length; i ++) {
      p_yx[i] = complex_mul(infreq[i], complex_conjg(reffreq[i]));
      float i_module = complex_abs(p_yx[i]);
      phat_p_yx[i] = complex_div_real(p_yx[i], i_module);
    }
    // 拼接数据格式，进行irfft
    // rfft irfft的数据对应格式都是 0 a_r b_r c_r N/2  c_i b_i a_i形式
    // 还是说前面存前半部分的实部，后面存后半部分的虚部
    float *phat_in = (float *)calloc(inputdata_length, sizeof(xcomplex));
    float *phat_out = (float *)calloc(inputdata_length, sizeof(xcomplex));
    phat_in[0] = phat_p_yx[0].r;
    phat_in[inputdata_length / 2] = phat_p_yx[inputdata_length / 2].r;
    for (int i = 0; i < inputdata_length / 2; i ++) {
      phat_in[i] = phat_p_yx[i].r;
    }
    for (int i = inputdata_length / 2 + 1; i < inputdata_length; i ++) {
      phat_in[i] = phat_p_yx[i].i;
    }
    dios_ssp_share_irfft_process(rfft_handle, phat_in, phat_out);
    // 寻找互相关函数最值，其索引即为时延估计结果
    float r_yx_max = -1.0;
    for (int i = 0; i < inputdata_length; i ++) {
      if(phat_out[i] > r_yx_max) {
        r_yx_max = phat_out[i];
        delay = i;
      }
    }

    free(inbuffer);
    free(inbuffer_out);

    free(refbuffer);

    free(refbuffer_out);

    free(infreq);

    free(reffreq);

    free(p_yx);

    free(phat_p_yx);

    free(phat_in);
    free(phat_out);
    dios_ssp_share_rfft_uninit(rfft_handle);

    return delay;
}

