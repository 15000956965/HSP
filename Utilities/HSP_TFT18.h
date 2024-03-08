#ifndef _HSP_TFT18_H
#define _HSP_TFT18_H

#include "gd32f4xx.h"
#include "hsp_spi.h"
#include "hsp_gpio.h"

#define TFT18_SPI_BASEADDR	SPI4
#define	TFT_X_MAX	160		// TFT18 LCD x dots
#define TFT_Y_MAX	128		// TFT18 LCD y dots

#define BLACK		0x0000	/*   0,   0,   0 */
#define NAVY		0x000F	/*   0,   0, 128 */
#define DARKBLUE	0x0011	// Dark blue
#define BLUE		0x001F	/*   0,   0, 255 */
#define DARKGREEN	0x03E0	/*   0, 128,   0 */
#define DARKCYAN	0x03EF	/*   0, 128, 128 */
#define GREEN		0x07E0	/*   0, 255,   0 */
#define CYAN		0x07FF	/*   0, 255, 255 */
#define INDIGO		0x4810	// Indigo
#define MAROON		0x7800	/* 128,   0,   0 */
#define PURPLE		0x780F	/* 128,   0, 128 */
#define OLIVE		0x7BE0	/* 128, 128,   0 */
#define DARKGREY	0x7BEF	/* 128, 128, 128 */
#define GREENYELLOW	0xAFE5	/* 173, 255,  47 */
#define LIGHTGREY	0xC618	/* 192, 192, 192 */
#define CHOCOLATE	0xD343	// Chocolate
#define RED			0xF800	/* 255,   0,   0 */
#define PINK		0xF81F	/* 255,   0, 255 */
#define DEEPPINK	0xF8B2	// Deep pink
#define DARKORANGE	0xFC60	// Dark orange
#define ORANGE		0xFD20	/* 255, 165,   0 */
#define GOLD		0xFEA0	// Gold
#define YELLOW		0xFFE0	/* 255, 255,   0 */
#define WHITE		0xFFFF	/* 255, 255, 255 */
#define MAGENTA 	0xF81F
#define GRAY0		0xEF7D
#define GRAY1		0x8410
#define GRAY2		0x4208

#define COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
#define GRAY2RGB16(t) ((t >> 3)|((t & ~3) << 3)|((t & ~7) << 8))
#define GRAY2RGB16_hl(t) ((t << 5) | ((t&~3)<<11) | (t & ~7))

void hsp_tft18_init(void);
void hsp_tft18_write_cmd(uint8_t dat);
void hsp_tft18_write_byte(uint8_t dat);
void hsp_tft18_write_2byte(uint16_t dat);
void hsp_tft18_set_region(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void hsp_tft18_clear(uint16_t Color);
void hsp_tft18_set_xy(uint16_t x, uint16_t y);
void hsp_tft18_test_color(void);

void hsp_tft18_show_char(uint16_t x, uint16_t y, uint8_t data);
void hsp_tft18_show_char_color(uint16_t x, uint16_t y, uint8_t data, uint16_t fcolor, uint16_t bcolor);
void hsp_tft18_show_char16(uint16_t x, uint16_t y, uint8_t dat);
void hsp_tft18_show_char24(uint16_t x, uint16_t y, uint8_t dat);
void hsp_tft18_show_str(uint16_t x, uint16_t y, uint8_t data[]);
void hsp_tft18_show_str_color(uint16_t x, uint16_t y, uint8_t data[], uint16_t fcolor, uint16_t bcolor);
void hsp_tft18_show_int8(uint16_t x, uint16_t y, int8_t data);
void hsp_tft18_show_int8_color(uint16_t x, uint16_t y, int8_t data, uint16_t fcolor, uint16_t bcolor);
void hsp_tft18_show_uint8(uint16_t x, uint16_t y, uint8_t data);
void hsp_tft18_show_uint8_color(uint16_t x, uint16_t y, uint8_t data, uint16_t fcolor, uint16_t bcolor);
void hsp_tft18_show_int16(uint16_t x, uint16_t y, int16_t data);
void hsp_tft18_show_int16_color(uint16_t x, uint16_t y, int16_t data, uint16_t fcolor, uint16_t bcolor);
void hsp_tft18_show_uint16(uint16_t x, uint16_t y, uint16_t data);
void hsp_tft18_show_uint16_color(uint16_t x, uint16_t y, uint16_t data, uint16_t fcolor, uint16_t bcolor);

void hsp_tft18_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void hsp_tft18_draw_line_h(uint16_t x_start, uint16_t y_start, uint16_t length, uint16_t color);
void hsp_tft18_draw_line_v(uint16_t x_start, uint16_t y_start, uint16_t length, uint16_t color);
void hsp_tft18_draw_frame(uint8_t x_start, uint8_t y_start, uint8_t dx, uint8_t dy, uint16_t color);
void hsp_tft18_draw_block(uint8_t x_start, uint8_t y_start, uint8_t dx, uint8_t dy, uint16_t color);
void hsp_tft18_draw_menu_cursor(uint16_t x, uint8_t line, uint16_t color);
void hsp_tft18_draw_image(const uint8_t *p);
void hsp_tft18_draw_image_dma(const uint8_t *p);

static uint8_t hsp_spi_write_byte(uint8_t byte);

#endif