/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED 'AS IS' AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Changed from original python code to C source code.
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : layer_shapes_CNN_FE.h
* Version      : 1.00
* Description  : Initializations
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.06.2017 1.00     First Release
***********************************************************************************************************************/

#include "Typedef.h"
#ifndef LAYER_SHAPES_cnn_fe_H_
#define LAYER_SHAPES_cnn_fe_H_
 
// TPrecision* dnn_compute_CNN_FE(TPrecision*, TsInt*);
 
TPrecision dnn_buffer1[6480];
TPrecision dnn_buffer2[720];
 
struct shapes_CNN_FE{
    TsInt conv2d_shape[12];
    TsInt conv2d_relu_shape;
    TsInt max_pooling2d_shape[11];
    TsInt conv2d_1_shape[12];
    TsInt conv2d_1_relu_shape;
    TsInt max_pooling2d_1_shape[11];
    TsInt dense_shape[4];
    TsInt dense_relu_shape;
    TsInt dense_1_shape[4];
    TsInt dense_1_softmax_shape;
};
 
struct shapes_CNN_FE layer_shapes_CNN_FE ={
    {1,1,32,20,12,1,3,3,30,18,1,1},
    6480,
    {1,12,30,18,10,6,3,3,3,3,0},
    {1,12,10,6,12,12,3,3,8,4,1,1},
    384,
    {1,12,8,4,4,2,2,2,2,2,0},
    {1,96,96,64},
    64,
    {1,64,64,39},
    39
};
 
#endif
