/*
 * subBoardKeys.h
 *
 *  Created on: 29.01.2020
 *      Author: a5050305
 */

#ifndef RENESAS_APPLICATION_SUBBOARDKEYS_H_
#define RENESAS_APPLICATION_SUBBOARDKEYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "subBoardKeysTdef.h"

extern switchBoardType boardSwitchInfoStruct     ;

void initBoardSwitch ( void ) ;
void readBoardSwitch ( void ) ;

#ifdef __cplusplus
}
#endif

#endif /* RENESAS_APPLICATION_SUBBOARDKEYS_H_ */

