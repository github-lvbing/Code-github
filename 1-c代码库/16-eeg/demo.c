//
//  demo.c
//  STM_CoBrain
//
//  Created by 曾科 on 20/01/2018.
//  Copyright © 2018 Ke Zeng. All rights reserved.
//

#include "CoBrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME 50

void test_stm();
void test_stm2();
void translate();
int translat(char c);
int htoi(char *str, int len);
void test_decode();



int main()
{
    test_stm();
//    test_decode();
//    test_stm2();
//    int rawdata;
//    char str[] = {'f','b','9','0','6','0'};
//    rawdata = htoi(str,6);
//    printf("rawdata = %d\n", rawdata);
    
    return 0;
}

void test_decode()
{
    unsigned char byte[3] = {251, 144,96};
    double rawdata = 0;
    
    rawdata = decode_HML(byte, 3);
    
    printf("rawdata = %f\n", rawdata);
}

int translat(char c)
{
    if(c<='9'&&c>='0') return c-'0';
    if(c>='a' && c<='f') return c-87;
    if(c>='A' && c<='F') return c-55;
    return -1;//其它字符返回-1
}

int htoi(char *str, int len)
{
    int number, sum;
    int i;
    
    if(len == 0) return 0;
    
    sum = 0;
    for(i = 0; i < len; ++i)
    {
        number = translat(str[i]);//防错处理
        if(number >= 0) sum = sum*16 + number;
    }
    return sum;
}

void test_stm2()
{
    FILE *fp;
    char byte16[7];
    unsigned char byte10[3];
    int i, j, count;
    float p[20], att;
    int rawdata[FS*20];
    
    CoBrain cbrain;
    InitCoBrain(&cbrain);

    fp = fopen("byte.txt","r");
    if ( !fp )
    {
        printf("failed to open file!\n");
        return;
    }
    
    count = 0;
    for ( i = 0; i < FS*20; ++ i)
    {
        fgets(byte16, 7, fp);
        for (j = 0; j < 3; ++ j)
        {
            byte10[j] = htoi(byte16+j*2,2);
        }
        
//        rawdata[i] = decode_HML(byte10, 3);
        
        LoadRawEEGByte(&cbrain, byte10, 3);

        if((i+1)%FS == 0)
        {
            p[count] = cbrain.eeg_power[THETA];
            ++ count;
            att = GetAttentionValue(&cbrain);
        }
    }
    
    fclose(fp);
}


void test_stm()
{
    FILE *fp;
    float sig[FS*TIME];
    int i, count;
    float p[TIME], att[TIME], med[TIME], blink[TIME], emg[TIME];
    
    fp = fopen("dyb1222.txt","r");
    if ( !fp )
    {
        printf("failed to open file!\n");
        return;
    }
    
    for ( i = 0; i < FS*TIME; ++i)
    {
        fscanf(fp,"%f",sig+i);
    }
    
    CoBrain cbrain;
    InitCoBrain(&cbrain);
    
    count = 0;
    for ( i = 0; i < FS*TIME; ++i)
    {
        LoadRawEEG(&cbrain, sig[i]);
        
        if((i+1)%FS == 0)
        {
            p[count] = -log(cbrain.eeg_power[THETA]/cbrain.count[THETA]);
            att[count] = GetAttentionValue(&cbrain);
            med[count] = GetMediationValue(&cbrain);
            blink[count] = GetBlinkValue(&cbrain);
            emg[count] = GetEMGValue(&cbrain);
            
            ++ count;

        }
    }
    
    fclose(fp);
}
