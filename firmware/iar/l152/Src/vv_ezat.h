//vv_ezat.h
#ifndef __VV_EZAT_H
#define __VV_EZAT_H
	
	#include "includes.h"

	#define EZAT_UART	huart1

	#define EZAT_RX_DMA_BUFFER_LENGTH 128
	#define EZAT_RX_RAW_BUFFER_LENGTH 128

	#define MAX_EZAT_CMD_LENGTH 48

	#define MAX_EZAT_CMD_MAILBOX_LENGTH 16


	typedef struct {
			uint8_t index;
			uint8_t cmd_length;
			uint8_t cmd[MAX_EZAT_CMD_LENGTH];
		} ezat_cmd_t;

	typedef uint8_t (*cmd_func_handle)(uint8_t * cmd, uint8_t * return_string);


	#define PMBIT_ALL	0xFF
	#define PMBIT_NULL	0

	extern uint8_t ez_rx_dma_buffer[EZAT_RX_DMA_BUFFER_LENGTH];
	extern uint8_t ez_rx_raw_buffer[EZAT_RX_RAW_BUFFER_LENGTH];

	void task_ez_rx_func_top(void const * argument);
	void vv_task_ezat_top(void const * argument);
	uint8_t vv_ezat_search_cmd_from_buffer (uint8_t * cmd_buffer, ezat_cmd_t * ezat_cmd_ptr);
	void vv_ezat_uart_send_string (uint8_t * inchar, uint8_t length);

	//uint8_t vv_stm8_do_ezat (uint8_t *cmd, uint8_t * return_string);
	uint8_t vv_stm8_do_ezat (uint8_t *cmd);
	uint64_t read_number_from_rfcmd (uint8_t * ptr_begin, uint8_t * ptr_end);


#endif