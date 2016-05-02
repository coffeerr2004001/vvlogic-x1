/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

static uint8_t vv_pa_gpio_init_flg = 0;
static void vv_pa_init_gpio (void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.Pin    = PA_SER_DATA_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PA_SER_DATA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = PA_SER_CLK_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PA_SER_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = PA_SER_LE_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PA_SER_LE_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = EN_VPA_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(EN_VPA_PORT, &GPIO_InitStruct);
}

void vv_pa_en_pwr (uint8_t value)
{
 	if (vv_pa_gpio_init_flg == 0)
	{
		vv_pa_gpio_init_flg = 1;
		vv_pa_init_gpio();
	}
	
	switch (value)
	{
	case 0:
		HAL_GPIO_WritePin(EN_VPA_PORT, EN_VPA_PIN, GPIO_PIN_RESET); 
		
	break;
	case 1:
		HAL_GPIO_WritePin(EN_VPA_PORT, EN_VPA_PIN, GPIO_PIN_SET); 
	break;
	default:
		printf("err pa pwr en value.\r\n");
	break;

	}
}

void vv_pa_write_gain(uint8_t value)
{
 	if (vv_pa_gpio_init_flg == 0)
	{
		vv_pa_gpio_init_flg = 1;
		vv_pa_init_gpio();
	}

	if (value > 63)
	{
		printf("Err PA gain settings.\r\n");
		return;
	}

	HAL_GPIO_WritePin(PA_SER_LE_PORT, PA_SER_LE_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PA_SER_CLK_PORT, PA_SER_CLK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PA_SER_DATA_PORT, PA_SER_DATA_PIN, GPIO_PIN_RESET);

	for (int i = 0; i < 6; i++)
	{
		HAL_GPIO_WritePin(PA_SER_CLK_PORT, PA_SER_CLK_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(PA_SER_DATA_PORT, PA_SER_DATA_PIN, (value & (0x20 >> i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(PA_SER_CLK_PORT, PA_SER_CLK_PIN, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(PA_SER_CLK_PORT, PA_SER_CLK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PA_SER_DATA_PORT, PA_SER_DATA_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PA_SER_LE_PORT, PA_SER_LE_PIN, GPIO_PIN_SET);

}
