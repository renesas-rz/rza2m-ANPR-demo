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
/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include <fcntl.h>

#include "r_typedefs.h"
#include "iodefine.h"

#include "r_bcd_ae.h"
#include "r_bcd_camera.h"
#include "draw.h"
#include "r_mmu_drv_api.h"
#include "r_riic_drv_api.h"
#include "r_devlink_wrapper.h"
#include "r_riic_drv_sc_cfg.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/


#define ANA

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
static uint8_t gain_set_count;

extern void R_BCD_gain_setting(uint32_t);
extern void R_BCD_gain_table_setting(uint32_t);
extern void R_BCD_shutter_setting(uint32_t);

/*******************************************************************************
Private global variables and functions
*******************************************************************************/

/******************************************************************************
* Function Name: R_BCD_AeInit
* Description  : Initialize AE
* Arguments    : pAeSet
*              :   AE setting struct address
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeInit(r_bcd_ae_setting_t* pAeSet)
{
    uint16_t mipi_set;

    gain_set_count = 0;
    pAeSet->mode               = R_BCD_AE_MODE_INIT;
    pAeSet->brightness         = 0;
    pAeSet->brightness_old     = 0;
	pAeSet->adapted            = 0 ;
	pAeSet->outOfRange         = 0 ;
    pAeSet->shutter_speed      = R_BCD_AE_S_SPEED_DEFAULT;
    pAeSet->global_gain        = R_BCD_AE_G_GAIN_DEFAULT; //R_BCD_AE_G_GAIN_DEFAULT;

    (void)R_BCD_shutter_setting(pAeSet->shutter_speed) ;
    (void)R_BCD_AeSetAnalogGain(pAeSet, pAeSet->global_gain);

    return R_BCD_AE_MODE_INIT;
}

/******************************************************************************
* Function Name: R_BCD_AeStop
* Description  : Stop AE
* Arguments    : pAeSet
*              :   AE setting struct address
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeStop(r_bcd_ae_setting_t* pAeSet)
{
    uint16_t mipi_set;

    gain_set_count = 0;
    pAeSet->mode           = R_BCD_AE_MODE_STOP;
    pAeSet->shutter_speed  = R_BCD_AE_S_SPEED_DEFAULT;
    pAeSet->global_gain    = R_BCD_AE_G_GAIN_DEFAULT; //R_BCD_AE_G_GAIN_DEFAULT;
    pAeSet->brightness_old = 0;
    (void)R_BCD_shutter_setting(pAeSet->shutter_speed) ;
    (void)R_BCD_AeSetAnalogGain(pAeSet, pAeSet->global_gain);

    return R_BCD_AE_MODE_STOP;
}

/******************************************************************************
* Function Name: R_BCD_AeStart
* Description  : Start AE
* Arguments    : pAeSet
*              :   AE setting struct address
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeStart(r_bcd_ae_setting_t* pAeSet)
{
    pAeSet->mode           = R_BCD_AE_MODE_INIT;
    (void)R_BCD_AeSetAnalogGain(pAeSet, pAeSet->global_gain);

    return R_BCD_AE_MODE_INIT;
}

/******************************************************************************
* Function Name: R_BCD_AeGetMode
* Description  : Get present AE mode 
* Arguments    : pAeSet
*              :   AE setting struct address
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeGetMode(r_bcd_ae_setting_t* pAeSet)
{
    return pAeSet->mode;
}

/******************************************************************************
* Function Name: R_BCD_AeMakeGammaTable
* Description  : Make gamma table
* Arguments    : pAeSet
*              :   AE setting struct address
*              : gamma
*              :   Gamma value
*              : pTable
*              :   Gamma table address
* Return Value : -
******************************************************************************/
void    R_BCD_AeMakeGammaTable(r_bcd_ae_setting_t* pAeSet, double gamma, uint8_t* pTable)
{
    int32_t i;
    
    pAeSet->brightness = 0;
    if (pAeSet->mode != R_BCD_AE_MODE_STOP) {
        pAeSet->mode = R_BCD_AE_MODE_INIT;
    }
    for( i = 0; i < 256; i++ ) {
        pTable[i] = (uint8_t)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);
        if ((pTable[i] >= BRIGHTNESS_SET) && (pAeSet->brightness == 0)) {
            pAeSet->brightness = (uint16_t)i;
        }
    }
    return;
}

/******************************************************************************
* Function Name: R_BCD_AeRunAutoExpousure
* Description  : Set average luminance for AE 
* Arguments    : pAeSet
*              :   AE setting struct address
*              : ave_lum
*              :   Average luminance
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeRunAutoExpousure(r_bcd_ae_setting_t* pAeSet, uint16_t ave_lum)
{

	uint16_t  shutterSpeedOld =  pAeSet-> shutter_speed ;
	uint16_t  globalGainOld   =  pAeSet-> global_gain ;
    uint8_t   outOfRange      =  0 ;

    if ((ave_lum <= BRIGHTNESS_MAX) && (pAeSet->mode != R_BCD_AE_MODE_STOP)) {

        uint8_t nError;
        
        if (ave_lum > (pAeSet->brightness * (100 + BRIGHTNESS_TH_HIGH)) / 100)       //OverEposed
        {
            outOfRange = 1;
            if (pAeSet->global_gain == 0)
            {
                nError = ave_lum - pAeSet->brightness;
                if (nError > (0.45*pAeSet->brightness)) //Coarse decrease
                {
	                pAeSet->shutter_speed = (uint16_t)(((((uint32_t)pAeSet->shutter_speed * pAeSet->brightness) / ave_lum) * 2 + pAeSet->shutter_speed) / 3);
                }
                else
                {
                    if (pAeSet->shutter_speed > R_BCD_AE_S_SPEED_GAIN) {
                        pAeSet->shutter_speed = pAeSet->shutter_speed - R_BCD_AE_S_SPEED_GAIN;
                    } else {
                        pAeSet->shutter_speed = pAeSet->shutter_speed = 0;
                    }
                }

                if (pAeSet->shutter_speed < R_BCD_AE_S_SPEED_MIN)
                {
                    pAeSet->shutter_speed = R_BCD_AE_S_SPEED_MIN;
                }
                 (void)R_BCD_AeSetShutterSpeed(pAeSet, pAeSet->shutter_speed);
            }
            else if (pAeSet->global_gain > 0)                   //Reduce gain on priority to reduce noise
            {
                if (gain_set_count == 0) {
                    gain_set_count = 2;
                nError = ave_lum - pAeSet->brightness;

                if ((nError > (0.45*pAeSet->brightness)) && (pAeSet->global_gain > 2)) //Coarse decrease
                {
                    pAeSet->global_gain = pAeSet->global_gain - 2;
                } else {
                    pAeSet->global_gain = pAeSet->global_gain - 1;
                }
                     (void)R_BCD_AeSetAnalogGain(pAeSet, pAeSet->global_gain);
                } else {
                    gain_set_count--;
                }
            }
        }
        else if (ave_lum < (pAeSet->brightness * (100 - BRIGHTNESS_TH_LOW)) / 100)  //Underexposed
        {
        	outOfRange = 1;
            if (pAeSet->shutter_speed < R_BCD_AE_S_SPEED_MAX)
            {
                nError = pAeSet->brightness - ave_lum;
                if (nError > (0.45*pAeSet->brightness))
                {
	                pAeSet->shutter_speed = (uint16_t)(((((uint32_t)pAeSet->shutter_speed * pAeSet->brightness) / ave_lum) * 2 + pAeSet->shutter_speed)/3);
                }
                else
                {
                    pAeSet->shutter_speed = pAeSet->shutter_speed + R_BCD_AE_S_SPEED_GAIN;
                }

                if (pAeSet->shutter_speed > R_BCD_AE_S_SPEED_MAX)
                {
                    pAeSet->shutter_speed = R_BCD_AE_S_SPEED_MAX;
                }
                 (void)R_BCD_AeSetShutterSpeed(pAeSet, pAeSet->shutter_speed);
            }
            else if (pAeSet->global_gain < 16)
            {
				pAeSet->brightness_old++;
                if (gain_set_count == 0) {
                    gain_set_count = 2;
                nError = pAeSet->brightness - ave_lum;
                if ((nError > (0.45*pAeSet->brightness)) && (pAeSet->global_gain < 15))
                {
                    pAeSet->global_gain = pAeSet->global_gain + 2;
                } else {
                    pAeSet->global_gain = pAeSet->global_gain + 1;
                }
                     (void)R_BCD_AeSetAnalogGain(pAeSet, pAeSet->global_gain);
                } else {
                    gain_set_count--;
                }
            }
        }
//        pAeSet->brightness_old = ave_lum;

    }

	if ( shutterSpeedOld != pAeSet-> shutter_speed || globalGainOld   !=  pAeSet->global_gain ) {
		pAeSet -> adapted = 1 ;
		outOfRange        = 0 ;
    } else {
		pAeSet -> adapted = 0 ;
    }
	pAeSet -> outOfRange = outOfRange ;
    return pAeSet->mode;
}

/******************************************************************************
* Function Name: R_BCD_AeSetShutterSpeed
* Description  : Set Camera shutter speed
* Arguments    : pAeSet
*              :   AE setting struct address
*              : speed
*              :   Shutter speed
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeSetShutterSpeed(r_bcd_ae_setting_t* pAeSet, uint16_t speed)
{
    pAeSet->shutter_speed = speed;
    (void)R_BCD_shutter_setting(speed) ;
    return pAeSet->mode;
}

/******************************************************************************
* Function Name: R_BCD_AeSetAnalogGain
* Description  : Set Camera analog gain
* Arguments    : pAeSet
*              :   AE setting struct address
*              : analog_gain
*              :   Analog gain parameter
* Return Value : AE mode
******************************************************************************/
uint8_t R_BCD_AeSetAnalogGain(r_bcd_ae_setting_t* pAeSet, uint8_t analog_gain)
{
	if (analog_gain > 16) {
		analog_gain = 16;
	}
    pAeSet->global_gain = analog_gain;
    
    (void)R_BCD_gain_table_setting(analog_gain);

    return pAeSet->mode;
}

/* End of File */
