#ifndef __VV_EZAT_CMD_FUNCTION_H
#define __VV_EZAT_CMD_FUNCTION_H
	
	#include "includes.h"
	uint8_t cmd_function_readver ( uint8_t * cmd, uint8_t * return_string );
	uint8_t cmd_function_readid ( uint8_t * cmd, uint8_t * return_string );
	uint8_t cmd_function_setfreq ( uint8_t * cmd, uint8_t * return_string );
	uint8_t cmd_function_setgain ( uint8_t * cmd, uint8_t * return_string );
	uint8_t cmd_function_vv ( uint8_t * cmd, uint8_t * return_string );
	uint8_t cmd_function_calib_gain(uint8_t * cmd, uint8_t * return_string);
#endif