//vv_dgb.h
#ifndef __VV_DBG_H
#define __VV_DBG_H
	void vv_dbg_led_init_gpio (void);
	void vv_dbg_led_sw (uint8_t value);
	extern UART_HandleTypeDef huart1;
	extern UART_HandleTypeDef huart2;

	void print_bytes(uint8_t *data, uint16_t length);
	void vv_dbg_enable_all_gpio_clk (void);

#endif
