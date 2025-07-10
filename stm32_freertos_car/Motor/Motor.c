#include "stm32f4xx_hal.h"
#include "tim.h"

void MotorL_Move(void)
{
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
}

void MotorL_Back(void)
{
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
}

void MotorL_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
}

void MotorR_Move(void)
{
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim11, TIM_CHANNEL_1);
}

void MotorR_Back(void)
{
	HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
}

void MotorR_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim11, TIM_CHANNEL_1);
}


