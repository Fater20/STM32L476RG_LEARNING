
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

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

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
