/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

void vv_pwr_init_pwr_ctrl (void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
  
	/* Enable the GPIO_LED clock */
	__GPIOB_CLK_ENABLE();

	/* Configure the GPIO_LED pin */
	GPIO_InitStruct.Pin    = PWR_VDD5V_SW_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(PWR_VDD5V_SW_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(PWR_VDD5V_SW_PORT, PWR_VDD5V_SW_PIN, GPIO_PIN_SET);
}


