/*
 * subBoardKeysTdef.h
 *
 *  Created on: 29.01.2020
 *      Author: a5050305
 */

#ifndef RENESAS_APPLICATION_SUBKEYSTDEF_H_
#define RENESAS_APPLICATION_SUBKEYSTDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "r_typedefs.h"

typedef struct {
	int_t SW2_ModeCnt     ; // actual cnt (0,1) by default (controlled by boardKeysInside.h, SW2_ModeCntMax)
	int_t SW2_IsModified  ; // true --> new SW2_ModeCnt available (set to false to allow next key read)
	int_t SW2_ModeCntMax  ; // max count for SW2_ModeCnt

	int_t SW3_ModeCnt     ; // actual cnt (0,1) by default (controlled by boardKeysInside.h, SW3_ModeCntMax)
	int_t SW3_IsModified  ; // true --> new SW3_ModeCnt available (set to false to allow next key read)
	int_t SW3_ModeCntMax  ; // max count for SW3_ModeCnt

} switchBoardType ;

#ifndef RENESAS_APPLICATION_SUBBOARDKEYS_H_
extern switchBoardType boardSwitchInfoStruct     ;
#endif

#ifdef __cplusplus
}
#endif

#endif /* RENESAS_APPLICATION_SUBKEYSTDEF_H_ */
