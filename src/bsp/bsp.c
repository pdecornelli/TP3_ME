/*
 * bsp.c
 *
 *  Created on: 26 de ago. de 2016
 *      Author: Matias Labedz
 */
#include "bsp.h"

UART_HandleTypeDef UART3_Handle;
ADC_HandleTypeDef ADC_HandleStruct;

uint8_t dato;

extern void APP_GetData(uint8_t dato);

void BSP_Init(void) {
	BSP_RCC_Init();
	BSP_UART_Init();
	BSP_SW_Init();
	BSP_ADC_Init();
}

void BSP_RCC_Init(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__PWR_CLK_ENABLE()
	;

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
			| RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


void BSP_UART_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOD_CLK_ENABLE()
	;

	__HAL_RCC_USART3_CLK_ENABLE()
	;

	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	UART3_Handle.Instance = USART3;
	UART3_Handle.Init.BaudRate = 115200;
	UART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART3_Handle.Init.StopBits = UART_STOPBITS_1;
	UART3_Handle.Init.Parity = UART_PARITY_NONE;
	UART3_Handle.Init.Mode = UART_MODE_TX_RX;
	UART3_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART3_Handle);

	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	HAL_UART_Receive_IT(&UART3_Handle, &dato, 1);
}

void TransmitData(uint8_t *buffer, uint8_t size) {
	HAL_UART_Transmit(&UART3_Handle, buffer, size, 100);

}

void USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(&UART3_Handle);
	APP_GetData(dato);
	HAL_UART_Receive_IT(&UART3_Handle, &dato, 1);
}

const uint16_t SW_PIN[SWn] = {SW_UP_PIN, SW_LEFT_PIN, SW_DOWN_PIN, SW_RIGHT_PIN};

void BSP_SW_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;

	SW_GPIO_CLK_ENABLE();

	    GPIO_InitStruct.Pin = SW_UP_PIN | SW_LEFT_PIN | SW_DOWN_PIN | SW_RIGHT_PIN;
	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pull = GPIO_PULLUP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	    HAL_GPIO_Init(SW_PORT, &GPIO_InitStruct);

}

uint32_t BSP_SW_GetState(SW_TypeDef sw){
	return HAL_GPIO_ReadPin(SW_PORT, SW_PIN[sw]);
}

void BSP_ADC_Init(void){
	ADC_ChannelConfTypeDef ChannelConfStruct;
		GPIO_InitTypeDef GPIO_InitStruct;

		EXP_BOARD_POT_PIN_CLK_ENABLE()
		;
		EXP_BOARD_POT_ADC_CLK_ENABLE()
		;

		ADC_HandleStruct.Instance = ADC1;

		ADC_HandleStruct.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
		ADC_HandleStruct.Init.Resolution = ADC_RESOLUTION_12B;
		ADC_HandleStruct.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		ADC_HandleStruct.Init.ScanConvMode = DISABLE;
		ADC_HandleStruct.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		ADC_HandleStruct.Init.ContinuousConvMode = DISABLE;
		ADC_HandleStruct.Init.NbrOfConversion = 1;
		ADC_HandleStruct.Init.DiscontinuousConvMode = DISABLE;
		ADC_HandleStruct.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		ADC_HandleStruct.Init.DMAContinuousRequests = DISABLE;
		ADC_HandleStruct.Init.NbrOfDiscConversion = 0;
		ADC_HandleStruct.Init.ExternalTrigConvEdge =
		ADC_EXTERNALTRIGCONVEDGE_NONE;
		ADC_HandleStruct.Init.EOCSelection = DISABLE;

		GPIO_InitStruct.Pin = EXP_BOARD_POT_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

		HAL_GPIO_Init(EXP_BOARD_POT_PORT, &GPIO_InitStruct);

		ChannelConfStruct.Channel = EXP_BOARD_POT_CHANNEL;
		ChannelConfStruct.Offset = 0;
		ChannelConfStruct.Rank = 1;
		ChannelConfStruct.SamplingTime = ADC_SAMPLETIME_15CYCLES;

		HAL_ADC_Init(&ADC_HandleStruct);
		HAL_ADC_ConfigChannel(&ADC_HandleStruct, &ChannelConfStruct);
		HAL_ADC_Start(&ADC_HandleStruct);
}

uint16_t BSP_ADC_GetValue(void){
	HAL_ADC_Start(&ADC_HandleStruct);
		return HAL_ADC_GetValue(&ADC_HandleStruct);
}
