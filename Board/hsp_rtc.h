#ifndef _HSP_RTC_H
#define _HSP_RTC_H

#include "gd32f4xx.h"

void hsp_rtc_config(void);
void hsp_rtc_setup(uint8_t YY, uint8_t MM, uint8_t DD, uint8_t hh, uint8_t mm, uint8_t ss);
uint8_t rtc_week_day(uint16_t year, uint8_t month, uint8_t day);
uint8_t bcd2bin(uint8_t bcd);
uint8_t bin2bcd(uint8_t bin);

#endif