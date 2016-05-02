/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */
#include "includes.h"

static uint8_t vv_dds_gpio_init_flg = 0;
static void vv_dds_init_gpio (void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.Pin    = DDS_CLK_UPD_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DDS_CLK_UPD_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = DDS_SER_CLK_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DDS_SER_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = DDS_SER_DATA_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DDS_SER_DATA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = DDS_RESET_PIN;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DDS_RESET_PORT, &GPIO_InitStruct);
}

static void vv_dds_sw_reset (uint8_t value)
{
	switch (value)
	{
	case 0:
		HAL_GPIO_WritePin(DDS_RESET_PORT, DDS_RESET_PIN, GPIO_PIN_RESET); 
		break;
	case 1:
		HAL_GPIO_WritePin(DDS_RESET_PORT, DDS_RESET_PIN, GPIO_PIN_SET); 
		break;
	default:
		printf("dds reset error value.\r\n");
		break;
	}
}

static void vv_dds_sw_fqud (uint8_t value)
{
	switch (value)
	{
	case 0:
		HAL_GPIO_WritePin(DDS_CLK_UPD_PORT, DDS_CLK_UPD_PIN, GPIO_PIN_RESET); 
		break;
	case 1:
		HAL_GPIO_WritePin(DDS_CLK_UPD_PORT, DDS_CLK_UPD_PIN, GPIO_PIN_SET); 
		break;
	default:
		printf("dds fqud error value.\r\n");
		break;
	}
}

static void vv_dds_sw_serclk (uint8_t value)
{
	switch (value)
	{
	case 0:
		HAL_GPIO_WritePin(DDS_SER_CLK_PORT, DDS_SER_CLK_PIN, GPIO_PIN_RESET); 
		break;
	case 1:
		HAL_GPIO_WritePin(DDS_SER_CLK_PORT, DDS_SER_CLK_PIN, GPIO_PIN_SET); 
		break;
	default:
		printf("dds serclk error value.\r\n");
		break;
	}
}

static void vv_dds_sw_serdata (uint8_t value)
{
	switch (value)
	{
	case 0:
		HAL_GPIO_WritePin(DDS_SER_DATA_PORT, DDS_SER_DATA_PIN, GPIO_PIN_RESET); 
		break;
	case 1:
		HAL_GPIO_WritePin(DDS_SER_DATA_PORT, DDS_SER_DATA_PIN, GPIO_PIN_SET); 
		break;
	default:
		printf("dds serdata error value.\r\n");
		break;
	}
}

//reset ad9850

static void vv_dds_ad9850_reset(void)
{
	vv_dds_sw_serclk(0);
	vv_dds_sw_fqud(0);
	
	vv_dds_sw_reset(0);
	vv_dds_sw_reset(1);
	//osDelay(1);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	vv_dds_sw_reset(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
}

static void vv_dds_ad9850_reset_serial(void)
{
	vv_dds_sw_serclk(0);
	vv_dds_sw_fqud(0);
	
	vv_dds_sw_reset(0);
	vv_dds_sw_reset(1);
	vv_dds_sw_reset(0);
	
	vv_dds_sw_serclk(0);
	vv_dds_sw_serclk(1);
	vv_dds_sw_serclk(0);
	
	vv_dds_sw_fqud(0);
	vv_dds_sw_fqud(1);
	vv_dds_sw_fqud(0);
}


static void vv_dds_ad9850_write_reg (uint8_t reg_value)
{
	for (int i = 0; i < 8; i++)
	{
		if ((reg_value >> i) & 0x01)
		{
			vv_dds_sw_serdata(1);
		} 
		else
		{
			vv_dds_sw_serdata(0);
		}

		vv_dds_sw_serclk(1);
		vv_dds_sw_serclk(0);
	}
}

void vv_dds_set_freq(uint8_t w0, uint32_t frequence, uint32_t ref_clk)
{
	uint8_t w;
	uint64_t y;

	if (vv_dds_gpio_init_flg == 0)
	{
		vv_dds_gpio_init_flg = 1;
		vv_dds_init_gpio();
		
		//enable serial mode.
		vv_dds_sw_fqud(0);
		vv_dds_sw_serclk(0);
		vv_dds_sw_serclk(1);
		vv_dds_sw_serclk(0);
	
		vv_dds_sw_fqud(1);	
		vv_dds_sw_fqud(0);
	}
	

	//vv_dds_sw_reset(1);
	//osDelay(10);
	//vv_dds_sw_reset(0);
	//
	//vv_dds_ad9850_reset();

	y = (uint64_t)4294967296 * frequence / ref_clk ;
	
	//printf("frequence = %llu, y = %llu.\r\n",frequence,y);
	//printf("write reg:");
	for (int i = 0 ; i<4; i++)
	{
		w = (uint8_t) (y >> (i*8));
		//printf("0x%02x,",w);
		vv_dds_ad9850_write_reg(w);
	}
	//printf("\r\n");
	
	
	//write w0
	vv_dds_ad9850_write_reg(w0);
	vv_dds_sw_fqud(1);
	vv_dds_sw_fqud(0);
}


