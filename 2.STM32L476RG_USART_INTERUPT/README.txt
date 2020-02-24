STM32L476RG_USART_INTERUPT
This project is about sending and reciving interupt by USART(Asynchronous).

We need to add these codes in the main.c after generating code by STM32CubeMx:
#include "stdio.h"

#define UART1RxBuffer_Size 1

uint8_t UART1RxBuffer[UART1RxBuffer_Size];

/**
  * @brief USART2 Redirection(for using function 'printf')
  * @param ch, *f
  * @retval 0
  */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 1000);
	HAL_Delay(1);
	return 0;
}

/**
  * @brief USART Callback function
  * @param huart
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if( huart->Instance == USART1)
    {
			HAL_UART_Transmit(huart, UART1RxBuffer, UART1RxBuffer_Size, 100);
			HAL_UART_Receive_IT(huart, UART1RxBuffer, UART1RxBuffer_Size);
    }
}

HAL_Delay(1000);


