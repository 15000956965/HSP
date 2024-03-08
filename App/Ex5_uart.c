#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "Ex3.h"
#include "Ex5.h"

uint8_t string_tx[200];

// USART1 connect to Open-MV connector J4: 115200,n,8,1
void Ex5_1_uart1(void)
{
	uint16_t i = 0;
	
	hsp_uart_init();
	hsp_usart1_config();
	
	while(1)
	{
		i++;
		sprintf(string_tx, "Loop counter: %08d\n\r", i);
		delay_1ms(100);
	}
}

// USART2 connect to DAP Link: 512000,n,8,1
void Ex5_2_uart2_opensda(void)
{
	uint8_t i=0x20U;

	hsp_uart_init();
	hsp_usart2_config();

	while(1)
	{
		delay_1ms(200);
	    usart_data_transmit(USART2, i);
    	while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
		i++;
		if (i>127) i=0x20U;
	}
}

// USART5 connect to wireless module HC-04: 9600,n,8,1
void Ex5_3_uart5_wireless(void)
{
	uint8_t i = 0;
	uint8_t temp = 0, charx = 0, chary = 0;

	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);

	// initialize USART5
	hsp_uart_init();
	hsp_usart5_config();

	// show chars from HC-06 on 20*8 text window
	while(1)
	{
//		i++;
//		if (i>127) i=0x20U;
//		delay_1ms(100);
//	    usart_data_transmit(USART5, i);
//    	while(RESET == usart_flag_get(USART5, USART_FLAG_TBE));
		if(SET == usart_flag_get(USART5, USART_FLAG_RBNE))
		{
			temp = usart_data_receive(USART5);
			if ((temp >= 0x20U) && (temp <= 0x7EU))
			{
				charx = (i%20)*8;
				chary = (i/20)*16;
				hsp_tft18_show_char(charx, chary, temp);
				i++;
				if(i >= 160)
					i = 0;
			}

		}
	}
}

// UART6 connect to K210 connector J11: 115200,n,8,1
void Ex5_4_uart6(void)
{
	uint16_t i = 0;

	hsp_uart_init();
	hsp_uart6_config();

	while(1)
	{
		i++;
		sprintf(string_tx, "Loop counter: %08d\n\r", i);
		delay_1ms(100);
	}
}
