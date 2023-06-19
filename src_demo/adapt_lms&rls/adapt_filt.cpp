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

double p[filter_length][filter_length];
double k[filter_length];
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
// 该函数的实现中，输出y(n)和误差e(n)均为1维。且期望输出即为输入的白噪声。
int adapt_filtering_lms(short input, double* adapt_filter, int filter_length, short* err) {
    double filter_output = 0.0;
    double error = 0.0;
    double mu = 0.2;

    // 完成翻转 移位，同时进行归一化
    for(int i = filter_length - 1; i > 0; i--) {
        inputdata[i] = inputdata[i - 1];
    }
    inputdata[0] = double(input / 32768.0);
    
    // e(n) = d(n) - y(n)  其中d(n) = x(n)  y(n) = x(n) * w(n) 线性卷积
    // 此处使用的是w(n - 1),实际计算的时候使用先验估计和后验估计一般没有什么区别
    for (int i = 0; i < filter_length; i ++) {
        filter_output += adapt_filter[i] * inputdata[i]; // if time_index=2, w0*x2 + w1*x1 + w2*x0
    }

    error = inputdata[0] - filter_output;

    // w(n + 1) = w(n) + 2ux(n)e(n)
    for (int i = 0; i < filter_length; i ++) {
        adapt_filter[i] = adapt_filter[i] + 2 * mu * inputdata[i] * error;
    }

        // 2. update filter: adapt_filter, for future iterations

        // end adaptive filtering algorithm

        // output error
        err[0] = short(error * 32768.0);

    return 0;
}


int adapt_filtering_rls(short input, double* adapt_filter, int filter_length, short* err) {
    double filter_output = 0.0;
    double prior_error = 0.0;
    double lamda = 0.99;

    // 完成翻转 移位，同时进行归一化，得到计算时所用的x(n)
    for(int i = filter_length - 1; i > 0; i--) {
        inputdata[i] = inputdata[i - 1];
    }
    inputdata[0] = double(input / 32768.0);

    // 先验误差 prior_error(n) = d(n) - w(n - 1) * x(n)  其中d(n) = x(n)
    // 此处计算与lms算法流程一样，但理论推导是不同的，误差含义也不同
    for (int i = 0; i < filter_length; i ++) {
        filter_output += adapt_filter[i] * inputdata[i]; // if time_index=2, w0*x2 + w1*x1 + w2*x0
    }

    prior_error = inputdata[0] - filter_output;

    // 计算增益向量k(n) = p(n - 1)x(x) / [lamda + x_T(n)p(n - 1)x(n)]
    double p_mul_x[filter_length];
    for (int i = 0; i < filter_length; i ++) {
        p_mul_x[i] = 0.0;
        for (int j = 0; j < filter_length; j ++) {
          p_mul_x[i] += p[i][j] * inputdata[j];
        }
    }
    double tmp = 0.0;
    for (int i = 0; i < filter_length; i ++) {
        tmp += inputdata[i] * p_mul_x[i];
    }
    for (int i = 0; i < filter_length; i ++) {
        k[i] = p_mul_x[i] / (lamda + tmp);
    }

    // 更新逆矩阵 p(n) = 1/lamda * [p(n-1) - k(n)x_T(n)p(n-1)]
    // 先从后侧开始计算这样避免了两个 n*n的矩阵的点乘，减少复杂度
    double tmp_matirx[filter_length][filter_length];
    for (int i = 0; i < filter_length; i ++) {
        for (int j = 0; j < filter_length; j ++) {
          tmp_matirx[i][j] = 0.0;
          for (int l = 0; l < filter_length; l ++) {
            tmp_matirx[i][j] += k[i] * p[l][j] * inputdata[l];
          }
        }
    }
    for (int i = 0; i < filter_length; i ++) {
        for (int j = 0; j < filter_length; j ++) {
          p[i][j] = (p[i][j] - tmp_matirx[i][j]) / lamda;
        }
    }

    // w(n) = w(n - 1) + k(n)e(n)
    for (int i = 0; i < filter_length; i++)
    {
      adapt_filter[i] = adapt_filter[i] + k[i] * prior_error;
    }

    // output error
    err[0] = short(prior_error * 32768.0);
    return 0;
}
