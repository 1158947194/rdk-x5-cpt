#ifndef __PID_H
#define __PID_H

typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative
 
  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float Ek2;                      //��ǰһ����� e(k-2)
  float Sum;                      //�ۼƻ���λ��
}PID_SpdTypeDef;

float PID_Spd(float SetValue, float ActualValue, PID_SpdTypeDef *PID);
	
#endif
