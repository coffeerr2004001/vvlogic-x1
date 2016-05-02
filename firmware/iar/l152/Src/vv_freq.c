/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

void vv_task_freq_top(void const * argument)
{
	vv_freq_init();

	while(1)
	{
		osDelay(100);
	}
}

//Unit Hz
uint8_t vv_freq_set (uint64_t freq)
{
	uint32_t tmp_int;
	uint32_t pll_freq;
	uint32_t tmp_status;

	if ((freq < FREQ_RANGE_0) || (freq > FREQ_RANGE_7)) //100Khz
	{
		printf("Unsupported freq = [%llu].\r\n", freq);
		return STATUS_ERROR;
	} 
	else if (freq >= FREQ_RANGE_3)  //from range 3 to range 7, signal generated from pll main channel.
	{
		return vv_pll_set_freq(freq, 1, 0, 3, 0,  0);
	}
	else if (freq >= FREQ_RANGE_1)  //from range 1 to range 2, signal generated from pll aux channel.
	{
		return vv_pll_set_freq(freq, 1, 1, 3, 0,  0);
	} 
	else //freq < 35MHz
	{
		//vv_stm32_rfgen_source_select(FREQ_LOW);
		tmp_int = 140000000 / freq;

		if (tmp_int > 10)
		{
			tmp_int = tmp_int / 10 * 10;
		}

		pll_freq = tmp_int * freq;
		tmp_status = vv_pll_set_freq(pll_freq, 1, 1, 0, 3, 0);
		vv_dds_set_freq(0, freq, pll_freq);
		return tmp_status;

	}
}

void vv_freq_init(void)
{
	vv_pa_en_pwr(1);
	osMutexWait(sem_gpio_initHandle,osWaitForever);
	osMutexRelease(sem_gpio_initHandle);
	
	vv_load_calib_table();
	vv_print_calib_table();
	
	vv_clksrc_sel(CLK_SRC_OSC_TO_PLL_AND_SYNC);

}
