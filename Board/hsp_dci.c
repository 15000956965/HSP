#include "hsp_dci.h"
#include "hsp_gpio.h"

extern uint8_t image_raw[22560];        // defined elsewhere to hold 188*120 raw data
extern uint8_t image_use[112][160];     // resized CAM data to fit LCD
extern uint8_t image_ready;				// MT9V034 new data frame ready in buffer
extern uint8_t image2_use[40][188];		// use 1/3 of the original image (40 continuous lines in the middle)
extern uint8_t image_size;				// 0: full size; 1: half size; 2: 1/3 sized

void hsp_dci_init(void)
{
    dci_parameter_struct dci_struct;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_DCI);
    
    /* DCI GPIO AF configuration */ 
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_4);     // DCI_HSYNC(PA4)
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_6);     // DCI_PIXCLK(PA6)
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_9);     // DCI_D0(PA9)
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_10);    // DCI_D1(PA10)
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_6);     // DCI_D5(PB6)
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_7);     // DCI_VSYNC(PB7)
    gpio_af_set(GPIOE, GPIO_AF_13, GPIO_PIN_0);     // DCI_D2(PE0)
    gpio_af_set(GPIOE, GPIO_AF_13, GPIO_PIN_1);     // DCI_D3(PE1)
    gpio_af_set(GPIOE, GPIO_AF_13, GPIO_PIN_4);     // DCI_D4(PE4)
    gpio_af_set(GPIOE, GPIO_AF_13, GPIO_PIN_5);     // DCI_D6(PE5)
    gpio_af_set(GPIOE, GPIO_AF_13, GPIO_PIN_6);     // DCI_D7(PE6)

    /* configure DCI_PIXCLK(PA6), DCI_VSYNC(PB7), DCI_HSYNC(PA4), DCI_D0(PA9), DCI_D1(PA10)
                 DCI_D2(PE0), DCI_D3(PE1), DCI_D4(PE4), DCI_D5(PB6), DCI_D6(PE5), DCI_D7(PE6) */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_10);
    
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6 | GPIO_PIN_7);
    
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | \
                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | \
                GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

	/* DCI configuration */ 
    dci_struct.capture_mode     = DCI_CAPTURE_MODE_CONTINUOUS;
	//dci_struct.clock_polarity   = DCI_CK_POLARITY_RISING;
    dci_struct.clock_polarity   = DCI_CK_POLARITY_FALLING;
    //dci_struct.hsync_polarity   = DCI_HSYNC_POLARITY_HIGH;
    dci_struct.hsync_polarity   = DCI_HSYNC_POLARITY_LOW;
    //dci_struct.vsync_polarity   = DCI_VSYNC_POLARITY_HIGH;
    dci_struct.vsync_polarity   = DCI_VSYNC_POLARITY_LOW;
    dci_struct.frame_rate       = DCI_FRAME_RATE_ALL;
    dci_struct.interface_format = DCI_INTERFACE_FORMAT_8BITS;
    dci_init(&dci_struct);

	dci_capture_enable();
	dci_enable();
}

//void hsp_dci_dma_config(uint8_t *buff)
void hsp_dci_dma_config(void)
{
    dma_single_data_parameter_struct dma_single_struct;

    /* DCI DMA configuration */ 
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH7);
    //dma_single_struct.periph_addr   = (uint32_t)DCI_DR_ADDRESS;
	dma_single_struct.periph_addr   = DCI_DR_ADDRESS;
	//dma_single_struct.periph_addr   = (uint32_t)&DCI_DATA;
    //dma_single_struct.memory0_addr  = (uint32_t)(0xC0000000);   // ImageData[120][188]
	dma_single_struct.memory0_addr  = (uint32_t)&image_raw[0];
	//dma_single_struct.memory0_addr  = (uint32_t)buff;
    dma_single_struct.direction     = DMA_PERIPH_TO_MEMORY;
	dma_single_struct.number        = 5640;			// bin4x4 120*188/4
    dma_single_struct.periph_inc    = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_struct.memory_inc    = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_single_struct.priority      = DMA_PRIORITY_HIGH;
    dma_single_struct.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_single_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI1);
    
	dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
    nvic_irq_enable(DMA1_Channel7_IRQn, 0, 0);
	dma_channel_enable(DMA1, DMA_CH7);
}

void DMA1_Channel7_IRQHandler(void)
{
	uint8_t i,j;
    /* 120*188 size image convert to 112*160 size image */ 

    if(dma_interrupt_flag_get(DMA1, DMA_CH7, DMA_INTF_FTFIF))
    {         
        dma_channel_disable(DMA1, DMA_CH7);
		image_ready = SET;
		LED_G_TOGGLE();

		// for MT9V034 test purpose
		if (0 == image_size)	// extract 112*160 from 120*188
		{
			for(i=0; i<112; i++)
			{
				for(j=0; j<160; j++)
				{
					image_use[i][j] = image_raw[(uint8_t)(i*(120-1)/112)*188+j*(188-1)/(160-1)];
				}
			}
		}
		if (2 == image_size)	// extract 40*188 from 120*188
		{
			for(i=0; i<40; i++)
			{
				for(j=0; j<188; j++)
				{
					image2_use[i][j] = image_raw[(i+40)*188+j];
				}
			}
		}

        dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INTF_FTFIF);
        dma_channel_enable(DMA1, DMA_CH7);
    }       
}
