The following setings are for the GR-Mango board                   B.H. 24.11.2020
----------------------------------------------------------------------------------
Short introduction for adding the key read routines of the subBoard.

Please start the "Smart Configurator":

1.)	For SW0 SW1
  Port
  PD_6  SW0
  PD_7  SW1
  
  
Note:  Click on PinName selects the PinNumber tab and jumps to the pin
  
       (X) PD_6  PD_6/RIIC35CL/IRQ6 D20 IO                       (TAB: Pin Fuction)
               Function     PD_6                                 (TAB: PinNumber  )
               Direction    I                                    (TAB: PinNumber  )  
			   Output LEvel None                                 (TAB: PinNumber  )
               Interrupt    Disabled                             (TAB: PinNumber  )
			   Drive        -                                    (TAB: PinNumber  )
			   Init         By GPIO Init                         (TAB: PinNumber  )
 
       (X) PD_7  PD_7/RIIC3SDA/IRQ7 C21 IO                       (TAB: Pin Fuction)
               Function     PD_7                                 (TAB: PinNumber  )
               Direction    I                                    (TAB: PinNumber  )  
			   Output LEvel None                                 (TAB: PinNumber  )
               Interrupt    Disabled                             (TAB: PinNumber  )
			   Drive        -                                    (TAB: PinNumber  )
			   Init         By GPIO Init                         (TAB: PinNumber  )
 

3.) LED configuration in codegen 
    'tap ‚Pins‘ please enable and configure both
	
  PORT  LED
  P0_1  green
  P0_3  yellow 
  p0_5  orange
  p8_2  red

  Please make the following configurations:
  
       (X) P0_1  P0_1/D1/DRP25/DV0_DATA_18/MTIOC6C/GTIOC4A L2 IO (TAB: Pin Fuction)
               Function     P0_1                                 (TAB: PinNumber  )
               Direction    O                                    (TAB: PinNumber  )  
			   Output LEvel Low                                  (TAB: PinNumber  )
               Interrupt    None                                 (TAB: PinNumber  )
			   Drive        4mA                                  (TAB: PinNumber  )
			   Init         ByManual                             (TAB: PinNumber  )
 
       (X) P0_3  P0_3/D3/DRP27/DV0_DATA20/MTIOC7A/GTIOC6A M4 IO (TAB: Pin Fuction)
               Function     P0_3                                (TAB: PinNumber  )
               Direction    O                                   (TAB: PinNumber  )  
			   Output LEvel Low                                 (TAB: PinNumber  )
               Interrupt    None                                (TAB: PinNumber  )
			   Drive        4mA                                 (TAB: PinNumber  )
			   Init         ByManual                            (TAB: PinNumber  )
 
       (X) P0_5  P0_5 P0_5/D5/DRP29/DV0_DATA32/MTIOC7C/GTIOC7A M2 IO (TAB: Pin Fuction)
               Function     P0_5                                (TAB: PinNumber  )
               Direction    O                                   (TAB: PinNumber  )  
			   Output LEvel Low                                 (TAB: PinNumber  )
               Interrupt    None                                (TAB: PinNumber  )
			   Drive        4mA                                 (TAB: PinNumber  )
			   Init         ByManual                            (TAB: PinNumber  )
   
       (X) P8_2  P8_2/A2/DRP22/DVO_DATA15/GTIOC5A/IRQ2 C2 IO (TAB: Pin Fuction)
               Function      P8_2                               (TAB: PinNumber  )
               Direction    O                                   (TAB: PinNumber  )  
			   Output LEvel Low                                 (TAB: PinNumber  )
               Interrupt    None                                (TAB: PinNumber  )
			   Drive        4mA                                 (TAB: PinNumber  )
			   Init         ByManual                            (TAB: PinNumber  )


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
    boardSwitchInfoStruct.SW0_ModeCntMax = 2; // example: counter will cnt 0, 1, 2, 0, 1, 2 .. by every key press

    /* init LEDs LED1 */
    initCpuBoardLed() ;

    cpuBoardLedRed    ( 0 ) ; // red led off
    cpuBoardLedOrange ( 0 ) ; // orange led off 
    cpuBoardLedYellow ( 0 ) ; // yellow led off 
	cpuBoardLedGreen  ( 0 ) ; // green led off 
	
…..


while( 1) {

       readBoardSwitch ( ) ;

       if ( boardSwitchInfoStruct.SW0_IsModified ) {
             printf("SW0 mod %d cnt %d \n",boardSwitchInfoStruct.SW0_IsModified, boardSwitchInfoStruct.SW0_ModeCnt);
             boardSwitchInfoStruct.SW0_IsModified = 0 ;
       	     cpuBoardLedRed    ( boardSwitchInfoStruct.SW0_ModeCnt ) ;
       }

       if ( boardSwitchInfoStruct.SW1_IsModified ) {
             printf("SW1 mod %d cnt %d \n",boardSwitchInfoStruct.SW1_IsModified, boardSwitchInfoStruct.SW1_ModeCnt);
             boardSwitchInfoStruct.SW1_IsModified = 0 ;
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
   
…..


Please remove existing code for acessing SW0/SW1.

Please remove the old code for AE handling:


the "keystatus" variables/code can also be removed 
