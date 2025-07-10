/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Motor.h"
#include "OLED.h"
#include "tim.h"
#include "MPU6050.h"
#include "stdio.h"
#include "serial.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern int16_t Speed_L, Speed_R;
extern int ret;
float Yaw;
extern uint8_t num_buff[3];
extern Ctrl_information chassis_ctrl;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId OLEDHandle;
osThreadId MoveHandle;
osThreadId mpu6050Handle;
osThreadId SerialHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void OLED_Control(void const * argument);
void Move_Control(void const * argument);
void mpu6050_Control(void const * argument);
void Serial_Control(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of OLED */
  osThreadDef(OLED, OLED_Control, osPriorityBelowNormal, 0, 128);
  OLEDHandle = osThreadCreate(osThread(OLED), NULL);

  /* definition and creation of Move */
  osThreadDef(Move, Move_Control, osPriorityNormal, 0, 512);
  MoveHandle = osThreadCreate(osThread(Move), NULL);

  /* definition and creation of mpu6050 */
  osThreadDef(mpu6050, mpu6050_Control, osPriorityBelowNormal, 0, 256);
  mpu6050Handle = osThreadCreate(osThread(mpu6050), NULL);

  /* definition and creation of Serial */
  osThreadDef(Serial, Serial_Control, osPriorityNormal, 0, 512);
  SerialHandle = osThreadCreate(osThread(Serial), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_OLED_Control */
/**
* @brief Function implementing the OLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_Control */
void OLED_Control(void const * argument)
{
  /* USER CODE BEGIN OLED_Control */
	OLED_ShowString(0,0,"left:",OLED_8X16);
	OLED_ShowString(0,17,"right:",OLED_8X16);
	OLED_ShowString(73,0,"cm/s",OLED_8X16);
	OLED_ShowString(82,17,"cm/s",OLED_8X16);
	OLED_ShowString(0,33,"yaw=",OLED_8X16);
	
	OLED_ShowString(0,54,"s_L",OLED_6X8);
	OLED_ShowString(37,54,"s_R",OLED_6X8);
  /* Infinite loop */
  for(;;)
  {
    OLED_ShowSignedNum(41,0,Speed_L,3,OLED_8X16);
		OLED_ShowSignedNum(49,17,-Speed_R,3,OLED_8X16);
		
		OLED_ShowFloatNum(33,33,Yaw,3,2,OLED_8X16);
		
		OLED_ShowFloatNum(19,54,chassis_ctrl.leftSpeedSet,3,0,OLED_6X8);
		OLED_ShowFloatNum(55,54,chassis_ctrl.leftSpeedSet,3,0,OLED_6X8);
		OLED_Update();
		
		
  }
  /* USER CODE END OLED_Control */
}

/* USER CODE BEGIN Header_Move_Control */
/**
* @brief Function implementing the Move thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Move_Control */
void Move_Control(void const * argument)
{
  /* USER CODE BEGIN Move_Control */
	//HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//MotorL_Move();
	//MotorR_Move();
	//__HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1,1680 * 5.7);
	//__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2,1680 * 5.7);
  /* Infinite loop */
  for(;;)
  {
		osDelay(20);
  }
  /* USER CODE END Move_Control */
}

/* USER CODE BEGIN Header_mpu6050_Control */
/**
* @brief Function implementing the mpu6050 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_mpu6050_Control */
void mpu6050_Control(void const * argument)
{
  /* USER CODE BEGIN mpu6050_Control */
	
  /* Infinite loop */
  for(;;)
  {
	  MPU6050_DMP_Get_Date(&Yaw);
		Yaw=(Yaw*100+0.5)/100;
  }
  /* USER CODE END mpu6050_Control */
}

/* USER CODE BEGIN Header_Serial_Control */
/**
* @brief Function implementing the Serial thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Serial_Control */
void Serial_Control(void const * argument)
{
  /* USER CODE BEGIN Serial_Control */
  /* Infinite loop */
  for(;;)
  {
    usartSendData(Speed_L, -Speed_R, Yaw, num_buff[1]);
		osDelay(20);
  }
  /* USER CODE END Serial_Control */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
