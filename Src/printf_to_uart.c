/*
 * printf_to_uart.c
 *
 *  Created on: 23 maj 2020
 *      Author: USER
 */

#include "usart.h"

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 1000);
	return ch;
}

