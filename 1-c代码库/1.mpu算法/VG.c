

//********************VG.c**********************************
#include <math.h>
#include "VG.h"
#include "CYMath.h"

static xdata const double PI = 3.1415927;
static xdata const double G0 = 9.8015;

static xdata double AttMatrix[3][3];
static xdata double Quaternion[4];
xdata struct VGDataPackage VGDataPkg;
extern float fGyroRange;
extern float fAccRange;
xdata double dbAGRollP = 0.01,dbAGPitchP = 0.0001,dbAGR = 0.01,dbAGQ = 0.005;
xdata double dbAGRollP1 = 1,dbAGPitchP1 = 1,dbAGRollP2 = 1,dbAGPitchP2 = 1;
double xdata RollAcc,PitchAcc,RollGyro,PitchGyro;
//computer attitude matrix useing rool pitch and yaw( ���˶ʿ����P10 )
void CalAttMat_RPY(double roll,double pitch,double yaw,double AttMatrix[3][3])
{
	AttMatrix[0][0] = cos(roll) * cos(yaw) - sin(roll) * sin(yaw) * sin(pitch);
	AttMatrix[0][1] = -sin(yaw) * cos(pitch);
	AttMatrix[0][2] = cos(yaw) * sin(roll) + sin(yaw) * sin(pitch) * cos(roll);
	AttMatrix[1][0] = cos(roll) * sin(yaw) + sin(roll) * cos(yaw) * sin(pitch);
	AttMatrix[1][1] = cos(yaw) * cos(pitch);
	AttMatrix[1][2] = sin(yaw) * sin(roll) - cos(yaw) * sin(pitch) * cos(roll);
	AttMatrix[2][0] = -cos(pitch) * sin(roll);
	AttMatrix[2][1] = sin(pitch);
	AttMatrix[2][2] = cos(pitch) * cos(roll);
}
//����Ԫ��������̬����
void CalAttMat_Q(double AttMatrix[3][3],double q[4])
{

    AttMatrix[0][0] = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
    AttMatrix[0][1] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
    AttMatrix[0][2] = 2.0 * (q[1] * q[3] + q[0] * q[2]);

    AttMatrix[1][0] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
    AttMatrix[1][1] = q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];
    AttMatrix[1][2] = 2.0 * (q[2]	* q[3] - q[0] * q[1]);

    AttMatrix[2][0] = 2.0 * (q[1] * q[3] - q[0] * q[2]);
    AttMatrix[2][1] = 2.0 * (q[0] * q[1] + q[2]	* q[3]);
    AttMatrix[2][2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
	
}
//��Ԫ��������
void CrossQ(double q[4])
{
	double xdata norm;
    norm = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    q[0] = q[0] / norm;
    q[1] = q[1] / norm;
    q[2] = q[2] / norm;
    q[3] = q[3] / norm;
    
}
/*
������ת�����ʸ�����Ԫ����WirlX��WirlY��WirlZ�ֱ��ʾ��X��Y��Z����ת�����ʣ���λ
�ǻ���/�롣Timer��ʾʱ��������λ��
*/

void UpdateQ(double q[4],double WirlX,double WirlY,double WirlZ,double Timer)
{
//�ο����Ե���������Ԫ��316ҳ
	xdata double Q0,Q1,Q2,Q3;
	xdata double DertX,DertY,DertZ,DertT;
	xdata double C11,C12,C13,C03,C14,C21,C22,C23,C24,C31,C32,C33,C34,C41,C42,C43,C44;
	DertX = WirlX * Timer;
	DertY = WirlY * Timer;
	DertZ = WirlZ * Timer;
	DertT = sqrt(DertX * DertX + DertY * DertY + DertZ * DertZ );

	C11 = cos(DertT / 2);
	C12 = -(DertX / DertT) * sin(DertT / 2);
	C13 = -(DertY / DertT) * sin(DertT / 2); 
	C14 = -(DertZ / DertT) * sin(DertT / 2);  
	C21 =  (DertX / DertT) * sin(DertT / 2);
	C22 = cos(DertT / 2);
	C23 =  (DertZ / DertT) * sin(DertT / 2);
	C24 = -(DertY / DertT) * sin(DertT / 2); 
	C31 =  (DertY / DertT) * sin(DertT / 2);
	C32 = -(DertZ / DertT) * sin(DertT / 2);
	C33 = cos(DertT / 2); 
	C34 =  (DertX / DertT) * sin(DertT / 2);
	C41 =  (DertZ / DertT) * sin(DertT / 2); 
	C42 =  (DertY / DertT) * sin(DertT / 2);
	C43 = -(DertX / DertT) * sin(DertT / 2);
	C44 = cos(DertT / 2); 
	Q0 = C11 * q[0] + C12 * q[1] + C13 * q[2] + C14 * q[3];
	Q1 = C21 * q[0] + C22 * q[1] + C23 * q[2] + C24 * q[3];
	Q2 = C31 * q[0] + C32 * q[1] + C33 * q[2] + C34 * q[3];
	Q3 = C41 * q[0] + C42 * q[1] + C43 * q[2] + C44 * q[3];
	q[0] = Q0;
	q[1] = Q1;
	q[2] = Q2;
	q[3] = Q3;




}
//����̬���������Ԫ��
void CalQ_AttMat(double q[4],double AttMatrix[3][3])
{


	xdata double qq0 = 1;
	qq0 = 1+AttMatrix[0][0]+AttMatrix[1][1]+AttMatrix[2][2];
	if(qq0 < 0 || qq0 == 0)
	{
		;
	}
	else
	{   q[0]= sqrt(qq0)/2;	
		q[1]=(AttMatrix[2][1]-AttMatrix[1][2])/(4*(q[0]));
		q[2]=(AttMatrix[0][2]-AttMatrix[2][0])/(4*(q[0]));
	    q[3]=(AttMatrix[1][0]-AttMatrix[0][1])/(4*(q[0]));  
	}

}
//����̬������㺽�򡢸��������
void CalPitchRollYaw_AttMat(double *roll,double *pitch,double *yaw,double AttMatrix[3][3])
{
	
		xdata double Roll1,Roll2,Temp;
		if(AttMatrix[2][1] > 1) AttMatrix[2][1] = 1;
		else if(AttMatrix[2][1] < -1) AttMatrix[2][1] = -1;
		Temp = sqrt(1 - AttMatrix[2][1] * AttMatrix[2][1]);
		if(Temp == 0)
			;
		else
		{
			Roll1 = -AttMatrix[2][0] / Temp;
			Roll2 =  AttMatrix[2][2] / Temp;
			*roll = atan2(Roll1,Roll2);
		}
		*pitch = asin(AttMatrix[2][1]);
		*yaw = atan2(-AttMatrix[0][1],AttMatrix[1][1]);
//		*yaw = 0;
	
}
//��X��Y��Z���ٶȼ�����������������
void InitRollPitchYaw(double *roll,double *pitch,double *yaw,double AccX,double AccY,double AccZ)
{

	double xdata norm;
	norm = sqrt(AccX * AccX + AccY * AccY + AccZ * AccZ);
	if(norm == 0)
		;
	else
	{
		*pitch = asin(AccY / norm);
		*roll = -atan2(AccX,AccZ);
	}
	*yaw = 0;
}
//dbX1-------RollAcc��ʹ�ü��ٶȼƼ���õ��ĺ����
//dbX2-------RollGyro���������Ǽ���õ��ĺ����
void VGKalmanFilter(double *dbX,double dbX1,double *dbP1,double dbR1,double dbQ1,double dbX2,double *dbP2,double dbR2,double dbQ2)
{
	xdata double dbTempK0,dbTempK1,dbTempK2;
	xdata double K1,K2;	
//	dbTempK1 = sqrt((*dbX - dbX1) * (*dbX - dbX1) + (*dbP1) * (*dbP1));
//	dbTempK2 = sqrt((*dbX - dbX2) * (*dbX - dbX2) + (*dbP2) * (*dbP2));
	K1 = fabs(*dbX - dbX1);
	K2 = fabs(*dbX - dbX2);
	if(K1 > PI || K1 == PI)
		K1 = K1 - PI;
	if(K2 > PI || K2 == PI)
		K2 = K2 - PI;
	dbTempK2 = (K1+dbQ1) / (K2+K1+dbQ1+dbR1) + (*dbP1)*(*dbP1)*(*dbP1) + (*dbP1)*(*dbP1) + (*dbP1);
	dbTempK1 = (K2+dbQ2) / (K2+K1+dbQ2+dbR2) + (*dbP2)*(*dbP2)*(*dbP2) + (*dbP2)*(*dbP2) + (*dbP2);
	if(fabs(dbX1 - dbX2) > PI || fabs(dbX1 - dbX2) == PI)
	{
		dbTempK1 = 0;
		dbTempK2 = 1;
	}
//	dbTempK1 = *dbP1;
//	dbTempK2 = *dbP2;
	dbTempK0 = (dbTempK1 + dbTempK2);
	*dbX = (dbTempK1 * dbX1 + dbTempK2 * dbX2) / dbTempK0;
//	*dbP1 = (fabs(dbX1 - *dbX) + dbQ1) / (fabs(dbX1 - *dbX) + dbQ1 + dbR1);
//	*dbP2 = (fabs(dbX2 - *dbX) + dbQ2) / (fabs(dbX2 - *dbX) + dbQ2 + dbR2);
	K1 = fabs(dbX1 - *dbX);
	K2 = fabs(dbX2 - *dbX);
	if(K1 > PI || K1 == PI)
		K1 = K1 - PI;
	if(K2 > PI || K2 == PI)
		K2 = K2 - PI;
	*dbP1 = K1 / (K1 + K2 + dbR1);
	*dbP2 = K2 / (K1 + K2 + dbR2);	
//	*dbP1 = K1 / (K1 + K2);
//	*dbP2 = K2 / (K1 + K2);	
}

//����һ�������ⲿ����
//���������������������Ľ���������Kalman�˲�������������������������������
void GetRollPitchYaw(double *dbRoll,double *dbPitch,double *dbYaw,double dbGyro_Acc[7])
{/*
	static xdata unsigned char InitFlag = 0;
	double xdata AccNorm;
	static double xdata aRollAcc,aPitchAcc,aRollGyro,aPitchGyro,Yaw,Roll_1 = 1,Pitch_1 = 1,dbRollP = 1,dbPitchP = 1;
	static double xdata dbRollR = 0.01,dbRollQ = 0.001,dbPitchR = 0.01,dbPitchQ = 0.001;
	double xdata GyroX,GyroY,GyroZ,AccX,AccY,AccZ;
	static double xdata dbRollTemp = 0,dbPitchTemp = 0,dbYawTemp = 0;
	static double xdata dbKalmanP1 = 0.1,dbKalmanP2 = 0.1,dbKalmanP3 = 0.1,dbKalmanP4 = 0.1;
	static double xdata dbKalmanPRoll = 1,dbKalmanPPitch = 1;

	xdata unsigned char Flag = 0;
*/
	static xdata unsigned char InitFlag = 0;
	double xdata AccNorm;
	static double xdata Yaw;
	static double xdata dbRollR = 0.01,dbRollQ = 0.001,dbPitchR = 0.01,dbPitchQ = 0.001;
	double xdata GyroX,GyroY,GyroZ,AccX,AccY,AccZ;

	static double xdata dbRollTemp = 0,dbPitchTemp = 0,dbRollTemp1 = 0,dbPitchTemp1 = 0,dbYawTemp = 0;
	static double xdata dbRollTemp2 = 0,dbPitchTemp2 = 0;
	static double xdata dbKalmanP1 = 0.1,dbKalmanP2 = 0.1,dbKalmanP3 = 0.1,dbKalmanP4 = 0.1,dbKalmanP5 = 0.1,dbKalmanP6 = 0.1,dbKalmanP7 = 0.1,dbKalmanP8 = 0.1;
	static double xdata dbKalmanP9 = 0.1,dbKalmanP10 = 0.1,dbKalmanP11 = 0.1,dbKalmanP12 = 0.1;

	static double xdata dbKalmanPRoll = 1,dbKalmanPPitch = 1;

	xdata unsigned char Flag = 0;

	GyroX = dbGyro_Acc[0];
	GyroY = dbGyro_Acc[1];
	GyroZ = dbGyro_Acc[2];
//	GyroZ = 0;///////////////!!!!!!!!!!!!!!!!���Ҫ��AHRS��Ҫȥ�������
	AccX = dbGyro_Acc[3];
	AccY = dbGyro_Acc[4];
	AccZ = dbGyro_Acc[5];
	AccNorm = sqrt(AccX * AccX + AccY * AccY + AccZ * AccZ);
	if(InitFlag == 0)//����Ѿ���ʼ��
	{	
		InitFlag = 1;//��ʼ�����
		InitRollPitchYaw(&RollAcc,&PitchAcc,&Yaw,AccX,AccY,AccZ);//�ɼ��ٶȼ�����������������
		*dbRoll = RollAcc;//���ظ����������������Ϣ
		*dbPitch = PitchAcc;
		*dbYaw = Yaw;
		CalAttMat_RPY(RollAcc,PitchAcc,Yaw,AttMatrix);//�ɺ�������������������̬����
		CalQ_AttMat(Quaternion,AttMatrix);//����̬���������Ԫ��
		CrossQ(Quaternion);//��Ԫ��������
	}
	else if(InitFlag == 1)
	{
		UpdateQ(Quaternion,GyroX/180.0*PI,GyroY/180.0*PI,GyroZ/180.0*PI,0.0100218);//��ת�ٸ�����Ԫ��
//		UpdateQ2(Quaternion,GyroX/180.0*PI*0.01,GyroY/180.0*PI*0.01,GyroZ/180.0*PI*0.01);
		CrossQ(Quaternion);//��Ԫ��������
		CalAttMat_Q(AttMatrix,Quaternion);//����Ԫ��������̬����
		CalPitchRollYaw_AttMat(&RollGyro,&PitchGyro,&Yaw,AttMatrix);//����̬���������̬
		InitRollPitchYaw(&RollAcc,&PitchAcc,&dbYawTemp,AccX,AccY,AccZ);//�ü��ٶȼ�����̬
	
		if(AccNorm < 1.015 && AccNorm > 0.985) 
		{
/*
			VGKalmanFilter(&dbRollTemp,RollAcc,&dbKalmanP1,0.05,0.002,RollGyro,&dbKalmanP2,0.05,0.002);
			VGKalmanFilter(&dbPitchTemp,PitchAcc,&dbKalmanP3,0.05,0.002,PitchGyro,&dbKalmanP4,0.05,0.002);
			*dbRoll = dbRollTemp;//���ظ����������������Ϣ
			*dbPitch = dbPitchTemp;
			*dbYaw = Yaw;
			CalAttMat_RPY(dbRollTemp,dbPitchTemp,Yaw,AttMatrix);//����̬������̬����
			CalQ_AttMat(Quaternion,AttMatrix);//����̬���������Ԫ��
			CrossQ(Quaternion);//��������Ԫ��
*/
				VGKalmanFilter(&dbRollTemp,RollAcc,&dbKalmanP1,0.05,0.9,RollGyro,&dbKalmanP2,0.05,0.002);
				VGKalmanFilter(&dbPitchTemp,PitchAcc,&dbKalmanP3,0.05,0.9,PitchGyro,&dbKalmanP4,0.05,0.002);

				VGKalmanFilter(&dbRollTemp1,dbRollTemp,&dbKalmanP5,0.05,0.9,RollGyro,&dbKalmanP6,0.05,0.002);
				VGKalmanFilter(&dbPitchTemp1,dbPitchTemp,&dbKalmanP7,0.05,0.9,PitchGyro,&dbKalmanP8,0.05,0.002);

				VGKalmanFilter(&dbRollTemp2,dbRollTemp1,&dbKalmanP9,0.05,0.9,RollGyro,&dbKalmanP10,0.05,0.002);
				VGKalmanFilter(&dbPitchTemp2,dbPitchTemp1,&dbKalmanP11,0.05,0.9,PitchGyro,&dbKalmanP12,0.05,0.002);

				*dbRoll = dbRollTemp2;//���ظ����������������Ϣ
				*dbPitch = dbPitchTemp2;
				*dbYaw = Yaw;
				CalAttMat_RPY(dbRollTemp,dbPitchTemp,Yaw,AttMatrix);//����̬������̬����
				CalQ_AttMat(Quaternion,AttMatrix);//����̬���������Ԫ��
				CrossQ(Quaternion);//��������Ԫ��
		}
		else
		{
			*dbRoll = RollGyro;//���ظ����������������Ϣ
			*dbPitch = PitchGyro;
			*dbYaw = Yaw;
			CalAttMat_RPY(*dbRoll,*dbPitch,Yaw,AttMatrix);//����̬������̬����
			CalQ_AttMat(Quaternion,AttMatrix);//����̬���������Ԫ��
			CrossQ(Quaternion);//��������Ԫ��	
		}

	}	
}





