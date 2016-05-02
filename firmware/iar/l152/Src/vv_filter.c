/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

static uint8_t vv_filter_gpio_init_flg = 0;
static void  vv_filter_init_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin    = FILTER_CTRL0_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL1_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL2_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL3_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL4_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL4_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL5_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL5_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL6_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL6_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = FILTER_CTRL7_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(FILTER_CTRL7_PORT, &GPIO_InitStruct);
}

//range 1: 10K - 35M
//range 2: 35M - 75M
//range 3: 75M - 175M
//range 4: 175M - 425M
//range 5: 425M - 995M
//range 6: 995M - 2215M
//range 7: 2215M - 4000M
//hmc241 ctrlab = 00 rf1, 10 rf2, 01 rf3, 11rf4
//sw1,sw2,sw3 control pin connect to a inverter!!

//switch 1, u30, rfin = pll p out,
//rf1: range 4
//rf2: range 5
//rf3: range 6
//rf4: range 7
//ctrla = ctrl 0, ctrl b = ctrl 1
static void vv_filter_sw1(uint8_t value)
{
	switch (value)
	{
	  case 1:
		  HAL_GPIO_WritePin(FILTER_CTRL0_PORT, FILTER_CTRL0_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL1_PORT, FILTER_CTRL1_PIN, GPIO_PIN_SET);
		  break;
	  case 2:
		  HAL_GPIO_WritePin(FILTER_CTRL0_PORT, FILTER_CTRL0_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL1_PORT, FILTER_CTRL1_PIN, GPIO_PIN_SET);
		  break;
	  case 3:
		  HAL_GPIO_WritePin(FILTER_CTRL0_PORT, FILTER_CTRL0_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL1_PORT, FILTER_CTRL1_PIN, GPIO_PIN_RESET);
		  break;
	  case 4:
		  HAL_GPIO_WritePin(FILTER_CTRL0_PORT, FILTER_CTRL0_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL1_PORT, FILTER_CTRL1_PIN, GPIO_PIN_RESET);
		  break;
	  default:
		  printf("err switch 1 value.\r\n");
		  break;

	}
}

//switch 1, u31, rfin = rx_out,
//rf1: range 7
//rf2: range 6
//rf3: range 5
//rf4: range 4
//ctrla = ctrl 2, ctrl b = ctrl 3
static void vv_filter_sw2(uint8_t value)
{
	switch (value)
	{
	  case 1:
		  HAL_GPIO_WritePin(FILTER_CTRL2_PORT, FILTER_CTRL2_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL3_PORT, FILTER_CTRL3_PIN, GPIO_PIN_SET);
		  break;
	  case 2:
		  HAL_GPIO_WritePin(FILTER_CTRL2_PORT, FILTER_CTRL2_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL3_PORT, FILTER_CTRL3_PIN, GPIO_PIN_SET);
		  break;
	  case 3:
		  HAL_GPIO_WritePin(FILTER_CTRL2_PORT, FILTER_CTRL2_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL3_PORT, FILTER_CTRL3_PIN, GPIO_PIN_RESET);
		  break;
	  case 4:
		  HAL_GPIO_WritePin(FILTER_CTRL2_PORT, FILTER_CTRL2_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL3_PORT, FILTER_CTRL3_PIN, GPIO_PIN_RESET);
		  break;


	  default:
		  printf("err switch 2 value.\r\n");
		  break;

	}
}


//ctrl 7, 6, low freq switch, hws408, u28
static void vv_filter_sw3(uint8_t value)
{
	if ((value > 2) || (value < 1))
	{
		printf("err filter sw4 value.\r\n");
		return;
	}

	switch (value)
	{
	  case 1:
		  HAL_GPIO_WritePin(FILTER_CTRL7_PORT, FILTER_CTRL7_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL6_PORT, FILTER_CTRL6_PIN, GPIO_PIN_SET);
		  break;
	  case 2:
		  HAL_GPIO_WritePin(FILTER_CTRL7_PORT, FILTER_CTRL7_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL6_PORT, FILTER_CTRL6_PIN, GPIO_PIN_RESET);
		  break;
	  default:
		  printf("err switch 4 value.\r\n");
		  break;

	}
}


//switch 3, u32, rfc = out,
//rf1: range 4,5,6,7
//rf2: range 3
//rf3: range 2
//rf4: range 1
//ctrla = ctrl 4, ctrl b = ctrl 5, 
//hmc241 ctrlab = 00 rf1, 10 rf2, 01 rf3, 11rf4
static void vv_filter_sw4(uint8_t value)
{
	switch (value)
	{
	  case 1:
		  HAL_GPIO_WritePin(FILTER_CTRL4_PORT, FILTER_CTRL4_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL5_PORT, FILTER_CTRL5_PIN, GPIO_PIN_SET);
		  break;	  
	  case 2:
		  HAL_GPIO_WritePin(FILTER_CTRL4_PORT, FILTER_CTRL4_PIN, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FILTER_CTRL5_PORT, FILTER_CTRL5_PIN, GPIO_PIN_RESET);
		  break;
		case 3:
		  HAL_GPIO_WritePin(FILTER_CTRL4_PORT, FILTER_CTRL4_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL5_PORT, FILTER_CTRL5_PIN, GPIO_PIN_SET);
		  break;
	  case 4:
		  HAL_GPIO_WritePin(FILTER_CTRL4_PORT, FILTER_CTRL4_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(FILTER_CTRL5_PORT, FILTER_CTRL5_PIN, GPIO_PIN_RESET);
		  break;


	  default:
		  printf("err switch 2 value.\r\n");
		  break;
	}
}

static uint8_t vv_filter_get_channel_by_freq (uint64_t freq)
{
	if ( (freq<FREQ_RANGE_0) || (freq>FREQ_RANGE_7))
	{
		printf("freq out range err.\r\n");
		return 0;
	}
	else if (freq < FREQ_RANGE_1)
	{
		return 1;
	}
	else if (freq < FREQ_RANGE_2)
	{
		return 2;
	}
	else if (freq < FREQ_RANGE_3)
	{
		return 3;
	}
	else if (freq < FREQ_RANGE_4)
	{
		return 4;
	}
	else if (freq < FREQ_RANGE_5)
	{
		return 5;
	}
	else if (freq < FREQ_RANGE_6)
	{
		return 7;
	}
	else if (freq <= FREQ_RANGE_7)
	{
		return 6;
	}
	
	printf("freq out range err.\r\n");
	return 0;
}

//range 1 - 7
static void vv_filter_channel(uint8_t channel)
{
	if ((channel < 1) || (channel > 7))
	{
		printf("err vv filter value.\r\n");
		return ;
	}

	if (vv_filter_gpio_init_flg == 0)
	{
		vv_filter_gpio_init_flg = 1;
		vv_filter_init_gpio();
	}

	switch (channel)
	{
		//range 1: 10K - 35M
	  case 1:
		  vv_filter_sw1(1);
		  vv_filter_sw2(4);
		  vv_filter_sw3(1);
		  vv_filter_sw4(4);
		  break;

		  //range 2: 35M - 75M
	  case 2:
		  vv_filter_sw1(1);
		  vv_filter_sw2(4);
		  vv_filter_sw3(1);
		  vv_filter_sw4(3); //?
		  break;

		  //range 3: 75M - 175M
	  case 3:
		  vv_filter_sw1(1);
		  vv_filter_sw2(4);
		  vv_filter_sw3(2);
		  vv_filter_sw4(2);
		  break;

		  //range 4: 175M - 425M
	  case 4:
		  vv_filter_sw1(1);
		  vv_filter_sw2(4);
		  vv_filter_sw3(1);
		  vv_filter_sw4(1);
		  break;

		  //range 5: 425M - 995M
	  case 5:
		  vv_filter_sw1(2);
		  vv_filter_sw2(3);
		  vv_filter_sw3(1);
		  vv_filter_sw4(1);
		  break;

		  //range 6: 995M - 2215M
	  case 6:
		  vv_filter_sw1(3);
		  vv_filter_sw2(2);
		  vv_filter_sw3(1);
		  vv_filter_sw4(1);
		  break;

		  //range 7: 2215M - 4000M
	  case 7:
		  vv_filter_sw1(4);
		  vv_filter_sw2(1);
		  vv_filter_sw3(1);
		  vv_filter_sw4(1);
		  break;

	  default:
		  break;
	}
}

uint8_t vv_filter_freq (uint64_t freq)
{
	uint8_t channel = 0;
	if ( (freq<FREQ_RANGE_0) || (freq>FREQ_RANGE_7))
	{
		printf("vv_filter_freq freq out range err.\r\n");
		return STATUS_ERROR;
	}

	channel = vv_filter_get_channel_by_freq(freq);
	printf("filter channel = %d.\r\n",channel);
	vv_filter_channel(channel);
	return STATUS_OK;
}
