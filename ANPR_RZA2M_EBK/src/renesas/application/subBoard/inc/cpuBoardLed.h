/*
 * cpuBoardLed.h
 *
 *  Created on: 29.01.2020
 *      Author: a5050305
 */

#ifndef RENESAS_APPLICATION_CPUBOARDLED_H_
#define RENESAS_APPLICATION_CPUBOARDLED_H_

#ifdef __cplusplus
extern "C" {
#endif

void initCpuBoardLed   (void) ;
void cpuBoardLedRed    ( int_t drive ) ;
void cpuBoardLedGreen  ( int_t drive ) ; // Note: (if the red LED is on the green LED looks like yellow)

#ifdef __cplusplus
}
#endif

#endif /* RENESAS_APPLICATION_CPUBOARDLED_H_ */

