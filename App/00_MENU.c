#include "00_MENU.h"
#include "Ex3.h"
#include "Ex8.h"

// https://github.com/Ybalrid/mma8451_pi/blob/master/src/mma8451_pi.c
// https://github.com/coocox/cox/blob/master/CoX/Driver/Sensor_Accelerometer/MMA8451/Driver/lib/MMA8451.c

#define DEVICEIDREG1    0x1FFF7A10  // UNIQUE_ID[31:0]
#define DEVICEIDREG2    0x1FFF7A14  // UNIQUE_ID[63:32]
#define DEVICEIDREG3    0x1FFF7A18  // UNIQUE_ID[95:64]
#define STORAGE_INFO    0x1FFF7A20  // StorageSize

rtc_parameter_struct   rtc_initpara1;
uint8_t datetime1[6];
char LcdLineStr[20];
uint16_t QESVar;
uint16_t tQESVar;

#define RTC_CLOCK_SOURCE_LXTAL 
#define BKP_VALUE    0x32F1

typedef enum {
    keyUP,
    keyDOWN,
    keySTILL
} QESDir;

QESDir Scroll;

enum RtcIdx {
    YY,
    MM,
    DD,
    hh,
    mm,
    ss
};

enum Arrow {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

struct McuInfo_t
{
    uint16_t FlashSize;
    uint16_t SramSize;
    uint8_t UniqueID[12];
} McuInfo;

// value limit for date & time
const uint8_t rtclimit[6][2]={
    {20, 99},   // for year
    {1, 12},    // for month
    {1, 31},    // for date
    {0, 23},    // for hour
    {0, 59},    // for minute
    {0, 59}     // for second
};

// Menu Cursor bitmap
const unsigned char MenuCursor16x16[]={
    0x80,0x00,0x80,0x01,0x80,0x03,0x80,0x07,
    0xFF,0x0F,0xFF,0x1F,0xFF,0x3F,0xFF,0x7F,
    0xFF,0xFF,0xFF,0x7F,0xFF,0x3F,0xFF,0x1F,
    0xFF,0x0F,0x80,0x07,0x80,0x03,0x80,0x01,
};

// Menu Cursor bitmap
const unsigned char Arrow16x16[4][32]={
   {0x00,0x00,0x80,0x00,0xC0,0x01,0xE0,0x03,    // UP
    0xF0,0x07,0xF8,0x0F,0xFC,0x1F,0xFE,0x3F,
    0xE0,0x03,0xE0,0x03,0xE0,0x03,0xE0,0x03,
    0xE0,0x03,0xE0,0x03,0xE0,0x03,0x00,0x00},
   {0x00,0x00,0xE0,0x03,0xE0,0x03,0xE0,0x03,    // DOWN
    0xE0,0x03,0xE0,0x03,0xE0,0x03,0xE0,0x03,
    0xFE,0x3F,0xFC,0x1F,0xF8,0x0F,0xF0,0x07,
    0xE0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00},
   {0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x07,    // RIGHT
    0x00,0x0F,0xFE,0x1F,0xFE,0x3F,0xFE,0x7F,
    0xFE,0x3F,0xFE,0x1F,0x00,0x0F,0x00,0x07,
    0x00,0x03,0x00,0x01,0x00,0x00,0x00,0x00},
   {0x00,0x00,0x80,0x00,0xC0,0x00,0xE0,0x00,    // LEFT
    0xF0,0x00,0xF8,0x7F,0xFC,0x7F,0xFE,0x7F,
    0xFC,0x7F,0xF8,0x7F,0xF0,0x00,0xE0,0x00,
    0xC0,0x00,0x80,0x00,0x00,0x00,0x00,0x00},
};


uint8_t menu_item0[8][20]=
{
	"1.BasicIO(HMI)",
	"2.ADC(Sensors)",
	"3.I2C(Bar/SEG)",
	"4.I2C(MMA8451)",
	"5.LCD(Bitmaps)",
	"6.LCD(GIFdemo)",
	"7.RealTimeCloc",
	"8.SystemInform",
};

uint8_t hsp_menu_loop(void)
{
    uint8_t ItemNumber=0;
    uint8_t CmdIdx=0, CmdOk=0;
    uint8_t StatusA, StatusB;
    uint8_t tStatusA, tStatusB;
    //QESDir Scroll;

	Ex3_3_bitmap();
	delay_1ms(1500);

	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
    
    hsp_tft18_show_str(32, 0, menu_item0[0]);
    hsp_tft18_show_str(32, 1, menu_item0[1]);
    hsp_tft18_show_str(32, 2, menu_item0[2]);
    hsp_tft18_show_str(32, 3, menu_item0[3]);
    hsp_tft18_show_str(32, 4, menu_item0[4]);
    hsp_tft18_show_str(32, 5, menu_item0[5]);
    hsp_tft18_show_str(32, 6, menu_item0[6]);
    hsp_tft18_show_str(32, 7, menu_item0[7]);
    
    show_menu_cursor(ItemNumber, WHITE);
    Scroll = keySTILL;

    StatusA = PHA2();   StatusB = PHB2();
    tStatusA = StatusA; tStatusB = StatusB;
    
    while (1)
    {
        StatusA = PHA2();   StatusB = PHB2();
        if ((tStatusA==StatusA) || (RESET==StatusA))
        {
            Scroll = keySTILL;
            if (!S1())
            {
                delay_1ms(2);
                if (!S1())
                    Scroll = keyUP;
                while(!S1());
            }
            if (!S2())
            {
                delay_1ms(2);
                if (!S2())
                    Scroll = keyDOWN;
                while(!S2());
            }
        }
        else
        {
            if (RESET == StatusB)
                Scroll = keyDOWN;
            if (SET == StatusB)
                Scroll = keyUP;
        }
        
        if (keyUP == Scroll)    // cursor move up
        {
            if (ItemNumber>0)
            {
                show_menu_cursor(ItemNumber--, BLACK);
            }
            else if (!SW4())
            {
                BUZZ_ON();  delay_1ms(1); BUZZ_OFF();  delay_1ms(5); 
            }
            if (!SW4()) { BUZZ_ON();  delay_1ms(1); BUZZ_OFF(); }
            show_menu_cursor(ItemNumber, WHITE);
            //Scroll = keySTILL;
        }
        if (keyDOWN == Scroll)    // cursor move down
        {
            if (ItemNumber<7)
            {
                show_menu_cursor(ItemNumber++, BLACK);
            }
            else if (!SW4())
            {
                BUZZ_ON();  delay_1ms(1); BUZZ_OFF();  delay_1ms(5); 
            }
            if (!SW4()) { BUZZ_ON();  delay_1ms(1); BUZZ_OFF(); }
            show_menu_cursor(ItemNumber, WHITE);
            //Scroll = keySTILL;
        }
        tStatusA = StatusA; tStatusB = StatusB;
        Scroll = keySTILL;
        
        if ((!PUSH()) || (!S3()))    // push button pressed        
        {
            delay_1ms(50);  // de-jitter
            if ((!PUSH()) || (!S3()))
            {
                while(!PUSH());
                return ItemNumber;
            }
        }
    }
}

// MenuCursor16x16
void show_menu_cursor(uint8_t ItemNumber, uint16_t color)
{
    uint8_t i,j;
    uint8_t temp1, temp2;
    
    for(i=0; i<16; i++)
    {
        hsp_tft18_set_region(12, ItemNumber*16+i+2, 28, ItemNumber*16+i+2);
        temp1 = MenuCursor16x16[i*2];
        temp2 = MenuCursor16x16[i*2+1];
        for(j=0; j<8; j++)
        {
            if(temp1&0x01)
                hsp_tft18_write_2byte(color);
            else
                hsp_tft18_write_2byte(BLACK);
            temp1 >>= 1;
        }
        for(j=0; j<8; j++)
        {
            if(temp2&0x01)
                hsp_tft18_write_2byte(color);
            else
                hsp_tft18_write_2byte(BLACK);
            temp2 >>= 1;
        }
    }
}

void Ex0_gif_poweron(void)
{
	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(WHITE);

	while(1)
	{
		testAll();
	}
}

void hsp_rtc_demo(void)
{
    uint16_t i;
    uint8_t StatusPHA, StatusPHB;
    uint8_t jStatusPHA, jStatusPHB;
    uint8_t ItemNumber=0;

	__IO uint32_t prescaler_a = 0, prescaler_s = 0;
	uint8_t year, month, day, hour, minute, second;
	uint8_t tyear, tmonth, tday, thour, tminute, tsecond;
	rtc_parameter_struct   rtc_now, rtc_justnow;

    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();

    //rtc_pre_config();
	//hsp_rtc_config();
    rcu_all_reset_flag_clear();
    
    rtc_current_time_get(&rtc_now);
    year = BCD2BIN(rtc_now.year);   month = BCD2BIN(rtc_now.month);     day = BCD2BIN(rtc_now.date);
    hour = BCD2BIN(rtc_now.hour);   minute = BCD2BIN(rtc_now.minute);   second = BCD2BIN(rtc_now.second);
    datetime1[0] = year;     datetime1[1] = month;        datetime1[2] = day;
    datetime1[3] = hour;     datetime1[4] = minute;       datetime1[5] = second;
    
    LED1_ON();  LED2_OFF();
	
	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
    DrawRtcDemoScreen();
    for (i=0; i<6; i++)
        UpdateSetValue(i);
    
    StatusPHA = PHA2();     jStatusPHA = StatusPHA;
    StatusPHB = PHB2();     jStatusPHB = StatusPHB;
	
	if(!SW3())
		hsp_rtc_setup(24U, 2U, 26U, 11U, 10U, 0U);;

    while (1)
    {
        rtc_current_time_get(&rtc_now);
        year = BCD2BIN(rtc_now.year);   month = BCD2BIN(rtc_now.month);     day = BCD2BIN(rtc_now.date);
        hour = BCD2BIN(rtc_now.hour);   minute = BCD2BIN(rtc_now.minute);   second = BCD2BIN(rtc_now.second);

        if (tsecond != second)
        {
            hsp_tft18_show_char16(1,2,2);			hsp_tft18_show_char16(17,2,0);
            hsp_tft18_show_char16(33,2,year/10);	hsp_tft18_show_char16(49,2,year%10);
            hsp_tft18_show_char16(81,2,month/10);	hsp_tft18_show_char16(97,2,month%10);
            hsp_tft18_show_char16(129,2,day/10);	hsp_tft18_show_char16(145,2,day%10);
            
            hsp_tft18_show_char16(33,22,hour/10);		hsp_tft18_show_char16(49,22,hour%10);
            hsp_tft18_show_char16(81,22,minute/10);		hsp_tft18_show_char16(97,22,minute%10);
            hsp_tft18_show_char16(129,22,second/10);	hsp_tft18_show_char16(145,22,second%10);

            tyear = year;   tmonth = month;     tday = day;
            thour = hour;   tminute = minute;   tsecond = second;
            LED1_TOGGLE();  LED2_TOGGLE();
        }
        
        Scroll = keySTILL;
        if (!S1())
        {
            delay_1ms(2);
            if (!S1())
                Scroll = keyUP;
            while(!S1());
        }
        if (!S2())
        {
            delay_1ms(2);
            if (!S2())
                Scroll = keyDOWN;
            while(!S2());
        }
        if (keyUP == Scroll)    // cursor move up
        {
            if (ItemNumber>0)
            {
                UpdateArrowPos(ItemNumber--, BLACK, BLACK);
            }
            else if (!SW4())
            {
                BUZZ_ON();  delay_1ms(10); BUZZ_OFF();  delay_1ms(5); 
            }
            if (!SW4()) { BUZZ_ON();  delay_1ms(1); BUZZ_OFF(); }
            UpdateArrowPos(ItemNumber, CHOCOLATE, BLACK);
            Scroll = keySTILL;
        }
        if (keyDOWN == Scroll)    // cursor move down
        {
            if (ItemNumber<5)
            {
                UpdateArrowPos(ItemNumber++, BLACK, BLACK);
            }
            else if (!SW4())
            {
                BUZZ_ON();  delay_1ms(10); BUZZ_OFF();  delay_1ms(5); 
            }
            if (!SW4()) { BUZZ_ON();  delay_1ms(1); BUZZ_OFF(); }
            UpdateArrowPos(ItemNumber, CHOCOLATE, BLACK);
            Scroll = keySTILL;
        }

        StatusPHA = PHA2();   StatusPHB = PHB2();
        if ((jStatusPHA!=StatusPHA) && (RESET==StatusPHA))
        {
            if (SET == StatusPHB) // CW to increase
            {
                if(datetime1[ItemNumber] < rtclimit[ItemNumber][1])
                    //datetime1[ItemNumber] = datetime1[ItemNumber] + 1;
                datetime1[ItemNumber]++;
            }
            if (RESET == StatusPHB)   // CCW to decrease
            {
                if(datetime1[ItemNumber] > rtclimit[ItemNumber][0])
                    //datetime[ItemNumber] = datetime1[ItemNumber] - 1;
                datetime1[ItemNumber]--;
            }
            UpdateSetValue(ItemNumber);
            delay_1ms(40);
        }
        jStatusPHA = StatusPHA;
        jStatusPHB = StatusPHB;
        
        // update RTC when PUSH button impressed
        if (!PUSH())
        {
            delay_1ms(200);     // ensure operation on purpose
            if (!PUSH())
            {
                rtc_initpara1.factor_asyn = prescaler_a;
                rtc_initpara1.factor_syn = prescaler_s;
                rtc_initpara1.display_format = RTC_24HOUR;
                rtc_initpara1.day_of_week = RTC_SATURDAY;
                rtc_initpara1.am_pm = RTC_AM;
                rtc_initpara1.year   = BIN2BCD(datetime1[YY]);
                rtc_initpara1.month  = BIN2BCD(datetime1[MM]);
                rtc_initpara1.date   = BIN2BCD(datetime1[DD]);
                rtc_initpara1.hour   = BIN2BCD(datetime1[hh]);
                rtc_initpara1.minute = BIN2BCD(datetime1[mm]);
                rtc_initpara1.second = BIN2BCD(datetime1[ss]);
                rtc_init(&rtc_initpara1);
            }
//            if (!SW1())
//            {
//                BUZZ_ON();
//                delay_1ms(40);
//                BUZZ_OFF();
//            }
            BUZZ_ON();
            delay_1ms(40);
            BUZZ_OFF();

            while (!PUSH());
            delay_1ms(100);
        }

        i += 1;
        hsp_tft18_draw_block(1, 26, 29, 14, i);
		
		if(!S3()) break;
    }
}

void rtc_show_time(void)
{
    char line1[20], line2[20];
    rtc_current_time_get(&rtc_initpara1);  
    printf("Current time: %0.2x:%0.2x:%0.2x\n\r", \
          rtc_initpara1.hour, rtc_initpara1.minute, rtc_initpara1.second);
    //sprintf(line1, "%0.2x-%0.2x-%0.2x",rtc_initpara1.year, rtc_initpara1.month, rtc_initpara1.date);
    //sprintf(line2, "%0.2x:%0.2x:%0.2x",rtc_initpara1.hour, rtc_initpara1.minute, rtc_initpara1.second);
}

void DrawRtcDemoScreen()
{
    hsp_tft18_show_str_color(1, 3, "S1:  ", YELLOW, BLUE);
    hsp_tft18_show_str_color(41, 3, "S2:  ", BLUE, GREEN);
    hsp_tft18_show_str_color(81, 3, "PUSH:Enter", WHITE, RED);
    // draw UP arrow
    ShowArrow16x16(25, 50, UP, YELLOW, BLUE);
    // draw DOWN arrow
    ShowArrow16x16(65, 50, DOWN, BLUE, GREEN);

    // block for decorating seperator
    hsp_tft18_draw_block(1, 43, 160, 5, GRAY2);

    hsp_tft18_show_str_color(1, 4, "         ", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(5, 4, "YY-MM-DD", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(89, 4, "         ", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(93, 4, "hh:mm:ss", DARKBLUE, GOLD);
    
    // input content indicator for YY-MM-DD
    hsp_tft18_show_str_color(17, 5, "YY:", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(17, 6, "MM:", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(17, 7, "DD:", DARKBLUE, GOLD);
    // input content indicator for hh:mm:ss
    hsp_tft18_show_str_color(105, 5, "hh:", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(105, 6, "mm:", DARKBLUE, GOLD);
    hsp_tft18_show_str_color(105, 7, "ss:", DARKBLUE, GOLD);

    // block for vertical decorating seperator
    hsp_tft18_draw_block(73, 66, 16, 63, GRAY2);

    // YY-MM-DD seperators
    hsp_tft18_show_char16(65,2,10);    hsp_tft18_show_char16(113,2,10);
    // hh:mm:ss seperators
    hsp_tft18_show_char16(65,22,11);   hsp_tft18_show_char16(113,22,11);

    // draw RIGHT arrow
    ShowArrow16x16(1, 82, RIGHT, CHOCOLATE, BLACK);
    // draw LEFT arrow
    ShowArrow16x16(57, 82, LEFT, CHOCOLATE, BLACK);
    
}

uint8_t BCD2BIN(uint8_t bcd)
{
    return (((bcd>>4)&0x0F)*10 + (bcd&0x0F));
}

uint8_t BIN2BCD(uint8_t bin)
{
    return (bin/10<<4 | bin%10);
}

// show arrow mark, font: MenuCursor16x16
void ShowArrow16x16(uint8_t x, uint8_t y, uint8_t no, uint16_t fColor, uint16_t bColor)
{
    uint8_t i,j;
    uint8_t temp1, temp2;
    
    if (no > 3) // up to 4 arrows defined
        return;
    for(i=0; i<16; i++)
    {
        hsp_tft18_set_region(x, y+i, x+16, y+i);
        temp1 = Arrow16x16[no][i*2];
        temp2 = Arrow16x16[no][i*2+1];
        for(j=0; j<8; j++)
        {
            if(temp1&0x01)
                hsp_tft18_write_2byte(fColor);
            else
                hsp_tft18_write_2byte(bColor);
            temp1>>=1;
        }
        for(j=0; j<8; j++)
        {
            if(temp2&0x01)
                hsp_tft18_write_2byte(fColor);
            else
                hsp_tft18_write_2byte(bColor);
            temp2>>=1;
        }
    }
}

// clear arrow mark
void ClearArrow16x16(uint8_t x, uint8_t y, uint16_t color)
{
    uint8_t i,j;
    
    for(i=0; i<16; i++)
    {
        hsp_tft18_set_region(x, y+i, x+16, y+i);
        for(j=0; j<16; j++)
        {
            hsp_tft18_write_2byte(color);
        }
    }
}

void UpdateArrowPos(uint8_t no, uint16_t fColor, uint16_t bColor)
{
    uint8_t posx, posy;
    
    // define (x,y) position for each input item
    switch (no)
    {
        case 1:     // for MM
            posx = 1;
            posy = 98;
            break;
        case 2:     // for DD
            posx = 1;
            posy = 114;
            break;
        case 3:     // for hh
            posx = 89;
            posy = 82;
            break;
        case 4:     // for mm
            posx = 89;
            posy = 98;
            break;
        case 5:     // for ss
            posx = 89;
            posy = 114;
            break;
        //case 0:
        default:    // for YY
            posx = 1;
            posy = 82;
            break;
    }
    ShowArrow16x16(posx, posy, RIGHT, fColor, bColor);
    ShowArrow16x16(posx+56, posy, LEFT, fColor, bColor);
}

// no: from 0~5 for YY-MM-DD / hh:mm:ss, pos defined on RTC screen
//void UpdateSetValue(uint8_t no, uint8_t number)
void UpdateSetValue(uint8_t no)
{
    uint8_t posx, posy;
    uint8_t linestr[20];

    if (no > 5)     // up to 6 idx defined
        return;
    
    // define (x,y) position for each input item
    switch (no)
    {
        case 1:     // for MM
            posx = 41;      // point
            posy = 6;       // line no.
            break;
        case 2:     // for DD
            posx = 41;      // point
            posy = 7;       // line no.
            break;
        case 3:     // for hh
            posx = 129;     // point
            posy = 5;       // line no.
            break;
        case 4:     // for mm
            posx = 129;     // point
            posy = 6;       // line no.
            break;
        case 5:     // for ss
            posx = 129;     // point
            posy = 7;       // line no.
            break;
        //case 0:
        default:    // for YY
            posx = 41;      // point
            posy = 5;       // line no.
            break;
    }
    //sprintf(LineStr, "%02d", number);
    sprintf(linestr, "%02d", datetime1[no]);
    hsp_tft18_show_str_color(posx, posy, linestr, BLACK, WHITE);
}

uint8_t hsp_demo_hmi()
{
    uint16_t i=0, j=0;
    int16_t encoder=0;
    uint8_t barspeed=0;
    uint8_t SWCode, tSWcode;

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
    hsp_i2c_init();
    hsp_cat9555_init();
    hsp_demo_frame_sysinfo();
    hsp_tft18_show_str(5, 4, "QES:00000");
    
    SWCode = hsp_get_taskid();
    tSWcode = SWCode+1;
    tQESVar = QESVar;

    while(1)
    {
        i++;

        SWCode = hsp_get_taskid();
        
        if (tQESVar != QESVar)
        {
            if (tQESVar < QESVar)
            {
                encoder++;
            }
            else if (tQESVar > QESVar)
            {
                encoder--;
            }
            tQESVar = QESVar;
            sprintf(LcdLineStr, "QES:%05d", encoder);
            hsp_tft18_show_str(5, 4, (uint8_t *)LcdLineStr);
        }
        
        if (tSWcode != SWCode)
        {
            if (SW4())
            {
                hsp_tft18_draw_block(82, 64, 17, 12, BLACK);  // Upper BLACK
                hsp_tft18_draw_block(82, 81, 17, 12, WHITE);  // Lower WHITE
            }
            else
            {
                hsp_tft18_draw_block(82, 64, 17, 12, WHITE);  // Upper BLACK
                hsp_tft18_draw_block(82, 81, 17, 12, BLACK);  // Lower WHITE
            }
            if (SW3())
            {
                hsp_tft18_draw_block(102, 64, 17, 12, BLACK);  // Upper BLACK
                hsp_tft18_draw_block(102, 81, 17, 12, WHITE);  // Lower WHITE
            }
            else
            {
                hsp_tft18_draw_block(102, 64, 17, 12, WHITE);  // Upper BLACK
                hsp_tft18_draw_block(102, 81, 17, 12, BLACK);  // Lower WHITE
            }
            if (SW2())
            {
                hsp_tft18_draw_block(122, 64, 17, 12, BLACK);  // Upper BLACK
                hsp_tft18_draw_block(122, 81, 17, 12, WHITE);  // Lower WHITE
            }
            else
            {
                hsp_tft18_draw_block(122, 64, 17, 12, WHITE);  // Upper BLACK
                hsp_tft18_draw_block(122, 81, 17, 12, BLACK);  // Lower WHITE
            }
            if (SW1())
            {
                hsp_tft18_draw_block(142, 64, 17, 12, BLACK);  // Upper BLACK
                hsp_tft18_draw_block(142, 81, 17, 12, WHITE);  // Lower WHITE
            }
            else
            {
                hsp_tft18_draw_block(142, 64, 17, 12, WHITE);  // Upper BLACK
                hsp_tft18_draw_block(142, 81, 17, 12, BLACK);  // Lower WHITE
            }
            tSWcode = SWCode;
        }
        
        if (!PUSH())
        {
            hsp_tft18_draw_block(20, 115, 27, 12, RED);
            encoder = 0;
            hsp_tft18_show_str(5, 4, "QES:00000");
			BUZZ_ON();
        }
        else
        {
            hsp_tft18_draw_block(20, 115, 27, 12, GRAY2);
            BUZZ_OFF();
        }
        if (!S1())
        {
            hsp_tft18_draw_block(76, 115, 27, 12, BLUE);
            LED2_ON();
        }
        else
        {
            hsp_tft18_draw_block(76, 115, 27, 12, GRAY2);
            LED2_OFF();
        }
        if (!S2())
        {
            hsp_tft18_draw_block(132, 115, 27, 12, GREEN);
            LED1_ON();
        }
        else
        {
            hsp_tft18_draw_block(132, 115, 27, 12, GRAY2);
            LED1_OFF();
        }
		
		if(!S3()) break;
    }
}

void hsp_demo_frame_sysinfo()
{
    uint16_t LineColor = RED;

    hsp_sysinfo();

    sprintf(LcdLineStr, "ChipID[..%02X%02X%02X%02X%02X]", \
            McuInfo.UniqueID[7], McuInfo.UniqueID[8],
            McuInfo.UniqueID[9], McuInfo.UniqueID[10], McuInfo.UniqueID[11]);
    hsp_tft18_show_str(1, 0, (uint8_t *)LcdLineStr);

    sprintf(LcdLineStr, "FlashSize[%04dKByte]", McuInfo.FlashSize);
    hsp_tft18_show_str(1, 1, (uint8_t *)LcdLineStr);

    sprintf(LcdLineStr, "SramSize=[%04dKByte]", McuInfo.SramSize);
    hsp_tft18_show_str(1, 2, (uint8_t *)LcdLineStr);
    
    //sprintf(LcdLineStr, "********************");
    sprintf(LcdLineStr, "====================");
    hsp_tft18_show_str(1, 3, (uint8_t *)LcdLineStr);
    
    //hsp_tft18_show_str(1, 4, "QES:");
    hsp_tft18_show_str_color(1, 6, " BUZZ ", YELLOW, RED);
    hsp_tft18_show_str_color(57, 6, " LED1 ", WHITE, BLUE);
    hsp_tft18_show_str_color(113, 6, " LED2 ", BLACK, GREEN);
    hsp_tft18_show_str(1, 7, "PB     S1     S2    ");
    
    LineColor = RED;
    hsp_tft18_draw_line_h(80, 62, 80, LineColor);
	hsp_tft18_draw_line_h(80, 94, 80, LineColor);
    hsp_tft18_draw_line_v(80, 62, 32, LineColor);
	hsp_tft18_draw_line_v(100, 62, 32, LineColor);
    hsp_tft18_draw_line_v(120, 62, 32, LineColor);
	hsp_tft18_draw_line_v(140, 62, 32, LineColor);
    hsp_tft18_draw_line_v(160, 62, 33, LineColor);

    hsp_tft18_draw_line_h(1, 83, 79, RED);
    hsp_tft18_draw_line_h(1, 85, 79, GREEN);
    hsp_tft18_draw_line_h(1, 87, 79, BLUE);
    hsp_tft18_draw_line_h(0, 97, 161, GRAY0);
    
    // block for PB
    hsp_tft18_draw_frame(18, 112, 30, 16, RED);
    hsp_tft18_draw_block(20, 115, 27, 12, GRAY2);
    // block for S1
    hsp_tft18_draw_frame(74, 112, 30, 16, BLUE);
    hsp_tft18_draw_block(76, 115, 27, 12, GRAY2);
    // block for S2
    hsp_tft18_draw_frame(130, 112, 30, 16, GREEN);
    hsp_tft18_draw_block(132, 115, 27, 12, GRAY2);
}

void hsp_sysinfo(void)
{
    uint8_t i;
    uint32_t storage;
    
	storage = *(uint32_t *)STORAGE_INFO;
    McuInfo.FlashSize = (storage>>16);
    McuInfo.SramSize = (storage&0xFFFF);
	
//	printf("SRAM Size=%dk\r\n", McuInfo.FlashSize);
//	printf("FLASH Size=%dk\r\n", McuInfo.SramSize);
//	printf("UniqueID:[");
	for(i=0; i<12; i++)
	{
		McuInfo.UniqueID[i] = *(uint8_t *)(DEVICEIDREG1+i);
//		printf(" %02X", McuInfo.UniqueID[i]);
	}
//	printf("]\r\n");
}
