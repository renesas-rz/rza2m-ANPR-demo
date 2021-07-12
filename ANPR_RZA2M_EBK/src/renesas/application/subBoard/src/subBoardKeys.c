/*
 * demoBoardKeys.c
 *
 *  Created on: 29.01.2020
 *      Author: a5050305
 */

/******************************************************************************
Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine.h"

#include "compiler_settings.h"

#include "r_adc_drv_api.h"

#include "subBoardKeys.h"
#include "subBoardKeysTdef.h"

/******************************************************************************
Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

switchBoardType boardSwitchInfoStruct  = { 	0,0,1,        // SW2_ModeCnt, SW2_IsModified, SW2_ModeCntMax
											0,0,1,        // SW3_ModeCnt, SW3_IsModified, SW3_ModeCntMax
											0,0,1,        // SW4_ModeCnt, SW4_IsModified, SW4_ModeCntMax
											0,0,1 };      // SW5_ModeCnt, SW5_IsModified, SW5_ModeCntMax

/******************************************************************************
local global variables
 ******************************************************************************/

static int_t s_adc_handle = -1 ;
static st_r_drv_adc_config_t adc_config;

static int_t SW3_lastKey     = 1 ;
static int_t SW4_lastKey     = 1 ;
static int_t SW5_lastKey     = 1 ;

static uint16_t channel_6_conversionPrev = 0 ;

/******************************************************************************
 * Function Name: nmi_callback
 * Description  : callback for nmi ISR to handle switch press
 * Arguments    : .
 * Return Value : .
 *****************************************************************************/
static void nmi_callback(uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

	if ( ! boardSwitchInfoStruct.SW2_IsModified ) {
		if (boardSwitchInfoStruct.SW2_ModeCnt < boardSwitchInfoStruct.SW2_ModeCntMax ) {
			boardSwitchInfoStruct.SW2_ModeCnt++ ;
		} else {
			boardSwitchInfoStruct.SW2_ModeCnt = 0 ;
		}
		boardSwitchInfoStruct.SW2_IsModified = 1;
	}

}


/******************************************************************************
 * Function Name: registerNmiInterrupt
 * Description  : register the nmi interrupt function
 * Arguments    : .
 * Return Value : .
 *****************************************************************************/
static void registerNmiInterrupt ( void ) {
	/* do something with the nmi interrut SW2 */
	/* provide an isr function for nmi        */
	R_INTC_RegistIntFunc(INTC_VID_NMI, nmi_callback);
}


/**********************************************************************************************************************
 * Function Name: initSW3gpioRead
 * Description  : initialization for SW3
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void initSW3gpioRead ( void ) {
	/* SW3 Readout setting */
	PORTJ.PMR.BIT.PMR1 = 1;
	PORTJ.PMR.BIT.PMR1 = 0; /* SW3 peripheral function */
	PORTJ.PDR.BIT.PDR1 = 2; /* SW-3  : input  */

	PORTJ.PIDR.BIT.PIDR1;   /* Dummy read */
}

/**********************************************************************************************************************
 * Function Name: initADC4swRead
 * Description  : initialization of AD converter
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void initADC4swRead ( void ) {
	int_t ret_val;
	e_r_drv_adc_channel_t channel;

	s_adc_handle = open(DEVICE_INDENTIFIER "adc", O_RDWR);

	/* on error */
	if (s_adc_handle < 0)
	{
		/* stop execution */
		while (1)
		{
			R_COMPILER_Nop();
		}
	}

	/* set up an ADC configuration */
	adc_config.config.alignment          = ADC_ALIGN_RIGHT;
	adc_config.config.interrupt_priority = INTC_PRIORITY_20;
	adc_config.config.resolution         = ADC_RESOLUTION_12;
	adc_config.config.scan_mode          = ADC_SCAN_MODE_SINGLE;
	adc_config.config.p_adcCallback      = NULL;

	/* set the per-channel configuration to enable channel 6 (AN006) only */
	for (channel = ADC_CHANNEL_AN000; channel < ADC_CHANNELS; channel++)
	{
		adc_config.config.channels[channel].sample_time = 0;
		adc_config.config.channels[channel].trigger_source = ADC_TRIGGER_DISABLED;
	}

	adc_config.config.channels[ADC_CHANNEL_AN006].sample_time = 50;
	adc_config.config.channels[ADC_CHANNEL_AN006].trigger_source = ADC_TRIGGER_NORMAL;

	/* call control */
	ret_val = control(s_adc_handle, CTL_ADC_SET_CONFIGURATION, &adc_config);

	/* on error */
	if (DRV_SUCCESS != ret_val)
	{
		/* stop execution */
		while (1)
		{
			R_COMPILER_Nop();
		}
	}
}


/**********************************************************************************************************************
 * Function Name: readAdcSwitch
 * Description  : read adc converter values of channel 6
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void readAdcSwitch ( void ) {
	static uint8_t buffer[2];
	int_t          count;
	uint16_t       channel_6_conversion;
	int_t sw4= -1, sw5= -1 ;
	/* call control to trigger the ADC conversion */
	/* no callback is set, so this will block until the conversion has been completed (a microsecond or so) */
	control(s_adc_handle, CTL_ADC_SOFTWARE_TRIGGER, NULL);

	count = read(s_adc_handle, buffer, 2);
	//printf("count %d  %d\n",count,s_adc_handle) ;
	/* if we successfully read the 2 bytes of conversion data for AN006 */
	if (2 == count) {
		/* data is read LSB first */
		channel_6_conversion = buffer[1];
		channel_6_conversion = (uint16_t)(channel_6_conversion << 8u);
		channel_6_conversion = (uint16_t)(channel_6_conversion | buffer[0]);
		if ( abs(channel_6_conversionPrev - channel_6_conversion)> 5 ) { // processing only if there is a small difference in conversion
			channel_6_conversionPrev = channel_6_conversion ;
			return ;
		}
		channel_6_conversionPrev = channel_6_conversion ;

		//printf("ADC AN006: 0x%X\r\n", channel_6_conversion);

		/* SW4  SW5  Value   switch point
           Off  Off  0xFFF
                              0xBFA
           Off  On   0x7F6
                              0x5EC
           On   Off  0x3E3
                              0x381
           On   On   0x31F
		 */
		if ( channel_6_conversion > 0xBFA ) {
			sw4 = 1 ;
			sw5 = 1 ;
		}

		if ( channel_6_conversion < 0xBFA && channel_6_conversion > 0x5EC ) {
			sw4 = 1 ;
			sw5 = 0 ;
		}

		if ( channel_6_conversion < 0x5EC && channel_6_conversion > 0x381 ) {
			sw4 = 0 ;
			sw5 = 1 ;
		}

		if ( channel_6_conversion < 0x381 ) {
			sw4 = 0 ;
			sw5 = 0 ;
		}

		// look on the keys in detail

		// sw4
		if ( ! boardSwitchInfoStruct.SW4_IsModified && sw4 == 0 ) {
			if ( sw4 != SW4_lastKey ) {
				if (boardSwitchInfoStruct.SW4_ModeCnt < boardSwitchInfoStruct.SW4_ModeCntMax ) {
					boardSwitchInfoStruct.SW4_ModeCnt++ ;
				} else {
					boardSwitchInfoStruct.SW4_ModeCnt = 0 ;
				}
				boardSwitchInfoStruct.SW4_IsModified = 1;
				SW4_lastKey = sw4 ;
			}
		}
		if ( ! boardSwitchInfoStruct.SW4_IsModified ) {
			SW4_lastKey = sw4 ;
		}

		// sw5
		if ( ! boardSwitchInfoStruct.SW5_IsModified && sw5 == 0 ) {
			if ( sw5 != SW5_lastKey ) {
				if (boardSwitchInfoStruct.SW5_ModeCnt < boardSwitchInfoStruct.SW5_ModeCntMax ) {
					boardSwitchInfoStruct.SW5_ModeCnt++ ;
				} else {
					boardSwitchInfoStruct.SW5_ModeCnt = 0 ;
				}
				boardSwitchInfoStruct.SW5_IsModified = 1;
				SW5_lastKey = sw5 ;
			}
		}
		if ( ! boardSwitchInfoStruct.SW5_IsModified ) {
			SW5_lastKey = sw5 ;
		}

	}
}

/**********************************************************************************************************************
 * Function Name: readSW3Switch
 * Description  : read port bit data PORTJ.PIDR.BIT.PIDR1
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void readSW3Switch ( void ) {
	uint8_t data = PORTJ.PIDR.BIT.PIDR1;

	if ( ! boardSwitchInfoStruct.SW3_IsModified && data == 0 ) {
		if ( data != SW3_lastKey ) {
			if (boardSwitchInfoStruct.SW3_ModeCnt < boardSwitchInfoStruct.SW3_ModeCntMax ) {
				boardSwitchInfoStruct.SW3_ModeCnt++ ;
			} else {
				boardSwitchInfoStruct.SW3_ModeCnt = 0 ;
			}
			boardSwitchInfoStruct.SW3_IsModified = 1;
			SW3_lastKey = data ;
		}
	}
	if ( ! boardSwitchInfoStruct.SW3_IsModified ) {
		SW3_lastKey = data ;
	}
	return;
}


/**********************************************************************************************************************
 * global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: initBoardSwitch
 * Description  : initialization of AD converter, SW3 and interrupts
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

void initBoardSwitch ( void ) {
	boardSwitchInfoStruct  = (switchBoardType){ 0,0,1,        // SW2_ModeCnt, SW2_IsModified, SW2_ModeCntMax
												0,0,1,        // SW3_ModeCnt, SW3_IsModified, SW3_ModeCntMax
												0,0,1,        // SW4_ModeCnt, SW4_IsModified, SW4_ModeCntMax
												0,0,1 };      // SW5_ModeCnt, SW5_IsModified, SW5_ModeCntMax
	initADC4swRead ( ) ;
	initSW3gpioRead ( ) ;
	registerNmiInterrupt ( ) ;

}


/**********************************************************************************************************************
 * Function Name: readBoardSwitch
 * Description  : read ad converter values
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/
void readBoardSwitch ( void ) {
	readAdcSwitch (  ) ; // read the adc switches by voltage levels SW4, SW5
	readSW3Switch (  ) ; // read sw3 by pin polling
						 // sw2 is nmi interrupt controlled
}

