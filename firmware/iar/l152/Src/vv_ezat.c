/** 
 * About vvlogic X1 firmware. 
 *  
 * This project is licensed under the 
 * terms of the MIT license. 
 *  
 * coffeerr2004001@gmail.com
 */

/**
 * EZAT framework was created to process command from serial port.
 * Command example: AT+VV? \\r\\n
 * Device return: APT,OK.\r\n
 */

#include "includes.h"
uint8_t ez_rx_dma_buffer[EZAT_RX_DMA_BUFFER_LENGTH];
uint8_t ez_rx_raw_buffer[EZAT_RX_RAW_BUFFER_LENGTH];

osMailQDef (ezat_cmd_mbx, MAX_EZAT_CMD_MAILBOX_LENGTH, ezat_cmd_t);
osMailQId  (ezat_cmd_mbx_id);

extern DMA_HandleTypeDef hdma_usart1_rx;

struct command_link_table
{
	uint8_t *command_name;
	cmd_func_handle command_function;	
	uint8_t permission_code;
};

struct command_link_table ezat_cmd[32] = 
{	
	{"+READVER", cmd_function_readver,PMBIT_ALL},
	{"+READID", cmd_function_readid, PMBIT_ALL}, 			
	{"+SETFREQ", cmd_function_setfreq, PMBIT_ALL}, 			
	{"+SETGAIN", cmd_function_setgain, PMBIT_ALL}, 
	{"+VV", cmd_function_vv, PMBIT_ALL}, 
	{"+CALIBGAIN", cmd_function_calib_gain, PMBIT_ALL},
};


void task_ez_rx_func_top(void const * argument)
{
	ezat_cmd_t  ezat_cmd_ptr;
	ezat_cmd_t  * ezat_cmd_mail_ptr;
	
	HAL_UART_Receive_DMA(&EZAT_UART, ez_rx_dma_buffer, EZAT_RX_DMA_BUFFER_LENGTH);
	ezat_cmd_mbx_id = osMailCreate(osMailQ(ezat_cmd_mbx), NULL);
	
	/* Infinite loop */
	while (1)
	{
	 	osDelay(10);
		//printf("ez_rx_dma_buffer_length = %ld.\r\n", strlen(ez_rx_dma_buffer));
	 	if (strlen(ez_rx_raw_buffer) <  5)
		{
			if (strlen(ez_rx_dma_buffer) != 0)
			{		
			 	osDelay(5); //wait for current command transfer finish.
				if ( strlen(ez_rx_raw_buffer) == 0)
				{
					memcpy(ez_rx_raw_buffer,ez_rx_dma_buffer,EZAT_RX_RAW_BUFFER_LENGTH);
				}
				else
				{
				 	memcpy(ez_rx_raw_buffer + strlen(ez_rx_raw_buffer),ez_rx_dma_buffer,EZAT_RX_RAW_BUFFER_LENGTH - strlen(ez_rx_raw_buffer));
				}
				
				HAL_UART_DMAStop(&EZAT_UART);
				memset(ez_rx_dma_buffer,0,sizeof(ez_rx_dma_buffer));				
				if (HAL_OK == HAL_UART_Receive_DMA(&EZAT_UART, ez_rx_dma_buffer, EZAT_RX_DMA_BUFFER_LENGTH))
				{
				 	//printf("*");
				}
				else
				{
					printf("uart dma start receive error.\r\n");
				}
				
			}
			else
			{
				//printf("~");
			}			
		}
		else
		{
		 	//printf("-");
			
			memset(&ezat_cmd_ptr,0,sizeof(ezat_cmd_t));
			if (STATUS_OK == vv_ezat_search_cmd_from_buffer(ez_rx_raw_buffer, &ezat_cmd_ptr))
			{
				//printf("cmd = [%s], len = [%d].\r\n",ezat_cmd_ptr.cmd, ezat_cmd_ptr.cmd_length);
			 
				memmove(ez_rx_raw_buffer, ez_rx_raw_buffer + ezat_cmd_ptr.cmd_length + 2,  EZAT_RX_RAW_BUFFER_LENGTH - ezat_cmd_ptr.cmd_length - 2);
				
				ezat_cmd_mail_ptr = (ezat_cmd_t *) osMailAlloc (ezat_cmd_mbx_id, 1);
				memcpy(ezat_cmd_mail_ptr, &ezat_cmd_ptr, sizeof(ezat_cmd_t));

				if (osOK != osMailPut(ezat_cmd_mbx_id,ezat_cmd_mail_ptr))
				{
				   printf("ezat command put error, discard this command.\r\n");
				}
				
			} 
			else //discard all command buffer.
			{
			 	memset(ez_rx_raw_buffer,0,EZAT_RX_RAW_BUFFER_LENGTH);			 	
			}
		}
	}
}

void vv_task_ezat_top(void const * argument)
{

	while (1)
	{
		osDelay(10);
		osEvent event = osMailGet(ezat_cmd_mbx_id, 1);
		if (event.status == osEventMail)
		{
			ezat_cmd_t * received = (ezat_cmd_t *)event.value.p;
			//printf("get mail = [%s].\r\n",received->cmd);
			osMailFree(ezat_cmd_mbx_id, received); 
			vv_stm8_do_ezat(received->cmd);
			//vv_ezat_uart_send_string("get mail",0);
		}
		else //mailbox empty
		{	
			
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
 	if( huart == &EZAT_UART)
  	{
    	HAL_DMA_GetError(&hdma_usart1_rx);
		printf("error, ezat uart buffer full.\r\n");
		memset(ez_rx_dma_buffer, 0,  sizeof(ez_rx_dma_buffer));
		HAL_UART_Receive_DMA(&EZAT_UART, ez_rx_dma_buffer, EZAT_RX_DMA_BUFFER_LENGTH);		
	}
	
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) 
{
	printf("uart error.\r\n");
}


static uint8_t vv_stm8_exec_cmd (uint8_t *cmd, uint8_t * return_string)
{
	uint8_t *seek_ptr;
	return_string[0] = '\0';
	uint16_t cmd_number = sizeof (ezat_cmd)/sizeof (ezat_cmd[0]);	
	for(uint8_t i = 0; i< cmd_number; i++)
	{
		seek_ptr = (char *)strstr ((char *)cmd, (char *)ezat_cmd[i].command_name);
		if(seek_ptr != NULL)
		{
			//check command permission.				
			if(
			   0xff
			   & 
			   (ezat_cmd[i].permission_code)
			   )
			{
				if (STATUS_OK == (* ezat_cmd[i].command_function)(cmd,return_string))
				{
					
				}
				else
				{
					strcat ( (char *)return_string , "CommandErr");
				}
			}
			else
			{				
				strcpy ( (char *)return_string , "CommandForbidden");				
			}
			
			return STATUS_OK;
		}
	}
	
	return STATUS_ERROR;	
}

uint8_t vv_stm8_do_ezat (uint8_t *cmd)
{
	uint8_t return_string_all [64] = {0};
	uint8_t return_string_temp [64] = {0};
	strcat ((int8_t * )return_string_all, "APT,"); 

	if (STATUS_OK == vv_stm8_exec_cmd(cmd, return_string_temp))
	{
		strcat ((int8_t * )return_string_all, return_string_temp);
	}
	else
	{
		strcat (return_string_all, "Unkown Command");
	}
	strcat (return_string_all, ".\r\n"); 

	vv_ezat_uart_send_string(return_string_all,0);
	return STATUS_OK;

}

uint8_t vv_ezat_search_cmd_from_buffer (uint8_t * cmd_buffer, ezat_cmd_t * ezat_cmd_ptr)
{
	uint8_t * seek_ptr_begin;
	uint8_t * seek_ptr_end;
	uint8_t * seek_ptr_end_2;
	uint32_t cmd_length = 0;

	seek_ptr_begin = strstr(cmd_buffer, "AT+");

	if (seek_ptr_begin == NULL)
	{
		//ezat_cmd_ptr = NULL;
		return STATUS_ERROR;
	}
	
	//printf("cmd_buffer = [0x%lx], seek_ptr_begin = [0x%lx].\r\n",(char*)cmd_buffer,(char*)seek_ptr_begin);
	//printf("cmd_buffer = [0x%08lx], seek_ptr_begin = [0x%08lx].\r\n",(char*)cmd_buffer,(char*)seek_ptr_begin);
	

	seek_ptr_end = strstr(cmd_buffer+3, ".\r\n");
	seek_ptr_end_2 = strstr(cmd_buffer+3, "?\r\n");

	if ( (seek_ptr_end == NULL) && (seek_ptr_end_2 == NULL))
	{
		//ezat_cmd_ptr = NULL;
		return STATUS_ERROR;
	}
	else if ( (seek_ptr_end == NULL) && (seek_ptr_end_2 != NULL))
	{
		seek_ptr_end = seek_ptr_end_2;
	}
	else if ( (seek_ptr_end != NULL) && (seek_ptr_end_2 == NULL))
	{
	
	}
	else
	{
	 	seek_ptr_end = ( seek_ptr_end_2 < seek_ptr_end )? seek_ptr_end_2:seek_ptr_end;		
	}
	
	//printf("cmd_buffer = [0x%lx], seek_ptr_begin = [0x%lx].\r\n",(char*)cmd_buffer,(char*)seek_ptr_begin);
	cmd_length = seek_ptr_end - seek_ptr_begin;
	//cmd_length = 0xff & (uint32_t)temp_ptr;
	//printf("cmd_buffer = [0x%lx], seek_ptr_begin = [0x%lx].\r\n",(char*)cmd_buffer,(char*)seek_ptr_begin);
	//printf("begin ptr = [0x%lx] - [0x%lx], len = [0x%lx].", (char*)seek_ptr_begin, (char*)seek_ptr_end, (char*)cmd_length);

	if (cmd_length > EZAT_RX_RAW_BUFFER_LENGTH)
	{
		return STATUS_ERROR;
	}

	memcpy ( &(ezat_cmd_ptr->cmd), seek_ptr_begin,cmd_length+1);
	ezat_cmd_ptr->cmd_length = (uint8_t)cmd_length + 1;

	return STATUS_OK;
}

/**
 *  send string to EZ_UART port by end char '\0' or by length.
 *  @param inchar
 *  @param length
 *  		0: length was determined by string length.
 *  		a non-zero number: user defined length.
 */
void vv_ezat_uart_send_string (uint8_t * inchar, uint8_t length)
{
	if(length == 0)
	{
		if (* inchar != '\0')
		{
			while (HAL_UART_Transmit_DMA(&EZAT_UART, inchar, strlen(inchar)) != HAL_OK)
			{
				//printf("dma send status = %d.\r\n",HAL_UART_Transmit_DMA(&EZAT_UART, "ezat.\r\n", 7));
				printf(".");
				osDelay(1);
			}
			//printf("ezat uart sent[%s].\r\n",inchar);
		}
		else
		{
			return;
		}

	}
	else 
	{
		while (HAL_UART_Transmit_DMA(&EZAT_UART, inchar, length) != HAL_OK)
		{
			//printf("dma send status = %d.\r\n",HAL_UART_Transmit_DMA(&EZAT_UART, "ezat.\r\n", 7));
			printf(".");
			osDelay(1);
		}
		//printf("ezat uart sent.\r\n");
	}	

	if (HAL_UART_GetState(&EZAT_UART) != HAL_UART_STATE_BUSY_TX)
	{	 	
		osDelay(5);
	}
}

/**
 * Convert a string to a uint64 by start pointer and end 
 * pointer. 
 * @param ptr_begin 
 * @param ptr_end 
 * 
 * @return uint64_t 
 */
uint64_t read_number_from_rfcmd (uint8_t * ptr_begin, uint8_t * ptr_end)
{
	uint64_t number = 0;
	
	if(ptr_begin >= ptr_end)
	{
		printf("err input number.\r\n");
		return STATUS_ERROR;
	}
	
	for(int i = 0; i<(ptr_end - ptr_begin); i++)
	{
		
		if(	
		   	(*( ptr_begin + i) > '9')
		   ||( *( ptr_begin + i) < '0')
			)
		{
			printf("unkown number format.\r\n");
			return STATUS_ERROR;
		}
		
		number *= 10;
		number += ( *( ptr_begin + i) - '0');
	}
	
	return number;
}
