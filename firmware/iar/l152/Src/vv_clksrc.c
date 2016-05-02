/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

static uint8_t vv_clksrc_gpio_init_flg = 0;


static void vv_clksrc_init_gpio (void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.Pin    = CLK_SRC_SELECT_0_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(CLK_SRC_SELECT_0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = CLK_SRC_SELECT_1_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(CLK_SRC_SELECT_1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = CLK_SRC_SELECT_2_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(CLK_SRC_SELECT_2_PORT, &GPIO_InitStruct);
	
}

/**
 * Select Clock source for PLL, 
 * PARA VALUE: 
 * CLK_SRC_OSC_TO_PLL_ONLY : use 10M clock from osc. 
 * CLK_SRC_OSC_TO_PLL_AND_SYNC : use 10M clock from osc, and fanout this clock to sync connector, this is useful in frequency caliberation mode.
 * CLK_SRC_SYNC_TO_PLL: use external clock source, only 10Mhz support now.
 * @author coffeerr2004001
 */
void vv_clksrc_sel(CLK_SRC value)
{
	//check clk source gpio initialized
	if (vv_clksrc_gpio_init_flg == 0)
	{
		vv_clksrc_init_gpio();
		vv_clksrc_gpio_init_flg = 1;
	}

	if ((value == 0) || (value > 3))
	{
		printf("Err mode value:[%d].\r\n", value);
		return;
	}

	switch (value)
	{
	case CLK_SRC_OSC_TO_PLL_ONLY:
		HAL_GPIO_WritePin(CLK_SRC_SELECT_0_PORT, CLK_SRC_SELECT_0_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CLK_SRC_SELECT_1_PORT, CLK_SRC_SELECT_1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CLK_SRC_SELECT_2_PORT, CLK_SRC_SELECT_2_PIN, GPIO_PIN_RESET);
		break;
	case CLK_SRC_OSC_TO_PLL_AND_SYNC:
		HAL_GPIO_WritePin(CLK_SRC_SELECT_0_PORT, CLK_SRC_SELECT_0_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CLK_SRC_SELECT_1_PORT, CLK_SRC_SELECT_1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CLK_SRC_SELECT_2_PORT, CLK_SRC_SELECT_2_PIN, GPIO_PIN_RESET);
		break;
	case CLK_SRC_SYNC_TO_PLL:
		HAL_GPIO_WritePin(CLK_SRC_SELECT_0_PORT, CLK_SRC_SELECT_0_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CLK_SRC_SELECT_1_PORT, CLK_SRC_SELECT_1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CLK_SRC_SELECT_2_PORT, CLK_SRC_SELECT_2_PIN, GPIO_PIN_SET);
		break;
	default :
		break;
	}
}
