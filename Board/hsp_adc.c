#include "hsp_adc.h"
#include "systick.h"

uint16_t ADC01Value[7];		// DMA receiver to hold 7 channel 16-bit ADC results, REGULAR_INDEPENDENT
uint16_t ADC2Value[4];		// DMA receiver to hold 4 channel 16-bit ADC results, REGULAR_INDEPENDENT
uint16_t EM1, EM2, EM3, EM4, MEMSx, MEMSy, Vsense, CCDAO, POT, MIS1, MIS2;

void hsp_adc_init(void)
{
	// ADCs on PORTA: EM-PA5 can be configured as DAC
    // PA5/ADC01_IN5
	//rcu_periph_clock_enable(RCU_GPIOA);
	//gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5);
    
	// ADCs on PORTC: Vsense, POT1, CCD1_AO, CCD2_AO
    // PC0/ADC012_IN10, PC1/ADC012_IN11, PC2/ADC012_IN12, PC3/ADC012_IN13
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, \
	  				GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	// ADCs on PORTC: MIS1, MIS2
    // PC4/ADC01_IN14, PC5/ADC01_IN15
	gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_5);
    
	// ADCs on PORTF: EMs connector J13
    // PF3/ADC2_IN9; PF4/ADC2_IN14; PF5/ADC2_IN15, PF8/ADC2_IN6, PF9/ADC2_IN7, PF10/ADC2_IN8
	rcu_periph_clock_enable(RCU_GPIOF);
	//gpio_mode_set(GPIOF, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | \
	//  				GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_10);
    // PF3/ADC2_IN9; PF8/ADC2_IN6, PF9/ADC2_IN7, PF10/ADC2_IN8
	rcu_periph_clock_enable(RCU_GPIOF);
	gpio_mode_set(GPIOF, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, \
	  				GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_10);

	/* enable ADC clock */
	rcu_periph_clock_enable(RCU_ADC0);		// EM-PA5, Vsense, POT1, Vtemp
	rcu_periph_clock_enable(RCU_ADC1);		// CCD1_AO, CCD2_AO
	rcu_periph_clock_enable(RCU_ADC2);		// EM-PF8/PF9/PF10/PF3/PF4/PF5
	/* config ADC clock */
	adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
}

// ADC_IDATA0(ADC0): Vsense
// ADC_IDATA1(ADC0): POT1
// ADC_IDATA2(ADC0): Vtemp
// ADC_IDATA3(ADC0): Vbat (1/4 voltate of external battery)
void hsp_adc0_config(void)
{
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_INSERTED_CHANNEL, 4);

	/* ADC Vbat channel enable */
    adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH, ENABLE);
    /* ADC temperature and Vrefint enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);
	
    /* ADC on-board power supply voltage (Vsense) channel config */
    adc_inserted_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_112);
    /* ADC on-board potentiometer (POT1) channel config */
    adc_inserted_channel_config(ADC0, 1, ADC_CHANNEL_11, ADC_SAMPLETIME_112);
    /* ADC temperature sensor channel config */
	adc_inserted_channel_config(ADC0, 2, ADC_CHANNEL_16, ADC_SAMPLETIME_112);
    /* ADC 1/4 voltate of external battery channel config */
    adc_inserted_channel_config(ADC0, 3, ADC_CHANNEL_18, ADC_SAMPLETIME_112);

    /* ADC external trigger disabled */
    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    /* ADC data alignment config: right-aligned */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC SCAN function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}

// ADC_IDATA0(ADC1): CCD1 - PC2
// ADC_IDATA1(ADC1): CCD2 - PC3
// ADC_IDATA2(ADC1): MIS1 - PC4
// ADC_IDATA3(ADC1): MIS2 - PC5
void hsp_adc1_config(void)
{
    /* ADC channel length config */
    adc_channel_length_config(ADC1, ADC_INSERTED_CHANNEL, 4);

    /* ADC CCD1_AO channel config */
    adc_inserted_channel_config(ADC1, 0, ADC_CHANNEL_12, ADC_SAMPLETIME_112);
    /* ADC CCD2_AO channel config */
    adc_inserted_channel_config(ADC1, 1, ADC_CHANNEL_13, ADC_SAMPLETIME_112);
    /* ADC motor#1 current sense channel config */
    adc_inserted_channel_config(ADC1, 2, ADC_CHANNEL_14, ADC_SAMPLETIME_112);
    /* ADC motor#2 current sense channel config */
    adc_inserted_channel_config(ADC1, 3, ADC_CHANNEL_15, ADC_SAMPLETIME_112);

    /* ADC external trigger disabled */
    adc_external_trigger_config(ADC1, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    /* ADC data alignment config: right-aligned */
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    /* ADC SCAN function enable */
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    
    /* enable ADC interface */
    adc_enable(ADC1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
}

// ADC_IDATA0(ADC2): EM-PF3
// ADC_IDATA1(ADC2): EM-PF8
// ADC_IDATA2(ADC2): EM-PF9
// ADC_IDATA3(ADC2): EM-PF10
void hsp_adc2_config(void)
{
    /* ADC channel length config */
    adc_channel_length_config(ADC2, ADC_INSERTED_CHANNEL, 4);

    /* ADC on-board electro-magnetic signal (EM-PF3) channel config */
    adc_inserted_channel_config(ADC2, 0, ADC_CHANNEL_9, ADC_SAMPLETIME_3);
    /* ADC on-board electro-magnetic signal (EM-PF4) channel config */
    adc_inserted_channel_config(ADC2, 1, ADC_CHANNEL_6, ADC_SAMPLETIME_3);
    /* ADC on-board electro-magnetic signal (EM-PF6) channel config */
    adc_inserted_channel_config(ADC2, 2, ADC_CHANNEL_7, ADC_SAMPLETIME_3);
    /* ADC on-board electro-magnetic signal (EM-PF8) channel config */
    adc_inserted_channel_config(ADC2, 3, ADC_CHANNEL_8, ADC_SAMPLETIME_3);

    /* ADC external trigger disabled */
    adc_external_trigger_config(ADC2, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    /* ADC data alignment config: right-aligned */
    adc_data_alignment_config(ADC2, ADC_DATAALIGN_RIGHT);
    /* ADC SCAN function enable */
    adc_special_function_config(ADC2, ADC_SCAN_MODE, ENABLE);
    
    /* enable ADC interface */
    adc_enable(ADC2);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC2);
}