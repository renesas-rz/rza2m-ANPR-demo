/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "r_typedefs.h"
#include "iodefine.h"
#include "r_cache_lld_rza2m.h"

#include "r_bcd_ae.h"
#include "r_bcd_camera.h"
#include "r_bcd_lcd.h"
#include "draw.h"
#include "perform.h"
#include "r_mmu_lld_rza2m.h"
#include "r_gpio_drv_api.h"

#include "r_dk2_if.h"

#include "r_drp_bayer2lrgb_color_correction_x_t6.h"
#include "r_drp_lrgb2l_gaussian_blur_x.h"
#include "r_drp_canny_calculate.h"
#include "r_drp_canny_hysterisis.h"
#include "r_drp_dilate.h"
#include "r_drp_binarization_adaptive.h"
#include "r_drp_find_contours_crop_x_t2.h"
#include "r_drp_resize_bilinear_crop_x_t6.h"

#include "subBoardKeys.h"
#include "cpuBoardLed.h"
#include "r_serial_terminal.h"

#include "Typedef.h"

#include "r_neural_networks.h"

#include "compiler_settings.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define TILE_0          (0)
#define TILE_1          (1)
#define TILE_2          (2)
#define TILE_3          (3)
#define TILE_4          (4)
#define TILE_5          (5)

#define DRP_NOT_FINISH  (0)
#define DRP_FINISH      (1)

#define DRP_DRV_ASSERT(x) if ((x) != 0) goto sample_finish
#define WCHAR(a,s,x) sprintf((char *)&a[0],"%s %d.%d[ms]", s, (int)(x / 1000), (int)((x - ((x / 1000) * 1000)) / 100))

#define WEAK_LEVEL      (0x28)
#define STRONG_LEVEL    (0x58)

#define HYSTERISIS_COUNT  (2)
#define FIND_CONTOURS_NUM (15)

/* DRP Parameter of Resize bilinear fixed */
#define DRP_RESIZE_HALF         (0x20)

/* Offset lines for Layer0 display */
#define LAYER0_DISP_OFFSET      (240)

/* Display result string length */
#define RESULT_DISP_BUF_SIZE    (80)

/* ISP parameter */
#define ISP_GAIN_R              (0x1500)
#define ISP_GAIN_G              (0x1000)
#define ISP_GAIN_B              (0x18E3)
#define ISP_GAMMA               (0x01)
#define ISP_COMPONENT           (0x01)
#define ISP_BIAS_R              (0x0000)
#define ISP_BIAS_G              (0x0000)
#define ISP_BIAS_B              (0x0000)
#define ISP_MEDIAN_A            (0x0100)
#define ISP_STRENGTH            (28)
#define ISP_CORING              (60)
#define GAMMA_VALUE             ((double)1.2)
#define AE_MARGIN               (BRIGHTNESS_TH * 3)

/* Status of isp_ae_mode */
#define ISP_AE_ON   (1)
#define ISP_AE_OFF  (0)

/* LED */
#define MAIN_PRV_LED_ON     (1)
#define MAIN_PRV_LED_OFF    (0)

/* flag indicating contour overlap */
#define NO_OVERLAP  (0)
#define OVERLAPPING (1)


/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern uint8_t g_drp_lib_canny_calculate[];
extern uint8_t g_drp_lib_canny_hysterisis[];
extern uint8_t g_drp_lib_dilate[];
extern uint8_t g_drp_lib_binarization_adaptive[];


typedef struct       /* contours_rect_t */
{
	uint16_t	x;
	uint16_t	y;
	uint16_t	width;
	uint16_t	height;
	uint32_t	num;
	uint32_t	addr;
} contours_rect_t;

typedef struct       /* contours_region_t */
{
	uint16_t	x;
	uint16_t	y;
} contours_region_t;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/

static uint8_t  look_up_table[256]                                             __attribute__ ((section("Uncache_IRAM")));

static r_drp_canny_calculate_t  param_canny_cal[1]                             __attribute__ ((section("Uncache_IRAM")));
static r_drp_canny_hysterisis_t param_canny_hyst                               __attribute__ ((section("Uncache_IRAM")));
static r_drp_dilate_t           param_dilate                                   __attribute__ ((section("Uncache_IRAM")));

static r_drp_lrgb2l_gaussian_blur_t        param_lrgb2l_gaussian_blur[2]       __attribute__ ((section("Uncache_IRAM")));

static r_drp_find_contours_crop_t          param_find_contours_crop            __attribute__ ((section("Uncache_IRAM")));

static r_drp_bayer2lrgb_color_correction_t param_bayer2lrgb_color_correction   __attribute__ ((section("Uncache_IRAM")));
static r_drp_bayer2lrgb_color_correction_layer_t    bayer2argb_layersum        __attribute__ ((section("Uncache_IRAM")));

static r_drp_binarization_adaptive_t       param_binarization_adaptive         __attribute__ ((section("Uncache_IRAM")));

static r_drp_resize_bilinear_crop_t        param_resize_bilinear_crop          __attribute__ ((section("Uncache_IRAM")));


static uint8_t drp_lib_id[R_DK2_TILE_NUM] = {0};
static volatile uint8_t drp_lib_status[R_DK2_TILE_NUM] = {DRP_NOT_FINISH};

static contours_rect_t   contours_rect_adr[FIND_CONTOURS_NUM + 1]              __attribute__ ((section("Uncache_IRAM")));
static contours_region_t contours_region_adr[FIND_CONTOURS_NUM + 1]            __attribute__ ((section("Uncache_IRAM")));
static contours_rect_t   cropped_contours_rect_adr[FIND_CONTOURS_NUM + 1]      __attribute__ ((section("Uncache_IRAM")));
static contours_region_t cropped_contours_region_adr[FIND_CONTOURS_NUM + 1]    __attribute__ ((section("Uncache_IRAM")));

static contours_rect_t   number_plate_contours_rect_adr[FIND_CONTOURS_NUM + 1] __attribute__ ((section("Uncache_IRAM")));

static uint8_t frame_RAM_A[R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH]          __attribute__ ((section("ImageWork_RAM")));  /* Gaussian Blur        */
static uint8_t frame_RAM_B[R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH]          __attribute__ ((section("ImageWork_RAM")));  /* Canny Calculate      */
static uint8_t frame_RAM_C[R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH]          __attribute__ ((section("ImageWork_RAM")));  /* Canny Hysterisis     */
static uint8_t frame_RAM_D[R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH]          __attribute__ ((section("ImageWork_RAM")));  /* Dilate               */
static uint8_t frame_RAM_E[R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH]          __attribute__ ((section("ImageWork_RAM")));  /* BinarizationAdaptive */

static uint8_t work1[((R_BCD_CAMERA_HEIGHT +2) * R_BCD_CAMERA_WIDTH)*2]       __attribute__ ((section("ImageWork_RAM")));  /* Work area for Canny Calculate */
static uint8_t work2[R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH]                __attribute__ ((section("ImageWork_RAM")));  /* Work area for Canny Hysterisis and Find Conturs */
static uint8_t work3[((R_BCD_CAMERA_HEIGHT * R_BCD_CAMERA_WIDTH) / 64) + 2]   __attribute__ ((section("ImageWork_RAM")));  /* Work area for BinarizationAdaptive */

static uint8_t frame_RAM_CCR[FIND_CONTOURS_NUM][NN_IN_HEIGHT * NN_IN_WIDTH]   __attribute__ ((section("ImageWork_RAM")));   /* cropped character resized */

static float   frame_RAM_CCN_float[NN_IN_HEIGHT * NN_IN_WIDTH]                __attribute__ ((section("ImageWork_RAM")));   /* cropped character normalized to float*/

//static uint8_t frame_RAM_CCN_uint8[NN_IN_HEIGHT * NN_IN_WIDTH]                __attribute__ ((section("ImageWork_RAM")));   /* cropped character normalized to uint8 (for uint8 quantized input layer)*/
static int8_t frame_RAM_CCN_int8[NN_IN_HEIGHT * NN_IN_WIDTH]                  __attribute__ ((section("ImageWork_RAM")));   /* cropped character normalized to int8 (for int8 quantized input layer)*/


// GPIO
static int_t   gs_my_gpio_handle;

// LED
static uint8_t gs_main_led_flg;      /* LED lighting/turning off */

static display_mode_et          display_mode          = DISPLAY_PREDICTED_RESULT;

static nn_type_et               NN_type               = NO_NN;                  /* initially, no NN selected */


static void cb_drp_finish(uint8_t id);

/*******************************************************************************
* Function Name: cb_drp_finish
* Description  : This function is a callback function called from the
*              : DRP driver at the finish of the DRP library processing.
* Arguments    : id
*              :   The ID of the DRP library that finished processing.
* Return Value : -
*******************************************************************************/
static void cb_drp_finish(uint8_t id)
{
    uint32_t tile_no;

    /* Change the operation state of the DRP library notified by the argument to finish */
    for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
    {
        if (drp_lib_id[tile_no] == id)
        {
            drp_lib_status[tile_no] = DRP_FINISH;
            break;
        }
    }

    return;
}
/*******************************************************************************
* End of function cb_drp_finish
*******************************************************************************/


/*******************************************************************************
* Function Name: sample_main
* Description  : First function called after initialization is completed
* Arguments    : -
* Return Value : -
*******************************************************************************/
void sample_main(void)
{
    int32_t  frame_buf_id;
    uint8_t  buf[RESULT_DISP_BUF_SIZE];
    int32_t  ret_val;

    uint8_t  *input_bufadr;
    uint8_t  *output_bufadr;

    uint8_t  color;
    uint16_t x1 = 60;
    uint16_t y1 = 60;
    uint16_t length = 0;
    uint16_t height = 0;
    uint16_t char_height = 0;

    uint16_t brightness;
    r_bcd_ae_setting_t ae_setting;

    int isp_ae_mode;

    uint32_t tile_no;
    uint32_t bufsize;

    uint16_t x2;
    uint16_t y2;
    uint16_t re;                 /* right edge */
    uint16_t re2;                /* right edge */
    uint16_t be;                 /* bottom edge */
    uint16_t be2;                /* bottom edge */
    uint32_t overlap;

    /* character list required for display output                             */
    /* german characters Ä, Ö and Ü have been added to the font               */
    /* <project_dir>/src/renesas/application/common/render/fontdata_12x24.c   */
    char     char_list[41][2]   = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9","\x80","\x81","\x82","?","\0"};
    /* character list required for serial output (e.g. TeraTerm)              */
    char     char_listTT[41][3] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9","ae"  ,"oe"  ,"ue"  ,"?","\0"};

    float    prediction_init[39]        = { (float)0.0 };
    float    *prediction                = &prediction_init[0];

    // uint8_t  prediction_uint8_init[39]  = { (uint8_t)0 };
    // uint8_t  *prediction_uint8          = &prediction_uint8_init[0];

    int8_t   prediction_int8_init[39]   = { (int8_t)-128 };
    int8_t   *prediction_int8           = &prediction_int8_init[0];

    TsInt    dnn_compute_CNN_FE_errorcode;
    TsInt    *dnn_compute_CNN_FE_error    = &dnn_compute_CNN_FE_errorcode;

    float    predicted_accuracy[FIND_CONTOURS_NUM];
    // uint8_t  predicted_accuracy_uint8[FIND_CONTOURS_NUM];
    int8_t   predicted_accuracy_int8[FIND_CONTOURS_NUM];

    uint8_t  predicted_char_index[FIND_CONTOURS_NUM]          = { (uint8_t)39 };  // prediction unknown (="?")

    uint8_t  previous_predicted_char_index[FIND_CONTOURS_NUM] = { (uint8_t)0 };
    uint8_t  same_prediction_counter                          = (uint8_t)0;
    uint8_t  prediction_color                                 = RED;

    uint8_t  cn;      // contour number
    uint8_t  ccn;     // cropped contour number
    uint8_t  ccn2;    // cropped contour number 2
    uint8_t  mscn;    // most suitable contour number

    uint8_t  pot_obj_rect_number[FIND_CONTOURS_NUM + 1];
    uint8_t  amount_pot_obj;
    uint8_t  pot_char_rect_number[FIND_CONTOURS_NUM + 1];
    uint8_t  sort_char_rect_number;
    uint8_t  amount_pot_char[FIND_CONTOURS_NUM]               = { (uint8_t)0 };
    uint8_t  number_plate_char_rect_number[FIND_CONTOURS_NUM + 1];

    uint16_t cropped_x[FIND_CONTOURS_NUM];
    uint16_t cropped_y[FIND_CONTOURS_NUM];
    uint16_t cropped_width[FIND_CONTOURS_NUM];
    uint16_t cropped_height[FIND_CONTOURS_NUM];

    uint16_t extended_cropped_x;
    uint16_t extended_cropped_width;

    uint16_t c_cropped_x[FIND_CONTOURS_NUM];
    uint16_t c_cropped_y[FIND_CONTOURS_NUM];
    uint16_t c_cropped_width[FIND_CONTOURS_NUM];
    uint16_t c_cropped_height[FIND_CONTOURS_NUM];
    uint16_t c_cropped_width_delta;
    uint16_t c_cropped_height_delta;

    float    xy_ratio;

    uint16_t  ih;
    uint16_t  iw;

    /**************/
    /* Initialize */
    /**************/

    /* VDC6 L2 Cache ON */
    PRR.AXIBUSCTL6.BIT.VDC601ARCACHE = 0xF;
    PRR.AXIBUSCTL6.BIT.VDC602ARCACHE = 0xF;
    PRR.AXIBUSCTL7.BIT.VDC604ARCACHE = 0xF;

    /* DRP L2 Cache ON */
    PRR.AXIBUSCTL4.BIT.DRPARCACHE = 0xF;
    PRR.AXIBUSCTL4.BIT.DRPAWCACHE = 0xF;

    /* Initialization of VIN and MIPI driver */
    R_BCD_CameraInit();

    /* Initialization of LCD driver */
    R_BCD_LcdInit();

    /* Initialization of key read routines */
    initBoardSwitch () ;
    boardSwitchInfoStruct.SW5_ModeCntMax = 3; // we will use it

    /* Initialize AE mode (=ON) */
    isp_ae_mode = ISP_AE_ON;

    /* Capture Start */
    R_BCD_CameraClearCaptureStatus();
    R_BCD_CameraCaptureStart();

    /* Initialize of Performance counter */
    PerformInit();

    /* Initialization of DRP driver */
    R_DK2_Initialize();

    /* Initialization of Simple ISP */
    R_BCD_AeInit(&ae_setting);
    R_BCD_AeMakeGammaTable(&ae_setting, GAMMA_VALUE, look_up_table);

    /* ==== Output banner message ==== */
    /* ====  on SerialTerminal    ==== */
    printf("%s%s", sp_clear_screen, sp_cursor_home);
    show_welcome_msg(stdout, true);


    /* Initialization of GPIOs */
    gs_my_gpio_handle = open (DEVICE_INDENTIFIER "gpio", O_RDWR);
    /* On error */
    if ( gs_my_gpio_handle < 0 )
    {
        /* stop execute */
        while(1);
    }

    /* Initialization of LEDs */
    initCpuBoardLed();
    cpuBoardLedRed   ( 0 ) ; // red led off
    cpuBoardLedGreen ( 0 ) ; // green led off (The green LED looks yellow incase red is on, too


    /* initialize NN_type to one of the activated NNs */
    if (TRANSLATOR_ACTIVE) {
    	NN_type = TRANSLATOR;
    } else if (TFLITE_FLOAT_ACTIVE) {
    	NN_type = TFLITE_FLOAT;
    } else if (TFLITE_PTQ_ACTIVE) {
    	NN_type = TFLITE_PTQ;
    } else if (TFLITE_QAT_ACTIVE) {
    	NN_type = TFLITE_QAT;
    }

    /* Initialization of TensorFlow Lite models / interpreter */
    if (TFLITE_FLOAT_ACTIVE || TFLITE_PTQ_ACTIVE || TFLITE_QAT_ACTIVE) {
       	fprintf(stdout,"##### TensorFlow Model interpreter initialization :\n");
    	if (TFLITE_FLOAT_ACTIVE) {
    		fprintf(stdout,"#####------------setup TFLite floating point CNN model-------------#####\n");
    		tfl_float_dnn_setup();
    	}
    	if (TFLITE_PTQ_ACTIVE) {
    		fprintf(stdout,"#####-------setup TFLite 'post-training' quantized CNN model-------#####\n");
    		tfl_PTQ_dnn_setup();
    	}
    	if (TFLITE_QAT_ACTIVE) {
    		fprintf(stdout,"#####-----setup TFLite 'quantization aware training' CNN model-----#####\n");
    		tfl_QAT_dnn_setup();
    	}
    	fprintf(stdout,"#####--------------------------------------------------------------#####\n");
       	fprintf(stdout,"##### TensorFlow Model interpreter initialization completed.\n\n");
        fflush(stdout);
    }

    /*************/
    /* Main loop */
    /*************/
    while (1)
    {
        /* check SW push-buttons usage */
    	readBoardSwitch ( ) ;

        /* toggle Auto Exposure (AE) if SW3 has been pushed */
    	if ( boardSwitchInfoStruct.SW3_IsModified ) {
    		boardSwitchInfoStruct.SW3_IsModified = 0 ;
    		/* Change AE mode On/Off */
    		if ( isp_ae_mode == ISP_AE_OFF ) {
    			R_BCD_AeStart(&ae_setting);
    			isp_ae_mode = ISP_AE_ON;
    		} else {
    			R_BCD_AeStop(&ae_setting);
    			isp_ae_mode = ISP_AE_OFF;
    		}
    	}

        /* toggle neural network type if SW5 has been pushed */
        if ( boardSwitchInfoStruct.SW5_IsModified ) {
             boardSwitchInfoStruct.SW5_IsModified = 0 ;
             switch (NN_type) {
				case TRANSLATOR :
				    if (TFLITE_FLOAT_ACTIVE) {
				    	NN_type = TFLITE_FLOAT;
				    } else if (TFLITE_PTQ_ACTIVE) {
				    	NN_type = TFLITE_PTQ;
				    } else if (TFLITE_QAT_ACTIVE) {
				    	NN_type = TFLITE_QAT;
				    }
				    break;
				case TFLITE_FLOAT :
					if (TFLITE_PTQ_ACTIVE) {
						NN_type = TFLITE_PTQ;
					} else if (TFLITE_QAT_ACTIVE) {
						NN_type = TFLITE_QAT;
					} else if (TRANSLATOR_ACTIVE) {
				    	NN_type = TRANSLATOR;
					}
					break;
				case TFLITE_PTQ :
					if (TFLITE_QAT_ACTIVE) {
						NN_type = TFLITE_QAT;
					} else if (TRANSLATOR_ACTIVE) {
						NN_type = TRANSLATOR;
					} else if (TFLITE_FLOAT_ACTIVE) {
				    	NN_type = TFLITE_FLOAT;
				    }
					break;
				case TFLITE_QAT :
					if (TRANSLATOR_ACTIVE) {
						NN_type = TRANSLATOR;
					} else if (TFLITE_FLOAT_ACTIVE) {
						NN_type = TFLITE_FLOAT;
					} else if (TFLITE_PTQ_ACTIVE) {
				    	NN_type = TFLITE_PTQ;
				    }
				    break;
				case NO_NN :
					NN_type = NO_NN;
					break;
			}
        }

        /* Wait until camera capture is complete */
        while ((frame_buf_id = R_BCD_CameraGetCaptureStatus()) == R_BCD_CAMERA_NOT_CAPTURED)
        {
            /* DO NOTHING */
        }

        if (SerialTerminal_on) {
           	fprintf(stdout,"\n\n################################################################################\n\n");
    	    fflush(stdout);
        }

        /* Set start time of overall process*/
        PerformSetStartTime(0);

	    /********************************************************************/
		/* find all contours with dedicated aspect ratio                    */
	    /********************************************************************/

        if (1) {     /* find all contours */

			/**************************************/
			/* Load DRP Library                   */
			/*        +-------------------------+ */
			/* tile 0 |                         | */
			/*        |                         | */
			/* tile 1 |                         | */
			/*        |                         | */
			/* tile 2 |                         | */
			/*        |Bayer2lrgbColorCorrection| */
			/* tile 3 |     (WhiteBalance)      | */
			/*        |                         | */
			/* tile 4 |                         | */
			/*        |                         | */
			/* tile 5 |                         | */
			/*        +-------------------------+ */
			/**************************************/

			/* Set start time of process*/
			PerformSetStartTime(1);

			ret_val = R_DK2_Load(&g_drp_lib_bayer2lrgb_color_correction_t6[0],
								 R_DK2_TILE_0 ,
								 R_DK2_TILE_PATTERN_6, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);
			/***********************
			 Activate DRP Library
			***********************/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
			DRP_DRV_ASSERT(ret_val);

			/* Start Bayer2RGB */
			/* Set Bayer2RgbColorCorrection parameters */
			input_bufadr  = R_BCD_CameraGetFrameAddress(frame_buf_id);
			output_bufadr = R_BCD_LcdGetVramAddress();              /* address of the video buffer which is not currently being displayed */

			/**************************************
			 Set R_DK2_Start function parameters
			**************************************/

			/* Set the address of buffer to be read/write by DRP */
			R_MMU_VAtoPA((uint32_t)input_bufadr,  &(param_bayer2lrgb_color_correction.src));
			R_MMU_VAtoPA((uint32_t)output_bufadr, &(param_bayer2lrgb_color_correction.dst));

			/* Set Image size */
			param_bayer2lrgb_color_correction.width  = R_BCD_CAMERA_WIDTH;
			param_bayer2lrgb_color_correction.height = R_BCD_CAMERA_HEIGHT;

            /* Set magnification */
	        if ( R_BCD_CameraBayerMode ) { // PCAM5C has bayer mode 3
				param_bayer2lrgb_color_correction.gain_r = 0x1050;
				param_bayer2lrgb_color_correction.gain_g = 0x1000;
				param_bayer2lrgb_color_correction.gain_b = 0x1050;
	        } else { // IMX219_B has bayer mode 0
	            param_bayer2lrgb_color_correction.gain_r = 0x148F;
	            param_bayer2lrgb_color_correction.gain_g = 0x1000;
	            param_bayer2lrgb_color_correction.gain_b = 0x1A72;
	        }
			// camera specific bayer pattern R_BCD_CameraBayerMode 0-RGGB 3-BGGR 255-unset
	        param_bayer2lrgb_color_correction.pattern = R_BCD_CameraBayerMode ;

			param_bayer2lrgb_color_correction.dst_layersum = (uint32_t)&bayer2argb_layersum;
			R_MMU_VAtoPA((uint32_t)param_bayer2lrgb_color_correction.dst_layersum, &(param_bayer2lrgb_color_correction.dst_layersum));

			/* Initialize variables to be used in termination judgment of the DRP library */
			drp_lib_status[TILE_0] = DRP_NOT_FINISH;

			/********************
			 Start DRP Library
			********************/
			ret_val = R_DK2_Start(drp_lib_id[TILE_0], (void *)&param_bayer2lrgb_color_correction, sizeof(r_drp_bayer2lrgb_color_correction_t));
			DRP_DRV_ASSERT(ret_val);

			/**************************************
			 Wait until DRP processing is finish
			**************************************/
			while (drp_lib_status[TILE_0] == DRP_NOT_FINISH)
			{
				/* Wait drp finish */
			}

			/*********************
			 Unload DRP library
			*********************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[TILE_0]);
			DRP_DRV_ASSERT(ret_val);


			/* Set AE */
			if (bayer2argb_layersum.layeractivepixel != 0) {
				brightness = (uint16_t)(bayer2argb_layersum.layersum / bayer2argb_layersum.layeractivepixel);
				R_BCD_AeRunAutoExpousure(&ae_setting, brightness);
			}


			/* Clear the current capture state and enable the detection of the next capture completion */
			R_BCD_CameraClearCaptureStatus();

			/* Set end time of process*/
			PerformSetEndTime(1);

			/***********************************/
			/* Load DRP Library                */
			/*        +----------------------+ */
			/* tile 0 |                      | */
			/*        +   Canny Calculate    + */
			/* tile 1 |                      | */
			/*        +----------------------+ */
			/* tile 2 | lrgb2l_gaussian_blur | */
			/*        +----------------------+ */
			/* tile 3 | lrgb2l_gaussian_blur | */
			/*        +----------------------+ */
			/* tile 4 |                      | */
			/*        +----------------------+ */
			/* tile 5 |                      | */
			/*        +----------------------+ */
			/***********************************/

			/* Set start time of process*/
			PerformSetStartTime(2);

			/*****************************************/
			/* load DRP Library lrgb2l_gaussian_blur */
			/*****************************************/
			ret_val = R_DK2_Load(&g_drp_lib_lrgb2l_gaussian_blur[0],
								R_DK2_TILE_2 | R_DK2_TILE_3,
								R_DK2_TILE_PATTERN_2_1_1_1_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/*********************************************/
			/* Activate DRP Library lrgb2l_gaussian_blur */
			/*********************************************/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_2] | drp_lib_id[TILE_3], 0);
			DRP_DRV_ASSERT(ret_val);

		    /****************************************************************/
			/* Set R_DK2_Start function parameters　for　lrgb2l_gaussian_blur */
			/****************************************************************/
			/* Set the address of buffer to be read/write by DRP */
			for (tile_no = TILE_2; tile_no <= TILE_3; tile_no++)
			{
				/* Set Image size */

				param_lrgb2l_gaussian_blur[tile_no - 2].width  = R_BCD_CAMERA_WIDTH;
				param_lrgb2l_gaussian_blur[tile_no - 2].height = R_BCD_CAMERA_HEIGHT / 2;

				output_bufadr = R_BCD_LcdGetVramAddress();              /* address of the video buffer which is not currently being displayed */

				/* Set the address of buffer to be read/write by DRP */
			    param_lrgb2l_gaussian_blur[tile_no - 2].src = (uint32_t)(output_bufadr + (param_lrgb2l_gaussian_blur[tile_no - 2].width * param_lrgb2l_gaussian_blur[tile_no - 2].height * (uint16_t)((tile_no - 2u) * 4u)));
			    param_lrgb2l_gaussian_blur[tile_no - 2].dst = (uint32_t)&frame_RAM_A[(param_lrgb2l_gaussian_blur[tile_no - 2].width * param_lrgb2l_gaussian_blur[tile_no - 2].height * (uint16_t)(tile_no - 2u) )];
				R_MMU_VAtoPA((uint32_t)param_lrgb2l_gaussian_blur[tile_no - 2].src, &(param_lrgb2l_gaussian_blur[tile_no - 2].src ));
				R_MMU_VAtoPA((uint32_t)param_lrgb2l_gaussian_blur[tile_no - 2].dst, &(param_lrgb2l_gaussian_blur[tile_no - 2].dst ));

				/* Set whether to perform top or bottom edge border processing. */
				param_lrgb2l_gaussian_blur[tile_no - 2].top    = (tile_no == TILE_2) ? 1 : 0;
				param_lrgb2l_gaussian_blur[tile_no - 2].bottom = (tile_no == TILE_3) ? 1 : 0;

				param_lrgb2l_gaussian_blur[tile_no - 2].spare1 =  0;
				param_lrgb2l_gaussian_blur[tile_no - 2].spare2 =  0;

				/* Initialize variables to be used in termination judgment of the DRP application */
				drp_lib_status[tile_no] = DRP_NOT_FINISH;

				/*********************/
				/* Start DRP Library */
				/*********************/
				ret_val = R_DK2_Start(drp_lib_id[tile_no], (void *)&param_lrgb2l_gaussian_blur[tile_no - 2], sizeof(r_drp_lrgb2l_gaussian_blur_t));
				DRP_DRV_ASSERT(ret_val);
			}

			/*##################################*/
			/* load DRP Library Canny Calculate */
			/*##################################*/

			ret_val = R_DK2_Load(&g_drp_lib_canny_calculate[0],
								 R_DK2_TILE_0,
								 R_DK2_TILE_PATTERN_2_1_1_1_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/*##################################*/
			/* Activate DRP Lib Canny Calculate */
			/*##################################*/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
			DRP_DRV_ASSERT(ret_val);


			/***************************************/
			 /* Wait until DRP processing is finish */
			 /***************************************/
			for (tile_no = TILE_2; tile_no <= TILE_3; tile_no += 1)
			{
				/***************************************/
				/* Wait until DRP processing is finish */
				/***************************************/
				while (drp_lib_status[tile_no] == DRP_NOT_FINISH);
			}

			/**********************/
			/* Unload DRP library */
			/**********************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_2] | drp_lib_id[TILE_3], &drp_lib_id[TILE_2]);
			DRP_DRV_ASSERT(ret_val);

			/* Set end time of process*/
			PerformSetEndTime(2);

			/*############   <-  lrgb2l_gaussian_blur        ##############*/

			/*############         Canny Calculate      ->   ##############*/

			/* Set start time of process*/
			PerformSetStartTime(3);

			/***************************************/
			/* Set R_DK2_Start function parameters */
			/***************************************/
			tile_no = TILE_0;

			/* Set Image size */
			param_canny_cal[tile_no].width  = R_BCD_CAMERA_WIDTH;
			param_canny_cal[tile_no].height = R_BCD_CAMERA_HEIGHT;

			/* Set Buffer size */
			bufsize = (uint32_t)(param_canny_cal[tile_no].width * param_canny_cal[tile_no].height);

			/* Set the address of buffer to be read/write by DRP */
			param_canny_cal[tile_no].src = (uint32_t)&frame_RAM_A[(tile_no * bufsize)];
			param_canny_cal[tile_no].dst = (uint32_t)&frame_RAM_B[(tile_no * bufsize)];
			R_MMU_VAtoPA((uint32_t)param_canny_cal[tile_no].src, &(param_canny_cal[tile_no].src ));
			R_MMU_VAtoPA((uint32_t)param_canny_cal[tile_no].dst, &(param_canny_cal[tile_no].dst ));

			/* Set additional information (currently no additional information) */
			param_canny_cal[tile_no].work = (uint32_t)&work1[(tile_no * (bufsize + (R_BCD_CAMERA_WIDTH*2) ))*2];
			R_MMU_VAtoPA((uint32_t)param_canny_cal[tile_no].work, &(param_canny_cal[tile_no].work ));

			param_canny_cal[tile_no].threshold_high = STRONG_LEVEL;
			param_canny_cal[tile_no].threshold_low = WEAK_LEVEL;

			/* Set whether to perform top or bottom edge border processing. */
			param_canny_cal[tile_no].top    = (tile_no == TILE_0) ? 1 : 0;
			param_canny_cal[tile_no].bottom = (tile_no == TILE_0) ? 1 : 0;

			/* Initialize variables to be used in termination judgment of the DRP library */
			drp_lib_status[tile_no] = DRP_NOT_FINISH;

			/*********************/
			/* Start DRP Library */
			/*********************/
			ret_val = R_DK2_Start(drp_lib_id[tile_no], (void *)&param_canny_cal[tile_no / 2], sizeof(r_drp_canny_calculate_t));
			DRP_DRV_ASSERT(ret_val);

			/***************************************/
			/* Wait until DRP processing is finish */
			/***************************************/
			while (drp_lib_status[TILE_0] == DRP_NOT_FINISH);

			/**********************/
			/* Unload DRP library */
			/**********************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[TILE_0]);
			DRP_DRV_ASSERT(ret_val);

			/* Set end time of DRP processing */
			PerformSetEndTime(3);

			/*******************************/
			/* Load DRP Library            */
			/*        +------------------+ */
			/* tile 0 |                  | */
			/*        +                  + */
			/* tile 1 |                  | */
			/*        +                  + */
			/* tile 2 |                  | */
			/*        + Canny Hysterisis + */
			/* tile 3 |                  | */
			/*        +                  + */
			/* tile 4 |                  | */
			/*        +                  + */
			/* tile 5 |                  | */
			/*        +------------------+ */
			/*******************************/

			/* Set start time of Canny Hyst process */
			PerformSetStartTime(4);

			ret_val = R_DK2_Load(&g_drp_lib_canny_hysterisis[0],
								 R_DK2_TILE_0,
								 R_DK2_TILE_PATTERN_6, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/************************/
			/* Activate DRP Library */
			/************************/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
			DRP_DRV_ASSERT(ret_val);

			/***************************************/
			/* Set R_DK2_Start function parameters */
			/***************************************/
			/* Set Image size */
			param_canny_hyst.width = R_BCD_CAMERA_WIDTH;
			param_canny_hyst.height = R_BCD_CAMERA_HEIGHT;

			/* Set the address of buffer to be read/write by DRP */
			param_canny_hyst.src = (uint32_t)&frame_RAM_B[0];
			param_canny_hyst.dst = (uint32_t)&frame_RAM_C[0];
			R_MMU_VAtoPA((uint32_t)param_canny_hyst.src, &(param_canny_hyst.src ));
			R_MMU_VAtoPA((uint32_t)param_canny_hyst.dst, &(param_canny_hyst.dst ));

			/* Set additional information (currently no additional information) */
			param_canny_hyst.work = (uint32_t)&work2[0];
			R_MMU_VAtoPA((uint32_t)param_canny_hyst.work, &(param_canny_hyst.work ));
			param_canny_hyst.iterations = HYSTERISIS_COUNT;

			/* Initialize variables to be used in termination judgment of the DRP library */
			drp_lib_status[TILE_0] = DRP_NOT_FINISH;

			/*********************/
			/* Start DRP Library */
			/*********************/
			ret_val = R_DK2_Start(drp_lib_id[TILE_0], (void *)&param_canny_hyst, sizeof(r_drp_canny_hysterisis_t));
			DRP_DRV_ASSERT(ret_val);

			/***************************************/
			/* Wait until DRP processing is finish */
			/***************************************/
			while (drp_lib_status[TILE_0] == DRP_NOT_FINISH);

			/**********************/
			/* Unload DRP Library */
			/**********************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[TILE_0]);
			DRP_DRV_ASSERT(ret_val);

			/* Set end time of DRP processing */
			PerformSetEndTime(4);

			/************************************/
			/* Load DRP Library                 */
			/*        +-----------------------+ */
			/* tile 0 |                       | */
			/*        +                       + */
			/* tile 1 | BinarizationAdaptive  | */
			/*        +                       + */
			/* tile 2 |                       | */
			/*        +-----------------------+ */
			/* tile 3 |                       | */
			/*        +   FindContours_crop   + */    /* find countours with dedicated aspect ratio */
			/* tile 4 |                       | */
			/*        +-----------------------+ */
			/* tile 5 |        Dilate         | */
			/*        +-----------------------+ */
			/************************************/

			/* Set start time of process*/
			PerformSetStartTime(5);

			/*############             Dilate  ->           ##############*/

			ret_val = R_DK2_Load(&g_drp_lib_dilate[0],
								R_DK2_TILE_5,
								R_DK2_TILE_PATTERN_3_2_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/************************/
			/* Activate DRP Library */
			/************************/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_5], 0);
			DRP_DRV_ASSERT(ret_val);

			/**********************************************************/
			/* Set R_DK2_Start function parameters　for　Gaussian Blur */
			/*********************************************************/

			/* Set Image size */
			param_dilate.width  = R_BCD_CAMERA_WIDTH;
			param_dilate.height = R_BCD_CAMERA_HEIGHT;

			/* Set the address of buffer to be read/write by DRP */
			param_dilate.src = (uint32_t)&frame_RAM_C[0];
			param_dilate.dst = (uint32_t)&frame_RAM_D[0];
			R_MMU_VAtoPA((uint32_t)param_dilate.src, &(param_dilate.src ));
			R_MMU_VAtoPA((uint32_t)param_dilate.dst, &(param_dilate.dst ));

			/* Set whether to perform top or bottom edge border processing. */
			param_dilate.top    = 1;
			param_dilate.bottom = 1;

			/* Initialize variables to be used in termination judgment of the DRP application */
			drp_lib_status[TILE_5] = DRP_NOT_FINISH;

			/****************************/
			/* Start DRP Library Dilate */
			/****************************/
			ret_val = R_DK2_Start(drp_lib_id[TILE_5], (void *)&param_dilate, sizeof(r_drp_dilate_t));
			DRP_DRV_ASSERT(ret_val);


			/*#######################################*/
			/* Load DRP Library FindContours_crop    */
			/*#######################################*/

			ret_val = R_DK2_Load(&g_drp_lib_find_contours_crop_t2[0],
								  R_DK2_TILE_3,
								  R_DK2_TILE_PATTERN_3_2_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/*####################################*/
			/* Activate DRP Lib FindContours_crop */
			/*####################################*/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_3], 0);
			DRP_DRV_ASSERT(ret_val);


			/************************************************/
			/* Wait until DRP processing (Dilate) is finish */
			/************************************************/
			while (drp_lib_status[TILE_5] == DRP_NOT_FINISH);

			/**********************/
			/* Unload DRP library */
			/**********************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_5], &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/* Set end time of DRP processing */
			PerformSetEndTime(5);

			/*############   <-        Dilate               ##############*/

			/*############        FindContours_crop    ->   ##############*/

			/* Set start time of process*/
			PerformSetStartTime(6);     // FindContours_crop

			/*************************************************************/
			/* Set R_DK2_Start function parameters for FindContours_crop */
			/*************************************************************/

			param_find_contours_crop.src = (uint32_t)&frame_RAM_D[0];               /* Address of input one lane image */
			R_MMU_VAtoPA((uint32_t)param_find_contours_crop.src, &(param_find_contours_crop.src ));

			param_find_contours_crop.dst_rect = (uint32_t)&contours_rect_adr[0];    /* Address of rectangle information */
			R_MMU_VAtoPA((uint32_t)param_find_contours_crop.dst_rect, &(param_find_contours_crop.dst_rect ));

			param_find_contours_crop.dst_rect_size         = FIND_CONTOURS_NUM;    /* max amount of rectangles to be generated */

			/* Set Image size */
			param_find_contours_crop.width  = R_BCD_CAMERA_WIDTH;   /* The horizontal size (pixels) of image multiple of 8byte */
			param_find_contours_crop.height = R_BCD_CAMERA_HEIGHT;  /* The vertical size (pixels) of image	minimum 5 lines    */

			/* Set whether to perform top or bottom edge border processing. */
			param_find_contours_crop.work = (uint32_t)&work2[0];    /* Address of work area; minimum size = width * height / 4 + 8 */
			R_MMU_VAtoPA((uint32_t)param_find_contours_crop.work, &(param_find_contours_crop.work ));

			param_find_contours_crop.dst_region = (uint32_t)&contours_region_adr[0];  /* Address of region information */
			R_MMU_VAtoPA((uint32_t)param_find_contours_crop.dst_region, &(param_find_contours_crop.dst_region ));

			param_find_contours_crop.dst_region_size       = 0;     /* 0: no region output */
			// param_find_contours_crop.dst_region_size = 10;       /* region information output for 10 pixel*/

			param_find_contours_crop.threshold_widthOrMin  = aspectfloat4contourfixedint(3.0); 	/* Threshold of width or min value		*/
			param_find_contours_crop.threshold_heightOrMax = aspectfloat4contourfixedint(5.5); 	/* Threshold of height or max value		*/

			param_find_contours_crop.x_crop                = 8;                    /* Crop window x position     */
			param_find_contours_crop.y_crop                = 8;                    /* Crop window y position     */

			param_find_contours_crop.width_crop            = (uint16_t)(R_BCD_CAMERA_WIDTH  - 16);  /* Crop window width  (min 3) */
			param_find_contours_crop.height_crop           = (uint16_t)(R_BCD_CAMERA_HEIGHT - 16);  /* Crop window height (min 3) */

			param_find_contours_crop.mode                  = 0b00000011 ;
				/* 0000RFFF (bit field)		  	 	    									*/
				/*  R - origin of output coordinates										*/
				/*		  0 - off (coordinates are relative to src picture)					*/
				/*		  1 - on  (coordinates relative to crop	window upper left (0 0)) 	*/
				/*  F - filter for box/region found											*/
				/*		000 - boxWidth	>=	threshold_widthOrMin && boxHeight > threshold_heightOrMax																	*/
				/*		001 - regionCnt >   threshold_region																											*/
				/*		010 - boxWidth	>=	threshold_widthOrMin			  && boxHeight > threshold_heightOrMax					&& regionCnt >	threshold_region	*/
				/*		011 - boxWidth/boxHeight*256 >= threshold_widthOrMin  && boxWidth/boxHeight*256 <= threshold_heightOrMax	&& regionCnt >	threshold_region	*/
				/*		100 - boxWidth/boxHeight*256 >= threshold_widthOrMin  && regionCnt >	threshold_region														*/
				/*		101 - boxWidth/boxHeight*256 <= threshold_heightOrMax && regionCnt >	threshold_region														*/
				/*      110 - reserved																																	*/
				/*      111 - reserved																																	*/

			param_find_contours_crop.dmode                  = 0b00000001 ;
				/* WCBP000A (bit field)		  	 	    					*/
				/*  W - draw cropping window 						-> dst_rgbDebug(RGB)	*/
				/*		  0 - off, 1 -on 													*/
				/*  C - draw contour data stored in dst_region into -> dst_rgbDebug(RGB)	*/
				/*		  0 - off, 1 -on 													*/
				/*  B - boundingBox	 draw bounding box into 		-> dst_rgbDebug(RGB)	*/
				/*		  0 - off, 1 -on 													*/
				/*  P - debug		 copy picture src 				-> dst_rgbDebug(RGB)	*/
				/*		  0 - off, 1 -on 													*/
				/*  A - output format for debug 					-> dst_rgbDebug(RGB)	*/
				/*		  0 - RGB (24bit), 1 -ARGB(32bit) A channel set to 0				*/

			param_find_contours_crop.threshold_region       = 500 ;		/* Threshold contour count (number of found contour pixel) (example 30)                            */
			param_find_contours_crop.dst_rgbDebug           = 0 ; 		/* Output address for RGB picture, debug bounding box(blue), found region(red), found pixel(green) */
																		/*please set to NULL if not used. Size is same as "width" * "height" of "src"                      */


			/* Initialize variables to be used in termination judgment of the DRP library */
			drp_lib_status[TILE_3] = DRP_NOT_FINISH;

			/***************************************/
			/* Start DRP Library FindContours_crop */
			/***************************************/
			ret_val = R_DK2_Start(drp_lib_id[TILE_3], (void *)&param_find_contours_crop, sizeof(r_drp_find_contours_crop_t));
			DRP_DRV_ASSERT(ret_val);


			/*############    -  BinarizationAdaptive  -    ##############*/

			/*****************************************/
			/* Load DRP Library BinarizationAdaptive */
			/*****************************************/
			ret_val = R_DK2_Load(&g_drp_lib_binarization_adaptive[0],
			                      R_DK2_TILE_0,
				                  R_DK2_TILE_PATTERN_3_2_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
			DRP_DRV_ASSERT(ret_val);

			/*****************************************/
			/* Activate DRP Lib BinarizationAdaptive */
			/*****************************************/
			ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
			DRP_DRV_ASSERT(ret_val);

			/****************************************************************/
			/* Set R_DK2_Start function parameters for BinarizationAdaptive */
			/****************************************************************/

			param_binarization_adaptive.src        = (uint32_t)&frame_RAM_A[0];      /* Address of image to be binarized. */
			param_binarization_adaptive.dst        = (uint32_t)&frame_RAM_E[0];      /* Address to output binarized result. */
			param_binarization_adaptive.width      = (uint16_t)R_BCD_CAMERA_WIDTH;   /* The horizontal size (pixels) of the image to be binarized. */
			param_binarization_adaptive.height     = (uint16_t)R_BCD_CAMERA_HEIGHT;  /* The vertical size (pixels) of the image to be binarized. */
			param_binarization_adaptive.work       = (uint32_t)&work3[0];            /* Address of the work buffer used by the library. */
			param_binarization_adaptive.range      = 0x0;                            /* A luminance difference to judge whether it is effective average luminance.(8x8 pixel) */

			/* Initialize variables to be used in termination judgment of the DRP application */
			drp_lib_status[TILE_0] = DRP_NOT_FINISH;

			/******************************************/
			/* Start DRP Library BinarizationAdaptive */
			/******************************************/
			ret_val = R_DK2_Start(drp_lib_id[TILE_0], (void *)&param_binarization_adaptive, sizeof(r_drp_binarization_adaptive_t));
			DRP_DRV_ASSERT(ret_val);


			/*****************************************/
			/* Wait until FindContours_crop finished */
			/*****************************************/
			while (drp_lib_status[TILE_3] == DRP_NOT_FINISH);  // FindContours_crop

			/*############   <-   FindContours_crop         ##############*/

			/* Set end time of DRP processing */
			PerformSetEndTime(6);

		} /* find all contours */

	    /********************************************************************/
		/* select dedicated contours                                        */
	    /********************************************************************/

        amount_pot_obj = 0;
		mscn           = FIND_CONTOURS_NUM + 2;

		for (cn = 0; cn <= FIND_CONTOURS_NUM; cn++)   /* select contours with number plate aspect ratio */
		{
			x1     = contours_rect_adr[cn].x;
			y1     = contours_rect_adr[cn].y;
			length = contours_rect_adr[cn].width;
			height = contours_rect_adr[cn].height;

			if ((x1 == 0) && (y1 == 0) && (length == 0) && (height == 0)) {
				break;
			}


			xy_ratio = (float)length / (float)height;

			/* print contour info for debugging */
		    if (SerialTerminal_on && SerialTerminal_show_contour_info) {
				fprintf(stdout,"contours [#,x,y,w,h,xy_ratio] : %d,%d,%d,%d,%d,%f\n",cn, x1,y1,length,height,xy_ratio);
				fflush(stdout);
			}

			if ((((xy_ratio  > 3.0 ) && (xy_ratio  < 5.5 ))    ) &&             /* number plate with 1 row */
				( length * height > 2000)                               )       /* minimum size to detect numbers */
			{
				overlap = NO_OVERLAP;
				if (amount_pot_obj > 0) {
					re      = (uint16_t)(x1 + length);
					be      = (uint16_t)(y1 + height);
					for (ccn2 = 0; ccn2 < amount_pot_obj; ccn2++)     /* search for overlapping plate contours */
					{
						x2 = contours_rect_adr[pot_obj_rect_number[ccn2]].x;
						y2 = contours_rect_adr[pot_obj_rect_number[ccn2]].y;

						re2 = (uint16_t)(x2 + contours_rect_adr[pot_obj_rect_number[ccn2]].width);
						be2 = (uint16_t)(y2 + contours_rect_adr[pot_obj_rect_number[ccn2]].height);

						if (!((re2 < x1) || (be2 < y1) || (x2 > re) || (y2 > be)))      /* overlap */
						{
							overlap = OVERLAPPING;
							if ((length * height) < (contours_rect_adr[pot_obj_rect_number[ccn2]].width * contours_rect_adr[pot_obj_rect_number[ccn2]].height))
							{
								pot_obj_rect_number[ccn2] = cn;   /* in case of overlapping contours, switch to smaller contour */
							}
							break;
						}
					}
					if (overlap == NO_OVERLAP)
					{
						pot_obj_rect_number[amount_pot_obj] = cn;
						amount_pot_obj++;
					}
				} else {
					pot_obj_rect_number[amount_pot_obj] = cn;
					amount_pot_obj++;
				}
			}
		}

		/* print amount of potential number plates and their indexes for debugging */
	    if (SerialTerminal_on && SerialTerminal_show_pot_number_plate) {
			fprintf(stdout,"\namount of potential number plates             : %d  (",amount_pot_obj);
			if (amount_pot_obj > 0) {
				for (cn = 0; cn < amount_pot_obj; cn++)
				{
					fprintf(stdout,"%d,",pot_obj_rect_number[cn]);
				}
			}
			fprintf(stdout,")\n\n");
			fflush(stdout);
		}


	    /********************************************************************/
		/* find contours in all potential objects (number plate contours);  */
		/* select the most suitable one and start character classification  */
	    /********************************************************************/

		if (amount_pot_obj > 0) {

			/************************************/
			/* continue to use loaded DRP libs  */
			/*        +-----------------------+ */
			/* tile 0 |                       | */
			/*        +                       + */
			/* tile 1 |                       | */
			/*        +                       + */
			/* tile 2 |                       | */
			/*        +-----------------------+ */
			/* tile 3 |                       | */
			/*        +   FindContours_crop   + */
			/* tile 4 |                       | */
			/*        +-----------------------+ */
			/* tile 5 |                       | */
			/*        +-----------------------+ */
			/************************************/

			/* Set start time of process*/
			PerformSetStartTime(20);

			for (cn = 0; cn < amount_pot_obj; cn++)   /* find contours in all potential number plate contours and select most suitable one */
			{

				x1     = contours_rect_adr[pot_obj_rect_number[cn]].x;
				y1     = contours_rect_adr[pot_obj_rect_number[cn]].y;
				length = contours_rect_adr[pot_obj_rect_number[cn]].width;
				height = contours_rect_adr[pot_obj_rect_number[cn]].height;

				cropped_x[cn]      = x1;
				cropped_y[cn]      = y1;
				cropped_width[cn]  = length;
				cropped_height[cn] = height;

//			    if (cropped_x[cn] > (0.3 * cropped_width[cn]) - 8) {
//			    	extended_cropped_x     = (uint16_t)(cropped_x[cn] - (uint16_t)(0.3 * cropped_width[cn]));
//			    } else {
//			    	extended_cropped_x     = 8;
//			    }
//		    	extended_cropped_width = (uint16_t)(cropped_width[cn] + (cropped_x[cn] - extended_cropped_x));
//			    if (cropped_x[cn] + (1.3 * cropped_width[cn]) < R_BCD_CAMERA_WIDTH - 8) {
//			    	extended_cropped_width = (uint16_t)(extended_cropped_width + (uint16_t)(0.3 * cropped_width[cn]));
//			    } else {
//			    	extended_cropped_width = R_BCD_CAMERA_WIDTH - 8 - extended_cropped_x;
//			    }

				// extend contour in x direction
				if (cropped_x[cn] > (0.05 * cropped_width[cn]) - 8) {
			    	extended_cropped_x     = (uint16_t)(cropped_x[cn] - (uint16_t)(0.05 * cropped_width[cn]));
			    } else {
			    	extended_cropped_x     = 8;
			    }
		    	extended_cropped_width = (uint16_t)(cropped_width[cn] + (cropped_x[cn] - extended_cropped_x));
			    if (cropped_x[cn] + (1.05 * cropped_width[cn]) < R_BCD_CAMERA_WIDTH - 8) {
			    	extended_cropped_width = (uint16_t)(extended_cropped_width + (uint16_t)(0.05 * cropped_width[cn]));
			    } else {
			    	extended_cropped_width = R_BCD_CAMERA_WIDTH - 8 - extended_cropped_x;
			    }

				/***************************************/
				/* Set R_DK2_Start function parameters */
				/***************************************/

				param_find_contours_crop.src = (uint32_t)&frame_RAM_D[0];                       /* Address of input one lane image */
				R_MMU_VAtoPA((uint32_t)param_find_contours_crop.src, &(param_find_contours_crop.src ));

				param_find_contours_crop.dst_rect = (uint32_t)&cropped_contours_rect_adr[0];    /* Address of rectangle information */
				R_MMU_VAtoPA((uint32_t)param_find_contours_crop.dst_rect, &(param_find_contours_crop.dst_rect ));

				param_find_contours_crop.dst_rect_size         = FIND_CONTOURS_NUM;    /* max amount of rectangles to be generated */

				/* Set Image size */
				param_find_contours_crop.width  = R_BCD_CAMERA_WIDTH;   /* The horizontal size (pixels) of image multiple of 8byte */
				param_find_contours_crop.height = R_BCD_CAMERA_HEIGHT;  /* The vertical size (pixels) of image	minimum 5 lines    */

				/* Set whether to perform top or bottom edge border processing. */
				param_find_contours_crop.work = (uint32_t)&work2[0];    /* Address of work area; minimum size = width * height / 4 + 8 */
				R_MMU_VAtoPA((uint32_t)param_find_contours_crop.work, &(param_find_contours_crop.work ));

				param_find_contours_crop.dst_region = (uint32_t)&cropped_contours_region_adr[0];    /* Address of region information */
				R_MMU_VAtoPA((uint32_t)param_find_contours_crop.dst_region, &(param_find_contours_crop.dst_region ));

				param_find_contours_crop.dst_region_size       = 0;      /* 0: no region output */
				// param_find_contours_crop.dst_region_size = 10;        /* region information output for 10 pixel*/

				param_find_contours_crop.threshold_widthOrMin  = 15; 	/* Threshold of width or min value		*/
				param_find_contours_crop.threshold_heightOrMax = 24; 	/* Threshold of height or max value		*/

				param_find_contours_crop.x_crop                = extended_cropped_x;          /* find characters in row across whole image */
				param_find_contours_crop.y_crop                = cropped_y[cn];        /* Crop window x position     */

				param_find_contours_crop.width_crop            = extended_cropped_width;    /* find characters in row across whole image */
				param_find_contours_crop.height_crop           = cropped_height[cn];   /* Crop window height (min 3) */

				param_find_contours_crop.mode                  = 0b00000000 ;
					/* 0000RFFF (bit field)		  	 	    									*/
					/*  R - origin of output coordinates										*/
					/*		  0 - off (coordinates are relative to src picture)					*/
					/*		  1 - on  (coordinates relative to crop	window upper left (0 0)) 	*/
					/*  F - filter for box/region found											*/
					/*		000 - boxWidth	>=	threshold_widthOrMin && boxHeight > threshold_heightOrMax																	*/
					/*		001 - regionCnt >   threshold_region																											*/
					/*		010 - boxWidth	>=	threshold_widthOrMin			  && boxHeight > threshold_heightOrMax					&& regionCnt >	threshold_region	*/
					/*		011 - boxWidth/boxHeight*256 >= threshold_widthOrMin  && boxWidth/boxHeight*256 <= threshold_heightOrMax	&& regionCnt >	threshold_region	*/
					/*		100 - boxWidth/boxHeight*256 >= threshold_widthOrMin  && regionCnt >	threshold_region														*/
					/*		101 - boxWidth/boxHeight*256 <= threshold_heightOrMax && regionCnt >	threshold_region														*/
					/*      110 - reserved																																	*/
					/*      111 - reserved																																	*/

				param_find_contours_crop.dmode                  = 0b00000001 ;
					/* WCBP000A (bit field)		  	 	    					*/
					/*  W - draw cropping window 						-> dst_rgbDebug(RGB)	*/
					/*		  0 - off, 1 -on 													*/
					/*  C - draw contour data stored in dst_region into -> dst_rgbDebug(RGB)	*/
					/*		  0 - off, 1 -on 													*/
					/*  B - boundingBox	 draw bounding box into 		-> dst_rgbDebug(RGB)	*/
					/*		  0 - off, 1 -on 													*/
					/*  P - debug		 copy picture src 				-> dst_rgbDebug(RGB)	*/
					/*		  0 - off, 1 -on 													*/
					/*  A - output format for debug 					-> dst_rgbDebug(RGB)	*/
					/*		  0 - RGB (24bit), 1 -ARGB(32bit) A channel set to 0				*/

				param_find_contours_crop.threshold_region       = 20 ;		/* Threshold contour count (number of found contour pixel) (example 30)                            */
				param_find_contours_crop.dst_rgbDebug           = 0 ; 		/* Output address for RGB picture, debug bounding box(blue), found region(red), found pixel(green) */
																			/*please set to NULL if not used. Size is same as "width" * "height" of "src"                      */

				/* Initialize variables to be used in termination judgment of the DRP library */
				drp_lib_status[TILE_3] = DRP_NOT_FINISH;

				/*********************/
				/* Start DRP Library */
				/*********************/
				ret_val = R_DK2_Start(drp_lib_id[TILE_3], (void *)&param_find_contours_crop, sizeof(r_drp_find_contours_crop_t));
				DRP_DRV_ASSERT(ret_val);

				/************************************************************/
				/* Wait until DRP processing BinarizationAdaptive is finish */
				/************************************************************/
				while (drp_lib_status[TILE_3] == DRP_NOT_FINISH);

				/********************************************************************/
				/* select dedicated contours                                        */
				/********************************************************************/
				amount_pot_char[cn]      = 0;

				/* print contour info for debugging */
				if (SerialTerminal_on && SerialTerminal_show_pot_number_plate) {
					fprintf(stdout,"potential number plate number [x,y,w,h] -> extended [x,y,w,h] : %d [%d,%d,%d,%d] - [%d,%d,%d,%d]\n",pot_obj_rect_number[cn], cropped_x[cn], cropped_y[cn], cropped_width[cn], cropped_height[cn], extended_cropped_x, cropped_y[cn], extended_cropped_width, cropped_height[cn]);
					fflush(stdout);
				}

				for (ccn = 0; ccn <= FIND_CONTOURS_NUM; ccn++)
				{
					x1     = cropped_contours_rect_adr[ccn].x;
					y1     = cropped_contours_rect_adr[ccn].y;
					length = cropped_contours_rect_adr[ccn].width;
					height = cropped_contours_rect_adr[ccn].height;

					if ((x1 == 0) && (y1 == 0) && (length == 0) && (height == 0)) {
						break;
					}

					xy_ratio = (float)length / (float)height;

					/* print contour info for debugging */
				    if (SerialTerminal_on && SerialTerminal_show_char_contours) {
						fprintf(stdout,"   char contours within number plate [x,y,w,h] xy_ratio : [%d,%d,%d,%d] %f\n",x1,y1,length,height,xy_ratio);
						fflush(stdout);
					}

					if ((xy_ratio  > 0.40) &&
						(xy_ratio  < 0.85) &&
						(height    > 0.6 * cropped_height[cn]) &&
						(height    < 0.9 * cropped_height[cn])
						)
					{
						/* check overlapping; in case of overlapping contours, switch to smaller contour */
						/* 4 pixel overlap on the right or left side allowed */
						overlap = NO_OVERLAP;          /* check overlapping; in case of overlapping contours, switch to smaller contour */
						if (amount_pot_char[cn] > 0) {
							re  = (uint16_t)(x1 + length);
							be  = (uint16_t)(y1 + height);
							for (ccn2 = 0; ccn2 < amount_pot_char[cn]; ccn2++)     /* skip double char contours */
							{
								x2 = cropped_contours_rect_adr[pot_char_rect_number[ccn2]].x;
								y2 = cropped_contours_rect_adr[pot_char_rect_number[ccn2]].y;

								re2 = (uint16_t)(x2 + cropped_contours_rect_adr[pot_char_rect_number[ccn2]].width);
								be2 = (uint16_t)(y2 + cropped_contours_rect_adr[pot_char_rect_number[ccn2]].height);

								// fprintf(stdout,"      %d < %d  or  %d < %d  or  %d > %d  or  %d > %d ? ",re2,x1,be2,y1,x2,re,y2,be);

								if (!((re2 < (x1 + 4)) || (be2 < y1) || ((x2 + 4) > re) || (y2 > be)))      /* overlap */
								{
									overlap = OVERLAPPING;
									if ((length * height) < (cropped_contours_rect_adr[pot_char_rect_number[ccn2]].width * cropped_contours_rect_adr[pot_char_rect_number[ccn2]].height))
									{
										pot_char_rect_number[ccn2] = ccn;   /* in case of overlapping contours, switch to smaller contour */
									}
									// fprintf(stdout,"  NO  => overlapping contour \n");
									break;
								}
							}
							if (overlap == NO_OVERLAP)
							{
								pot_char_rect_number[amount_pot_char[cn]] = ccn;
								amount_pot_char[cn]++;
								// fprintf(stdout,"  YES  => contour not overlapping \n");
							}
						} else {
							pot_char_rect_number[amount_pot_char[cn]] = ccn;
							amount_pot_char[cn]++;
						}
					}
				}

				if (mscn == (FIND_CONTOURS_NUM + 2)){    /* select most suitable number plate contour */
					if ((amount_pot_char[cn] > 3)){
						mscn = cn;
						for (ccn = 0; ccn <= FIND_CONTOURS_NUM; ccn++)
						{
							number_plate_contours_rect_adr[ccn] = cropped_contours_rect_adr[ccn];
							number_plate_char_rect_number[ccn]  = pot_char_rect_number[ccn];
						}
					}
				} else {
					if ((amount_pot_char[cn] > 3) && (amount_pot_char[mscn] <= amount_pot_char[cn] )){
						mscn = cn;
						for (ccn = 0; ccn <= FIND_CONTOURS_NUM; ccn++)
						{
							number_plate_contours_rect_adr[ccn] = cropped_contours_rect_adr[ccn];
							number_plate_char_rect_number[ccn]  = pot_char_rect_number[ccn];
						}
					}
				}

				/* print info for debugging */
				if(SerialTerminal_on) {
					fprintf(stdout,"potential chars in number plate %d :  %d\n", pot_obj_rect_number[cn], amount_pot_char[cn]);
					fflush(stdout);
				}
			}

			/*******************************************************/
			/* Wait until BinarizationAdaptive processing finished */
			/*******************************************************/
			while (drp_lib_status[TILE_0] == DRP_NOT_FINISH);  // BinarizationAdaptive

			/*******************************************/
			/* Unload DRP library BinarizationAdaptive */
			/*******************************************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[TILE_0]);
			DRP_DRV_ASSERT(ret_val);

			/****************************************/
			/* Unload DRP library FindContours_crop */
			/****************************************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_3], &drp_lib_id[TILE_3]);
			DRP_DRV_ASSERT(ret_val);


			/* Set end time of DRP processing */
			PerformSetEndTime(20);


			/*****************************************************************/
			/* crop, resize and normalize found characters; start inference  */
			/*****************************************************************/

			if (mscn != (FIND_CONTOURS_NUM + 2)){    // crop chars and start inference

				/*************************************************************************************/
				/* sort char characters in x-direction                                               */
				/* evaluate most reasonable char height (special treatment for german letters Ä,Ü,Ö) */
				/*************************************************************************************/

				char_height = 0;
				for (ccn = 0; ccn < amount_pot_char[mscn] - 1; ccn++)
				{
					if (char_height < number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].height)
					{
						char_height = number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].height;
					}
					for (ccn2 = (uint8_t)(ccn + 1); ccn2 < amount_pot_char[mscn]; ccn2++)
					{
						if (number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].x > number_plate_contours_rect_adr[number_plate_char_rect_number[ccn2]].x)
						{
							sort_char_rect_number               = number_plate_char_rect_number[ccn];
							number_plate_char_rect_number[ccn]  = number_plate_char_rect_number[ccn2];
							number_plate_char_rect_number[ccn2] = sort_char_rect_number;
						}
					}
				}
			    if (SerialTerminal_on && SerialTerminal_show_char_contours) {
			    	fprintf(stdout,"\nmax char_height            : %d\n", char_height);
				    fflush(stdout);
			    }
				for (ccn = 0; ccn < amount_pot_char[mscn]; ccn++)
				{
					if (char_height > 1.1 * number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].height)
					{
					    if (SerialTerminal_on && SerialTerminal_show_char_contours) {
					    	fprintf(stdout,"height of char %d changed from %d to %d\n", ccn +1 , number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].height, char_height);
						    fflush(stdout);
					    }
						number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].height = char_height;
						if (ccn == 0) {                                  /* first charachter */
							number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].y = (uint16_t)number_plate_contours_rect_adr[number_plate_char_rect_number[ccn + 1]].y;
						} else {
							if (ccn == amount_pot_char[mscn] - 1) {      /* last character   */
								number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].y = (uint16_t)number_plate_contours_rect_adr[number_plate_char_rect_number[ccn - 1]].y;
							} else {                                     /* middle character */
								number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].y = (uint16_t)((number_plate_contours_rect_adr[number_plate_char_rect_number[ccn - 1]].y + number_plate_contours_rect_adr[number_plate_char_rect_number[ccn + 1]].y) / (uint16_t)2);
							}
						}
					}
				}

			    if (SerialTerminal_on) {
			    	fprintf(stdout,"\nmost potential number plate number            : %d\n", pot_obj_rect_number[mscn]);
				    fflush(stdout);
			    }

				/*******************************/
				/* Load DRP Library            */
				/*        +------------------+ */
				/* tile 0 |                  | */
				/*        +                  + */
				/* tile 1 |                  | */
				/*        +                  + */
				/* tile 2 |                  | */
				/*        + Resize Bilinear  + */
				/* tile 3 |      Crop        | */
				/*        +                  + */
				/* tile 4 |                  | */
				/*        +                  + */
				/* tile 5 |                  | */
				/*        +------------------+ */
				/*******************************/

				/* Set start time of Resize Bilinear Crop process */
				PerformSetStartTime(22);

				ret_val = R_DK2_Load(&g_drp_lib_resize_bilinear_crop_t6[0],
									 R_DK2_TILE_0,
									 R_DK2_TILE_PATTERN_6, NULL, &cb_drp_finish, &drp_lib_id[0]);
				DRP_DRV_ASSERT(ret_val);

				/************************/
				/* Activate DRP Library */
				/************************/
				ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
				DRP_DRV_ASSERT(ret_val);

		        memset(predicted_char_index, (uint8_t)39, FIND_CONTOURS_NUM);   // prediction unknown (="?")

				for (ccn = 0; ccn < amount_pot_char[mscn]; ccn++)
				{
					/***************************************/
					/* Set R_DK2_Start function parameters */
					/***************************************/

					/* Set start time of resize_bilinear_crop */
					PerformSetStartTime(30);

					x1     = number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].x;
					y1     = number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].y;
					length = number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].width ;
					height = number_plate_contours_rect_adr[number_plate_char_rect_number[ccn]].height;

					/* cropped character with border of 1 pixel (width and height + 2) */
					c_cropped_width[ccn]  = (uint16_t)(length + 2);
					c_cropped_height[ccn] = (uint16_t)(height + 2);

					/* centralize cropped image in revised shape */
					c_cropped_width_delta  = (uint16_t)((c_cropped_width[ccn]  - length)/ 2);
					c_cropped_height_delta = (uint16_t)((c_cropped_height[ccn] - height)/ 2);

					if (c_cropped_width[ccn]  > R_BCD_CAMERA_WIDTH ) {
						c_cropped_width[ccn]  = R_BCD_CAMERA_WIDTH ;
						c_cropped_x[ccn]      = 0 ;
					} else {
						if (x1 >= c_cropped_width_delta) {
							c_cropped_x[ccn]      = (uint16_t)(x1 - c_cropped_width_delta);
						} else {
							c_cropped_x[ccn]      = 0 ;
						}
					}

					if (c_cropped_height[ccn] > R_BCD_CAMERA_HEIGHT) {
						c_cropped_height[ccn] = R_BCD_CAMERA_HEIGHT;
						c_cropped_y[ccn]      = 0 ;
					} else {
						if (y1 >= c_cropped_height_delta) {
							c_cropped_y[ccn]      = (uint16_t)(y1 - c_cropped_height_delta);
						} else {
							c_cropped_y[ccn]      = 0 ;
						}
					}

					if ((c_cropped_x[ccn] + c_cropped_width[ccn] ) > R_BCD_CAMERA_WIDTH ) { c_cropped_x[ccn] = R_BCD_CAMERA_WIDTH  - 1 - c_cropped_width[ccn] ;}
					if ((c_cropped_y[ccn] + c_cropped_height[ccn]) > R_BCD_CAMERA_HEIGHT) { c_cropped_y[ccn] = R_BCD_CAMERA_HEIGHT - 1 - c_cropped_height[ccn];}

					// fprintf(stdout,"crop coordinates of pot. character (x,y,w,h)  : (%d,%d,%d,%d)\n", c_cropped_x[ccn], c_cropped_y[ccn],c_cropped_width[ccn],c_cropped_height[ccn]);
				    // fflush(stdout);

				    param_resize_bilinear_crop.src   = (uint32_t)&frame_RAM_E[0];            /* Address of input image (BinarizationAdaptive image) */
					param_resize_bilinear_crop.dst   = (uint32_t)&frame_RAM_CCR[ccn][0];     /* Address of output image. */

					param_resize_bilinear_crop.src_width         = (uint16_t)R_BCD_CAMERA_WIDTH;    /* The horizontal size (pixels) of input image. */
					param_resize_bilinear_crop.src_height        = (uint16_t)R_BCD_CAMERA_HEIGHT;   /* The vertical size (pixels) of input image. */

					param_resize_bilinear_crop.src_window_x      = c_cropped_x[ccn];      /* x position o crop window start. */
					param_resize_bilinear_crop.src_window_y      = c_cropped_y[ccn];      /* y position  of crop window start */
					param_resize_bilinear_crop.src_window_width  = c_cropped_width[ccn];  /* The horizontal size (pixels) of input image crop area. */
					param_resize_bilinear_crop.src_window_height = c_cropped_height[ccn]; /* The vertical size (pixels) of input image. crop area*/

					param_resize_bilinear_crop.dst_width        = (uint16_t)NN_IN_WIDTH;  /* The horizontal size (pixels) of output image. */
					param_resize_bilinear_crop.dst_height       = (uint16_t)NN_IN_HEIGHT; /* The vertical size (pixels) of output image. */


					/* Initialize variables to be used in termination judgment of the DRP library */
					drp_lib_status[TILE_0] = DRP_NOT_FINISH;

					/*********************/
					/* Start DRP Library */
					/*********************/
					ret_val = R_DK2_Start(drp_lib_id[TILE_0], (void *)&param_resize_bilinear_crop, sizeof(r_drp_resize_bilinear_crop_t));
					DRP_DRV_ASSERT(ret_val);

					/***************************************/
					/* Wait until DRP processing is finish */
					/***************************************/
					while (drp_lib_status[TILE_0] == DRP_NOT_FINISH);

					PerformSetEndTime(30);

					/* neural network */

					/* data normalization */
					PerformSetStartTime(31);
		            switch (NN_type)
		            {
						case TRANSLATOR :
		            	case TFLITE_FLOAT :
				            for (ih = 0 ; ih < NN_IN_HEIGHT; ih ++) {
								for (iw = 0 ; iw < NN_IN_WIDTH; iw ++) {
									/* data normalization for NN */
									frame_RAM_CCN_float[ih * NN_IN_WIDTH + iw] = (float)(frame_RAM_CCR[ccn][ih * NN_IN_WIDTH + iw] / 255.0);
								}
							}
		            		break;
		            	case TFLITE_PTQ :
		            	case TFLITE_QAT :
		            		/* quantization with uint8 input data => no data normalization required here */
							//frame_RAM_CCN_uint8[] = frame_RAM_CCR[ccn];

		            		/* quantization with int8 input data => uint8 -> int8 transformation required */
				            for (ih = 0 ; ih < NN_IN_HEIGHT; ih ++) {
								for (iw = 0 ; iw < NN_IN_WIDTH; iw ++) {
									frame_RAM_CCN_int8[ih * NN_IN_WIDTH + iw] = (int8_t)(frame_RAM_CCR[ccn][ih * NN_IN_WIDTH + iw] - 128);
								}
				            }
		            		break;
		            	case NO_NN :
		            		break;
		            }
					PerformSetEndTime(31);

					/*************/
					/* inference */
					/*************/

					// for power measurements only
					// (continuously repeats the inference of the first potential character)
					while (1) {
						fprintf(stdout," NOP \n");
						fflush(stdout);
						for (x2 = 0; x2 < 10; x2 ++)
						{
							for (y2 = 0; y2 < 1000; y2 ++)
							{
								for (ih = 0; ih < 1000; ih ++)
								{
									for (iw = 0; iw < 384; iw ++)
									{
										R_COMPILER_Nop();
									}
								}
							}
						}
						//R_OS_TaskSleep(30000);    //  NOP : wait 10 seconds (10000 milliseconds)

						fprintf(stdout," eAI translator \n");
						fflush(stdout);
						vTaskSuspendAll();
						{
							/* The scheduler is suspended here. */
							for (iw = 0 ; iw < 357 * 30 ; iw ++) {
								prediction  = dnn_compute_CNN_FE(&frame_RAM_CCN_float[0], dnn_compute_CNN_FE_error) ;                   // TRANSLATOR
							}
						}
						xTaskResumeAll();

						fprintf(stdout," NOP \n");
						fflush(stdout);
						for (x2 = 0; x2 < 10; x2 ++)
						{
							for (y2 = 0; y2 < 1000; y2 ++)
							{
								for (ih = 0; ih < 1000; ih ++)
								{
									for (iw = 0; iw < 384; iw ++)
									{
										R_COMPILER_Nop();
									}
								}
							}
						}
						//R_OS_TaskSleep(30000);    //  NOP : wait 10 seconds (10000 milliseconds)

						fprintf(stdout," TensorFlow Lite (float) \n");
						fflush(stdout);
						vTaskSuspendAll();
						{
							/* The scheduler is suspended here. */
							for (iw = 0 ; iw < 385 * 30 ; iw ++) {
								prediction  = tfl_float_dnn_compute(&frame_RAM_CCN_float[0]) ;         // TFLITE_FLOAT
							}
						}
						xTaskResumeAll();

						fprintf(stdout," NOP \n");
						fflush(stdout);
						for (x2 = 0; x2 < 10; x2 ++)
						{
							for (y2 = 0; y2 < 1000; y2 ++)
							{
								for (ih = 0; ih < 1000; ih ++)
								{
									for (iw = 0; iw < 384; iw ++)
									{
										R_COMPILER_Nop();
									}
								}
							}
						}
						//R_OS_TaskSleep(30000);    //  NOP : wait 10 seconds (10000 milliseconds)

						fprintf(stdout," TensorFlow Lite (post-training quantization) \n");
						fflush(stdout);
						vTaskSuspendAll();
						{
							/* The scheduler is suspended here. */
							for (iw = 0 ; iw < 233 * 30 ; iw ++) {
								// prediction  = tfl_PTQ_dnn_compute(&frame_RAM_CCN_float[0]) ;       // TFLITE_PTQ
								// prediction_uint8  = tfl_PTQ_dnn_compute(&frame_RAM_CCR[ccn][0]) ;  // TFLITE_PTQ (uint8)
								prediction_int8  = tfl_PTQ_dnn_compute(&frame_RAM_CCN_int8[0]) ;      // TFLITE_PTQ (int8)
							}
						}
						xTaskResumeAll();

						fprintf(stdout," NOP \n");
						fflush(stdout);
						for (x2 = 0; x2 < 10; x2 ++)
						{
							for (y2 = 0; y2 < 1000; y2 ++)
							{
								for (ih = 0; ih < 1000; ih ++)
								{
									for (iw = 0; iw < 384; iw ++)
									{
										R_COMPILER_Nop();
									}
								}
							}
						}
						//R_OS_TaskSleep(30000);    //  NOP : wait 10 seconds (10000 milliseconds)

						fprintf(stdout," TensorFlow Lite (quantization aware training) \n");
						fflush(stdout);
						vTaskSuspendAll();
						{
							/* The scheduler is suspended here. */
							for (iw = 0 ; iw < 250 * 30 ; iw ++) {
								// prediction_uint8  = tfl_QAT_dnn_compute(&frame_RAM_CCR[ccn][0]) ;  // TFLITE_QAT (uint8)
								prediction_int8  = tfl_QAT_dnn_compute(&frame_RAM_CCN_int8[0]) ;  // TFLITE_QAT (int8)
							}
						}
						xTaskResumeAll();
					}

					PerformSetStartTime(32);
		            switch (NN_type)
		            {
						case TRANSLATOR :
						    if (TRANSLATOR_ACTIVE) {
						    	prediction  = dnn_compute_CNN_FE(&frame_RAM_CCN_float[0], dnn_compute_CNN_FE_error) ;
						    }
							break;
		            	case TFLITE_FLOAT :
						    if (TFLITE_FLOAT_ACTIVE) {
						    	prediction  = tfl_float_dnn_compute(&frame_RAM_CCN_float[0]) ;
						    }
		            		break;
		            	case TFLITE_PTQ :
						    if (TFLITE_PTQ_ACTIVE) {
						    	// prediction        = tfl_PTQ_dnn_compute(&frame_RAM_CCN_float[0]) ;  // float
						    	// prediction_uint8  = tfl_PTQ_dnn_compute(&frame_RAM_CCR[ccn][0]) ;   // uint8
						    	prediction_int8   = tfl_PTQ_dnn_compute(&frame_RAM_CCN_int8[0]) ;   // int8
						    }
							break;
		            	case TFLITE_QAT :
						    if (TFLITE_QAT_ACTIVE) {
						    	// prediction_uint8  = tfl_QAT_dnn_compute(&frame_RAM_CCR[ccn][0]) ;     // uint8
						    	prediction_int8  = tfl_QAT_dnn_compute(&frame_RAM_CCN_int8[0]) ;     // int8
						    }
							break;
		            	case NO_NN :
		            		break;
		            }
					PerformSetEndTime(32);


					/* inference result preparation */
					PerformSetStartTime(33);
					switch (NN_type)
					{
						case TRANSLATOR :
						case TFLITE_FLOAT :
							predicted_accuracy[ccn]       = 0.0;
							for (ih = 0 ; ih < 39; ih ++)
							{
								if (prediction[ih] > predicted_accuracy[ccn] ){
									predicted_accuracy[ccn]   = prediction[ih];
									predicted_char_index[ccn] = (uint8_t)ih;
								}
							}
							break;
						case TFLITE_PTQ :
						case TFLITE_QAT :
							// predicted_accuracy_uint8[ccn]  = 0;
							// for (ih = 0 ; ih < 39; ih ++)
							// {
							// 	if (prediction_uint8[ih] > predicted_accuracy_uint8[ccn] ){
							//		predicted_accuracy_uint8[ccn] = prediction_uint8[ih];
							//		predicted_char_index[ccn]     = (uint8_t)ih;
							//	}
							// }
							predicted_accuracy_int8[ccn]  = -128;
							for (ih = 0 ; ih < 39; ih ++)
							{
								if (prediction_int8[ih] > predicted_accuracy_int8[ccn] ){
									predicted_accuracy_int8[ccn] = prediction_int8[ih];
									predicted_char_index[ccn]     = (uint8_t)ih;
								}
							}
							break;
						case NO_NN :
							predicted_accuracy[ccn]       = 0.0;
							break;
					}
					PerformSetEndTime(33);

					/* print char image and predition result for debugging */
				    if (SerialTerminal_on && SerialTerminal_show_char_image) {
						fprintf(stdout,"\nCNN image data :\n");
						fflush(stdout);
						for (ih = 0 ; ih < NN_IN_HEIGHT; ih ++)
						{
							for (iw = 0 ; iw < NN_IN_WIDTH; iw ++)
							{
					            switch (NN_type)
					            {
					            	case NO_NN :
									case TRANSLATOR :
					            	case TFLITE_FLOAT :
										fprintf(stdout," %3.0f", frame_RAM_CCN_float[ih * NN_IN_WIDTH + iw] * 100.0);
										break;
					            	case TFLITE_PTQ :
					            	case TFLITE_QAT :
										// fprintf(stdout," %3d", frame_RAM_CCR[ccn][ih * NN_IN_WIDTH + iw]);  // uint8
										// fprintf(stdout," %3d", frame_RAM_CCN_uint8[ih * NN_IN_WIDTH + iw]); // uint8
										fprintf(stdout," %4d", frame_RAM_CCN_int8[ih * NN_IN_WIDTH + iw]);  // int8
										break;
					            }
							}
							fprintf(stdout,"\n");
							fflush(stdout);
						}
			            switch (NN_type)
			            {
			            	case NO_NN :
			            	case TRANSLATOR :
			            	case TFLITE_FLOAT :
								fprintf(stdout,"prediction :\n");
								fprintf(stdout,"  A: %3.2f  B: %3.2f  C: %3.2f  D: %3.2f  E: %3.2f  F: %3.2f  G: %3.2f  H: %3.2f  I: %3.2f  J: %3.2f\n", prediction[0] , prediction[1] , prediction[2] , prediction[3] , prediction[4] , prediction[5] , prediction[6] , prediction[7] , prediction[8] , prediction[9] );
								fprintf(stdout,"  K: %3.2f  L: %3.2f  M: %3.2f  N: %3.2f  O: %3.2f  P: %3.2f  Q: %3.2f  R: %3.2f  S: %3.2f  T: %3.2f\n", prediction[10], prediction[11], prediction[12], prediction[13], prediction[14], prediction[15], prediction[16], prediction[17], prediction[18], prediction[19]);
								fprintf(stdout,"  U: %3.2f  V: %3.2f  W: %3.2f  X: %3.2f  Y: %3.2f  Z: %3.2f  0: %3.2f  1: %3.2f  2: %3.2f  3: %3.2f\n", prediction[20], prediction[21], prediction[22], prediction[23], prediction[24], prediction[25], prediction[26], prediction[27], prediction[28], prediction[29]);
								fprintf(stdout,"  4: %3.2f  5: %3.2f  6: %3.2f  7: %3.2f  8: %3.2f  9: %3.2f  ae:%3.2f  oe:%3.2f  ue:%3.2f\n",           prediction[30], prediction[31], prediction[32], prediction[33], prediction[34], prediction[35], prediction[36], prediction[37], prediction[38]);
								fprintf(stdout,"  => %s  (%3.2f)\n",  char_listTT[predicted_char_index[ccn]],  predicted_accuracy[ccn]);
								fflush(stdout);
								break;
			            	case TFLITE_PTQ :
			            	case TFLITE_QAT :
								fprintf(stdout,"prediction :\n");
								fprintf(stdout,"  A: %4d  B: %4d  C: %4d  D: %4d  E: %4d  F: %4d  G: %4d  H: %4d  I: %4d  J: %4d\n", prediction_int8[0] , prediction_int8[1] , prediction_int8[2] , prediction_int8[3] , prediction_int8[4] , prediction_int8[5] , prediction_int8[6] , prediction_int8[7] , prediction_int8[8] , prediction_int8[9] );
								fprintf(stdout,"  K: %4d  L: %4d  M: %4d  N: %4d  O: %4d  P: %4d  Q: %4d  R: %4d  S: %4d  T: %4d\n", prediction_int8[10], prediction_int8[11], prediction_int8[12], prediction_int8[13], prediction_int8[14], prediction_int8[15], prediction_int8[16], prediction_int8[17], prediction_int8[18], prediction_int8[19]);
								fprintf(stdout,"  U: %4d  V: %4d  W: %4d  X: %4d  Y: %4d  Z: %4d  0: %4d  1: %4d  2: %4d  3: %4d\n", prediction_int8[20], prediction_int8[21], prediction_int8[22], prediction_int8[23], prediction_int8[24], prediction_int8[25], prediction_int8[26], prediction_int8[27], prediction_int8[28], prediction_int8[29]);
								fprintf(stdout,"  4: %4d  5: %4d  6: %4d  7: %4d  8: %4d  9: %4d  ae:%4d  oe:%4d  ue:%4d\n",         prediction_int8[30], prediction_int8[31], prediction_int8[32], prediction_int8[33], prediction_int8[34], prediction_int8[35], prediction_int8[36], prediction_int8[37], prediction_int8[38]);
								fprintf(stdout,"  => %s  (%4d)\n",  char_listTT[predicted_char_index[ccn]],  predicted_accuracy_int8[ccn]);
								fflush(stdout);
								break;
			            }
				    }
				}

				/**********************/
				/* Unload DRP Library */
				/**********************/
				ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[TILE_0]);
				DRP_DRV_ASSERT(ret_val);

				/* Set end time of DRP processing */
				PerformSetEndTime(22);
			}

		} else {

			/*******************************************************/
			/* Wait until BinarizationAdaptive processing finished */
			/*******************************************************/
			while (drp_lib_status[TILE_0] == DRP_NOT_FINISH);  // BinarizationAdaptive

			/*******************************************/
			/* Unload DRP library BinarizationAdaptive */
			/*******************************************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[TILE_0]);
			DRP_DRV_ASSERT(ret_val);

			/******************************************/
			/* Unload DRP library find_countours_crop */
			/******************************************/
			ret_val = R_DK2_Unload(drp_lib_id[TILE_3], &drp_lib_id[TILE_3]);
			DRP_DRV_ASSERT(ret_val);
		}


        /* Set end time of overall process*/
        PerformSetEndTime(0);


        /******************************************/
        /* Prepare data to be shown on display    */
        /******************************************/

        if (mscn != FIND_CONTOURS_NUM + 2){
			if (memcmp(previous_predicted_char_index, predicted_char_index, amount_pot_char[mscn]) == 0 )
			{
				if (same_prediction_counter != 6) {
					same_prediction_counter++ ;
				}
			} else {
				same_prediction_counter = 0;
			}
			if (same_prediction_counter == (uint8_t)6 ) {
				prediction_color = GREEN;
			} else if (same_prediction_counter == (uint8_t)3 ) {
				prediction_color = YELLOW;
			} else {
				prediction_color = RED;
			}
		} else {
			same_prediction_counter = 0;
			prediction_color        = RED;
		}
		memcpy(previous_predicted_char_index, predicted_char_index, sizeof(predicted_char_index));


		/* check usage of push-button SW4  */
        if ( boardSwitchInfoStruct.SW4_IsModified ) {
        	boardSwitchInfoStruct.SW4_IsModified = 0 ;
            switch (display_mode)
            {
				case DISPLAY_PREDICTED_RESULT :
					display_mode          = DISPLAY_FILTER_MODE1;
					break;
            	case DISPLAY_FILTER_MODE1 :
            		display_mode          = DISPLAY_FILTER_MODE2;
            		break;
            	case DISPLAY_FILTER_MODE2 :
            		display_mode          = DISPLAY_FILTER_MODE3;
            		break;
            	case DISPLAY_FILTER_MODE3 :
            		display_mode          = DISPLAY_FILTER_MODE4;
            		break;
            	case DISPLAY_FILTER_MODE4 :
            		display_mode          = DISPLAY_FILTER_MODE5;
            		break;
            	case DISPLAY_FILTER_MODE5 :
            		display_mode          = DISPLAY_PREDICTED_RESULT;
            		break;
            }
		}

        /* Clear Graphics buffer*/
        R_BCD_LcdClearGraphicsBuffer();               /* Clear overlap buffer */

        /* display result */
        switch (display_mode)
        {
        	case DISPLAY_PREDICTED_RESULT :

	            switch (NN_type)
	            {
					case TRANSLATOR :
						sprintf((void *)&buf[0],"RENESAS");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"eAI-translator");
						R_BCD_LcdWriteString(&buf[0], 636, 34, DEEPSKYBLUE);
						break;
	            	case TFLITE_FLOAT :
						sprintf((void *)&buf[0],"TF Lite");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"float");
						R_BCD_LcdWriteString(&buf[0], 675, 34, DEEPSKYBLUE);
	            		break;
	            	case TFLITE_PTQ :
						sprintf((void *)&buf[0],"TF Lite");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"post-training");
						R_BCD_LcdWriteString(&buf[0], 637, 34, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"quantization");
						R_BCD_LcdWriteString(&buf[0], 640, 64, DEEPSKYBLUE);
	            		break;
	            	case TFLITE_QAT :
						sprintf((void *)&buf[0],"TF Lite");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"quant. aware");
						R_BCD_LcdWriteString(&buf[0], 637, 34, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"  training");
						R_BCD_LcdWriteString(&buf[0], 637, 64, DEEPSKYBLUE);
	            		break;
	            	case NO_NN :
	            		break;
	            }

	            if (mscn != FIND_CONTOURS_NUM + 2){

	        		x1     = cropped_x[mscn];
					y1     = cropped_y[mscn];
					length = cropped_width[mscn];
					height = cropped_height[mscn];

					color = GREEN;
					R_BCD_LcdWriteLine( x1          ,  y1          , (x1 + length),  y1          , color);
					R_BCD_LcdWriteLine((x1 + length),  y1          , (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          , (y1 + height), (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          ,  y1          ,  x1          , (y1 + height), color);

					for (ccn = 0; ccn < amount_pot_char[mscn]; ccn++)
					{
						for (ih = 0; ih < NN_IN_HEIGHT; ih ++)
						{
							for (iw = 0; iw < NN_IN_WIDTH; iw ++)
							{
								output_bufadr[(((uint16_t)ih + (uint16_t)(ccn + 3) * (uint16_t)NN_IN_HEIGHT) * (uint16_t)R_BCD_CAMERA_WIDTH  + (uint16_t)618 + (uint16_t)iw) * (uint16_t)4 + (uint16_t)1] = frame_RAM_CCR[ccn][(uint16_t)ih * (uint16_t)NN_IN_WIDTH + (uint16_t)iw];
								output_bufadr[(((uint16_t)ih + (uint16_t)(ccn + 3) * (uint16_t)NN_IN_HEIGHT) * (uint16_t)R_BCD_CAMERA_WIDTH  + (uint16_t)618 + (uint16_t)iw) * (uint16_t)4 + (uint16_t)2] = frame_RAM_CCR[ccn][(uint16_t)ih * (uint16_t)NN_IN_WIDTH + (uint16_t)iw];
								output_bufadr[(((uint16_t)ih + (uint16_t)(ccn + 3) * (uint16_t)NN_IN_HEIGHT) * (uint16_t)R_BCD_CAMERA_WIDTH  + (uint16_t)618 + (uint16_t)iw) * (uint16_t)4 + (uint16_t)3] = frame_RAM_CCR[ccn][(uint16_t)ih * (uint16_t)NN_IN_WIDTH + (uint16_t)iw];
							}
						}

			            switch (NN_type)
			            {
			            	case NO_NN :
			            	case TRANSLATOR :
			            	case TFLITE_FLOAT :
								sprintf((void *)&buf[0],"%2s   %5.4f", char_list[predicted_char_index[ccn]], predicted_accuracy[ccn]);
								R_BCD_LcdWriteString(&buf[0], 640, (uint32_t)((ccn +3) * 32 + 4), WHITE);
								break;
			            	case TFLITE_PTQ :
			            	case TFLITE_QAT :
								// sprintf((void *)&buf[0],"%2s   %3d/255", char_list[predicted_char_index[ccn]], predicted_accuracy_uint8[ccn]);
								sprintf((void *)&buf[0],"%2s  %4d/255", char_list[predicted_char_index[ccn]], predicted_accuracy_int8[ccn] + 128);
								R_BCD_LcdWriteString(&buf[0], 640, (uint32_t)((ccn +3) * 32 + 4), WHITE);
								break;
			            }

						sprintf((void *)&buf[0],"%s", char_list[predicted_char_index[ccn]]);
						R_BCD_LcdWriteString(&buf[0], (uint32_t)(650 + ccn * (12 + 2)), 400, prediction_color);

						x1     = c_cropped_x[ccn];
						y1     = c_cropped_y[ccn];
						length = c_cropped_width[ccn];
						height = c_cropped_height[ccn];

						color = RED;
						R_BCD_LcdWriteLine( x1          ,  y1          , (x1 + length),  y1          , color);
						R_BCD_LcdWriteLine((x1 + length),  y1          , (x1 + length), (y1 + height), color);
						R_BCD_LcdWriteLine( x1          , (y1 + height), (x1 + length), (y1 + height), color);
						R_BCD_LcdWriteLine( x1          ,  y1          ,  x1          , (y1 + height), color);
					}

				}

				sprintf((void *)&buf[0],"%6.2fms", PerformGetElapsedTime_us(0)/1000.0);
				R_BCD_LcdWriteString(&buf[0], 650, 440, YELLOW);

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (R_BCD_LY2_WIDTH * R_BCD_LY2_HEIGHT));

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(output_bufadr, ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) * 4));

				/* Clear the current capture state and enable the detection of the next capture completion */
				R_BCD_CameraClearCaptureStatus();

				/* Display overlay buffer written processing time */
				R_BCD_LcdSwapGraphicsBuffer();
				R_BCD_LcdSwapVideoBuffer();

				break;

        	case DISPLAY_FILTER_MODE1 :    /* display Gaussian Blur image */

				output_bufadr = R_BCD_LcdGetvisableVramAddress();    /* address of the video buffer which is currently being displayed */

        		for (ih = 0; ih < R_BCD_CAMERA_HEIGHT; ih ++)
        		{
        			for (iw = 0; iw < R_BCD_CAMERA_WIDTH; iw ++)
        			{
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 1] = frame_RAM_A[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 2] = frame_RAM_A[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 3] = frame_RAM_A[ih * R_BCD_CAMERA_WIDTH + iw];
        			}
        		}

				sprintf((void *)&buf[0],"Gaussian Blur");
				R_BCD_LcdWriteString(&buf[0],   2,   2, GREEN);

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (R_BCD_LY2_WIDTH * R_BCD_LY2_HEIGHT));

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(output_bufadr, ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) * 4));

				/* Clear the current capture state and enable the detection of the next capture completion */
				R_BCD_CameraClearCaptureStatus();

				/* Display overlay buffer written processing time */
				R_BCD_LcdSwapGraphicsBuffer();

        		break;

        	case DISPLAY_FILTER_MODE2 :    /* display Canny Hysteris image */

				output_bufadr = R_BCD_LcdGetvisableVramAddress();    /* address of the video buffer which is currently being displayed */

        		for (ih = 0; ih < R_BCD_CAMERA_HEIGHT; ih ++)
        		{
        			for (iw = 0; iw < R_BCD_CAMERA_WIDTH; iw ++)
        			{
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 1] = frame_RAM_C[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 2] = frame_RAM_C[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 3] = frame_RAM_C[ih * R_BCD_CAMERA_WIDTH + iw];
        			}
        		}

				sprintf((void *)&buf[0],"Canny Hysteresis");
				R_BCD_LcdWriteString(&buf[0],   2,   2, GREEN);

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (R_BCD_LY2_WIDTH * R_BCD_LY2_HEIGHT));

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(output_bufadr, ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) * 4));

				/* Clear the current capture state and enable the detection of the next capture completion */
				R_BCD_CameraClearCaptureStatus();

				/* Display overlay buffer written processing time */
				R_BCD_LcdSwapGraphicsBuffer();

        		break;

        	case DISPLAY_FILTER_MODE3 :    /* display Dilate image */

				output_bufadr = R_BCD_LcdGetvisableVramAddress();    /* address of the video buffer which is currently being displayed */

        		for (ih = 0; ih < R_BCD_CAMERA_HEIGHT; ih ++)
        		{
        			for (iw = 0; iw < R_BCD_CAMERA_WIDTH; iw ++)
        			{
           				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 1] = frame_RAM_D[ih * R_BCD_CAMERA_WIDTH + iw];
           				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 2] = frame_RAM_D[ih * R_BCD_CAMERA_WIDTH + iw];
           				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 3] = frame_RAM_D[ih * R_BCD_CAMERA_WIDTH + iw];
        			}
        		}

				color = GREEN;
				for (tile_no = 0; tile_no <= FIND_CONTOURS_NUM; tile_no++)
				{
					if (((contours_rect_adr[tile_no].x      < (uint32_t)(x1     + 10)) && (contours_rect_adr[tile_no].x      > (uint32_t)(x1     - 10)))
					 && ((contours_rect_adr[tile_no].y      < (uint32_t)(y1     + 10)) && (contours_rect_adr[tile_no].y      > (uint32_t)(y1     - 10)))
					 && ((contours_rect_adr[tile_no].width  < (uint32_t)(length + 10)) && (contours_rect_adr[tile_no].width  > (uint32_t)(length - 10)))
					 && ((contours_rect_adr[tile_no].height < (uint32_t)(height + 10)) && (contours_rect_adr[tile_no].height > (uint32_t)(height - 10)))) {
						continue;
					}

					x1     = contours_rect_adr[tile_no].x;
					y1     = contours_rect_adr[tile_no].y;
					length = contours_rect_adr[tile_no].width;
					height = contours_rect_adr[tile_no].height;

					if ((x1 == 0) && (y1 == 0) && (length == 0) && (height == 0)) {
						break;
					}

					R_BCD_LcdWriteLine( x1          ,  y1          , (x1 + length),  y1          , color);
					R_BCD_LcdWriteLine((x1 + length),  y1          , (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          , (y1 + height), (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          ,  y1          ,  x1          , (y1 + height), color);
				}

				sprintf((void *)&buf[0],"Dilate");
				R_BCD_LcdWriteString(&buf[0],   2,   2, GREEN);

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (R_BCD_LY2_WIDTH * R_BCD_LY2_HEIGHT));

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(output_bufadr, ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) * 4));

				/* Clear the current capture state and enable the detection of the next capture completion */
				R_BCD_CameraClearCaptureStatus();

				/* Display overlay buffer written processing time */
				R_BCD_LcdSwapGraphicsBuffer();

        		break;

        	case DISPLAY_FILTER_MODE4 :    /* display Dilate image plus pre-selected contours (potential number plates) */

				output_bufadr = R_BCD_LcdGetvisableVramAddress();    /* address of the video buffer which is currently being displayed */

        		for (ih = 0; ih < R_BCD_CAMERA_HEIGHT; ih ++)
        		{
        			for (iw = 0; iw < R_BCD_CAMERA_WIDTH; iw ++)
        			{
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 1] = frame_RAM_D[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 2] = frame_RAM_D[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 3] = frame_RAM_D[ih * R_BCD_CAMERA_WIDTH + iw];
        			}
        		}

				sprintf((void *)&buf[0],"Dilate");
				R_BCD_LcdWriteString(&buf[0],   2,   2, GREEN);

		        sprintf((void *)&buf[0],"amount of potential number plate : %d", amount_pot_obj);
				R_BCD_LcdWriteString(&buf[0], 2, 32, GREEN);

				if (mscn != FIND_CONTOURS_NUM + 2){
					sprintf((void *)&buf[0],"most significant                 : %d", mscn);
					R_BCD_LcdWriteString(&buf[0], 2, 62, GREEN);
				} else {
					sprintf((void *)&buf[0],"most significant                 : none");
					R_BCD_LcdWriteString(&buf[0], 2, 62, GREEN);
				}

        		for (ih = 0; (ih < amount_pot_obj) && (ih < 12) ; ih ++)
        		{
			        sprintf((void *)&buf[0],"[%2d]: X:%3d  Y:%3d  W:%3d  Ht:%3d  # pot char:%3d",
			        		pot_obj_rect_number[ih],
			        		contours_rect_adr[pot_obj_rect_number[ih]].x,
							contours_rect_adr[pot_obj_rect_number[ih]].y,
							contours_rect_adr[pot_obj_rect_number[ih]].width,
							contours_rect_adr[pot_obj_rect_number[ih]].height,
							amount_pot_char[ih]);
			        R_BCD_LcdWriteString(&buf[0], 2, (uint32_t)(92 + (ih * 30)), GREEN);
        		}

				color = GREEN;
				for (cn = 0; cn < amount_pot_obj; cn++)
				{
					/* cropped object contour coordinates */
					x1     = cropped_x[cn];
					y1     = cropped_y[cn];
					length = cropped_width[cn];
					height = cropped_height[cn];

					R_BCD_LcdWriteLine( x1          ,  y1          , (x1 + length),  y1          , color);
					R_BCD_LcdWriteLine((x1 + length),  y1          , (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          , (y1 + height), (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          ,  y1          ,  x1          , (y1 + height), color);
				}

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (R_BCD_LY2_WIDTH * R_BCD_LY2_HEIGHT));

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(output_bufadr, ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) * 4));

				/* Clear the current capture state and enable the detection of the next capture completion */
				R_BCD_CameraClearCaptureStatus();

				/* Display overlay buffer written processing time */
				R_BCD_LcdSwapGraphicsBuffer();
        		break;

        	case DISPLAY_FILTER_MODE5 :    /* display BinarizationAdaptive image plus character contours and classification result  */

	            switch (NN_type)
	            {
					case TRANSLATOR :
						sprintf((void *)&buf[0],"RENESAS");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"eAI-translator");
						R_BCD_LcdWriteString(&buf[0], 636, 34, DEEPSKYBLUE);
						break;
	            	case TFLITE_FLOAT :
						sprintf((void *)&buf[0],"TF Lite");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"float");
						R_BCD_LcdWriteString(&buf[0], 675, 34, DEEPSKYBLUE);
	            		break;
	            	case TFLITE_PTQ :
						sprintf((void *)&buf[0],"TF Lite");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"post-training");
						R_BCD_LcdWriteString(&buf[0], 637, 34, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"quantization");
						R_BCD_LcdWriteString(&buf[0], 640, 64, DEEPSKYBLUE);
	            		break;
	            	case TFLITE_QAT :
						sprintf((void *)&buf[0],"TF Lite");
						R_BCD_LcdWriteString(&buf[0], 670,  4, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"quantization");
						R_BCD_LcdWriteString(&buf[0], 637, 34, DEEPSKYBLUE);
						sprintf((void *)&buf[0],"aware train");
						R_BCD_LcdWriteString(&buf[0], 637, 64, DEEPSKYBLUE);
	            		break;
	            	case NO_NN :
	            		break;
	            }

        		output_bufadr = R_BCD_LcdGetvisableVramAddress();    /* address of the video buffer which is currently being displayed */

        		for (ih = 0; ih < R_BCD_CAMERA_HEIGHT; ih ++)
        		{
        			for (iw = 0; iw < R_BCD_CAMERA_WIDTH; iw ++)
        			{
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 1] = frame_RAM_E[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 2] = frame_RAM_E[ih * R_BCD_CAMERA_WIDTH + iw];
        				output_bufadr[( ih * R_BCD_CAMERA_WIDTH + iw) * 4 + 3] = frame_RAM_E[ih * R_BCD_CAMERA_WIDTH + iw];
        			}
        		}

				if (mscn != FIND_CONTOURS_NUM + 2){

	        		x1     = cropped_x[mscn];
					y1     = cropped_y[mscn];
					length = cropped_width[mscn];
					height = cropped_height[mscn];

					color = GREEN;
					R_BCD_LcdWriteLine( x1          ,  y1          , (x1 + length),  y1          , color);
					R_BCD_LcdWriteLine((x1 + length),  y1          , (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          , (y1 + height), (x1 + length), (y1 + height), color);
					R_BCD_LcdWriteLine( x1          ,  y1          ,  x1          , (y1 + height), color);

					for (ccn = 0; ccn < amount_pot_char[mscn]; ccn++)
					{
						x1     = c_cropped_x[ccn];
						y1     = c_cropped_y[ccn];
						length = c_cropped_width[ccn];
						height = c_cropped_height[ccn];

						color = RED;
						R_BCD_LcdWriteLine( x1          ,  y1          , (x1 + length),  y1          , color);
						R_BCD_LcdWriteLine((x1 + length),  y1          , (x1 + length), (y1 + height), color);
						R_BCD_LcdWriteLine( x1          , (y1 + height), (x1 + length), (y1 + height), color);
						R_BCD_LcdWriteLine( x1          ,  y1          ,  x1          , (y1 + height), color);

			            switch (NN_type)
			            {
			            	case NO_NN :
							case TRANSLATOR :
			            	case TFLITE_FLOAT :
								sprintf((void *)&buf[0],"%2s   %5.4f", char_list[predicted_char_index[ccn]], predicted_accuracy[ccn]);
								R_BCD_LcdWriteString(&buf[0], 640, (uint32_t)((ccn +3) * 32 + 4), WHITE);
								break;
			            	case TFLITE_PTQ :
			            	case TFLITE_QAT :
								// sprintf((void *)&buf[0],"%2s   %3d/255", char_list[predicted_char_index[ccn]], predicted_accuracy_uint8[ccn]);
								sprintf((void *)&buf[0],"%2s  %4d/255", char_list[predicted_char_index[ccn]], predicted_accuracy_int8[ccn] + 128);
								R_BCD_LcdWriteString(&buf[0], 640, (uint32_t)((ccn +3) * 32 + 4), WHITE);
								break;
			            }

						sprintf((void *)&buf[0],"%s", char_list[predicted_char_index[ccn]]);
						R_BCD_LcdWriteString(&buf[0], (uint32_t)(650 + ccn * (12 + 2)), 400, prediction_color);
					}

				}

				sprintf((void *)&buf[0],"BinarizationAdaptive");
				R_BCD_LcdWriteString(&buf[0],   2,   2, GREEN);

				sprintf((void *)&buf[0],"%6.2fms", PerformGetElapsedTime_us(0)/1000.0);
				R_BCD_LcdWriteString(&buf[0], 650, 440, YELLOW);

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (R_BCD_LY2_WIDTH * R_BCD_LY2_HEIGHT));

				/* Write the data(buf) on the cache to physical memory */
				R_CACHE_L1DataCleanLine(output_bufadr, ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) * 4));

				/* Clear the current capture state and enable the detection of the next capture completion */
				R_BCD_CameraClearCaptureStatus();

				/* Display overlay buffer written processing time */
				R_BCD_LcdSwapGraphicsBuffer();

        		break;

        }

		/* print result and TATs */
	    if (SerialTerminal_on) {
            switch (NN_type) {
				case TRANSLATOR :
		           	fprintf(stdout,"\nRenesas eAI translator : \n");
					break;
				case TFLITE_FLOAT :
		           	fprintf(stdout,"\nTensorflow Lite for MCU (float) : \n");
					break;
				case TFLITE_PTQ :
		           	fprintf(stdout,"\nTensorflow Lite for MCU (post training quantization) : \n");
					break;
				case TFLITE_QAT :
		           	fprintf(stdout,"\nTensorflow Lite for MCU (quantization aware training) : \n");
					break;
				case NO_NN :
		           	fprintf(stdout,"\no neural network active : \n");
					break;
            }
			fprintf(stdout,"   predicted number plate : ");
			if (mscn != FIND_CONTOURS_NUM + 2){
				for (ccn = 0; ccn < amount_pot_char[mscn]; ccn++)
				{
					fprintf(stdout,"%s", char_listTT[predicted_char_index[ccn]]);
				}
			}
			fprintf(stdout,"\n");
	    }
	    if (SerialTerminal_on && SerialTerminal_show_TAT) {
			fprintf(stdout,"\nTATs :\n");
			fprintf(stdout,"find all contours    :\n");
			fprintf(stdout,"   bayer2lrgb_color_correction                  : %8.3f ms\n", PerformGetElapsedTime_us(1)/1000.0);
			fprintf(stdout,"   lrgb2l_gaussian_blur                         : %8.3f ms\n", PerformGetElapsedTime_us(2)/1000.0);
			fprintf(stdout,"   Canny Calculate                              : %8.3f ms\n", PerformGetElapsedTime_us(3)/1000.0);
			fprintf(stdout,"   Canny Hysteresis                             : %8.3f ms\n", PerformGetElapsedTime_us(4)/1000.0);
			fprintf(stdout,"   Dilate                                       : %8.3f ms\n", PerformGetElapsedTime_us(5)/1000.0);
			fprintf(stdout,"   FindContours_crop                            : %8.3f ms\n", PerformGetElapsedTime_us(6)/1000.0);
			fprintf(stdout,"find char contours in one number plate:\n");
			fprintf(stdout,"   FindContours_crop (%2d times)                 : %8.3f ms\n", amount_pot_obj, PerformGetElapsedTime_us(20)/1000.0);
			fprintf(stdout,"   resize_bilinear_crop & inference (%2d chars)  : %8.3f ms\n", amount_pot_char[mscn], PerformGetElapsedTime_us(22)/1000.0);
			fprintf(stdout,"\n");
			fprintf(stdout,"Overall                                         : %8.3f ms\n", PerformGetElapsedTime_us(0)/1000.0);
			fprintf(stdout,"\n");
			fprintf(stdout,"resize_bilinear_crop (1 char)                   : %8.3f ms\n", PerformGetElapsedTime_us(30)/1000.0);
			fprintf(stdout,"\n");
			fprintf(stdout,"CNN processing :\n");
			fprintf(stdout,"data normalization (1 char)                     : %8.3f ms\n", PerformGetElapsedTime_us(31)/1000.0);
			if (NN_type == TRANSLATOR) {
				fprintf(stdout,"   convolution_without_pad                      : %8.3f ms\n", PerformGetElapsedTime_us(60)/1000.0);
				fprintf(stdout,"   relu                                         : %8.3f ms\n", PerformGetElapsedTime_us(61)/1000.0);
				fprintf(stdout,"   pooling_without_pad                          : %8.3f ms\n", PerformGetElapsedTime_us(62)/1000.0);
				fprintf(stdout,"   convolution_without_pad                      : %8.3f ms\n", PerformGetElapsedTime_us(63)/1000.0);
				fprintf(stdout,"   relu                                         : %8.3f ms\n", PerformGetElapsedTime_us(64)/1000.0);
				fprintf(stdout,"   pooling_without_pad                          : %8.3f ms\n", PerformGetElapsedTime_us(65)/1000.0);
				fprintf(stdout,"   innerproduct                                 : %8.3f ms\n", PerformGetElapsedTime_us(66)/1000.0);
				fprintf(stdout,"   relu                                         : %8.3f ms\n", PerformGetElapsedTime_us(67)/1000.0);
				fprintf(stdout,"   innerproduct                                 : %8.3f ms\n", PerformGetElapsedTime_us(68)/1000.0);
				fprintf(stdout,"   softmax                                      : %8.3f ms\n", PerformGetElapsedTime_us(69)/1000.0);
				fprintf(stdout,"                                                 -------------\n");
			}
			fprintf(stdout,"inference (1 char)                              : %8.3f ms\n", PerformGetElapsedTime_us(32)/1000.0);
			fprintf(stdout,"inference result preparation (1 char)           : %8.3f ms\n", PerformGetElapsedTime_us(33)/1000.0);
			fprintf(stdout,"\n\n");
			fflush(stdout);
		}

        /* ==== LED blink ==== */
        gs_main_led_flg ^= 1;

        if (MAIN_PRV_LED_ON == gs_main_led_flg) {
        	if (prediction_color == GREEN) {
        		cpuBoardLedGreen ( 1 ) ;  // green led on  (The green LED looks yellow in case red is on, too)
        	} else {
                cpuBoardLedRed   ( 1 ) ;  // red led on    (The green LED looks yellow in case red is on, too)
        	}
        } else {
            cpuBoardLedRed   ( 0 ) ;  // red led off
            cpuBoardLedGreen ( 0 ) ;  // green led off
        }

    }

sample_finish:
    return;
}

/*******************************************************************************
* End of function sample_main
*******************************************************************************/


/* End of File */
