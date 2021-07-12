/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**************************************************************************//**
* @file         camera_mipi_imx219_b_regdata.c
* @version      0.01
* @brief        MIPI Camera Register Setting
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "mipi_camera.h"
#if (CAM_MIPI == CAM_IMX219_B)
#include    "r_typedefs.h"
#include    "camera_mipi_imx219_b_regdata.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

const uint16_t gain_table[17] = {
  0,	/*     0dB */
 28,	/*  1.01dB */
 53,	/*  2.01dB */
 75,	/*  3.01dB */
 95,	/*  4.03dB */
112,	/*  5.00dB */
128,	/*  6.02dB */
142,	/*  7.03dB */
155,	/*  8.08dB */
166,	/*  9.08dB */
176,	/* 10.10dB */
186,	/* 11.26dB */
196,	/* 12.04dB */
206,	/* 14.19dB */
216,	/* 16.12dB */
226,	/* 18.62dB */
232		/* 20.56dB */};

const st_mipi_reg_t g_mipi_reg_init_table[] =
{
    /* Initialize */
        /* 2-Lane */
        {0x01, 0x14, 0x01},
        /* timing setting:manual mode */
        {0x01, 0x28, 0x01},
        /* INCK = 12MHz */
        {0x01, 0x2A, 0x0C},
        {0x01, 0x2B, 0x00},
		/* frame length(height) = 2600(=A28H) */
		{0x01, 0x60, 0x0A},
		{0x01, 0x61, 0x28},
		/* line  length(width)  = 3448(=D78H)(default) */
		//{0x01, 0x62, 0x0D}, // default
		//{0x01, 0x63, 0x78}, // default
#if (CAM_BINNING_ENABLE == 1 )
		/* X range */       // x1 = (3280 - 2*640) /2 = 1000 -> 0x03E8
							// x2 = x1 + 2*640 -1     = 2279 -> 0x08E7
		{0x01, 0x64, 0x03}, // x_addr_start top left corner visible area high-byte
		{0x01, 0x65, 0xE8}, // x_addr_start top left corner visible area low-byte
		{0x01, 0x66, 0x08}, // x_addr_end bottom right corner visible area high-byte
		{0x01, 0x67, 0xE7}, // x_addr_end bottom right corner visible area low-byte
		/* Y range */       // y1 = (2464 - 480*2) / 2 =  752  --> 0x2F0
							// y2 = y1 + 2*480 -1      =  1711  --> 0x6af
		{0x01, 0x68, 0x02}, // y_addr_start top left corner visible area high-byte
		{0x01, 0x69, 0xF0}, // y_addr_start top left corner visible area low-byte
		{0x01, 0x6A, 0x06}, // y_addr_end bottom right corner visible area high-byte
		{0x01, 0x6B, 0xAF}, // y_addr_end bottom right corner visible area low-byte
		/* binning (digital) mode */
		{0x01, 0x74, 0x01}, // h mode 0-no 1-x2 2-x4, 3-analog
		{0x01, 0x75, 0x01}, // v mode 0-no 1-x2 2-x4, 3-analog
		{0x01, 0x76, 0x01}, // h bmode 0-average 1-sum
		{0x01, 0x77, 0x01}, // v bmode 0-average 1-sum
#elif (CAM_BINNING_ENABLE == 2 )
		/* X range */       // x1 = (3280 - 4*640) /2 = 360 -> 0x0168
							// x2 = x1 + 4*640 -1     = 2919 -> 0x0B67
		{0x01, 0x64, 0x01}, // x_addr_start top left corner visible area high-byte
		{0x01, 0x65, 0x68}, // x_addr_start top left corner visible area low-byte
		{0x01, 0x66, 0x0B}, // x_addr_end bottom right corner visible area high-byte
		{0x01, 0x67, 0x67}, // x_addr_end bottom right corner visible area low-byte
		/* Y range */       // y1 = (2464 - 480*4) / 2 =  272  --> 0x110
							// y2 = y1 + 4*480 -1      =  2192  --> 0x88f
		{0x01, 0x68, 0x01}, // y_addr_start top left corner visible area high-byte
		{0x01, 0x69, 0x10}, // y_addr_start top left corner visible area low-byte
		{0x01, 0x6A, 0x08}, // y_addr_end bottom right corner visible area high-byte
		{0x01, 0x6B, 0x8F}, // y_addr_end bottom right corner visible area low-byte
		/* binning (digital) mode */
		{0x01, 0x74, 0x02}, // h mode 0-no 1-x2 2-x4, 3-analog
		{0x01, 0x75, 0x02}, // v mode 0-no 1-x2 2-x4, 3-analog
		{0x01, 0x76, 0x00}, // h bmode 0-average 1-sum
		{0x01, 0x77, 0x00}, // v bmode 0-average 1-sum
#else
		/* X range */       // x1 = (3280 - 640) /2 = 1320 -> 0x0528
							// x2 = x1 + 640 -1     = 1959 -> 0x07A7
		{0x01, 0x64, 0x05}, // x_addr_start top left corner visible area high-byte
		{0x01, 0x65, 0x28}, // x_addr_start top left corner visible area low-byte
		{0x01, 0x66, 0x07}, // x_addr_end bottom right corner visible area high-byte
		{0x01, 0x67, 0xa7}, // x_addr_end bottom right corner visible area low-byte
		/* Y range */       // y1 = (2464 - 480) / 2 =  992  --> 0x3E0
							// y2 = y1 + 480 -1      =  1471 --> 0x5BF
		{0x01, 0x68, 0x03}, // y_addr_start top left corner visible area high-byte
		{0x01, 0x69, 0xE0}, // y_addr_start top left corner visible area low-byte
		{0x01, 0x6A, 0x05}, // y_addr_end bottom right corner visible area high-byte
		{0x01, 0x6B, 0xBF}, // y_addr_end bottom right corner visible area low-byte
		/* binning (digital) mode */
		{0x01, 0x74, 0x00}, // h off
		{0x01, 0x75, 0x00}, // v off
#endif
		/* signal range */
		{0x01, 0x6C, 0x02}, // 640 output image size x-dir high-byte
		{0x01, 0x6D, 0x80}, // output image size x-dir low-byte
		{0x01, 0x6E, 0x01}, // 480 output image size y-dir high-byte
		{0x01, 0x6F, 0xE0}, // output image size y-dir low-byte
		/* output format RAW8 */
		{0x01, 0x8C, 0x08},
		{0x01, 0x8D, 0x08},
		/* Vertical/horizontal Flip */
		{0x01, 0x72, 0x00}, // vertical[1] horizontal[0]
         /* Long shutter time */
        {0x01, 0x5A, 0x0B}, // default 03
        {0x01, 0x5B, 0xAD}, // default E8
        /* analog gain(LONG) control */
        {0x01, 0x57, 0xD0}, // 0xE0
        /* Timing settings */
        {0x01, 0x1C, 0x00}, /* 0x00 THS_ZERO_MIN */
        {0x01, 0x1D, 0x57}, /* 0x57 THS_ZERO_MIN */
        {0x01, 0x24, 0x00}, /* 0x00 TCLK_ZERO */
        {0x01, 0x25, 0xBF}, /* 0xbf TCLK_ZERO */
        /* clock setting */
        {0x03, 0x01, 0x05},
        {0x03, 0x04, 0x02},
        {0x03, 0x05, 0x02},
        {0x03, 0x06, 0x00},
        {0x03, 0x07, 0x2b}, //0x2b
        {0x03, 0x09, 0x08},
        {0x03, 0x0C, 0x00},
        {0x03, 0x0D, 0x2e}, //0x2e
		{0xFF, 0xFF, 0xFF}, // End record
};

#endif /* CAM_MIPI == CAM_IMX219_B */
