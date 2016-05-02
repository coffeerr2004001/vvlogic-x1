/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */

#include "includes.h"

//AT_READVER?
uint8_t cmd_function_readver(uint8_t * cmd, uint8_t * return_string)
{
	uint8_t temp_string[24];

	sprintf(temp_string, "Halver=%08x,", HAL_GetHalVersion());
	strcat(return_string, "v3,");
	strcat(return_string, temp_string);
	memset(temp_string, 0, sizeof(temp_string));
	sprintf(temp_string, "revid=%08x", HAL_GetREVID());
	strcat(return_string, temp_string);
	return STATUS_OK;
}

uint8_t cmd_function_readid(uint8_t * cmd, uint8_t * return_string)
{
	strcat(return_string, "3");
	return STATUS_OK;
}

uint8_t cmd_function_vv(uint8_t * cmd, uint8_t * return_string)
{
	strcat(return_string, "OK");
	return STATUS_OK;
}

uint8_t cmd_function_setfreq(uint8_t * cmd, uint8_t * return_string)
{
	//mode 0
	//support command: "AT+SETFREQ=10M."**
	//support command: "AT+SETFREQ=10MHZ."
	//support command: "AT+SETFREQ=10KHZ."
	//support command: "AT+SETFREQ=10K." **
	//support command: "AT+SETFREQ=10HZ."
	

	//mode 1
	//support command: "AT+SETFREQ=10MHZ,GAIN=10."
	//support command: "AT+SETFREQ=4000000000HZ,GAIN=10."
	//

	//mode 2
	//support command: "AT+SETFREQ=4000000000HZ,POWER=10DBM."
	//support command: "AT+SETFREQ=4000000000HZ,POWER=-10DBM."
	//
	
	uint8_t * seek_ptr;
	uint8_t * seek_ptr_2;
	uint8_t * begin_ptr;
	uint8_t cmd_len = strlen(cmd);
	uint64_t freq;
	uint8_t gain;
	int8_t power;
	uint8_t mode;

	seek_ptr = (uint8_t *)strstr(cmd, ",GAIN=");
	if (seek_ptr != NULL)
	{
		//printf("freq and gain command.\r\n");

		//printf("[%c] -- [%c],[%lu]-[%lu]\r\n", *(seek_ptr+6), *(seek_ptr + strlen(seek_ptr)-1),(seek_ptr+6), (seek_ptr + strlen(seek_ptr)-1));
		gain = read_number_from_rfcmd(seek_ptr + 6, seek_ptr + strlen(seek_ptr) - 1);
		//printf("gain = %u.\r\n", gain);
		mode = 1;
		
	}
	else
	{
		
		seek_ptr = (uint8_t *)strstr(cmd, ",POWER=");
		if (seek_ptr != NULL)
		{
			//seek_ptr_2 = seek_ptr;

			if ((*(seek_ptr + strlen(seek_ptr) - 4) == 'D')&&(*(seek_ptr + strlen(seek_ptr) - 3) == 'B')&&(*(seek_ptr + strlen(seek_ptr) - 2) == 'M'))
			{
				if (*(seek_ptr +7) == '-')
				{
					power = read_number_from_rfcmd(seek_ptr + 8, seek_ptr + strlen(seek_ptr) - 4);
					power = 0- power;
				}
				else
				{
					power = read_number_from_rfcmd(seek_ptr + 7, seek_ptr + strlen(seek_ptr) - 4);
				}
				//printf("power = [%d] dbm.\r\n", power);
				mode = 2;
			}

		}
		else
		{
			mode = 0;
			//printf("only freq command.\r\n");
			seek_ptr = (uint8_t *)(cmd + strlen(cmd) - 1);			
		}
		
	}

	//printf("cmd = [%s], length = [%lu], beginseek = [%c][%c][%c][%c][%c].\r\n", cmd, seek_ptr - cmd, *(seek_ptr - 5), *(seek_ptr - 4), *(seek_ptr - 3), *(seek_ptr - 2), *(seek_ptr - 1));

	if ((seek_ptr - cmd) > 26)
	{
		strcpy(return_string, "Err command length");
		return STATUS_ERROR;
	}


	if ((*(seek_ptr - 2) == 'H') && (*(seek_ptr - 1) == 'Z'))
	{
		seek_ptr -= 2;
	}
	else
	{

	}


	if (*(seek_ptr - 1) == 'M')
	{
		freq = read_number_from_rfcmd(cmd + 11, seek_ptr - 1);
		printf("%lld mhz.\r\n", freq);
		freq *= 1000000;
	}
	else if (*(seek_ptr - 1) == 'K')
	{
		freq = read_number_from_rfcmd(cmd + 11, seek_ptr - 1);
		printf("%lld khz.\r\n", freq);
		freq *= 1000;
	}
	else if ((*(seek_ptr - 1) >= '0') && (*(seek_ptr - 1) <= '9'))
	{
		freq = read_number_from_rfcmd(cmd + 11, seek_ptr);
		printf("%lld hz.\r\n", freq);
	}
	else
	{
		strcpy(return_string, "Err command");
		return STATUS_ERROR;
	}


	if (mode == 0) //set freq only.
	{

	}
	else if (mode == 1) //set freq and gain
	{
		vv_pa_write_gain(gain);
	}
	else //set freq and power
	{
		uint8_t ref_gain;
		ref_gain = vv_get_calib_value_by_freq(freq);

		gain = (uint8_t)(ref_gain + (power - (-10))*2) ;
		printf("ref gain  = %u, mode 2 gain = %u.\r\n", ref_gain, gain);

		if (gain >63)
		{
			strcpy(return_string, "Power out range,");
			return STATUS_ERROR;
		}

		vv_pa_write_gain(gain);
	}
	

	if ((freq < FREQ_RANGE_0) ||  (freq > FREQ_RANGE_7))
	{
		strcpy(return_string, "Err frequency input.\r\n");
		return STATUS_ERROR;
	}

	if (STATUS_OK != vv_freq_set(freq))
	{
		strcpy(return_string, "Unsupported freq,");
		return STATUS_ERROR;
	}

	if (STATUS_OK != vv_filter_freq(freq))
	{
		strcpy(return_string, "Filter channel err,");
		return STATUS_ERROR;
	}

	strcpy(return_string, "Set freq done");
	return STATUS_OK;
}


uint8_t cmd_function_setgain(uint8_t * cmd, uint8_t * return_string)
{
	//support command: "AT+SETGAIN=0."
	//support command: "AT+SETGAIN=15."
	uint8_t gain = 0;

	if (cmd[12] == '.')
	{

		gain = cmd[11] - '0';
	}
	else if (cmd[13] == '.')
	{
		gain = 10 * (cmd[11] - '0') + (cmd[12] - '0');
	}
	else
	{
		strcpy(return_string, "command format error");
		return STATUS_ERROR;
	}

	if (gain > 63)
	{
		strcpy(return_string, "gain out range error");
		return STATUS_ERROR;
	}

	printf("gain = %u.\r\n", gain);
	vv_pa_write_gain(gain);
	strcpy(return_string, "set gain done");
	return STATUS_OK;
}



uint8_t cmd_function_calib_gain(uint8_t * cmd, uint8_t * return_string)
{
	//support command: "AT+CALIBGAIN=1,INDEX=10."
	//support command: "AT+CALIBGAIN=10,INDEX=5."
	//support command: "AT+CALIBGAIN=10,INDEX=50."

	uint8_t * seek_ptr;
	

	uint8_t gain = 0;
	uint8_t index = 0;

	seek_ptr = (uint8_t *)strstr(cmd, ",INDEX=");
	if (seek_ptr != NULL)
	{
		if ((seek_ptr - cmd) == 14)
		{
			gain = read_number_from_rfcmd(cmd+13,cmd+14);
		}
		else if ((seek_ptr - cmd) == 15)
		{
			gain = read_number_from_rfcmd(cmd+13,cmd+15);
		}
		else
		{
			printf("Gain length error.\r\n");
			
			return STATUS_ERROR;
		}
	}
	else
	{
		return STATUS_ERROR;
	}

	index = read_number_from_rfcmd(seek_ptr+7,cmd+strlen(cmd)-1);
	
	vv_set_calib_point(index,gain);
	printf("Write calibration data, gain = %u, index = %u.\r\n", gain,index);

	strcpy(return_string, "Calib gain done");
	return STATUS_OK;
}
