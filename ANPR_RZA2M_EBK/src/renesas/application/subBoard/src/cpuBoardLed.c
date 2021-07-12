/*
 * cpuBoardLed.c
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

#include "r_gpio_drv_api.h"

#include "cpuBoardLed.h"


/******************************************************************************
Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
local global variables
 ******************************************************************************/


/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
//static uint32_t s_main_led_flg;      /* LED lighting/turning off */
static int_t s_my_gpio_handle;

static st_r_drv_gpio_pin_rw_t s_p60_on =
{
    GPIO_PORT_6_PIN_0,
    GPIO_LEVEL_HIGH,
    GPIO_SUCCESS
};

static st_r_drv_gpio_pin_rw_t s_p60_off =
{
    GPIO_PORT_6_PIN_0,
    GPIO_LEVEL_LOW,
    GPIO_SUCCESS
};


static st_r_drv_gpio_pin_rw_t s_pC1_on =
{
	GPIO_PORT_C_PIN_1,
    GPIO_LEVEL_HIGH,
    GPIO_SUCCESS
};

static st_r_drv_gpio_pin_rw_t s_pC1_off =
{
	GPIO_PORT_C_PIN_1,
    GPIO_LEVEL_LOW,
    GPIO_SUCCESS
};

static const r_gpio_port_pin_t s_led_pin_list[] =
{
    GPIO_PORT_6_PIN_0,
	GPIO_PORT_C_PIN_1,
};

static st_r_drv_gpio_pin_list_t pin_led ;


/**********************************************************************************************************************
 * global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: initCpuLed
 * Description  : initialize the gpio driver andport pins PC_1, P6_0
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/

 void initCpuBoardLed(void) {
          int_t err;

         /* For information only
          * Use stdio calls to open drivers once  the kernel is initialised
          *
          * i.e.
          * int_t ostm3_handle;
          * ostm3_handle = open (DEVICE_INDENTIFIER "ostm2", O_RDWR);
          * close (ostm3_handle);
          */

          s_my_gpio_handle = open(DEVICE_INDENTIFIER "gpio", O_RDWR);

          /* On error */
          if (s_my_gpio_handle < 0) {
             /* stop execute */
             while (1) {
                 R_COMPILER_Nop();
             }
           }

           pin_led.p_pin_list = s_led_pin_list;
           pin_led.count = (sizeof(s_led_pin_list)) / (sizeof(s_led_pin_list[0]));
           err = direct_control(s_my_gpio_handle, CTL_GPIO_INIT_BY_PIN_LIST, &pin_led);

           /* On error */
            if (err < 0)
            {
                /* stop execute */
                while (1)
                {
                    R_COMPILER_Nop();
                }
            }
        }



/**********************************************************************************************************************
 * Function Name: cpuBoardLedGreen
 * Description  : switch the green LED on/off (if the red LED is on the green LED looks like yellow)
 * Arguments    : drive (0-off, 1-on)
 * Return Value : -
 *********************************************************************************************************************/
void cpuBoardLedGreen ( int_t drive ) {
	if ( drive ) {
		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_pC1_on);
	} else {
		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_pC1_off);
	}
}


/**********************************************************************************************************************
 * Function Name: cpuBoardLedRed
 * Description  : switch the red LED on/off
 * Arguments    : drive (0-off, 1-on)
 * Return Value : -
 *********************************************************************************************************************/
void cpuBoardLedRed ( int_t drive ) {
	if ( drive ) {
		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p60_on);
	} else {
		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p60_off);
	}
}


