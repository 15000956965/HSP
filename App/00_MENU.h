#ifndef _00_MENU_H
#define _00_MENU_H

#include "GFX_FUNCTIONS.h"
#include "hsp_spi.h"
#include "HSP_TFT18.h"

uint8_t hsp_menu_loop(void);
void show_menu_cursor(uint8_t ItemNumber, uint16_t color);
void Ex0_gif_poweron(void);


void hsp_rtc_demo(void);
void rtc_pre_config(void);
void rtc_show_time(void);
void DrawRtcDemoScreen();
uint8_t BCD2BIN(uint8_t bcd);
uint8_t BIN2BCD(uint8_t bin);
void ShowArrow16x16(uint8_t x, uint8_t y, uint8_t no, uint16_t fColor, uint16_t bColor);
void ClearArrow16x16(uint8_t x, uint8_t y, uint16_t color);
void UpdateArrowPos(uint8_t no, uint16_t fColor, uint16_t bColor);
void UpdateSetValue(uint8_t no);

void hsp_demo_frame_sysinfo(void);
void hsp_sysinfo(void);

#endif