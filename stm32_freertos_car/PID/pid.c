#include "stm32f4xx_hal.h"
#include "pid.h"

/************************************************
函数名称 ： PID_Spd
功    能 ： PID速度(Speed)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID结构体
返 回 值 ： PIDSpd -------- PID速度
*************************************************/
float PID_Spd(float SetValue, float ActualValue, PID_SpdTypeDef *PID)
{
  float PIDSpd;                                  //速度
 
  PID->Ek = SetValue - ActualValue;
  PID->Sum += PID->Ek;                         //累计误差
 
	if (PID->Sum > 2000)
		PID->Sum = 2000;
  else if (PID->Sum < -2000)
		PID->Sum = -2000;
	
  PIDSpd = PID->Kp * PID->Ek + (PID->Ki * PID->Sum) + PID->Kd * (PID->Ek1 - PID->Ek);
 
  PID->Ek1 = PID->Ek;  
	return PIDSpd;
}
