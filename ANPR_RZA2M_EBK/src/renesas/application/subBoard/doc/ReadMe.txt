Short introduction for adding the key read routines of the subBoard.

Please start the "Smart Configurator":

1.)	For SW4, SW5
                 Add (‘Components’ tab) : r_adc – adc
                                           Select AN0006 Pin [x] used  all other off
                YOu should see the followin at 'tap ‚Pins‘:
                AN006 P5_6/AN0006/IRQ2   W20 I

2.)	For SW2 pin please activate RES# 'tap ‚Pins‘ .
After that the following entry should be existing:
RES#                   AB9                               I

3.) LED1 uses two ports P6_0 and PC_1 'tap ‚Pins‘ please enable and configure both
    P6_0 .. M20 IO   <- please enable if not done, yet, double click and set to (by pencil symbols)
        --> M20 P60/..  P6_0 O Low None 4mA By Manual

    PC_1 .. AA6 IO   <- please enable if not done, yet, double click and set to (by pencil symbols)
        --> AA6 PC_1/.. PC_1 O Low None 4mA ByManual

Please run code generation after the above is done.

copy the subBoard folder into the application directory
(please add the search path for "subBoard/inc" in the compiler settings)

Please perform the following changes r_bcd_main.c (or whatever your main is)

C-Source code:  

#include "subBoardKeys.h"
#include "cpuBoardLed.h"

…….

    /* init keys */
    initBoardSwitch () ;
    boardSwitchInfoStruct.SW5_ModeCntMax = 2; // example: counter will cnt 0, 1, 2, 0, 1, 2 .. by every key press

    /* init LEDs LED1 */
    initCpuBoardLed() ;

    cpuBoardLedRed   ( 0 ) ; // red led off
    cpuBoardLedGreen ( 0 ) ; // green led off (The green LED looks yellow incase red is on, too

…..


while( 1) {

        readBoardSwitch ( ) ;

       if ( boardSwitchInfoStruct.SW2_IsModified ) {
             printf("SW2 mod %d cnt %d \n",boardSwitchInfoStruct.SW2_IsModified, boardSwitchInfoStruct.SW2_ModeCnt);
             boardSwitchInfoStruct.SW2_IsModified = 0 ;
       	     cpuBoardLedRed    ( boardSwitchInfoStruct.SW2_ModeCnt ) ;
       }

           if ( boardSwitchInfoStruct.SW3_IsModified ) {
             printf("SW3 mod %d cnt %d \n",boardSwitchInfoStruct.SW3_IsModified, boardSwitchInfoStruct.SW3_ModeCnt);
             boardSwitchInfoStruct.SW3_IsModified = 0 ;
           /* Change AE mode On/Off */
            if ( isp_ae_mode == ISP_AE_OFF )
            {
                R_BCD_AeStart(&ae_setting);
                isp_ae_mode = ISP_AE_ON;
            }
            else
            {
                R_BCD_AeStop(&ae_setting);
                isp_ae_mode = ISP_AE_OFF;
            }
       }
   
      if ( boardSwitchInfoStruct.SW4_IsModified ) {
             printf("SW4 mod %d cnt %d \n",boardSwitchInfoStruct.SW4_IsModified, boardSwitchInfoStruct.SW4_ModeCnt);
             boardSwitchInfoStruct.SW4_IsModified = 0 ;
       	     cpuBoardLedGreen ( boardSwitchInfoStruct.SW4_ModeCnt ) ;
       }      

        if ( boardSwitchInfoStruct.SW5_IsModified ) {
             printf("SW5 mod %d cnt %d \n",boardSwitchInfoStruct.SW5_IsModified, boardSwitchInfoStruct.SW5_ModeCnt);
             boardSwitchInfoStruct.SW5_IsModified = 0 ;

       }      

…..


Please remove existing code for acessing SW3/SW4.

Example:

    /* SW3 Readout setting */
    /* Cast to an appropriate type */
    PORTJ.PMR.BIT.PMR1 = 1;
    /* Cast to an appropriate type */
    PORTJ.PMR.BIT.PMR1 = 0;
    /* Cast to an appropriate type */
    PORTJ.PDR.BIT.PDR1 = 2;
    /* Cast to an appropriate type */
    PORTJ.PIDR.BIT.PIDR1;   /* Dummy read */        

Please remove the old code for AE handling:

      /* Get key */
        key_status = (key_status << 1) | PORTJ.PIDR.BIT.PIDR1;
        if ( ( key_status & KEY_CHECK_BIT) == KEY_JUST_ON )
        {
            /* Change AE mode On/Off */
            if ( isp_ae_mode == ISP_AE_OFF )
            {
                R_BCD_AeStart(&ae_setting);
                isp_ae_mode = ISP_AE_ON;
            }
            else
            {
                R_BCD_AeStop(&ae_setting);
                isp_ae_mode = ISP_AE_OFF;
            }
        }

