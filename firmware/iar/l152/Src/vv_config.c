/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */

#include "includes.h"

extern ADC_HandleTypeDef hadc;
uint32_t freq_range[] = 
{
	FREQ_RANGE_0,
	FREQ_RANGE_1,
	FREQ_RANGE_2,
	FREQ_RANGE_3,
	FREQ_RANGE_4,
	FREQ_RANGE_5,
	FREQ_RANGE_6,
	FREQ_RANGE_7,
};

calib_point_t calib_table[CALIB_POINT_COUNT];

/**
 * Write a config bytes to stm32l internal eeprom.
 * 
 * @author coffeerr2004001 (2016/4/10)
 * 
 * @param index 
 * @param value 
 */
static void vv_write_config_byte (uint8_t index, uint8_t value)
{
	if (HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) 
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_ADDRESS+index, value);
		HAL_FLASHEx_DATAEEPROM_Lock ();
	}
	else
	{
		printf("Flash error.\r\n");
	}
}

static uint8_t vv_read_config_byte (uint8_t index)
{
	return *(uint8_t *)(DATA_EEPROM_ADDRESS + index);
}



static uint8_t vv_read_calib_byte (uint8_t index)
{
	return *(uint8_t *)(DATA_EEPROM_CALIB_ADDRESS + index);
}

static void vv_write_calib_byte (uint8_t index, uint8_t value)
{
 	printf("index = [%d], value = [%d].\r\n", index, value);
	if (HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) 
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_CALIB_ADDRESS + index, value);
		HAL_FLASHEx_DATAEEPROM_Lock ();
	}
	else
	{
		printf("Flash error.\r\n");
	}
}

uint16_t vv_get_calib_point_index (uint32_t freq)
{
	uint8_t start_index ;
	uint32_t inrange_interval;
	int i;
	for (i = 0; i<7; i++)
	{
		if ((freq >=freq_range[i]) &&(freq <freq_range[i+1]))
		{
			break;
		}
	}
	if (i >=7)
	{
		printf("Get start index error.\r\n");
		return 0;
	}
	else 
	{
		start_index = i;
	}

	inrange_interval = (freq_range[start_index+1] - freq_range[start_index])/CALIB_COUNT_PER_RANGE;
	
	return start_index*CALIB_COUNT_PER_RANGE + (freq-freq_range[start_index])/inrange_interval;
}


/**
 * Set the gain value for -10dbm.
 * 
 * @author coffeerr2004001 (2016/3/20)
 * 
 * @param index 
 * @param ref_gain 
 */
void vv_set_calib_point (uint16_t index, uint8_t gain)
{
	if (index >= CALIB_POINT_COUNT)
	{
		printf("Err calib point index.\r\n");
		return;
	}

	vv_write_calib_byte(index, gain);
}

/**
 * Get the gain value for -10dbm.
 * 
 * @author coffeerr2004001 (2016/3/20)
 * 
 * @param index 
 * @param ref_gain 
 */
uint8_t vv_get_calib_point (uint16_t index)
{
	return vv_read_calib_byte(index);
}

/**
 * Read a PA gain setting from calib_table[], which is read from
 * eeprom and initialized with device powerup. 
 * This calib table value comes from a caliberation process by a
 * rf power meter. 
 * 
 * @author coffeerr2004001 (2016/4/10)
 * 
 * @param freq 
 * 
 * @return uint8_t 
 */
uint8_t vv_get_calib_value_by_freq (uint32_t freq)
{
	uint16_t index_begin;
	uint16_t index_end;

	int gain = 0;
	int tmp_gain_range = 0;
	uint32_t tmp_freq_range = 0;

	if ((freq < FREQ_RANGE_0) ||  (freq > FREQ_RANGE_7))
	{
		printf("Err frequency input.\r\n");
		return 0;
	}

	for (int i = 0; i <CALIB_POINT_COUNT-1; i++)
	{
		if ((freq >= calib_table[i].freq) && (freq < calib_table[i+1].freq))
		{
			index_begin = i;
			index_end = i + 1;
			printf("[%d][%lu] -- [%d][%lu].\r\n",index_begin,calib_table[index_begin].freq,index_end,calib_table[index_end].freq);

			tmp_gain_range = (int)calib_table[index_end].gain - (int)calib_table[index_begin].gain;
			printf("gain rang = %d.\r\n", tmp_gain_range);

			tmp_freq_range = (calib_table[index_end].freq - calib_table[index_begin].freq);
			printf("freq rang = %lu.\r\n", tmp_freq_range);

			gain = (uint8_t)((long double)(freq - calib_table[index_begin].freq) * tmp_gain_range /tmp_freq_range + 0.5)  
					+ calib_table[index_begin].gain;
			printf("gain = %d.\r\n",gain);
		}
	}
	
	if (gain <= 63)
	{
		return (uint8_t)gain;
	}

	printf("gain out range.\r\n");
	return 0;
	
}


/**
 * Load calib_table[] from eeprom 
 * 
 * @author coffeerr2004001 (2016/4/10)
 */
void vv_load_calib_table (void)
{
	uint32_t freq_spacing = 0;
	uint32_t freq;

	//printf("Initial calibration table:\r\n");

	for (int i = 0 ;i < 7;i++)
	{
		freq_spacing = (freq_range[i+1] - freq_range[i])/(CALIB_COUNT_PER_RANGE-1);
		for (int j = 0; j<(CALIB_COUNT_PER_RANGE-1); j++)
		{
			freq = freq_range[i]+ freq_spacing*j;
			//printf("calib point = [%lu],write to [%u].\r\n",freq,i*CALIB_COUNT_PER_RANGE + j);
			calib_table[i*CALIB_COUNT_PER_RANGE + j].freq = freq;
			calib_table[i*CALIB_COUNT_PER_RANGE + j].gain = vv_get_calib_point(i*CALIB_COUNT_PER_RANGE + j);
		}
		freq = freq_range[i+1]-1;
		//printf("calib point = [%lu],write to [%u].\r\n\r\n",freq,i*CALIB_COUNT_PER_RANGE + CALIB_COUNT_PER_RANGE-1);
		calib_table[i*CALIB_COUNT_PER_RANGE + CALIB_COUNT_PER_RANGE-1].freq = freq;
		calib_table[i*CALIB_COUNT_PER_RANGE + CALIB_COUNT_PER_RANGE-1].gain = vv_get_calib_point(i*CALIB_COUNT_PER_RANGE + CALIB_COUNT_PER_RANGE-1);
	}
}

/**
 * Print calib_table[] for debug.
 * 
 * @author coffeerr2004001 (2016/4/10)
 */
void vv_print_calib_table(void)
{
	printf("Calibration table, [%d] points.\r\n", CALIB_POINT_COUNT);
	for (int i = 0; i<CALIB_POINT_COUNT; i++)
	{
		printf("[%u]:freq=[%lu],gain\(-10dBm\)=[%u].\r\n",i,calib_table[i].freq,calib_table[i].gain);
	}
}

/**
 * Generate a random byte by adc temperature channel, bit0
 * 
 * @author coffeerr2004001 (2016/4/10)
 * 
 * @return uint8_t 
 */
static uint8_t vv_random_byte (void)
{
	uint8_t value = 0;
	

	for (int i = 0; i<8; i++)
	{
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 1000);
		value += (
				  ((HAL_ADC_GetValue(&hadc)&0x01))<<i
				  );
	}
	return value;
}

/**
 * Generator a random frequency for debug.
 * 
 * @author coffeerr2004001 (2016/4/10)
 * 
 * @return uint32_t 
 */
uint32_t vv_random_freq (void)
{
	uint32_t freq = 0 ;
	while (1)
	{
		for (int i = 0; i< 4 ; i ++)
		{
			freq  = (freq << (i*8));
			freq += vv_random_byte();
		}

		if ((freq <= FREQ_RANGE_7) && (freq >= FREQ_RANGE_0))
		{
			break;
		}
	}

	return freq;
}

uint32_t vv_random_freq_10hz (void)
{
	uint32_t freq = 0 ;
	while (1)
	{
		for (int i = 0; i< 4 ; i ++)
		{
			freq  = (freq << (i*8));
			freq += vv_random_byte();
		}

		if ((freq <= FREQ_RANGE_7) && (freq >= FREQ_RANGE_0))
		{
			break;
		}
	}

	return (freq/10)*10;
}

void vv_config_tim6 (uint16_t period)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 32;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;

  htim6.Init.Period = period; //150
  HAL_TIM_Base_Init(&htim6);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

}

//only a beep ...
void vv_beep(uint8_t value)
{
	switch (value)
	{
		case STATUS_OK:
			vv_config_tim6(150);
			HAL_TIM_Base_Start(&htim6);	
			osDelay(50);
			HAL_TIM_Base_Stop(&htim6);
			osDelay(50);
			HAL_TIM_Base_Start(&htim6);	
			osDelay(50);
			HAL_TIM_Base_Stop(&htim6); 
			break;
		case STATUS_ERROR:
			vv_config_tim6(1500);
			HAL_TIM_Base_Start(&htim6);	
			osDelay(200);
			HAL_TIM_Base_Stop(&htim6); 
			break;
		default:
			vv_config_tim6(1500);
			HAL_TIM_Base_Start(&htim6);	
			osDelay(200);
			HAL_TIM_Base_Stop(&htim6); 
			break;
	}
}
