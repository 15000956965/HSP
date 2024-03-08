// Exercise #7: DCI connected to MT9V034
// https://blog.csdn.net/m0_66478571/article/details/122557273
// 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "systick.h"
#include "Ex3.h"
#include "Ex7.h"

//uint8_t image_use[112][160];
//uint8_t image_show[112][160];		// resized CAM data to fit LCD
image_t image_use;					// original image fit to LCD resolution
image2_t image2_use;				// use 1/3 of the original image (40 continuous lines in the middle)
image_t image_show;					// resized CAM data to fit LCD
image2_t image2_show;				// show 1/3 of the full size screen
image2_t image2_temp;				// show 1/3 of the full size screen
image_t image_gauss, image_binary;
uint8_t image_ready;				// MT9V034 new data frame ready in buffer

uint8_t image_size = 2;				// 0: full size; 1: half size; 2: 1/3 sized

__attribute__((aligned(32))) uint8_t img[35840];
__attribute__((aligned(32))) uint8_t img2[12800];

void Ex7_1_dcam(void)
{
	uint8_t i, j, threshold;
	
//	hsp_spi_init();
	hsp_mt9v034_init();
	hsp_dci_dma_config();
	image_ready = RESET;
	
	while(1)
	{
		if(image_ready == SET)
		{
			if(0 == image_size)
			{
				if(!S1() || !S2())
				{
					if(!S1())
						threshold = hsp_image_threshold_otsu(image_use);
					if(!S2())
						threshold = hsp_image_threshold_mean(image_use);
					for(j=0; j<IMAGEH; j++)
					{
						for(i=0; i<IMAGEW; i++)
							image_show[j][i] = image_use[j][i]>threshold ? WHITE : BLACK;
					}
				}
				else
				{
					for(j=0; j<IMAGEH; j++)
					{
						for(i=0; i<IMAGEW; i++)
							image_show[j][i] = image_use[j][i];
					}
				}
				//hsp_image_show(image_show);
				hsp_image_show_dma(image_show);
			}
			else if(2 == image_size)
			{
				//threshold = hsp_image2_threshold_otsu(image2_use);
				//threshold = hsp_image2_threshold_mean(image2_use);
				//threshold = hsp_image2_threshold_minmax(image2_use);
//				for(j=0; j<IMAGEH2; j++)
//				{
//					for(i=0; i<IMAGEW2; i++)
//						image2_show[j][i] = image2_use[j][i]>threshold ? WHITE : BLACK;
//				}
//				hsp_image2_erode(image2_show, image2_temp);
//				hsp_image2_erode(image2_temp, image2_show);
				//hsp_image2_show_dma(image2_use);
				//hsp_image2_show_dma(image2_show);

				hsp_image2_binary_minmax(image2_use, image2_temp);
				
				if(!PUSH())
				{
					hsp_image2_show_dma(image2_use);
				}
				else
				{
					hsp_image2_show_dma(image2_temp);
				}
			}

			image_ready = RESET;
		}
	}
}

void Ex7_2_edge(void)
{
	uint8_t i, j, threshold;

	hsp_mt9v034_init();
	hsp_dci_dma_config();
	image_ready = RESET;
	
	while(1)
	{
		if(image_ready == SET)
		{
			if(!SW1())		// OTSU method
				threshold = hsp_image_threshold_otsu(image_use);
			else			// mean method
				threshold = hsp_image_threshold_mean(image_use);

			hsp_image_fast_gauss_conv(image_use, image_gauss);
			//hsp_image_canny(image_gauss, image_binary, 20, 80);
			//hsp_image_sobel2(image_gauss, image_binary);
			hsp_image_binary_sobel(image_gauss, image_show);
			//hsp_image_binary(image_use, image_show, threshold);
			//hsp_image_erode(image_show, image_binary);
			//hsp_image_dilate(image_show, image_binary);
			
			image_ready = RESET;
			if(!SW1())
				//hsp_image_show(image_show);
				hsp_image_show_dma(image_show);
			else
				//hsp_image_show(image_use);
				hsp_image_show_dma(image_use);
		}
	}
}

void Ex7_3_guide(void)
{

}

void hsp_image_show(image_t image)
{
    uint16_t i,j; 
    uint16_t color = 0;
	uint16_t temp = 0;
	
	for(i=0; i<IMAGEW; i++)
    {
		hsp_tft18_set_region(i, 16, i, 128);
        for(j=0; j<IMAGEH; j++)
        {	
			temp = image[j][i];
			// color = (0x001f&((temp)>>3))<<11;
            // color = color|(((0x003f)&((temp)>>2))<<5);
            // color = color|(0x001f&((temp)>>3));
			color = GRAY2RGB16(temp);
            hsp_tft18_write_2byte(color);
        }
    }
}

void hsp_image_show_dma(image_t image)
{
    uint16_t i,j; 
    uint16_t point = 0;
	uint8_t temp = 0;
	//uint8_t img[112*160*2];
	
	hsp_tft18_set_region(0, 16, TFT_X_MAX-1, TFT_Y_MAX-1);
	point = 0;
	
	for(j=0; j<IMAGEH; j++)
	{	
		for(i=0; i<IMAGEW; i++)
		{
			temp = image[j][i];
//			img[point++] = (((0x003f)&((temp)>>2))<<5) | (0x001f&((temp)>>3));
//			img[point++] = (0x001f&((temp)>>3))<<3;
			img[point++] = (GRAY2RGB16(temp)>>8)&0xFF;
			img[point++] = GRAY2RGB16(temp)&0xFF;
        }
    }
	
	LCD_DC_HIGH();

    DMA_CHCNT(DMA1, DMA_CH4) = 35840;
	DMA_CH4M0ADDR(DMA1) = (uint32_t)&img[0];
    DMA_CHCTL(DMA1, DMA_CH4) |= DMA_CHXCTL_CHEN;
	
	while(SPI_STAT(SPI4) & SPI_STAT_TRANS);
	dma_interrupt_flag_clear(DMA1, DMA_CH4, DMA_INT_FLAG_FTF);
	
}

void hsp_image2_show_dma(image2_t image)
{
    uint16_t i,j; 
    uint16_t point = 0;
	uint8_t temp;
	
	hsp_tft18_set_region(0, 88, TFT_X_MAX-1, TFT_Y_MAX-1);
	point = 0;
	
	for(j=0; j<IMAGEH2; j++)
	{	
		for(i=14; i<174; i++)
		{
			temp = image[j][i];
			img2[point++] = (GRAY2RGB16(temp)>>8)&0xFF;
			img2[point++] = GRAY2RGB16(temp)&0xFF;
        }
    }
	
	LCD_DC_HIGH();

    DMA_CHCNT(DMA1, DMA_CH4) = 12800;
	DMA_CH4M0ADDR(DMA1) = (uint32_t)&img2[0];
    DMA_CHCTL(DMA1, DMA_CH4) |= DMA_CHXCTL_CHEN;
	
	while(SPI_STAT(SPI4) & SPI_STAT_TRANS);
	dma_interrupt_flag_clear(DMA1, DMA_CH4, DMA_INT_FLAG_FTF);
	
}

void hsp_image_binary(image_t input, image_t output, uint16_t threshold)
{
	uint16_t i=0, j=0, diff=4;		//diff=8
	
	for (i=0; i<IMAGEH; i++)
    {
		for(j=0; j<IMAGEW; j++)
			output[i][j] = input[i][j]>threshold ? 255 : 0;
	}

	for (i=1; i<IMAGEH-1; i++)
	{
		for(j=1; j<IMAGEW-1; j++)
		{
			if(output[i][j] == 0)
			{
				if( input[i][j]-input[i-1][j]<diff && abs(input[i][j]-input[i+1][j])<diff && 
					input[i][j]-input[i][j-1]<diff && abs(input[i][j]-input[i][j+1])<diff )
				{
					output[i][j] = 255;
				}
			}
		}
	}
}

// https://blog.csdn.net/qq_58114029/article/details/132050763
void hsp_image_binary_sobel(image_t input, image_t output)
{
    int i, j, k;
    int temp[4];
	
    for(i=1; i<(IMAGEH-1); i++)
	{
		for(j=1; j<(IMAGEW-1); j++)
		{
            /* 计算不同方向梯度幅值 */
            temp[0] = -(short)input[i-1][j-1] + (short)input[i-1][j+1]		//{{-1, 0, 1},
            		  -(short)input[ i ][j-1] + (short)input[ i ][j+1]		// {-1, 0, 1},
           			  -(short)input[i+1][j-1] + (short)input[i+1][j+1];		// {-1, 0, 1}};
            
            temp[1] = -(short)input[i-1][j-1] + (short)input[i+1][j-1]		//{{-1, -1, -1}
		              -(short)input[i-1][ j ] + (short)input[i+1][ j ]		// {0,  0,  0}
            		  -(short)input[i-1][j+1] + (short)input[i+1][j+1];		// {1,  1,  1}};
 
            temp[2] = -(short)input[i-1][ j ] + (short)input[ i ][j-1]		//{{0, -1, -1}
					  -(short)input[ i ][j+1] + (short)input[i+1][ j ]		// {1,  0, -1}
					  -(short)input[i-1][j+1] + (short)input[i+1][j-1];		// {1,  1,  0}};
 
            temp[3] = -(short)input[i-1][ j ] + (short)input[ i ][j+1]		//{{-1, -1,  0}
					  -(short)input[ i ][j-1] + (short)input[i+1][ j ]		// {-1,  0,  1}
					  -(short)input[i-1][j-1] + (short)input[i+1][j+1];		// { 0,  1,  1}};
			
            /* 找出梯度幅值最大值 */
			temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);
            for(k=1; k<4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }
			
            /* 使用像素点邻域内像素点之和的一定比例作为阈值 */
            temp[3] = (short) input[i-1][j-1] + (short) input[i-1][j] + (short) input[i-1][j+1]
                    + (short) input[ i ][j-1] + (short) input[ i ][j] + (short) input[ i ][j+1]
                    + (short) input[i+1][j-1] + (short) input[i+1][j] + (short) input[i+1][j+1];
            if(temp[0] > (temp[3]/12.0f))
            {
                output[i][j] = 0U;
            }
            else
            {
                output[i][j] = 255U;
            }
        }
    }
}

void hsp_image2_binary_minmax(image2_t input, image2_t output)
{
    uint8_t i, j, threshold;
	uint8_t min_val, max_val;
	
    for(i=0; i<IMAGEH2; i++)			// line
	{
		min_val = 255;
		max_val = 0;
		for(j=0; j<IMAGEW2; j++)		// column
		{
			if(min_val > input[i][j])
				min_val = input[i][j];
//			if(max_val < input[i][j])
//				max_val = input[i][j];
        }
		//threshold = min_val + hsp_get_taskid()<<1;
		threshold = min_val + 10 + hsp_get_taskid()*5;
		for(j=0; j<IMAGEW2; j++)		// column
		{
			output[i][j] = input[i][j]>threshold ? 255 : 0;
        }
		
    }
}

// threshold mode: OTSU
// https://blog.csdn.net/Youngmember/article/details/134622033
uint8_t hsp_image_threshold_otsu(image_t image)
{
	uint8_t threshold = 0;
	uint16_t i, j;
	uint16_t histogram[256];
	uint16_t min_value, max_value;
	uint32_t pixel_amount = 0;
	uint32_t pixel_integral = 0;
	uint32_t pixel_integral_back = 0;
	uint32_t pixel_integral_fore = 0;
	uint32_t pixel_back = 0;
	uint32_t pixel_fore = 0;
	float omega_back, omega_fore, micro_back, micro_fore, sigma_b, sigma; // 类间方差

	// grayscale to histogram
	for(j=0; j<256; j++)  histogram[j] = 0;
	for(j=0; j<IMAGEH; j++)
	{
		for(i=0; i<IMAGEW; i++)
			histogram[image[j][i]]++;			// 将灰度图转换为灰度直方图
	}

	for(min_value=0; (min_value<256)&&(histogram[min_value]==0); min_value++);				// 获取最小灰度值
	for(max_value=255; (max_value>min_value)&&(histogram[max_value]==0); max_value--);		// 获取最大灰度值

	if(max_value == min_value)
		return max_value;						// 图像中只存在一种颜色的情况
	if((min_value+1) == max_value)
		return min_value;						// 图像中只存在两种颜色的情况

	pixel_amount = 0;
	for(j=min_value; j<=max_value; j++)
		pixel_amount += histogram[j];			// 总灰度权重

	pixel_integral = 0;
	for(j=min_value; j<=max_value; j++)
	{
		pixel_integral += histogram[j] * j;		// 图像总灰度累加值
	}
	
	sigma_b = -1.0;
	for(j=min_value; j<max_value; j++)
	{
		pixel_back = pixel_back + histogram[j];   		// 背景像素点
		pixel_fore = pixel_amount - pixel_back;			// 前景像素点
		omega_back = (float)pixel_back/pixel_amount;	// 背景像素百分比
		omega_fore = (float)pixel_fore/pixel_amount;	// 前景像素百分比
		pixel_integral_back += histogram[j] * j;  		// 背景灰度值
		pixel_integral_fore = pixel_integral - pixel_integral_back;	// 前景灰度值
		micro_back = (float)pixel_integral_back / pixel_back;   	// 背景灰度平均值
		micro_fore = (float)pixel_integral_fore / pixel_fore;   	// 前景灰度平均值
		sigma = omega_back * omega_fore * (micro_back - micro_fore) * (micro_back - micro_fore);	// 计算类间方差
		if (sigma > sigma_b)							// 遍历最大类间方差，找出最大类间方差以及对应的阈值
		{
			sigma_b = sigma;
			threshold = j;
		}
	}

	//threshold = (uint8_t)(threshold*0.5)+70;	// adjust the value according to field condition
	return threshold;		// the threshold value found by OTSU method
}

// threshold mode: OTSU
uint8_t hsp_image2_threshold_otsu(image2_t image)
{
	uint8_t threshold = 0;
	uint16_t i, j;
	uint16_t histogram[256];
	uint16_t min_value, max_value;
	uint32_t pixel_amount = 0;
	uint32_t pixel_integral = 0;
	uint32_t pixel_integral_back = 0;
	uint32_t pixel_integral_fore = 0;
	uint32_t pixel_back = 0;
	uint32_t pixel_fore = 0;
	float omega_back, omega_fore, micro_back, micro_fore, sigma_b, sigma; // 类间方差

	// grayscale to histogram
	for(j=0; j<256; j++)  histogram[j] = 0;
	for(j=0; j<IMAGEH2; j++)
	{
		for(i=0; i<IMAGEW2; i++)
			histogram[image[j][i]]++;			// 将灰度图转换为灰度直方图
	}

	for(min_value=0; (min_value<256)&&(histogram[min_value]==0); min_value++);				// 获取最小灰度值
	for(max_value=255; (max_value>min_value)&&(histogram[max_value]==0); max_value--);		// 获取最大灰度值

	if(max_value == min_value)
		return max_value;						// 图像中只存在一种颜色的情况
	if((min_value+1) == max_value)
		return min_value;						// 图像中只存在两种颜色的情况

	pixel_amount = 0;
	for(j=min_value; j<=max_value; j++)
		pixel_amount += histogram[j];			// 总灰度权重

	pixel_integral = 0;
	for(j=min_value; j<=max_value; j++)
	{
		pixel_integral += histogram[j] * j;		// 图像总灰度累加值
	}
	
	sigma_b = -1.0;
	for(j=min_value; j<max_value; j++)
	{
		pixel_back = pixel_back + histogram[j];   		// 背景像素点
		pixel_fore = pixel_amount - pixel_back;			// 前景像素点
		omega_back = (float)pixel_back/pixel_amount;	// 背景像素百分比
		omega_fore = (float)pixel_fore/pixel_amount;	// 前景像素百分比
		pixel_integral_back += histogram[j] * j;  		// 背景灰度值
		pixel_integral_fore = pixel_integral - pixel_integral_back;	// 前景灰度值
		micro_back = (float)pixel_integral_back / pixel_back;   	// 背景灰度平均值
		micro_fore = (float)pixel_integral_fore / pixel_fore;   	// 前景灰度平均值
		sigma = omega_back * omega_fore * (micro_back - micro_fore) * (micro_back - micro_fore);	// 计算类间方差
		if (sigma > sigma_b)							// 遍历最大类间方差，找出最大类间方差以及对应的阈值
		{
			sigma_b = sigma;
			threshold = j;
		}
	}
	threshold = (uint8_t)(threshold*0.7)+20;	// adjust the value according to field condition

	return threshold;		// the threshold value found by OTSU method
}

// threshold mode: mean
uint8_t hsp_image_threshold_mean(image_t image)
{
	uint16_t i=0, j=0;
	uint8_t threshold;
	uint32_t tv=0;

	for(i=0; i<IMAGEH; i++)
	{
		for(j=0; j<IMAGEW; j++)
			tv += image[i][j];			// accumulation
	}
	threshold = tv/IMAGEH/IMAGEW;		// calculate the average value
	//threshold = threshold*8/10+10;	// adjust the value according to field condition

	return threshold;					// the threshold value found by mean method
}

// threshold mode: mean
uint8_t hsp_image2_threshold_mean(image2_t image)
{
	uint16_t i=0, j=0;
	uint8_t threshold;
	uint32_t tv=0;

	for(i=0; i<IMAGEH2; i++)
	{
		for(j=0; j<IMAGEW2; j++)
			tv += image[i][j];			// accumulation
	}
	threshold = tv/IMAGEH/IMAGEW;		// calculate the average value
	//threshold = threshold*8/10+10;	// adjust the value according to field condition

	return threshold;					// the threshold value found by mean method
}

// threshold mode: min-max
uint8_t hsp_image2_threshold_minmax(image2_t image)
{
	uint16_t i=0, j=0;
	uint8_t min=255, max=0, threshold;

	for(i=0; i<IMAGEH2; i++)
	{
		for(j=0; j<IMAGEW2; j++)
		{
			if(min > image[i][j])
				min = image[i][j];
			if(max < image[i][j])
				max =image[i][j];
		}
	}
	threshold = min + 20;

	return threshold;					// the threshold value found by mean method
}

/* ########################################################### */
static int dc_3[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
static int dr_3[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
static float k[3][3] = {{1/16, 2/16, 1/16}, {2/16, 4/16, 2/16}, {1/16, 2/16, 1/16}};
void hsp_image_fast_conv(image_t input, image_t output)
{
	uint16_t r, c;
	
    // convolution first line
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] = input[r-1][c-1] * k[0][0];
			output[r][c] += input[r-1][c] * k[0][1];
			output[r][c] += input[r-1][c+1] * k[0][2];
        }
    }
    // convolution second line
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] += input[r][c-1] * k[1][0];
			output[r][c] += input[r][c] * k[1][1];
			output[r][c] += input[r][c+1] * k[1][2];
        }
    }
    // convolution third line
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] += input[r+1][c-1] * k[2][0];
			output[r][c] += input[r+1][c] * k[2][1];
			output[r][c] += input[r+1][c+1] * k[2][2];
        }
    }
}

void hsp_image_fast_gauss_conv(image_t input, image_t output)
{
	uint16_t r, c;
	
    // convolution first line
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] = input[r-1][c-1] * 4921 >> 16;
			output[r][c] += input[r-1][c] * 8113 >> 16;
			output[r][c] += input[r-1][c+1] * 4921 >> 16;
        }
    }
    // convolution second line
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] += input[r][c-1] * 8113 >> 16;
			output[r][c] += input[r][c] * 13382 >> 16;
			output[r][c] += input[r][c+1] * 8113 >> 16;
        }
    }
    // convolution third line
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] += input[r+1][c-1] * 4921 >> 16;
			output[r][c] += input[r+1][c] * 8113 >> 16;
			output[r][c] += input[r+1][c+1] * 4921 >> 16;
        }
    }
}

void hsp_image_erode(image_t input, image_t output)
{
	uint16_t r, c, i;
	uint8_t tmp0=0, flag=0;
	
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] = 0;
			flag = 0;
			for(i=0; i<9; i++)
			{
				tmp0 = input[r+dr_3[i]][c+dc_3[i]];
				if(tmp0 == 255)
					flag++;
			}
			if(flag > 5)
			{
				output[r][c] = 255;
			}
		}
	}
}

void hsp_image2_erode(image2_t input, image2_t output)
{
	uint16_t r, c, i;
	uint8_t tmp0=0, flag=0;
	
    for(r=1; r<(IMAGEH2-1); r++)
	{
		for(c=1; c<(IMAGEW2-1); c++)
		{
			output[r][c] = 0;
			flag = 0;
			for(i=0; i<9; i++)
			{
				tmp0 = input[r+dr_3[i]][c+dc_3[i]];
				if(tmp0 == 255)
					flag++;
			}
			if(flag > 5)
			{
				output[r][c] = 255;
			}
		}
	}
}

void hsp_image_dilate(image_t input, image_t output)
{
	uint16_t r, c, i;
	uint8_t tmp0=0, flag=0;
	
    for(r=1; r<(IMAGEH-1); r++)
	{
		for(c=1; c<(IMAGEW-1); c++)
		{
			output[r][c] = 255;
			flag = 0;
			for(i=0; i<9; i++)
			{
				tmp0 = input[r+dr_3[i]][c+dc_3[i]];
				if(tmp0 == 0)
					flag++;
			}
			if(flag > 5)
			{
				output[r][c] = 0;
			}
		}
	}
}

void hsp_image2_dilate(image2_t input, image2_t output)
{
	uint16_t r, c, i;
	uint8_t tmp0=0, flag=0;
	
    for(r=1; r<(IMAGEH2-1); r++)
	{
		for(c=1; c<(IMAGEW2-1); c++)
		{
			output[r][c] = 255;
			flag = 0;
			for(i=0; i<9; i++)
			{
				tmp0 = input[r+dr_3[i]][c+dc_3[i]];
				if(tmp0 == 0)
					flag++;
			}
			if(flag > 3)
			{
				output[r][c] = 0;
			}
		}
	}
}

// https://www.cnblogs.com/ZYQS/p/14665190.html
void hsp_image_sobel1(image_t A, image_t B)
{
	uint16_t i, j;
	int Xg = 0; 	// 横向梯度值
	int Yg = 0; 	// 纵向梯度值
	int XY = 0;
	
	for (i=1; i<(IMAGEH-1); i++)
	{
		for (j=1; j<(IMAGEW-1); j++)
		{
			Xg	= (A[i-1][j+1])
			 	+ (A[ i ][j+1]<<1)
				+ (A[i+1][j+1])
				- (A[i-1][j-1])
				- (A[ i ][j-1]<<1)
				- (A[i+1][j-1]);
			if(Xg < 0)
				Xg = -Xg;

			Yg	= (A[i+1][j-1])
				+ (A[i+1][ j ]<<1)
				+ (A[i+1][j+1])
				- (A[i-1][j-1])
				- (A[i-1][ j ]<<1)
				- (A[i-1][j+1]);
			if(Yg < 0)
				Yg = -Yg;
			
			XY = Xg + Yg;
			if(XY > 255)
				XY = 255;
			B[i][j] = XY;
		}
	}
}

// https://shenchuan.blog.csdn.net/article/details/85250924
void hsp_image_sobel2(image_t A, image_t B)
{
	uint16_t i, j;
	int Xg = 0; 	// 横向梯度值
	int Yg = 0; 	// 纵向梯度值

	for (i=1; i<(IMAGEH-1); i++)
	{
		for (j=1; j<(IMAGEW-1); j++)
		{
			// sobel kernel in the horizontal direction
            Xg  = ((int)A[i-1][j+1]
                - (int)A[i-1][j]
                + ((int)A[i+0][j+1]<<1)
                - ((int)A[i+0][j]<<1)
                + (int)A[i+1][j+1]
                - (int)A[i+1][j]) >> 1;
	        // sobel kernel in the vertical direction
            Yg  = ((int)A[i][j-1]
                + ((int)A[i][j+0]<<1)
                + (int)A[i][j+1]
                - (int)A[i+1][j-1]
                - ((int)A[i+1][j+0]<<1)
                - (int)A[i+1][j+1]) >> 1;
			//B[i][j] = sqrt(Xg*Xg+Yg*Yg);		// 
			B[i][j] = fast_sqrtf(Xg*Xg+Yg*Yg);	// 
		}
	}
}

void hsp_image_canny(image_t A, image_t B, uint8_t low_thres, uint8_t high_thres)
{
	uint16_t i, j, x, y;
	static image_t CannyAm, Sector;
	int Xg = 0;		// 横向梯度值
	int Yg = 0;		// 纵向梯度值
	uint8_t loss = 2;
	uint8_t temp;

	for (i=1; i<(IMAGEH-1); i++)
	{
		for (j=1; j<(IMAGEW-1); j++)
		{
			// sobel kernel in the horizontal direction
            Xg  = ((int)A[i-1][j+1]
                - (int)A[i-1][j]
                + ((int)A[i+0][j+1]<<1)
                - ((int)A[i+0][j]<<1)
                + (int)A[i+1][j+1]
                - (int)A[i+1][j]) >> 1;

            // sobel kernel in the vertical direction
            Yg  = ((int)A[i][j-1]
                + ((int)A[i][j+0]<<1)
                + (int)A[i][j+1]
                - (int)A[i+1][j-1]
                - ((int)A[i+1][j+0]<<1)
                - (int)A[i+1][j+1]) >> 1;
			CannyAm[i][j] = sqrt(Xg*Xg+Yg*Yg);
			Sector[i][j] = atan4(Yg, Xg);
		}
	}
	// 
	for(y=1; y<(IMAGEH-1); y++)
	{
		for(x=1; x<(IMAGEW-1); x++)
		{
			switch(Sector[y][x])
			{
			case 0:
				if( CannyAm[y][x] < (CannyAm[y][x+1] - loss) || CannyAm[y][x] < (CannyAm[y][x-1] - loss) )
				{
					CannyAm[y][x] = 0;
				}
				break; // end of 0
			case 1:
				if( CannyAm[y][x] < (CannyAm[y-1][x+1] - loss) || CannyAm[y][x] < (CannyAm[y+1][x-1] - loss) )
				{
					CannyAm[y][x] = 0;
				}
				break; //end of 1
			case 2:
				if( CannyAm[y][x] < (CannyAm[y-1][x] - loss) || CannyAm[y][x] < (CannyAm[y+1][x] - loss) )
				{
					CannyAm[y][x] = 0;
				}
				break; //end of 2
			case 3:
				if( CannyAm[y][x] < (CannyAm[y-1][x-1] - loss) || CannyAm[y][x] < (CannyAm[y+1][x+1] - loss) )
				{
					CannyAm[y][x] = 0;
				}
				break; //end of 3
			}
		}	// end of for(x)
	}		// end of for(y)

	for(y=1; y<(IMAGEH-1); y++)
	{
		for(x=1; x<(IMAGEW-1); x++)
		{
			if( CannyAm[y][x] < low_thres )
			{
				B[y][x] = 255;
			}
			else if( CannyAm[y][x] > high_thres )
			{
				B[y][x] = 0;
			}
			else
			{
				temp = A[y+1][x-1];
				if(temp < A[y+1][x]) temp = A[y+1][x];
				if(temp < A[y+1][x+1]) temp = A[y+1][x+1];
				if(temp < A[y][x-1]) temp = A[y][x-1];
				if(temp < A[y][x]) temp =A[y][x];
				if(temp < A[y][x+1]) temp = A[y][x+1];
				if(temp < A[y-1][x-1]) temp = A[y-1][x-1];
				if(temp < A[y-1][x]) temp = A[y-1][x];
				if(temp < A[y-1][x+1]) temp = A[y-1][x+1];

				if(temp > high_thres)
				{
					B[y][x] = 0;
					CannyAm[y][x] = 0;
				}
				else
				{
					B[y][x] = 255;
					CannyAm[y][x] = 255;
				}
			}
		}	// end of for(x)
	}		// end of for(y)
}

float fast_sqrtf(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;

	i  = 0x5f375a86 - (i>>1);
	x = *(float*)&i;
	x = x*(1.5f-(xhalf*x*x));

	return 1/x;
}

//==================�����к���=================
/********************************************
*  ֱ�Ӽ��㼸���ֽ���ֵ,ֻ���ж� y/x ��ֵ��
* ��Ӧ�ĽǶ����ĸ���Χ���ɣ�����ʵʱ��atan
*--------------------------------------------
*    �Ƕȷ�Χ        |         ���ȷ�Χ
*--------------------------------------------
* 0     ~ 22.5  ------> 0         ~ 0.3926990
* 22.5  ~ 67.5  ------> 0.3926990 ~ 1.1780972
* 67.5  ~ 112.5 ------> 1.1780972 ~ 1.9634954
* 112.5 ~ 157.5 ------> 1.9634954 ~ 2.7488935
* 157.5 ~ 180   ------> 2.7488935 ~ 3.1415926
*--------------------------------------------
*         y/xֵ��Ӧ����
*  0          ----  0.41421356  ˮƽ����
*  0.41421347 ----  2.41421356  ���ϡ�����
*  2.41421326 ---- -2.41421356  ��ֱ����
* -2.41421362 ---- -0.41421356  ���ϡ�����
* -0.41421365 ----  0           ˮƽ����
*********************************************/
uint8_t atan4(float y, float x)
{
    float tan_num;
    uint8_t alpha; //���ؽǶ�
    tan_num = y/x;
	
    if( tan_num> -0.41421365 && tan_num< 0.41421356 )
    {
		alpha = 0; //ˮƽ����
    }
    else if( tan_num>= 0.41421356 && tan_num< 2.41421356)
    {
		alpha = 1; //���ϡ�����
    }
    else if( tan_num<= -0.41421356 && tan_num> -2.41421362)
    {
		alpha = 3; //���ϡ�����
    }
    else
	{
		alpha = 2; //��ֱ����
	}
	
    return alpha; //����
}