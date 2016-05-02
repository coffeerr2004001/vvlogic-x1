/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */

#include "includes.h"

int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void vv_dbg_led_init_gpio (void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
  
	/* Enable the GPIO_LED clock */
	__GPIOC_CLK_ENABLE();

	/* Configure the GPIO_LED pin */
	GPIO_InitStruct.Pin    = LED_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void vv_dbg_led_sw (uint8_t value)
{
	switch (value)
	{
		case 0: //led off
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
		break;
		case 1: //led on
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
			break;
		default:
			break;
	}
}

void print_bytes(uint8_t *data, uint16_t length)
{
	printf("\r\n************ Print bytes ***********\r\n");
	if (length < 16)
	{
		length = 16;
	}

	for (int i = 0; i < length / 16; i++)
	{
		printf("0x%08x : ",  i * 16);
		for (int j = 0; j < 16; j++)
		{
			printf("0x%02x ", *(data + i * 16 + j));
		}
		printf("\r\n");
	}
	printf("************     end     ***********\r\n\r\n");
}


void vv_dbg_enable_all_gpio_clk (void)
{
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	//__GPIOF_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
}
