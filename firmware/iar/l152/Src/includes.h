//includes.h
#ifndef __INCLUDES_H
#define __INCLUDES_H

	#define STATUS_ERROR 	0
	#define STATUS_OK 		1


	#include "stm32l1xx_hal.h"
	#include "cmsis_os.h"

	#include "string.h"
	#include "stdio.h"

	#include "board.h"
	#include "main.h"
	#include "vv_dbg.h"
	#include "vv_pll.h"
	#include "vv_filter.h"
	#include "vv_freq.h"
	#include "vv_pwr.h"
	#include "vv_dds.h"
	#include "vv_pa.h"
	#include "vv_clksrc.h"
	#include "vv_ezat.h"
	#include "vv_ezat_cmd_function.h"
	#include "vv_config.h"

#endif
