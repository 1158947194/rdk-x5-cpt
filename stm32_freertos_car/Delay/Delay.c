#include "stm32f4xx_hal.h"
#include "tim.h"

void delay_us(uint16_t nus)
{
	__HAL_TIM_SetCounter(&htim13,0);
	__HAL_TIM_ENABLE(&htim13);
	
	while(__HAL_TIM_GetCounter(&htim13)<nus);
	
	__HAL_TIM_DISABLE(&htim13);
}
