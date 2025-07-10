#include "stm32f4xx_hal.h"
#include "pid.h"

/************************************************
�������� �� PID_Spd
��    �� �� PID�ٶ�(Speed)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID�ṹ��
�� �� ֵ �� PIDSpd -------- PID�ٶ�
*************************************************/
float PID_Spd(float SetValue, float ActualValue, PID_SpdTypeDef *PID)
{
  float PIDSpd;                                  //�ٶ�
 
  PID->Ek = SetValue - ActualValue;
  PID->Sum += PID->Ek;                         //�ۼ����
 
	if (PID->Sum > 2000)
		PID->Sum = 2000;
  else if (PID->Sum < -2000)
		PID->Sum = -2000;
	
  PIDSpd = PID->Kp * PID->Ek + (PID->Ki * PID->Sum) + PID->Kd * (PID->Ek1 - PID->Ek);
 
  PID->Ek1 = PID->Ek;  
	return PIDSpd;
}
