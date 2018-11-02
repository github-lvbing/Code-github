//
//  CoBrain.h
//  STM_CoBrain
//
//  Created by 曾科 on 19/01/2018.
//  Copyright © 2018 Ke Zeng. All rights reserved.
//

#ifndef CoBrain_h
#define CoBrain_h

#include "config_cool.h"

typedef struct
{

    /* filter */
    double xs[NORDER],ys[NORDER];
    double xb[NORDER],yb[NORDER];
    double xd[NORDER],yd[NORDER];
    double xt[NORDER],yt[NORDER];
    double xa[NORDER],ya[NORDER];
    double xg[NORDER],yg[NORDER];
    
    /* power */
    float eeg_power[4];
    int count[4];
    char flag[4];
    
}CoBrain;


/* Initialize */
void InitCoBrain(CoBrain *cbrain);

/* Update */
void LoadRawEEG(CoBrain *cbrain, float rawdata);
void LoadRawEEGByte(CoBrain *cbrain, unsigned char *byte, int n);

/* Brain state */
float GetAttentionValue(CoBrain *cbrain);
float GetMediationValue(CoBrain *cbrain);
float GetBlinkValue(CoBrain *cbrain);
float GetEMGValue(CoBrain *cbrain);

double decode_HML(unsigned char *byte, int n);
float CaculateScale(float rawScore, float maxScale, float minScale);


#endif /* CoBrain_h */
