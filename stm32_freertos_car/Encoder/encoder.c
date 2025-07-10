#include "stm32f4xx_hal.h"
#include "tim.h"


int16_t Encode_Get_Left(void)
{
	int16_t temp;
	temp = __HAL_TIM_GetCounter(&htim3);
	__HAL_TIM_SetCounter(&htim3,0);             
	return temp;
}

int16_t Encode_Get_Right(void)
{
	int16_t temp;
	temp = __HAL_TIM_GetCounter(&htim2);
	__HAL_TIM_SetCounter(&htim2,0);             
	return temp;
}



//编码器参数：
//	一圈的脉冲数（四倍频模式）：1560
//  轮子周长：20.41cm
//  即轮子转动20.41cm产生1560个脉冲

int16_t Get_Left_Motor_Speed(void)
{
	int Left_Speed;
	Left_Speed = Encode_Get_Left() * 20.41 / 1560 * 200;  
	return Left_Speed;
}

int16_t Get_Right_Motor_Speed(void)
{
	int Right_Speed;
	Right_Speed = Encode_Get_Right() * 20.41 / 1560 * 200;  
	return Right_Speed;
}
