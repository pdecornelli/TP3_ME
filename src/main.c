/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f4_discovery.h"
#include "bsp/bsp.h"

uint8_t estado = 0;
uint8_t bi = 0;
uint8_t ba = 0;
uint8_t bdato = 0;
uint8_t r = 0;
uint8_t v = 0;
uint8_t n = 0;
uint8_t a = 0;
uint8_t size, input, mint, k, c;
uint8_t buffer[250];
uint16_t adc;

void APP_GetData(uint8_t dato);
void APP_GetBinario(uint16_t adc);
void APP_GetVoltage(uint16_t adc);

int main(void) {

	BSP_Init();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

	while (1) {
		switch (estado) {
		case 0:
			if (bi == 0) {
				size = sprintf(buffer,
						"1 - Valor ADC \r\n2 - Leds \r\n3 - Estado SW \r\n\n");
				TransmitData(buffer, size);
				bi = 1;
			}
			if (bdato == 1 && input != 0) {
				estado = input;
				bdato = 0;
				ba = 0;
			}
			break;
		case 1:
			if (ba == 0) {
				size =
						sprintf(buffer,
								"1 - Obtener binario \r\n2 - Obtener decimal \r\n3 - Obtener voltage\r\n\n");
				TransmitData(buffer, size);
				ba = 1;
			}
			if (bdato == 1) {
				mint = input;
				bdato = 0;
				switch (mint) {
				case 0:
					estado = mint;
					bi = 0;
					break;
				case 1:
					adc = BSP_ADC_GetValue();
					size = sprintf(buffer, "Valor AD binario = ");
					TransmitData(buffer, size);
					APP_GetBinario(adc);
					ba = 0;
					break;
				case 2:
					adc = BSP_ADC_GetValue();
					size = sprintf(buffer, "Valor AD decimal = %d \r\n", adc);
					TransmitData(buffer, size);
					ba = 0;
					break;
				case 3:
					adc = BSP_ADC_GetValue();
					APP_GetVoltage(adc);
					ba = 0;
					break;
				default:
					size = sprintf(buffer, "Dato no valido");
					TransmitData(buffer, size);
					ba = 0;
				}
			}
			break;
		case 2:
			if (ba == 0) {
				size =
						sprintf(buffer,
								"1 - Toggle Led R \r\n2 - Toggle Led V \r\n3 - Toggle Led N\r\n4 - Toggle Led A\r\n\n");
				TransmitData(buffer, size);
				ba = 1;
			}
			if (bdato == 1) {
				mint = input;
				bdato = 0;
				switch (mint) {
				case 0:
					estado = mint;
					bi = 0;
					break;
				case 1:
					if (r == 0) {
						r = 1;
						BSP_LED_On(LED5);
						size = sprintf(buffer, "Led Rojo ON \r\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						r = 0;
						BSP_LED_Off(LED5);
						size = sprintf(buffer, "Led Rojo OFF \r\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				case 2:
					if (v == 0) {
						v = 1;
						BSP_LED_On(LED4);
						size = sprintf(buffer, "Led Verde ON \r\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						v = 0;
						BSP_LED_Off(LED4);
						size = sprintf(buffer, "Led Verde OFF \r\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				case 3:
					if (n == 0) {
						n = 1;
						BSP_LED_On(LED3);
						size = sprintf(buffer, "Led Naranja ON \r\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						n = 0;
						BSP_LED_Off(LED3);
						size = sprintf(buffer, "Led Naranja OFF \r\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				case 4:
					if (a == 0) {
						a = 1;
						BSP_LED_On(LED6);
						size = sprintf(buffer, "Led Azul ON \r\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						a = 0;
						BSP_LED_Off(LED6);
						size = sprintf(buffer, "Led Azul OFF \r\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				default:
					size = sprintf(buffer, "Dato no valido");
					TransmitData(buffer, size);
					ba = 0;
				}
			}
			break;
		case 3:
			if (ba == 0) {
				size =
						sprintf(buffer,
								"1 - SW1 (UP) \r\n2 - SW2 (LEFT) \r\n3 - SW3 (DOWN) \r\n4 - SW4 (RIGHT) \r\n");
				TransmitData(buffer, size);
				ba = 1;
			}
			if (bdato == 1) {
				mint = input;
				bdato = 0;
				switch (mint) {
				case 0:
					estado = mint;
					bi = 0;
					break;
				case 1:
					if (BSP_SW_GetState(SW_UP) == 0) {
						size = sprintf(buffer, "SW1 (UP) Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						size = sprintf(buffer, "SW1 (UP) NO Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				case 2:
					if (BSP_SW_GetState(SW_LEFT) == 0) {
						size = sprintf(buffer, "SW2 (LEFT) Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						size = sprintf(buffer, "SW2 (LEFT) NO Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				case 3:
					if (BSP_SW_GetState(SW_DOWN) == 0) {
						size = sprintf(buffer, "SW3 (DOWN) Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						size = sprintf(buffer, "SW3 (DOWN) NO Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				case 4:
					if (BSP_SW_GetState(SW_RIGHT) == 0) {
						size = sprintf(buffer, "SW4 (RIGHT) Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					} else {
						size = sprintf(buffer, "SW4 (RIGHT) NO Presionado\n\n");
						TransmitData(buffer, size);
						ba = 0;
					}
					break;
				default:
					size = sprintf(buffer, "Dato no valido\n\n");
					TransmitData(buffer, size);
					ba = 0;
				}
			}
			break;
		default:
			size = sprintf(buffer, "Dato no valido\n\n");
			TransmitData(buffer, size);
			bi = 0;
			estado = 0;
		}
	}
}

void APP_GetData(uint8_t dato) {
	bdato = 1;
	input = dato - 48;
}
void APP_GetBinario(uint16_t adc) {
	uint16_t comp = 4096;
	for (int i = 12; i > 0; i--) {
		if (adc & comp) {
			size = sprintf(buffer, "1");
			TransmitData(buffer, size);
		} else {
			size = sprintf(buffer, "0");
			TransmitData(buffer, size);
		}
		comp /= 2;
	}
	size = sprintf(buffer, "\r\n");
	TransmitData(buffer, size);
}
void APP_GetVoltage(uint16_t adc) {
	adc = (adc * 3300) / 4095;
	uint16_t decimal = (adc % 1000);
	uint16_t entero = adc / 1000;
	size = sprintf(buffer, "Valor de Voltage = %d.%03d \r\n", entero, decimal);
	TransmitData(buffer, size);
}
