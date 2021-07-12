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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
#ifndef R_SERIAL_TERMINAL_H
#define R_SERIAL_TERMINAL_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

#define SerialTerminal_on                    (true) 	/* set to false if SerialTerminal is not used */
#define SerialTerminal_show_contour_info     (false)
#define SerialTerminal_show_pot_number_plate (false)
#define SerialTerminal_show_char_contours    (false)
#define SerialTerminal_show_char_image       (false) 	/* attention: setting to true has big impact to TAT */
#define SerialTerminal_show_TAT              (true)

#define TF_LITE_STRIP_ERROR_STRINGS                     /* if defined, all TFLITE library messages are are deactivated */
                                                        /* allows to reduce memory foodprint of the application        */


/******************************************************************************
Variable Externs
******************************************************************************/

/* Serial Terminal */
/* Terminal window escape sequences */
static const char_t * const sp_clear_screen = "\x1b[2J";
static const char_t * const sp_cursor_home  = "\x1b[H";

extern void cmd_console(FILE* pIn, FILE *pOut, char_t *pszPrompt);
extern void show_welcome_msg (FILE *p_out, bool_t clear_screen);


/******************************************************************************
Functions Prototypes
******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif  /* R_SERIAL_TERMINAL_H */

/* End of File */
