/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "tim.h"
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
uint8_t Led_Status=0;
uint16_t dutyCycle=0;
/* USER CODE END Variables */
osThreadId breath_ledHandle;
osThreadId readkeyHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void Startbreathled(void const * argument);
void Startreadkey(void const * argument);

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
  /* definition and creation of breath_led */
  osThreadDef(breath_led, Startbreathled, osPriorityNormal, 0, 128);
  breath_ledHandle = osThreadCreate(osThread(breath_led), NULL);

  /* definition and creation of readkey */
  osThreadDef(readkey, Startreadkey, osPriorityNormal, 0, 128);
  readkeyHandle = osThreadCreate(osThread(readkey), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Startbreathled */
/**
  * @brief  Function implementing the breath_led thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_Startbreathled */
void Startbreathled(void const * argument)
{
  /* USER CODE BEGIN Startbreathled */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* Infinite loop */
  for(;;)
  {
		switch(Led_Status)
		{
			case 0:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
				break;
			case 1:
				while (dutyCycle < 1000)
				{
					dutyCycle ++;
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, dutyCycle);
					//TIM3->CCR2 = dutyCycle;
					osDelay(1);
				}
				while (dutyCycle)
				{
					dutyCycle --;
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, dutyCycle);
					//TIM3->CCR2 = dutyCycle;
					osDelay(1);
				}
				break;
			case 2:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
				break;
			default:
				Led_Status=0;
		}
    osDelay(1);
  }
  /* USER CODE END Startbreathled */
}

/* USER CODE BEGIN Header_Startreadkey */
/**
* @brief Function implementing the readkey thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startreadkey */
void Startreadkey(void const * argument)
{
  /* USER CODE BEGIN Startreadkey */
  /* Infinite loop */
  for(;;)
  {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
		{
			osDelay(1);
			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET);	//Wait for the button PC13 to be released
			switch(Led_Status)
			{
				case 0:
					Led_Status++;
					break;
				case 1:
					Led_Status++;
					break;
				case 2:
					Led_Status=0;
					break;
				default:
					Led_Status=0;
			}
		}
    osDelay(10);
  }
  /* USER CODE END Startreadkey */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
