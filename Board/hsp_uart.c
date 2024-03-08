#include <string.h>
#include "hsp_uart.h"

void hsp_uart_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOF);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
    rcu_periph_clock_enable(RCU_USART2);
    rcu_periph_clock_enable(RCU_USART5);
    rcu_periph_clock_enable(RCU_UART6);

	// USART1 connect to Open-MV connector J4
	/* connect PD5 to USART1_TX */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_5);
    /* connect PD6 to USART1_RX */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_6);

	// USART2 connect to DAP-Link, applicable to GD-Link IAP
	/* connect PB10 to USART2_TX */
	gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_10);
    /* connect PB11 to USART2_RX */
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_11);

	// USART5 connect to wireless module HC-04 on base board
	/* connect PC6 to USART5_TX */
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_6);
    /* connect PC7 to USART5_RX */
    gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_7);

	// UART6 connect to K210 connector J11
	/* connect PF6 to UART6_RX */
	gpio_af_set(GPIOF, GPIO_AF_8, GPIO_PIN_6);
    /* connect PF7 to UART6_TX */
    gpio_af_set(GPIOF, GPIO_AF_8, GPIO_PIN_7);

	/* configure USART1 TX/RX as alternate function push-pull */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6);

	/* configure USART2 TX/RX as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10 | GPIO_PIN_11 );
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);

	/* configure USART5 TX/RX as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);

	/* configure UART6 TX/RX as alternate function push-pull */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
	
	hsp_usart1_config();    // Open-MV
	hsp_usart2_config();    // DAP-Link
	hsp_usart5_config();    // HC-04
	hsp_uart6_config();     // K210
	
	hsp_usart2_dma_config();
}

// USART1 connect to Open-MV connector J4: 115200,n,8,1
void hsp_usart1_config(void)
{
    usart_deinit(USART1);
	
    usart_baudrate_set(USART1, 115200U);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);

    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    
    usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);

    usart_flag_clear(USART1, USART_FLAG_TC);
    
    usart_interrupt_enable(USART1, USART_INT_IDLE);
    usart_interrupt_disable(USART1, USART_INT_RBNE);
    usart_interrupt_disable(USART1, USART_INT_IDLE);
    usart_interrupt_disable(USART1, USART_INT_EB);
    nvic_irq_enable(USART1_IRQn, 0, 0);
    
    usart_enable(USART1);
}

// USART2 connect to DAP Link: 512000,n,8,1
void hsp_usart2_config(void)
{
    usart_deinit(USART2);
	
    usart_baudrate_set(USART2, 512000U);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);

    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    
    usart_dma_transmit_config(USART2, USART_TRANSMIT_DMA_ENABLE);

    usart_flag_clear(USART2, USART_FLAG_TC);
    
    usart_interrupt_enable(USART2, USART_INT_IDLE);
    usart_interrupt_disable(USART2, USART_INT_RBNE);
    usart_interrupt_disable(USART2, USART_INT_IDLE);
    usart_interrupt_disable(USART2, USART_INT_EB);
    nvic_irq_enable(USART2_IRQn, 3, 3);
    
    usart_enable(USART2);
}

// USART5 connect to wireless module HC-04: 9600,n,8,1
void hsp_usart5_config(void)
{
    usart_deinit(USART5);
	
    usart_baudrate_set(USART5, 9600U);
    usart_word_length_set(USART5, USART_WL_8BIT);
    usart_stop_bit_set(USART5, USART_STB_1BIT);
    usart_parity_config(USART5, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART5, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART5, USART_CTS_DISABLE);

    usart_receive_config(USART5, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART5, USART_TRANSMIT_ENABLE);
    
    usart_dma_transmit_config(USART5, USART_TRANSMIT_DMA_ENABLE);

    usart_flag_clear(USART5, USART_FLAG_TC);
    
    usart_interrupt_enable(USART5, USART_INT_IDLE);
    usart_interrupt_disable(USART5, USART_INT_RBNE);
    usart_interrupt_disable(USART5, USART_INT_IDLE);
    usart_interrupt_disable(USART5, USART_INT_EB);
    nvic_irq_enable(USART5_IRQn, 3, 3);
    
    usart_enable(USART5);
}

// UART6 connect to K210 connector J11: 115200,n,8,1
void hsp_uart6_config(void)
{
    usart_deinit(UART6);
	
    usart_baudrate_set(UART6, 115200U);
    usart_word_length_set(UART6, USART_WL_8BIT);
    usart_stop_bit_set(UART6, USART_STB_1BIT);
    usart_parity_config(UART6, USART_PM_NONE);
    usart_hardware_flow_rts_config(UART6, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART6, USART_CTS_DISABLE);

    usart_receive_config(UART6, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART6, USART_TRANSMIT_ENABLE);
    
    usart_dma_transmit_config(UART6, USART_TRANSMIT_DMA_ENABLE);

    usart_flag_clear(UART6, USART_FLAG_TC);
    
    usart_interrupt_enable(UART6, USART_INT_IDLE);
    usart_interrupt_disable(UART6, USART_INT_RBNE);
    usart_interrupt_disable(UART6, USART_INT_IDLE);
    usart_interrupt_disable(UART6, USART_INT_EB);
    nvic_irq_enable(USART5_IRQn, 3, 3);
    
    usart_enable(UART6);
}

void hsp_usart2_dma_config(void)
{
    dma_single_data_parameter_struct dma_init_struct;
	
    /* enable DMA0 */
    rcu_periph_clock_enable(RCU_DMA0);
    /* deinitialize DMA0 channel3 (USART2_TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA0, DMA_CH3);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    //dma_init_struct.memory0_addr = (uint32_t)CCD2PC;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    //dma_init_struct.number = ARRAYNUM(tx_buffer);
    dma_init_struct.number = 260;
    //dma_init_struct.periph_addr = USART2_DATA_ADDRESS;
    dma_init_struct.periph_addr = (uint32_t)(&USART_DATA(USART2));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH3, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH3);
    dma_channel_subperipheral_select(DMA0, DMA_CH3, DMA_SUBPERI4);
    
}

void hsp_uart2_dma_send_ascii(uint8_t *buff, uint32_t count)
{
    usart_flag_clear(USART2, USART_FLAG_TC);

    dma_channel_disable(DMA0, DMA_CH3);
    dma_flag_clear(DMA0, DMA_CH3, DMA_FLAG_FTF);
    dma_memory_address_config(DMA0, DMA_CH3, DMA_MEMORY_0, (uint32_t)buff);
    dma_transfer_number_config(DMA0, DMA_CH3, count);
    dma_channel_enable(DMA0, DMA_CH3);
    while (usart_flag_get(USART2, USART_FLAG_TC) != RESET);
}

void hsp_uart2_dma_send_string(char *str)
{
    hsp_uart2_dma_send_ascii((uint8_t *)str, strlen(str));
}
