/*
 * demoBoardKeys.c
 *
 *  Created on: 24.11.2020
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

#include "subBoardKeysTdef.h"

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

switchBoardType boardSwitchInfoStruct  = { 	0,0,1,        // SW2_ModeCnt, SW2_IsModified, SW2_ModeCntMax
											0,0,1,        // SW3_ModeCnt, SW3_IsModified, SW3_ModeCntMax
                                         };     

/******************************************************************************
 local global variables
 ******************************************************************************/

static int_t SW2_lastKey     = 1 ;
static int_t SW3_lastKey     = 1 ;

/**********************************************************************************************************************
 * Function Name: initSW2gpioRead
 * Description  : initialization for SW2
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void initSW2gpioRead ( void ) {
	/* SW2 Readout setting */
	PORTD.PMR.BIT.PMR6 = 1;
	PORTD.PMR.BIT.PMR6 = 0; /* SW2 peripheral function */
	PORTD.PDR.BIT.PDR6 = 2; /* SW2  : input  */

	PORTD.PIDR.BIT.PIDR6;   /* Dummy read */
}

/**********************************************************************************************************************
 * Function Name: initSW3gpioRead
 * Description  : initialization for SW3
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void initSW3gpioRead ( void ) {
	/* SW3 Readout setting */
	PORTD.PMR.BIT.PMR7 = 1;
	PORTD.PMR.BIT.PMR7 = 0; /* SW3 peripheral function */
	PORTD.PDR.BIT.PDR7 = 2; /* SW3  : input  */

	PORTD.PIDR.BIT.PIDR7;   /* Dummy read */
}

/**********************************************************************************************************************
 * Function Name: readSW2Switch
 * Description  : read port bit data PORTJ.PIDR.BIT.PIDR1
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void readSW2Switch ( void ) {
	uint8_t data = PORTD.PIDR.BIT.PIDR6;

	if ( ! boardSwitchInfoStruct.SW2_IsModified && data == 0 ) {
		if ( data != SW2_lastKey ) {
			if (boardSwitchInfoStruct.SW2_ModeCnt < boardSwitchInfoStruct.SW2_ModeCntMax ) {
				boardSwitchInfoStruct.SW2_ModeCnt++ ;
			} else {
				boardSwitchInfoStruct.SW2_ModeCnt = 0 ;
			}
			boardSwitchInfoStruct.SW2_IsModified = 1;
			SW2_lastKey = data ;
		}
	}
	if ( ! boardSwitchInfoStruct.SW2_IsModified ) {
		SW2_lastKey = data ;
	}
	return;
}

/**********************************************************************************************************************
 * Function Name: readSW3Switch
 * Description  : read port bit data PORTD.PIDR.BIT.PIDR7
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

static void readSW3Switch ( void ) {
	uint8_t data = PORTD.PIDR.BIT.PIDR7;

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
 * Description  : initialization of AD converter, SW2, SW3 and interrupts
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

void initBoardSwitch ( void ) {
	boardSwitchInfoStruct  = (switchBoardType){ 0,0,1,        // SW2_ModeCnt, SW2_IsModified, SW2_ModeCntMax
												0,0,1,        // SW3_ModeCnt, SW3_IsModified, SW3_ModeCntMax
											  };       

	initSW2gpioRead ( ) ;
	initSW3gpioRead ( ) ;
}

/**********************************************************************************************************************
 * Function Name: readBoardSwitch
 * Description  :  
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

void readBoardSwitch ( void ) {
	readSW2Switch (  ) ; // read SW2 by pin polling
	readSW3Switch (  ) ; // read SW3 by pin polling
}

