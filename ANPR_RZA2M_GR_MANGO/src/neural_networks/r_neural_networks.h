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
#ifndef R_NEURAL_NETWORKS_H
#define R_NEURAL_NETWORKS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "dnn_compute_CNN_FE.h"

#include "tensorflowlite_float_dnnInit.h"
#include "CNN_FE_Schrift_20210705_171657_float.h"

#include "tensorflowlite_PTQ_dnnInit.h"
#include "CNN_FE_Schrift_20210705_171657_PTQ.h"

#include "tensorflowlite_QAT_dnnInit.h"
#include "CNN_FE_Schrift_20210702_183016_QAT.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

typedef enum _nn_type_et
{
	TRANSLATOR,       /* C-code generated with Renesas eAI_translator                                                           */
	TFLITE_FLOAT,     /* TensorFlow Lite model (resolution float)                                                               */
	TFLITE_PTQ,       /* TensorFlow Lite model (integer resolution (post training quantization))                                */
	TFLITE_QAT,       /* TensorFlow Lite model (pure integer resolution (quantized during training by using fake quantization)) */
	NO_NN             /* no neural network */
} nn_type_et;


/******************************************************************************
Macro definitions
******************************************************************************/

/* NN input data dimensions */
#define  NN_IN_WIDTH            (20)
#define  NN_IN_HEIGHT           (32)
#define  NN_IN_CHANNEL          (1)

/* define which NN model should be available in the demo (every setting mix allowed) */
#define  TRANSLATOR_ACTIVE      (true)
#define  TFLITE_FLOAT_ACTIVE    (true)
#define  TFLITE_PTQ_ACTIVE      (true)
#define  TFLITE_QAT_ACTIVE      (true)


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif  /* R_NEURAL_NETWORKS_H */

/* End of File */
