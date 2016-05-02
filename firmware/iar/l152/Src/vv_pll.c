/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

static uint32_t vv_pll_get_common_divisor(uint32_t a, uint32_t b)
{
	while (a != b)
	{
		if (a > b)
		{
			a -= b;
		}
		else
		{
			b -= a;
		}
	}
	return a;
}


static void vv_pll_reduction(uint32_t a, uint32_t b, uint32_t * result)
{
	uint32_t tmp_com_dvsr;
	tmp_com_dvsr = vv_pll_get_common_divisor(a, b);

	while (tmp_com_dvsr != 1)
	{

		a /= tmp_com_dvsr;
		b /= tmp_com_dvsr;
		tmp_com_dvsr = vv_pll_get_common_divisor(a, b);
	}

	*result = a;
	*(result + 1) = b;
}

static void vv_pll_spi_send_byte(uint8_t value)
{
	HAL_SPI_Transmit(&pll_spi_handle, &value, 1, 1000);
}

static uint8_t vv_pll_gpio_init_flg = 0;
static void vv_pll_init_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//HAL_GPIO_WritePin(PLL_SPI_CS_PORT, PLL_SPI_CS_PIN, GPIO_PIN_SET);

	GPIO_InitStruct.Pin    = PLL_SPI_CS_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PLL_SPI_CS_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = PLL_MUTE_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PLL_MUTE_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = PLL_MUXOUT_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PLL_MUXOUT_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = PLL_LD_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(PLL_LD_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(PLL_SPI_CS_PORT, PLL_SPI_CS_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PLL_MUTE_PORT, PLL_MUTE_PIN, GPIO_PIN_SET);

}

static void vv_pll_sw_cs(uint8_t value)
{
	switch (value)
	{
		case 1:
			HAL_GPIO_WritePin(PLL_SPI_CS_PORT, PLL_SPI_CS_PIN, GPIO_PIN_SET);
			break;
		case 0:
			HAL_GPIO_WritePin(PLL_SPI_CS_PORT, PLL_SPI_CS_PIN, GPIO_PIN_RESET);
			break;
		default:
			printf("err pll cs value.\r\n");
			break;
	}
}

static void vv_pll_write_1_reg(uint32_t reg)
{
	if (vv_pll_gpio_init_flg == 0)
	{
		vv_pll_gpio_init_flg = 1;
		vv_pll_init_gpio();
	}

	vv_pll_sw_cs(0);
	vv_pll_spi_send_byte((reg & 0xff000000) >> 24);
	vv_pll_spi_send_byte((reg & 0x00ff0000) >> 16);
	vv_pll_spi_send_byte((reg & 0x0000ff00) >> 8);
	vv_pll_spi_send_byte((reg & 0x000000ff));
	vv_pll_sw_cs(1);
}

static void vv_pll_write_all_reg(uint32_t * reg)
{
	for (int i = 0; i < 6; i++)
	{
		vv_pll_write_1_reg(*(reg + 5 - i));
	}
}

static uint8_t vv_pll_set_all_reg_and_wait_lock(uint32_t * reg)
{
	vv_pll_write_all_reg(reg);
	for (int i = 0; i < 100; i++)
	{
		if (HAL_GPIO_ReadPin(PLL_LD_PORT, PLL_LD_PIN) == GPIO_PIN_SET)
		{
			printf("pll locked.\r\n");
			return STATUS_OK;
		}
		else
		{
			printf("-");
			osDelay(50);
		}

	}
	printf("pll lock timeout.\r\n");
	return STATUS_ERROR;
}

uint8_t vv_pll_read_lock(void)
{
	if (HAL_GPIO_ReadPin(PLL_LD_PORT, PLL_LD_PIN) == GPIO_PIN_SET)
	{
		printf("+");
		return STATUS_OK;
	}
	else
	{
		printf("-");

	}
	return STATUS_ERROR;
}


//Here is a brief of adf4351 reg computing method, more details refer to adf4351 spec.
//
//rfout = (int + frac/mod) * fpfd/rfdivider
//frac = 0 - (mod-1)
//mod = refin / fres , range 2- 4095
//rfdivider choose from 1,2,4,8,16,32,64

//fpfd (freq of phase frequency detector) = refin * (1+D) / R /(1+T)
//why fpfd not equal to refin?
//there are some logic in front of pfd refin input,  contains a douber, a 10bit counter, and a /2 divider.
//so this formula used to caculate fpfd.

//D = refin doubler, 0 or 1.
//R = rf reference division factor 1 to 1023, indicate the 10bit counter.
//T = RF divide by 2 or not ,1 or 0

//example get a 2112.6MHZ at channel width 200khz by 10MHZ input

//first step, caculate vco freq:
//vco can generate freq from 2.2 to 2.4G, rfout in this case < 2.2 G,
//so a rfout divider should be used, and divide factor = 2.
//then, vco freq = 2112.6 * 2 = 4225.2Mhz;
//then, the channel width changed to 200*2 = 400KHZ.
//then, mod(modulus) = refin / channel width = 10M/400khz = 25.

//step2, caculate fpd freq:
//fpfd = refin * (1+D)/(R*(1+T)) = 10MHZ * (1+D) / (R*(1+T))
//because refin already less than 32M, so we choose fpfd = 10Mhz,
//so D = 0, R = 1, T = 0;

//so the total divider factor in feedback channel should be 4225.2Mhz / 10mhz
//= 422.52 , so we should make int + frac/mod = 422.52,
//which means int = 422 and frac/mod = 0.52, mod = 25(step2), so frac = 13.
//also should notice that the int value range 23 to 65535.

//notice, also should check the int value (n counter value) should between 75 - 65535 (8/9).

//#define pll_printf 	printf
#define pll_printf
/**
 * vv_pll_get_reg_by_freq, 
 * Caclute the reg value by a target frequency. 
 */
static uint8_t vv_pll_get_reg_by_freq(uint32_t freq, uint8_t main_ch_sw, uint8_t aux_ch_sw, uint8_t main_ch_power, uint8_t aux_ch_power, uint32_t * adf4351_reg_ptr)
{
	uint8_t rf_divider = 1;
	uint8_t rf_divider_reg;
	uint64_t rf_fvco = 0;
	//uint32_t rf_bd_vco = 0;

	uint32_t clk_in = 10000000; //10M clk in
	uint16_t clk_in_r_counter = 125; //10bit r counter range 1 - 1023
	uint16_t clk_in_doubler = 1; //1,2

	uint32_t if_mod = 0;
	uint32_t pfd_freq = 0;
	uint32_t if_int = 0;
	uint32_t if_frac = 0;

	uint32_t adf_reg_ref[6] = { 0x61A80000, 0x8008011, 0x181F5E42, 0x4B3, 0xA0113C, 0x580005 };

	if (freq < FREQ_RANGE_0) //uint32_t can not over 4.4ghz
	{
		pll_printf(("Err freq < 35Mhz.\r\n"));
		return 0;
	}
	else if (freq > 2200000000)
	{
		//freq will be generate from vco directly.
		rf_fvco = freq;
		rf_divider = 1;
	}
	else //rf < 2200000000, rf devider
	{
		rf_fvco = freq;

		for (int i = 2;  i <= 7; i++)
		{
			rf_fvco *= 2;
			rf_divider *= 2;
			if (rf_fvco  >= 2200000000)
			{
				break;
			}
			//pll_printf("freq = %lu,  current loop freq = %lu.\r\n", freq, pow(2, i) , freq * pow(2, i));
		}
	}

	if ((main_ch_power > 3) || (aux_ch_power > 3))
	{
		pll_printf("Err channel power input, return.\r\n");
		return STATUS_ERROR;
	}

	switch (rf_divider)
	{
		case 1:
			rf_divider_reg = 0;
			break;
		case 2:
			rf_divider_reg = 1;
			break;
		case 4:
			rf_divider_reg = 2;
			break;
		case 8:
			rf_divider_reg = 3;
			break;
		case 16:
			rf_divider_reg = 4;
			break;
		case 32:
			rf_divider_reg = 5;
			break;
		case 64:
			rf_divider_reg = 6;
			break;
		default:
			pll_printf("Err rf divider value = [%u]", rf_divider);
			break;
	}

	pll_printf("freq = [%luHz][%lfMHz].\r\n", freq, (float)freq / (float)1000000);
	pll_printf("rf_fvco = [%lluHz][%lluMHz].\r\n", rf_fvco, rf_fvco / 1000000);
	pll_printf("rf_divider = [%u].\r\n", rf_divider);

	uint8_t revise_pdf_freq = 0;
	uint32_t pdf_center_frq = 80000;
	pfd_freq = pdf_center_frq;
	uint8_t plus_mode = 1;
	uint32_t max_r_counter = 0;
	max_r_counter = (uint32_t)((uint64_t)(10000000) / (rf_fvco / 65536));

	pll_printf("max r counter = [%lu].\r\n", max_r_counter);

	while (1)
	{

		//clk_in_r_counter = clk_in / pfd_freq;
		pfd_freq = clk_in * clk_in_doubler / clk_in_r_counter;
		if_int =  rf_fvco / pfd_freq;
		pll_printf("set pdf freq  = [%lu].\r\n", pfd_freq);


		if (pfd_freq > 32000000) //max pfd freq 32MHz.
		{
			pll_printf("Error pfd freq = [%luMHz] > 32MHz. return.\r\n", pfd_freq / 10000000);
			return STATUS_ERROR;
		}
		else if (if_int > 65535)
		{
			pll_printf("Err if_int value > 65535. return.\r\n");
			return STATUS_ERROR;

		}
		else
		{
			pll_printf("pfd freq = %luHz, loop filter range = %u - %u, r_cntr = %d.\r\n", pfd_freq, pfd_freq / 20, pfd_freq / 10, clk_in_r_counter);
		}

		// rf_vco = (if_int + if_frac/if_mod) * pfd_freq;
		uint32_t if_temp, if_temp_2[2];
		if_temp = rf_fvco - if_int * pfd_freq;

		if (if_temp == 0)
		{
			if_frac = 0;
			if_mod = 2;
		}
		else
		{
			vv_pll_reduction(if_temp, pfd_freq, if_temp_2);
			if_frac = if_temp_2[0];
			if_mod  = if_temp_2[1];
		}

		if ((if_mod > 4095) || (if_mod < 2))
		{

			if (plus_mode == 1)
			{
				clk_in_r_counter++;


				if (clk_in_r_counter >= (2 * max_r_counter))
				{
					plus_mode = 0;
					clk_in_r_counter = 124;
					pll_printf(" -- mode.");
					continue;
				}
				else if (clk_in_r_counter >= max_r_counter)
				{
					clk_in_doubler = 2;
					pll_printf("Reset fpdf value = [],  if_mod = [%lu], if_frac = [%lu].\r\n",  if_mod, if_frac);
					continue;
				}
				else if (clk_in_r_counter > 250)
				{
					plus_mode = 0;
					pll_printf("Err if_mod vaule :[%lu], if_frac = [%lu], return, Maybe a unsupported freqency.\r\n", if_mod, if_frac);
					return STATUS_ERROR;
				}
				else
				{

					pll_printf("Reset fpdf value = [],  if_mod = [%lu], if_frac = [%lu].\r\n",  if_mod, if_frac);
					continue;
				}
			}
			else if (plus_mode == 0)
			{
				clk_in_r_counter--;
				if (clk_in_r_counter >= 80)
				{
					pll_printf("if_mod = [%lu], if_frac = [%lu].\r\n",  if_mod, if_frac);
					continue;
				}
				else
				{
					pll_printf("Err if_mod vaule :[%lu], if_frac = [%lu], return, Maybe a unsupported freqency.\r\n", if_mod, if_frac);
					return STATUS_ERROR;
				}
			}

		}
		else if (if_frac > (if_mod - 1))
		{
			pll_printf("Err if_frac vaule :[%d], return.\r\n", if_int);
			return STATUS_ERROR;
		}
		else
		{
			pll_printf("if_mod = [%lu], if_int = [%d], if_frac = [%d].\r\n", if_mod, if_int, if_frac);
			break;
		}
	}

	*adf4351_reg_ptr = (if_int << 15) + (if_frac << 3) + 0;
	*(adf4351_reg_ptr + 1) = (0xefff8000 & adf_reg_ref[1]) + (((clk_in_doubler == 2) ? 1 : 0) << 25) + (if_mod << 3) + 1;
	*(adf4351_reg_ptr + 2) = (adf_reg_ref[2] & 0xff003fff) + ((uint32_t)clk_in_r_counter << 14);
	*(adf4351_reg_ptr + 3) = adf_reg_ref[3];
	*(adf4351_reg_ptr + 4) = (adf_reg_ref[4] & 0xff8fffff) + ((uint32_t)rf_divider_reg << 20);

	if (main_ch_sw == 1) //db5
	{
		*(adf4351_reg_ptr + 4) |= (1 << 5);
	}
	else
	{
		*(adf4351_reg_ptr + 4) &= (~(1 << 5));
	}

	if (aux_ch_sw == 1) //db8
	{
		*(adf4351_reg_ptr + 4) |= (1 << 8);
	}
	else
	{
		*(adf4351_reg_ptr + 4) &= (~(1 << 8));
	}

	*(adf4351_reg_ptr + 4) &= 0xffffff27;
	*(adf4351_reg_ptr + 4) += ((main_ch_power << 3) + (aux_ch_power << 6));
	*(adf4351_reg_ptr + 5) = adf_reg_ref[5];
	pll_printf(" adf4351_reg = [0x%x][0x%x][0x%x][0x%x][0x%x][0x%x].\r\n", adf4351_reg_ptr[0], adf4351_reg_ptr[1], adf4351_reg_ptr[2], adf4351_reg_ptr[3], adf4351_reg_ptr[4], adf4351_reg_ptr[5]);
	return	STATUS_OK;
}

uint8_t vv_pll_set_freq(uint32_t freq, uint8_t main_ch_sw, uint8_t aux_ch_sw, uint8_t main_ch_power, uint8_t aux_ch_power, uint8_t power)
{
	uint32_t adf_reg[6];
	if (vv_pll_get_reg_by_freq(freq, main_ch_sw, aux_ch_sw, main_ch_power, aux_ch_power, adf_reg))
	{
		return vv_pll_set_all_reg_and_wait_lock(adf_reg);
	}
	else
	{
		return STATUS_ERROR;
	}
}

