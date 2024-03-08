#ifndef _EX7_H
#define _EX7_H

#include "hsp_timer.h"
#include "hsp_dci.h"
#include "HSP_MT9V034.h"

#define IMAGEH	112U  	// Camera image lines used for processing, image height fit for LCD display
#define IMAGEW	160U  	// Camera image columes used for processing, image width fit for LCD display
#define IMAGEH2	40U  	// Camera image lines used for processing, 1/3 image height
#define IMAGEW2	188U  	// Camera image columes used for processing, original image width

typedef uint8_t image_t[IMAGEH][IMAGEW];
typedef uint8_t image2_t[IMAGEH2][IMAGEW2];

void Ex7_1_dcam(void);
void Ex7_2_edge(void);
void Ex7_3_guide(void);
void hsp_image_show(image_t image);
void hsp_image_show_dma(image_t image);
void hsp_image2_show_dma(image2_t image);

void hsp_image_binary(image_t input, image_t output, uint16_t threshold);
void hsp_image_binary_sobel(image_t input, image_t output);
void hsp_image2_binary_minmax(image2_t input, image2_t output);
uint8_t hsp_image_threshold_otsu(image_t image);
uint8_t hsp_image_threshold_mean(image_t image);
uint8_t hsp_image2_threshold_otsu(image2_t image);
uint8_t hsp_image2_threshold_mean(image2_t image);
uint8_t hsp_image2_threshold_minmax(image2_t image);

void hsp_image_fast_conv(image_t input, image_t output);
void hsp_image_fast_gauss_conv(image_t input, image_t output);
void hsp_image_erode(image_t input, image_t output);
void hsp_image_dilate(image_t input, image_t output);
void hsp_image2_erode(image2_t input, image2_t output);
void hsp_image2_dilate(image2_t input, image2_t output);

void hsp_image_sobel1(image_t A, image_t B);
void hsp_image_sobel2(image_t A, image_t B);
void hsp_image_canny(image_t A, image_t B, uint8_t low_thres, uint8_t high_thres);
uint8_t atan4(float y, float x);
float fast_sqrtf(float x);

#endif