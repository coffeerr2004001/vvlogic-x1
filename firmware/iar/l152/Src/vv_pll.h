//vv_pll.h
#ifndef __VV_PLL_H
#define __VV_PLL_H

	#define pll_spi_handle  hspi1

	void vv_pll_adf4351_wait_lock(void);
	void vv_pll_write_1_reg (uint32_t value);
	void vv_pll_write_all_reg(uint32_t *reg);
	uint8_t vv_rfgen_set_freq(uint32_t freq);
	uint8_t vv_pll_set_freq(uint32_t freq, uint8_t main_ch_sw, uint8_t aux_ch_sw, uint8_t main_ch_power, uint8_t aux_ch_power, uint8_t power);
	uint8_t vv_pll_read_lock (void);
#endif
