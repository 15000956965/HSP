#include "hsp_rtc.h"

#define RTC_CLOCK_SOURCE_LXTAL 
#define BKP_VALUE    0x32F1

// RTC
__IO uint32_t prescaler_a = 0, prescaler_s = 0;

void hsp_rtc_config(void)
{
    #if defined (RTC_CLOCK_SOURCE_IRC32K) 
          rcu_osci_on(RCU_IRC32K);
          rcu_osci_stab_wait(RCU_IRC32K);
          rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);
  
          prescaler_s = 0x13F;
          prescaler_a = 0x63;
    #elif defined (RTC_CLOCK_SOURCE_LXTAL)
          rcu_osci_on(RCU_LXTAL);
          rcu_osci_stab_wait(RCU_LXTAL);
          rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    
          prescaler_s = 0xFF;
          prescaler_a = 0x7F;
    #else
    #error RTC clock source should be defined.
    #endif /* RTC_CLOCK_SOURCE_IRC32K */

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

void hsp_rtc_setup(uint8_t YY, uint8_t MM, uint8_t DD, uint8_t hh, uint8_t mm, uint8_t ss)
{
	rtc_parameter_struct rtc_initpara;

	/* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();

    hsp_rtc_config();
    rcu_all_reset_flag_clear();
	
    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = bin2bcd(YY);
    rtc_initpara.month = bin2bcd(MM);
    rtc_initpara.date = bin2bcd(DD);
	rtc_initpara.hour = bin2bcd(hh);
	rtc_initpara.minute = bin2bcd(mm);
	rtc_initpara.second = bin2bcd(ss);
    rtc_initpara.am_pm = RTC_AM;
    rtc_initpara.display_format = RTC_24HOUR;
	rtc_initpara.day_of_week = rtc_week_day(2000+YY, MM, DD);
    //rtc_initpara.day_of_week = RTC_SATURDAY;

	rtc_init(&rtc_initpara);
	//RTC_BKP0 = BKP_VALUE;
}

uint8_t rtc_week_day(uint16_t year, uint8_t month, uint8_t day)
{
	uint16_t W;
	uint8_t m;
	uint8_t year_h, year_l;
	
	year_h = year/100;
	year_l = year%100;
	
	if(month<3)
		m = month + 12;
	else
		m = month;
	
	W = ((year_h/4)-(2*year_h)+year_l+(year_l/4)+(26*(m+1)/10)+day-1) % 7; 
	
	return (uint8_t)W; 
}

uint8_t bcd2bin(uint8_t bcd)
{
    return (((bcd>>4)&0x0F)*10 + (bcd&0x0F));
}

uint8_t bin2bcd(uint8_t bin)
{
    return (bin/10<<4 | bin%10);
}
