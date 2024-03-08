#include "hsp_spi.h"
//#include "gd32f4xx_spi.h"
// https://github.com/Ybalrid/mma8451_pi/blob/master/src/mma8451_pi.c
// https://github.com/coocox/cox/blob/master/CoX/Driver/Sensor_Accelerometer/MMA8451/Driver/lib/MMA8451.c
// https://blog.csdn.net/qq_51930953/article/details/131360635

void hsp_spi_init()
{
    spi_parameter_struct spi_init_struct;

	// enable peripheral clock
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_SPI4);

    /* SPI4_SCK/PE12, SPI4_MOSI/PE14 */
    gpio_af_set(GPIOE, GPIO_AF_6, GPIO_PIN_12 | GPIO_PIN_14);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12 | GPIO_PIN_14);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_14);

    /* SPI4 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
	//spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE; // ���Ը���λ2
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI4, &spi_init_struct);

    /* SPI4 transmit only */
    //spi_bidirectional_transfer_config(SPI4, SPI_BIDIRECTIONAL_TRANSMIT);

	/* enable DMA trasmit */
	spi_dma_enable(SPI4, SPI_DMA_TRANSMIT);
	
    /* enable SPI4 */
    spi_enable(SPI4);

	hsp_spi_dma_config();
	
}

// SPI4_TX DMA
void hsp_spi_dma_config(void)
{
	dma_single_data_parameter_struct dma_init_struct;
    
    rcu_periph_clock_enable(RCU_DMA1);	
    dma_deinit(DMA1, DMA_CH4);
	//dma_deinit(DMA1, DMA_CH6);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;               //�ڴ�������
    dma_init_struct.memory0_addr = (uint32_t)0;                     //�ڴ��ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;        //�����ڴ��ַ����
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;    //�ڴ����ݿ���
    //dma_init_struct.number = LCD_W * LCD_H / 2;                     //������ 240*280/2 = 67200/2 = 33600
	dma_init_struct.number = 40960;                     			//������ 120*160*2 = 38400
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI4) ;		//�����ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;       //�ر������ַ����
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;             //���ȼ���
    dma_single_data_mode_init(DMA1, DMA_CH4, &dma_init_struct);     //�����ϲ�������DMA1��ͨ��4
	//dma_single_data_mode_init(DMA1, DMA_CH6, &dma_init_struct);     //�����ϲ�������DMA1��ͨ��4
    
    dma_circulation_disable(DMA1, DMA_CH4);	
    dma_channel_subperipheral_select(DMA1, DMA_CH4, DMA_SUBPERI2);
	//dma_circulation_disable(DMA1, DMA_CH6);	
    //dma_channel_subperipheral_select(DMA1, DMA_CH6, DMA_SUBPERI7);
    //dma_interrupt_enable(DMA1, DMA_CH4, DMA_CHXCTL_FTFIE);
    //nvic_irq_enable(DMA1_Channel4_IRQn, 0, 0);

	dma_channel_disable(DMA1, DMA_CH4);
	//dma_channel_disable(DMA1, DMA_CH6);
}

//��ʼ��ʾ
void LCD_Show_Gram(void)
{
    //���ñ�־λΪδ��ʾ���״̬
    //������ʾ��Χ
	//���ȫ���жϱ�־λ(�������ͨ��3��ȫ���жϱ�־λ)
    //DMA_INTC0(DMA1) = 0xfffffff;
	
    //���ô������ݴ�С
    //DMA_CHCNT(DMA1, DMA_CH4) = 33600;
	
    //���ô����ַ
	//DMA_CH4M0ADDR(DMA1) = (uint32_t)Show_Gram;
	
    //��ʼ����
    //DMA_CHCTL(DMA1, DMA_CH4) |= DMA_CHXCTL_CHEN;
}
