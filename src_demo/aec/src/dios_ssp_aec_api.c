/***************************************************************************
  * 
  * Homework for chapter 5 -- Acoustic Echo Cancellation
  *
  * Here is the realization of aec api. 
  *
  **************************************************************************/

#include "dios_ssp_aec_api.h"

typedef struct {
	// module structure definition
	objSubBand** st_subband_mic;
	objSubBand** st_subband_ref;
	objFirFilter** st_firfilter;

	float** input_mic_time;
	float** input_ref_time;
	xcomplex** input_mic_subband;
	xcomplex** input_ref_subband;
	xcomplex** firfilter_out;
	xcomplex** final_out;
	xcomplex** est_echo;
	int** band_table;
	float freq_div_table[5];
	int mic_num;
	int ref_num;
	int frm_len;
	float *mic_tde;
	float *ref_tde;
	int ref_buffer_len;
	float *ref_buffer;
	int* doubletalk_result;
} objAEC;

/**
 * @brief aec init
 *
 * @param mic_num    number of input signal
 * @param ref_num    number of reference signal
 * @param frm_len    framelength
 * @return           AEC handle
 */
void* dios_ssp_aec_init_api(int mic_num, int ref_num, int frm_len, int delay)
{
	int i;
	int i_mic;
	int i_ref;
	int ret = 0;
	void* ptr = NULL;

	if (mic_num <= 0 || ref_num <= 0 || frm_len != 128)
	{
		return NULL;
	}

    if(delay < 0 || delay > 16000)
    {
        printf("Invalid delay(%d).\n", delay);
        return NULL;
    }

	ptr = (void*)calloc(1, sizeof(objAEC));
	objAEC* srv = (objAEC*)ptr;
	
	srv->mic_num = mic_num;
	srv->ref_num = ref_num;
	srv->frm_len = frm_len;

	srv->ref_buffer_len = delay;

	srv->mic_tde = (float*)calloc(srv->mic_num * srv->frm_len, sizeof(float));
	srv->doubletalk_result = (int *)calloc(srv->mic_num, sizeof(int));
	srv->input_mic_time = (float**)calloc(srv->mic_num, sizeof(float*));
	srv->input_mic_subband = (xcomplex**)calloc(srv->mic_num, sizeof(xcomplex*));
	srv->firfilter_out = (xcomplex**)calloc(srv->mic_num, sizeof(xcomplex*));
	srv->final_out = (xcomplex**)calloc(srv->mic_num, sizeof(xcomplex*));
	srv->est_echo = (xcomplex**)calloc(srv->mic_num, sizeof(xcomplex*));
	srv->st_subband_mic = (objSubBand**)calloc(srv->mic_num, sizeof(objSubBand*));
	srv->st_firfilter = (objFirFilter**)calloc(srv->mic_num, sizeof(objFirFilter*));

	for (i_mic = 0; i_mic < srv->mic_num; i_mic++)
	{
		// 存储mic时域输入信号
		srv->input_mic_time[i_mic] = (float*)calloc(srv->frm_len, sizeof(float));
		// 子带信号
		srv->input_mic_subband[i_mic] = (xcomplex*)calloc(AEC_SUBBAND_NUM, sizeof(xcomplex));
		srv->firfilter_out[i_mic] = (xcomplex*)calloc(AEC_SUBBAND_NUM, sizeof(xcomplex));
		srv->final_out[i_mic] = (xcomplex*)calloc(AEC_SUBBAND_NUM, sizeof(xcomplex));
		srv->est_echo[i_mic] = (xcomplex*)calloc(AEC_SUBBAND_NUM, sizeof(xcomplex));
		
		/* sub module init */
		// 子带模块和fir模块针对每个Mic都有一个结构体
		srv->st_subband_mic[i_mic] = dios_ssp_share_subband_init(srv->frm_len);
		srv->st_firfilter[i_mic] = dios_ssp_aec_firfilter_init(srv->ref_num);

		/* module share memory and varvariable */
		srv->st_firfilter[i_mic]->sig_mic_rec = srv->input_mic_subband[i_mic];
		srv->st_firfilter[i_mic]->sig_spk_ref = srv->input_ref_subband;
		srv->st_firfilter[i_mic]->band_table = srv->band_table;
	}

	// 给参考信号加上了时延估计，使用一个新buffer
	srv->ref_buffer = (float*)calloc(srv->ref_num * (srv->ref_buffer_len + srv->frm_len), sizeof(float));
	srv->ref_tde = (float*)calloc(srv->ref_num * srv->frm_len, sizeof(float));
	srv->input_ref_subband = (xcomplex**)calloc(srv->ref_num, sizeof(xcomplex*));
	srv->input_ref_time = (float**)calloc(srv->ref_num, sizeof(float*));
	// 与st_subband_mic对应
	srv->st_subband_ref = (objSubBand**)calloc(srv->ref_num, sizeof(objSubBand*));
	for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
	{
		// 与 input_mic_time 和 input_mic_subband对应
		srv->input_ref_time[i_ref] = (float*)calloc(srv->frm_len, sizeof(float));
		srv->input_ref_subband[i_ref] = (xcomplex*)calloc(AEC_SUBBAND_NUM, sizeof(xcomplex));
	}
	for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
	{
		srv->st_subband_ref[i_ref] = dios_ssp_share_subband_init(srv->frm_len);
	}
	/**
	 * band_table是AEC模块中用于存储子带频率对应的通道号的二维数组，
	 * 其作用是将输入信号分成若干个子带，然后根据每个子带的频率范围进行相应的处理。
	 * 可以将整个频率范围分成多个子带，这些子带中的频率范围可以根据实际需要来确定，比如可以根据人耳听觉特性等因素来分带。
	 * 在代码中，band_table中的每一行存储一个子带的起始通道号和结束通道号，这些通道号对应着FFT变换后的幅度谱中的频率位置。
	 * freq_div_table是用于确定子带频率范围的表格，它是一个1维数组，存储了各个子带的频率上限。
	 * 在AEC中，根据采样率和FFT长度，可以确定每个通道的频率范围，即频率分辨率。
	 * 根据这个频率分辨率和freq_div_table中的值，可以轻松地确定子带的频率范围，
	 * 即第i个子带的频率范围为freq_div_table[i-1]~freq_div_table[i]，其中freq_div_table[0]为0，freq_div_table[N]为采样率的一半。
	 * 在代码中，根据freq_div_table和FFT长度计算出每个子带的起始通道号和结束通道号，然后存储在band_table中。
	 * 其中如果子带分解子带数设为128，那么最终band_table最后一个元素也就是128
	*/

	srv->band_table = (int**)calloc(ERL_BAND_NUM, sizeof(int*));
	for (i = 0; i < ERL_BAND_NUM; i++)
	{
		srv->band_table[i] = (int *)calloc(2, sizeof(int));
	}
	
	srv->freq_div_table[0] = 0;
	srv->freq_div_table[1] = 600;
	srv->freq_div_table[2] = 1200;
	srv->freq_div_table[3] = 3000;
	srv->freq_div_table[4] = 8000;

	srv->band_table[0][0] = AEC_LOW_CHAN;
	for (i = 1; i < ERL_BAND_NUM; i++)
	{
		srv->band_table[i][0] = (int)(srv->freq_div_table[i] / AEC_SAMPLE_RATE * AEC_FFT_LEN);
		srv->band_table[i - 1][1] = srv->band_table[i][0] - 1;
	}
	srv->band_table[ERL_BAND_NUM - 1][1] = AEC_HIGH_CHAN - 1;

	ret = dios_ssp_aec_reset_api(srv);
	if (0 != ret)
	{
		return NULL;
	}

	return (ptr);
}

/**
 * @brief aec reset
 *
 * @param ptr    AEC handle
 * @return 
 *     @retval 0    successfully
 */
int dios_ssp_aec_reset_api(void* ptr)
{
	int ret = 0;
	int i_mic;
	int i_ref;
	objAEC* srv = (objAEC*)ptr;
	
	if (NULL == ptr)
	{
		return ERR_AEC;
	}

	memset(srv->ref_buffer, 0, srv->ref_num * (srv->ref_buffer_len + srv->frm_len) * sizeof(float));

	if (0 != ret)
	{
		return ERR_AEC;
	}

	for (i_mic = 0; i_mic < srv->mic_num; i_mic++)
	{
		srv->doubletalk_result[i_mic] = SINGLE_TALK_STATUS;

		/* subband module reset */
		ret = dios_ssp_share_subband_reset(srv->st_subband_mic[i_mic]);
		if (0 != ret)
		{
			return ERR_AEC;
		}
		ret = dios_ssp_aec_firfilter_reset(srv->st_firfilter[i_mic]);
		if (0 != ret)
		{
			return ERR_AEC;
		}
	}

	for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
	{
		ret = dios_ssp_share_subband_reset(srv->st_subband_ref[i_ref]);
		if (0 != ret)
		{
			return ERR_AEC;
		}
	}
	
	return 0;
}

/**
 * @brief aec process
 *
 * @param ptr       AEC handle
 * @param io_buf    input and output data buffer
 * @param ref_buf   reference data buffer
 * @param dt_st     double talk status
 * @return 
 *     @retval 0    successfully
 */
int dios_ssp_aec_process_api(void* ptr, float* io_buf, float* ref_buf, int* dt_st)
{
	objAEC* srv = (objAEC*)ptr;
	int ret_process = 0;
	int i_mic;
	int i_ref;
	int ch;
    int i;

    if (NULL == srv)
    {
		return ERR_AEC;
    }
	memcpy(srv->mic_tde, io_buf, srv->mic_num * srv->frm_len * sizeof(float));
	memcpy(srv->ref_tde, ref_buf, srv->ref_num * srv->frm_len * sizeof(float));

	/* fixed delay process */
	// 当前帧的数据加入到ref_buffer末尾，进行时延对齐
	memcpy(srv->ref_buffer + srv->ref_num * srv->ref_buffer_len, srv->ref_tde, srv->ref_num * srv->frm_len * sizeof(float));
	// 从ref_buffer里复制时延对齐之后的数据到ref_tde中
	memcpy(srv->ref_tde, srv->ref_buffer, srv->ref_num * srv->frm_len * sizeof(float));
	// 去掉被复制走的数据
	memmove(srv->ref_buffer, srv->ref_buffer + srv->ref_num * srv->frm_len, srv->ref_num * srv->ref_buffer_len * sizeof(float));
	memset(srv->ref_buffer + srv->ref_num * srv->ref_buffer_len, 0, srv->ref_num * srv->frm_len * sizeof(float));
	
	/* get mic data */
	for (i_mic = 0; i_mic < srv->mic_num; i_mic++)
    {
		for (i = 0; i < srv->frm_len; i++)
		{
			srv->input_mic_time[i_mic][i] = (float)srv->mic_tde[i_mic * srv->frm_len + i];
		}
    }
	/* get ref data */
	for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
	{
		/* get ref data */
		for (i = 0; i < srv->frm_len; i++)
		{
			srv->input_ref_time[i_ref][i] = (float)srv->ref_tde[i_ref * srv->frm_len + i];
		}
	}

    /* reference subband analyse */
    for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
    {
		ret_process = dios_ssp_share_subband_analyse(srv->st_subband_ref[i_ref], srv->input_ref_time[i_ref], srv->input_ref_subband[i_ref]);
		if (0 != ret_process) 
		{
			return ERR_AEC;
		}		
    }
		
    for (i_mic = 0; i_mic < srv->mic_num; i_mic++)
    {
		/* mic subband analyse */
		ret_process = dios_ssp_share_subband_analyse(srv->st_subband_mic[i_mic], srv->input_mic_time[i_mic], srv->input_mic_subband[i_mic]);
		if (0 != ret_process) 
		{
			return ERR_AEC;
		}

		/* fir filter process */
		srv->st_firfilter[i_mic]->dt_status = &srv->doubletalk_result[i_mic];
		srv->st_firfilter[i_mic]->sig_mic_rec = srv->input_mic_subband[i_mic];
		srv->st_firfilter[i_mic]->sig_spk_ref = srv->input_ref_subband;
		srv->st_firfilter[i_mic]->band_table = srv->band_table;
		ret_process = dios_ssp_aec_firfilter_process(srv->st_firfilter[i_mic], srv->firfilter_out[i_mic], srv->est_echo[i_mic]);
		if (0 != ret_process) 
		{
			return ERR_AEC;
		}

		memcpy(srv->final_out[i_mic], srv->firfilter_out[i_mic], sizeof(xcomplex) * AEC_SUBBAND_NUM);
		
		/* subband compose */
		ret_process = dios_ssp_share_subband_compose(srv->st_subband_mic[i_mic], srv->final_out[i_mic], &io_buf[i_mic * srv->frm_len]);
	}
    dt_st[0] = srv->doubletalk_result[0];
    return 0;
}

/**
 * @brief aec uninit
 *
 * @param ptr    AEC handle
 * @return 
 *     @retval 0    successfully
 */
int dios_ssp_aec_uninit_api(void* ptr)
{
	int i;
	int i_mic;
	int i_ref;
	int ret = 0;
	objAEC* srv = (objAEC*)ptr;

	if (NULL == ptr)
	{
		return ERR_AEC;
	}

	for (i_mic = 0; i_mic < srv->mic_num; i_mic++)
	{
		free(srv->input_mic_time[i_mic]);
		free(srv->input_mic_subband[i_mic]);
		free(srv->firfilter_out[i_mic]);
		free(srv->final_out[i_mic]);
		free(srv->est_echo[i_mic]);		
	}
	free(srv->mic_tde);
	free(srv->doubletalk_result);
	free(srv->input_mic_time);
	free(srv->input_mic_subband);
	free(srv->firfilter_out);
	free(srv->final_out);
	free(srv->est_echo);
	
	for (i = 0; i < ERL_BAND_NUM; i++)
	{
		free(srv->band_table[i]);
	}
	free(srv->band_table);
	for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
	{
		free(srv->input_ref_time[i_ref]);
		free(srv->input_ref_subband[i_ref]);
	}
	free(srv->ref_buffer);
	free(srv->ref_tde);
	free(srv->input_ref_time);
	free(srv->input_ref_subband);

	for (i_ref = 0; i_ref < srv->ref_num; i_ref++)
	{
		ret = dios_ssp_share_subband_uninit(srv->st_subband_ref[i_ref]);
		if (0 != ret)
		{
			return ERR_AEC;
		}
	}
	free(srv->st_subband_ref);

	for (i_mic = 0; i_mic < srv->mic_num; i_mic++)
	{
		ret = dios_ssp_share_subband_uninit(srv->st_subband_mic[i_mic]);
		if (0 != ret)
		{
			return ERR_AEC;
		}
		ret = dios_ssp_aec_firfilter_uninit(srv->st_firfilter[i_mic]);
		if (0 != ret)
		{
			return ERR_AEC;
		}
	}
	free(srv->st_subband_mic);
	free(srv->st_firfilter);
	free(srv);

	return 0;
}

