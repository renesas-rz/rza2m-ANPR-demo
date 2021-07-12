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
* @file         lcd_analog_rgb.h
* @version      1.00
* @brief        LCD panel  definition header
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>

#include    "r_typedefs.h"

#include    "r_vdc.h"

#ifndef LCD_ANALOG_RGB_H
#define LCD_ANALOG_RGB_H

#define     SVGA_800_600_60HZ     		(0u)                // OK 60Hz refresh SG-8002DC_40MHz 3.3V in socket X5 on board required
#define     HDTV_720p_1280_720_60HZ     (1u)				// OK 60Hz refresh SG-8002DC_40MHz 3.3V in socket X5 on board required
#define     HDTV_720p_1280_720_50HZ     (2u)				// OK 60Hz refresh SG-8002DC_40MHz 3.3V in socket X5 on board required

#define     XGA_1024_768_70HZ     		(3u)                // 70Hz

#define     XGA_1024_768_60HZ			(4u)				/* OK 60Hz  */
#define     WXGA_1280_800_60HZ			(5u)				/* OK 60Hz  */
#define     FWXGA_1366_768_60HZ			(6u)				/* OK 60Hz  */

// following display drivers are working with out of allowed range off VDC PLL
// use on own risk
// modification of <proj>/generate/sc_drivers/r_vdc/src/r_vdc_check_parameter.c is required
// for the following settings
// #define VDC_LVDSPLL_FD_MIN <value>
//
#define     SVGA_720_480_60HZ_IPLL_A  	(7u)				/* OK with PLL forbidden modify r_vdc_check_parameter.c #define VDC_LVDSPLL_FD_MIN (13u) */
#define     SVGA_720_480_60HZ_IPLL_B  	(8u)				/* OK with PLL forbidden modify r_vdc_check_parameter.c #define VDC_LVDSPLL_FD_MIN (12u) */
#define     SVGA_800_600_60HZ_IPLL	  	(9u)				/* OK with PLL forbidden modify r_vdc_check_parameter.c #define VDC_LVDSPLL_FD_MIN (18u) */

#define     HDTV_720p_800_480_60HZ      (10u)				// OK 60Hz refresh SG-8002DC_40MHz 3.3V in socket X5 on board required
                                                            /* OK with PLL forbidden modify r_vdc_check_parameter.c #define VDC_LVDSPLL_FD_MIN (19u) */


// please define the driver to use
//#define     SELECT_MONITOR   SVGA_800_600_60HZ            /* preferred setting */

//#define     SELECT_MONITOR   HDTV_720p_1280_720_60HZ

#define     SELECT_MONITOR   WXGA_1280_800_60HZ


/******************************************************************************
Macro definitions
******************************************************************************/
#if ( SELECT_MONITOR == SVGA_800_600_60HZ)
	/* SVGA signal 800x600
		Pixel clock frequency: 40.0 MHz external quarz oscillator circuit in X5 socket
		V Refresh rate: 60.4 Hz
		H Frequency   : 37.90 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (800u)                       /* Horizontal visible area [pixel]     */
	#define     LCD_H_FRONT_PORCH      (40u)                        /* Horizontal front porch [pixel]      */
	#define     LCD_H_SYNC_WIDTH       (128u)                       /* Horizontal sync pulse width [pixel] */
	#define     LCD_H_BACK_PORCH       (88u)                        /* Horizontal back porch [pixel]       */
																	/* Horizontal total (one line) [pixel] */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse */

	#define     LCD_V_VISIBLE_AREA     (600u)                       /* Vertical visible area [line]      */
	#define     LCD_V_FRONT_PORCH      (1u)                         /* 4 Vertical front porch [line]       */
	#define     LCD_V_SYNC_WIDTH       (4u)                         /* Vertical sync pulse width [line]  */
	#define     LCD_V_BACK_PORCH       (23u)                        /* 23 Vertical back porch [line]        */
																	/* Vertical total (one frame) [line] */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse   */

#elif ( SELECT_MONITOR == HDTV_720p_1280_720_60HZ )
	/* HDTV_720p signal 1280x720
		Pixel clock frequency: 73.75 MHz
		V Refresh rate: 60.0 Hz
		H Frequency   : 45.0 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (1280u)                      /* Horizontal visible area [pixel]      */
	#define     LCD_H_FRONT_PORCH      (63u)                        /*  63 Horizontal front porch [pixel]       */
	#define     LCD_H_SYNC_WIDTH       (80u)                        /*  80 Horizontal sync pulse width [pixel]  */
	#define     LCD_H_BACK_PORCH       (216u)                       /* 216 Horizontal back porch [pixel]        */
																	/* Horizontal total (one line) [pixel]  */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/
	//600
	#define     LCD_V_VISIBLE_AREA     (720u)                       /* Vertical visible area [line]       */
	// 1, 4
	#define     LCD_V_FRONT_PORCH      (3u)                         /*  3 Vertical front porch [line]        */
	#define     LCD_V_SYNC_WIDTH       (5u)                         /*  5 Vertical sync pulse width [line]  */
	// 23, 20
	#define     LCD_V_BACK_PORCH       (22u)                        /* 22 Vertical back porch [line]         */
																	/* Vertical total (one frame) [line]  */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse */

#elif ( SELECT_MONITOR == HDTV_720p_1280_720_50HZ )
	/* HDTV_720p signal 1280x720
		Pixel clock frequency: 73.75 MHz
		V Refresh rate: 50.0 Hz
		H Frequency   : 45.0 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (1280u)                      /* Horizontal visible area [pixel]      */
	#define     LCD_H_FRONT_PORCH      (390u)                       /* 390 Horizontal front porch [pixel]       */
	#define     LCD_H_SYNC_WIDTH       (80u)                        /*  80 Horizontal sync pulse width [pixel]  */
	#define     LCD_H_BACK_PORCH       (216u)                       /* 216 Horizontal back porch [pixel]        */
																	/* Horizontal total (one line) [pixel]  */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/
	//600
	#define     LCD_V_VISIBLE_AREA     (720u)                       /* Vertical visible area [line]       */
	// 1, 4
	#define     LCD_V_FRONT_PORCH      (3u)                         /*  3 Vertical front porch [line]        */
	#define     LCD_V_SYNC_WIDTH       (5u)                         /*  5 Vertical sync pulse width [line]  */
	// 23, 20
	#define     LCD_V_BACK_PORCH       (22u)                        /* 22 Vertical back porch [line]         */
																	/* Vertical total (one frame) [line]  */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse */

#elif ( SELECT_MONITOR == XGA_1024_768_70HZ )
	/* XGA signal 1024x768
		Pixel clock frequency: 75.0 MHz
		V Refresh rate: 72.0 Hz
		H Frequency   : 58.90 KHz
		Polarity of horizontal sync pulse: Negative
		Polarity of vertical   sync pulse: Negative
	*/
	#define     LCD_H_VISIBLE_AREA     (1024u)                     /* Horizontal visible area [pixel] */
	#define     LCD_H_FRONT_PORCH      (24u)                       /* Horizontal front porch [pixel] */
	#define     LCD_H_SYNC_WIDTH       (136u)                      /* Horizontal sync pulse width [pixel] */
	#define     LCD_H_BACK_PORCH       (144u)                      /* Horizontal back porch [pixel] */
																   /* Horizontal total (one line) [pixel] */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_INVERTED)       /* Polarity of horizontal sync pulse */

	#define     LCD_V_VISIBLE_AREA     (768u)                       /* Vertical visible area [line] */
	#define     LCD_V_FRONT_PORCH      (3u)                         /* Vertical front porch [line] */
	#define     LCD_V_SYNC_WIDTH       (6u)                         /* Vertical sync pulse width [line] */
	#define     LCD_V_BACK_PORCH       (29u)                        /* Vertical back porch [line] */
																	/* Vertical total (one frame) [line] */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_INVERTED)       /* Polarity of vertical sync pulse */

#elif ( SELECT_MONITOR == XGA_1024_768_60HZ )
	/* XGA signal 1024x768
		Pixel clock frequency:
		V Refresh rate: 60.8 Hz
		H Frequency   : 49.00 KHz
		Polarity of horizontal sync pulse: Negative
		Polarity of vertical   sync pulse: Negative
	*/
	#define     LCD_H_VISIBLE_AREA     (1024u)                     /* Horizontal visible area [pixel] 1024*/
	#define     LCD_H_FRONT_PORCH      (24u)                       /* Horizontal front porch [pixel] */
	#define     LCD_H_SYNC_WIDTH       (136u)                      /* Horizontal sync pulse width [pixel] */
	#define     LCD_H_BACK_PORCH       (160u)                      /* Horizontal back porch [pixel] 144 */
																   /* Horizontal total (one line) [pixel] */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_INVERTED)       /* Polarity of horizontal sync pulse */

	#define     LCD_V_VISIBLE_AREA     (768u)                       /* Vertical visible area [line] */
	// 3, 4
	#define     LCD_V_FRONT_PORCH      (3u)                         /* Vertical front porch [line] 3 */
	#define     LCD_V_SYNC_WIDTH       (6u)                         /* Vertical sync pulse width [line] */
	//29, 28
	#define     LCD_V_BACK_PORCH       (29u)                        /* Vertical back porch [line] 29 */
																	/* Vertical total (one frame) [line] */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_INVERTED)       /* Polarity of vertical sync pulse */

#elif ( SELECT_MONITOR == WXGA_1280_800_60HZ )
	/* WXGA signal 1280x800
		Pixel clock frequency:
		V Refresh rate: 59.2 Hz
		H Frequency   : 49.00 KHz
		Polarity of horizontal sync pulse: Negative
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (1280u)                     /* Horizontal visible area [pixel] */
	#define     LCD_H_FRONT_PORCH      (64u)                       /* Horizontal front porch [pixel] */
	#define     LCD_H_SYNC_WIDTH       (136u)                      /* Horizontal sync pulse width [pixel] */
	#define     LCD_H_BACK_PORCH       (200u)                      /* Horizontal back porch [pixel] */
																   /* Horizontal total (one line) [pixel] */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_INVERTED)     /* Polarity of horizontal sync pulse */

	#define     LCD_V_VISIBLE_AREA     (800u)                      /* Vertical visible area [line] */
	#define     LCD_V_FRONT_PORCH      (4u)                        /* Vertical front porch [line] */
	#define     LCD_V_SYNC_WIDTH       (3u)                        /* Vertical sync pulse width [line] */
	#define     LCD_V_BACK_PORCH       (21u)                       /* Vertical back porch [line] */
																   /* Vertical total (one frame) [line] */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED) /* Polarity of vertical sync pulse */

#elif ( SELECT_MONITOR == FWXGA_1366_768_60HZ )
	/* FWXGA signal 1366x768
		Pixel clock frequency:
		V Refresh rate: 59.6 Hz
		H Frequency   : 47.40 KHz
		Polarity of horizontal sync pulse: Negative
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA      (1366u)                     /* Horizontal visible area [pixel] 1360*/
	#define     LCD_H_FRONT_PORCH       (72u)                       /* Horizontal front porch [pixel] 64 */
	#define     LCD_H_SYNC_WIDTH        (144u)                      /* Horizontal sync pulse width [pixel] 112*/
	#define     LCD_H_BACK_PORCH        (216u)                      /* Horizontal back porch [pixel] 256*/
																	/* Horizontal total (one line) [pixel] */
	#define     LCD_H_TOTAL             (LCD_H_VISIBLE_AREA +\
										 LCD_H_FRONT_PORCH +\
										 LCD_H_SYNC_WIDTH +\
										 LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY          (VDC_SIG_POL_INVERTED)  /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/

	#define     LCD_V_VISIBLE_AREA      (768u)                      /* Vertical visible area [line] 768*/
	#define     LCD_V_FRONT_PORCH       (1u)                        /* Vertical front porch [line] 4 */
	#define     LCD_V_SYNC_WIDTH        (3u)                        /* Vertical sync pulse width [line] 6*/
	#define     LCD_V_BACK_PORCH        (23u)                       /* Vertical back porch [line] 17 */
																	/* Vertical total (one frame) [line] */
	#define     LCD_V_TOTAL             (LCD_V_VISIBLE_AREA +\
										 LCD_V_FRONT_PORCH +\
										 LCD_V_SYNC_WIDTH +\
										 LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY          (VDC_SIG_POL_NOT_INVERTED)  /* Polarity of vertical sync pulse VDC_SIG_POL_NOT_INVERTED*/

//** dangerous versions
#elif ( SELECT_MONITOR == SVGA_720_480_60HZ_IPLL_A )
	/* SVGA signal 720x480
		Pixel clock frequency: xx MHz
		V Refresh rate: 58.8 Hz
		H Frequency   : 30.10 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (720u)                       /* Horizontal visible area [pixel]      */
	#define     LCD_H_FRONT_PORCH      (48u)                        /* Horizontal front porch [pixel]       */
	#define     LCD_H_SYNC_WIDTH       (32u)                        /* Horizontal sync pulse width [pixel]  */
	#define     LCD_H_BACK_PORCH       (160u)                       /* Horizontal back porch [pixel]        */
																	/* Horizontal total (one line) [pixel]  */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/
	//600
	#define     LCD_V_VISIBLE_AREA     (480u)                       /* Vertical visible area [line]       */
	// 1, 4
	#define     LCD_V_FRONT_PORCH      (3u)                         /* Vertical front porch [line]        */
	#define     LCD_V_SYNC_WIDTH       (10u)                         /* Vertical sync pulse width [line]  */
	// 23, 20
	#define     LCD_V_BACK_PORCH       (19u)                        /* Vertical back porch [line]         */
																	/* Vertical total (one frame) [line]  */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse  VDC_SIG_POL_NOT_INVERTED */

#elif ( SELECT_MONITOR == SVGA_720_480_60HZ_IPLL_B )
	/* SVGA signal 720x480
		Pixel clock frequency: xx MHz
		V Refresh rate: 59.5 Hz
		H Frequency   : 29.80 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (720u)                       /* Horizontal visible area [pixel]      */
	#define     LCD_H_FRONT_PORCH      (16u)                        /*24 16 Horizontal front porch [pixel]       */
	#define     LCD_H_SYNC_WIDTH       (62u)                        /*40 62 Horizontal sync pulse width [pixel]  */
	#define     LCD_H_BACK_PORCH       (100u)                       /*96 60 Horizontal back porch [pixel]        */
																	/* Horizontal total (one line) [pixel]  */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/
	//600
	#define     LCD_V_VISIBLE_AREA     (480u)                       /* Vertical visible area [line]       */
	// 1, 4
	#define     LCD_V_FRONT_PORCH      (8u)                         /* 9 Vertical front porch [line]        */
	#define     LCD_V_SYNC_WIDTH       (3u)                         /*3 6 Vertical sync pulse width [line]  */
	// 23, 20
	#define     LCD_V_BACK_PORCH       (10u)                        /* 30 Vertical back porch [line]         */
																	/* Vertical total (one frame) [line]  */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse  VDC_SIG_POL_NOT_INVERTED */

#elif ( SELECT_MONITOR == SVGA_800_600_60HZ_IPLL )                  /* internal PLL out of allowed range used */
	/* SVGA signal 800x600
		Pixel clock frequency:
		V Refresh rate: 59.1 Hz
		H Frequency   : 73.10 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (800u)                       /* Horizontal visible area [pixel]     800u  */
	#define     LCD_H_FRONT_PORCH      (40u)                        /* Horizontal front porch [pixel]       40u  */
	#define     LCD_H_SYNC_WIDTH       (128u)                       /* Horizontal sync pulse width [pixel] 128u  */
	#define     LCD_H_BACK_PORCH       (88u)                        /* Horizontal back porch [pixel]        88u  */
																	/* Horizontal total (one line) [pixel]       */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/
	//600
	#define     LCD_V_VISIBLE_AREA     (600u)                       /* Vertical visible area [line] 600u     */
	// 1, 4
	#define     LCD_V_FRONT_PORCH      (1u)                         /* Vertical front porch [line]      4u */
	#define     LCD_V_SYNC_WIDTH       (4u)                         /* Vertical sync pulse width [line] 4u */
	// 23, 20
	#define     LCD_V_BACK_PORCH       (23u)                        /* Vertical back porch [line]       20u */
																	/* Vertical total (one frame) [line] */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse  VDC_SIG_POL_NOT_INVERTED */

#elif ( SELECT_MONITOR == HDTV_720p_800_480_60HZ )
	/* HDTV_720p signal 800x480
		Pixel clock frequency: 28.5 MHz
		V Refresh rate: 60.0 Hz
		H Frequency   : 29.69 KHz
		Polarity of horizontal sync pulse: Positive
		Polarity of vertical   sync pulse: Positive
	*/
	#define     LCD_H_VISIBLE_AREA     (800u)                      /* Horizontal visible area [pixel]      */
	#define     LCD_H_FRONT_PORCH      (48u)                       /*  48 Horizontal front porch [pixel]       */
	#define     LCD_H_SYNC_WIDTH       (32u)                       /*  32 Horizontal sync pulse width [pixel]  */
	#define     LCD_H_BACK_PORCH       (80u)                       /*  80 Horizontal back porch [pixel]        */
																   /* Horizontal total (one line) [pixel]  */
	#define     LCD_H_TOTAL            (LCD_H_VISIBLE_AREA +\
										LCD_H_FRONT_PORCH +\
										LCD_H_SYNC_WIDTH +\
										LCD_H_BACK_PORCH)
	#define     LCD_H_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of horizontal sync pulse VDC_SIG_POL_NOT_INVERTED*/
	//600
	#define     LCD_V_VISIBLE_AREA     (480u)                       /* Vertical visible area [line]       */
	// 1, 4
	#define     LCD_V_FRONT_PORCH      (3u)                         /*  3 Vertical front porch [line]        */
	#define     LCD_V_SYNC_WIDTH       (7u)                         /*  7 Vertical sync pulse width [line]  */
	// 23, 20
	#define     LCD_V_BACK_PORCH       (6u)                         /*  6 Vertical back porch [line]         */
																	/* Vertical total (one frame) [line]  */
	#define     LCD_V_TOTAL            (LCD_V_VISIBLE_AREA +\
										LCD_V_FRONT_PORCH +\
										LCD_V_SYNC_WIDTH +\
										LCD_V_BACK_PORCH)
	#define     LCD_V_POLARITY         (VDC_SIG_POL_NOT_INVERTED)   /* Polarity of vertical sync pulse  VDC_SIG_POL_NOT_INVERTED */

#else
    #error  Undefined LCD panel!
#endif

#endif  /* LCD_ANALOG_RGB_H */
