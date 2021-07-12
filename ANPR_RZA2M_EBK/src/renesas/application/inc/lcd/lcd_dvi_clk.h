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
* @file         lcd_dvi_clk.h
* @version      0.01
* @brief        Defines for VDC clock.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>

#include    "r_typedefs.h"

#include    "pc_monitor.h"
#include    "r_vdc.h"

#ifndef DVI_CLK_H
#define DVI_CLK_H

/******************************************************************************
Macro definitions
******************************************************************************/
#if ( SELECT_MONITOR == SVGA_800_600_60HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     29    3    4        75.0  MHz, SVGA Signal  800  x 600 (60 Hz)
	*/
	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_EXT_0)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_EXT_0)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) /* DCDR  Not use */
	#define LVDS_PLL_NFD              (30u-1u)               /* NFD   Not use */
	#define LVDS_PLL_NRD              (4u-1u)                /* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)      /* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      /* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   /* Not use */

#elif ( SELECT_MONITOR == HDTV_720p_1280_720_60HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     52    7    4        73.75 MHz, HDTV 720p Signal  1280  x 720 (60 Hz)
	*/

	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_EXT_0)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) 	/* DCDR  Not use */
	#define LVDS_PLL_NFD              (59u-1u)              	/* NFD   Not use */
	#define LVDS_PLL_NRD              (8u-1u)               	/* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)     	/* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#elif ( SELECT_MONITOR == HDTV_720p_1280_720_50HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     59    8    4        73.75 MHz, HDTV 720p Signal  1280  x 720 (50 Hz)
	*/

	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_EXT_0)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) 	/* DCDR  Not use */
	#define LVDS_PLL_NFD              (59u-1u)              	/* NFD   Not use */
	#define LVDS_PLL_NRD              (8u-1u)               	/* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)     	/* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#elif ( SELECT_MONITOR == XGA_1024_768_70HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     27    5    4        xx.xx MHz, XGA Signal  1024 x 768 (72 Hz)
	*/
	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1)
	#define LVDS_PLL_NFD              (28u-1u)
	#define LVDS_PLL_NRD              (6u-1u)
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      /* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   /* Not use */

#elif ( SELECT_MONITOR == XGA_1024_768_60HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     31    7    4        xx.xx MHz, XGA Signal  1024 x 768 (60.8 Hz)
	*/
	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1)
	#define LVDS_PLL_NFD              (32u-1u)
	#define LVDS_PLL_NRD              (8u-1u)
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      /* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   /* Not use */

#elif ( SELECT_MONITOR == WXGA_1280_800_60HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     39    7    4        xx.xx MHz, XGA Signal  1280 x 800 (59.2 Hz)
	*/
	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1)
	#define LVDS_PLL_NFD              (40u-1u)
	#define LVDS_PLL_NRD              (8u-1u)
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#elif ( SELECT_MONITOR == FWXGA_1366_768_60HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     30    5    4        xx.xx MHz, WXGA Signal  1366 x 768 (59.6 Hz)
	*/
	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1)
	#define LVDS_PLL_NFD              (31u-1u)
	#define LVDS_PLL_NRD              (6u-1u)
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

//
//** dangerous version
//

#elif ( SELECT_MONITOR == SVGA_720_480_60HZ_IPLL_A )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     13    7    4        xx.xx MHz, SVGA Signal  720  x 480 (58.8 Hz)
	*/

	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) 	/* DCDR  Not use */
	#define LVDS_PLL_NFD              (14u-1u)               	/* NFD   Not use */
	#define LVDS_PLL_NRD              (8u-1u)                	/* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)      	/* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#elif ( SELECT_MONITOR == SVGA_720_480_60HZ_IPLL_B )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     12    7    4        xx.xx MHz, SVGA Signal  720  x 480 (59.5 Hz)
	*/

	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) 	/* DCDR  Not use */
	#define LVDS_PLL_NFD              (13u-1u)              	/* NFD   Not use */
	#define LVDS_PLL_NRD              (8u-1u)               	/* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)     	/* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#elif ( SELECT_MONITOR == SVGA_800_600_60HZ_IPLL )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     18    7    4        xx.xx MHz, SVGA Signal  800  x 600 (59.1 Hz)
	*/

	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)   //  VDC_PANEL_ICKSEL_LVDS
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_PERI) //  VDC_LVDS_INCLK_SEL_PERI
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) 	/* DCDR  Not use */
	#define LVDS_PLL_NFD              (19u-1u)               	/* NFD   Not use */
	#define LVDS_PLL_NRD              (8u-1u)                	/* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)      	/* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#elif ( SELECT_MONITOR == HDTV_720p_800_480_60HZ )
	/* Clock setting
			DCDR  NFD   NRD  NODIV
			1     13    5    4        xx.xx MHz, SVGA Signal  1280  x 720 (60 Hz)
	*/

	#define LCD_CH0_PANEL_CLK         (VDC_PANEL_ICKSEL_LVDS)
	#define LVDS_PLL_INPUT_CLK        (VDC_LVDS_INCLK_SEL_EXT_0)
	#define LCD_CH0_PANEL_CLK_DIV     (VDC_PANEL_CLKDIV_1_1) 	/* DCDR  Not use */
	#define LVDS_PLL_NFD              (20u-1u)              	/* NFD   Not use */
	#define LVDS_PLL_NRD              (7u-1u)               	/* NRD   Not use */
	#define LVDS_PLL_NODIV            (VDC_LVDS_NDIV_4)     	/* ODIV  Not use */

	#define LVDS_PLL_NIDV             (VDC_LVDS_NDIV_4)      	/* Not use */
	#define LVDS_PLL_NOD              (VDC_LVDS_PLL_NOD_4)   	/* Not use */

#else
    #error  Undefined LCD panel!
#endif

#endif  /* DVI_CLK_H */
