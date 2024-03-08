#ifndef _HSP_UART_H
#define _HSP_UART_H

#include "gd32f4xx.h"
#include "gd32f4xx_usart.h"

void hsp_uart_init(void);
void hsp_usart1_config(void);
void hsp_usart2_config(void);
void hsp_usart5_config(void);
void hsp_uart6_config(void);
void hsp_usart2_dma_config(void);
void hsp_uart2_dma_send_ascii(uint8_t *buff, uint32_t count);
void hsp_uart2_dma_send_string(char *str);


#endif