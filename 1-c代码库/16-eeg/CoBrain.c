//
//  CoBrain.c
//  STM_CoBrain
//
//  Created by 曾科 on 20/01/2018.
//  Copyright © 2018 Ke Zeng. All rights reserved.
//

#include <string.h>
#include <math.h>

#include "CoBrain.h"
#include "signal_process.h"


void InitCoBrain(CoBrain *cbrain)
{

    /* filter */
    memset(cbrain->xs, 0, NORDER*sizeof(double));
    memset(cbrain->ys, 0, NORDER*sizeof(double));
    
    memset(cbrain->xb, 0, NORDER*sizeof(double));
    memset(cbrain->yb, 0, NORDER*sizeof(double));
    
    memset(cbrain->xd, 0, NORDER*sizeof(double));
    memset(cbrain->yd, 0, NORDER*sizeof(double));
    
    memset(cbrain->xt, 0, NORDER*sizeof(double));
    memset(cbrain->yt, 0, NORDER*sizeof(double));
    
    memset(cbrain->xa, 0, NORDER*sizeof(double));
    memset(cbrain->ya, 0, NORDER*sizeof(double));
    
    memset(cbrain->xg, 0, NORDER*sizeof(double));
    memset(cbrain->yg, 0, NORDER*sizeof(double));
    
    /* power */
    memset(cbrain->eeg_power, 0, 4*sizeof(float));
    memset(cbrain->count, 0, 4*sizeof(int));
    memset(cbrain->flag, 0, 4*sizeof(char));

}

void LoadRawEEG(CoBrain *cbrain, float rawdata)
{
    double delta_value, theta_value, alpha_value, gamma_value;
    
    /* filter */
    delta_value = filter_delta(cbrain->xd, cbrain->yd, NORDER, rawdata);
    cbrain->eeg_power[DELTA] += delta_value * delta_value;
    ++ cbrain->count[DELTA];
    if (delta_value > ABNORMAL_VALUE | delta_value < -ABNORMAL_VALUE)
    {
        cbrain->flag[DELTA] = 1;
    }
    
    theta_value = filter_theta(cbrain->xt, cbrain->yt, NORDER, rawdata);
    cbrain->eeg_power[THETA] += theta_value * theta_value;
    ++ cbrain->count[THETA];
    if (theta_value > 200 | theta_value < -200)
    {
        cbrain->flag[THETA] = 1;
    }
    
    alpha_value = filter_alpha(cbrain->xa, cbrain->ya, NORDER, rawdata);
    cbrain->eeg_power[ALPHA] += alpha_value * alpha_value;
    ++ cbrain->count[ALPHA];
    if (alpha_value > 200 | alpha_value < -200)
    {
        cbrain->flag[ALPHA] = 1;
    }

    
    gamma_value = filter_gamma(cbrain->xg, cbrain->yg, NORDER, rawdata);
    cbrain->eeg_power[GAMMA] += gamma_value * gamma_value;
    ++ cbrain->count[GAMMA];
    if (gamma_value > 300 | gamma_value < -300)
    {
        cbrain->flag[GAMMA] = 1;
    }

}

void LoadRawEEGByte(CoBrain *cbrain, unsigned char *byte, int n)
{
    double rawdata;
    
    /* decode */
    rawdata = decode_HML(byte, n);
    
    /* load data */
    LoadRawEEG(cbrain, (float)rawdata);

}


float GetAttentionValue(CoBrain *cbrain)
{
    float rawScore, scaledScore;
    
    if (cbrain->count[THETA] != 0)
    {
        rawScore = -1 * log(cbrain->eeg_power[THETA]/cbrain->count[THETA]);
        scaledScore = CaculateScale(rawScore, MAX_ATTENTION, MIN_ATTENTION);
    }
    else
    {
        scaledScore = 0;
    }

    cbrain->eeg_power[THETA] = 0;
    cbrain->count[THETA] = 0;
    
    return scaledScore;
}

double decode_HML(unsigned char *byte, int n)
{
    int i;
    int rawdata;
    double vm, sig;
    
    /* xxHigh xxMid xxLow */
    rawdata = 0;
    for (i = 0; i < n; ++i)
    {
      rawdata |= byte[i] << (8*(n-1-i));
      // 00 HH MM  LL 
    }
    
    /* determine the sign of raw data */
    if (rawdata > 8388608)      // 2^23
    {
        rawdata -= 16777216;    // 2^24
    }
    else if (rawdata == 8388608)
    {
        rawdata = -8388608;
    }
    
    /* Conversion */
   // vm = 2.4/(8388607); // resolution
     vm = 2.42/(8388607); // resolution
    sig = rawdata*vm/8*1000000;
    
    return sig;
}

float GetMediationValue(CoBrain *cbrain)
{
    float rawScore, scaledScore;
    
    if (cbrain->flag[ALPHA] | !cbrain->count[ALPHA])
    {
        scaledScore = 0;
    }
    else
    {
        rawScore = log(cbrain->eeg_power[ALPHA]/cbrain->count[ALPHA]);
        scaledScore = CaculateScale(rawScore, MAX_MEDIATION, MIN_MEDIATION);
    }
    
    cbrain->eeg_power[ALPHA] = 0;
    cbrain->count[ALPHA] = 0;
    cbrain->flag[ALPHA] = 0;
    
    return scaledScore;
}

float GetBlinkValue(CoBrain *cbrain)
{
    float rawScore, scaledScore;
    
    if (cbrain->flag[DELTA] | !cbrain->count[DELTA])
    {
        scaledScore = 0;
    }
    else
    {
        rawScore = log(cbrain->eeg_power[DELTA]/cbrain->count[DELTA]);
        scaledScore = CaculateScale(rawScore, MAX_BLINK, MIN_BLINK);
    }
    
    cbrain->eeg_power[DELTA] = 0;
    cbrain->count[DELTA] = 0;
    cbrain->flag[DELTA] = 0;
    
    return scaledScore;
    
}

float GetEMGValue(CoBrain *cbrain)
{
    float rawScore, scaledScore;
    
    if (cbrain->flag[GAMMA] | !cbrain->count[GAMMA])
    {
        scaledScore = 0;
    }
    else
    {
        rawScore = log(cbrain->eeg_power[GAMMA]/cbrain->count[GAMMA]);
        scaledScore = CaculateScale(rawScore, MAX_EMG, MIN_EMG);
    }
    
    cbrain->eeg_power[GAMMA] = 0;
    cbrain->count[GAMMA] = 0;
    cbrain->flag[GAMMA] = 0;
    
    return scaledScore;

}

float CaculateScale(float rawScore, float maxScale, float minScale)
{
    float scaledScore = 0;
    
    if (rawScore < minScale)
    {
        return 0;
    }
    else if (rawScore > maxScale)
    {
        return 1;
    }
    else
    {
        scaledScore = (rawScore - minScale) / (maxScale - minScale);
        return scaledScore;
    }
    
}
