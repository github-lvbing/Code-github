

#ifndef _VG_H_
#define _VG_H_
//*********************************VG.h********************************
typedef struct VGDataPackage{
	unsigned char 	ucHead;
	unsigned char 	ucFlag;
	unsigned short  usTime;
	short			stRoll;
	short			stPitch;
	short		    stGyroX;
	short			stGyroY;
	short			stGyroZ;
	short			stAccX;
	short			stAccY;
	short			stAccZ;
	unsigned short	usTemp;
	unsigned char 	ucParity;
};


void GetRollPitchYaw(double *dbRoll,double *dbPitch,double *dbYaw,double dbGyro_Acc[6]);
void VGPackageData(unsigned char ucFlag, unsigned short Timer,double dbArray[10],unsigned char N);
void VGPkgCharRds(unsigned char *p,unsigned char N);
#endif
