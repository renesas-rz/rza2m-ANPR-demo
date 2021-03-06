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
* File Name    : dnn_compute_CNN_FE.c
* Version      : 1.00
* Description  : The function calls
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.06.2017 1.00     First Release
***********************************************************************************************************************/

 
#include "layer_shapes_CNN_FE.h"
#include "layer_graph_CNN_FE.h"
#include "weights_CNN_FE.h"
#include "perform.h"

TPrecision* dnn_compute_CNN_FEPre(TPrecision*, TsInt *);

TPrecision* dnn_compute_CNN_FEPre(TPrecision* conv2d_input, TsInt *errorcode)
{
	PerformSetStartTime(60);
  convolution_without_pad(conv2d_input,conv2d_weights,conv2d_biases,dnn_buffer1,layer_shapes_CNN_FE.conv2d_shape,errorcode);
	PerformSetEndTime(60);
	PerformSetStartTime(61);
  relu(dnn_buffer1,dnn_buffer1,layer_shapes_CNN_FE.conv2d_relu_shape);
	PerformSetEndTime(61);
	PerformSetStartTime(62);
  pooling_without_pad(dnn_buffer1,dnn_buffer2,layer_shapes_CNN_FE.max_pooling2d_shape);
	PerformSetEndTime(62);
	PerformSetStartTime(63);
  convolution_without_pad(dnn_buffer2,conv2d_1_weights,conv2d_1_biases,dnn_buffer1,layer_shapes_CNN_FE.conv2d_1_shape,errorcode);
	PerformSetEndTime(63);
	PerformSetStartTime(64);
  relu(dnn_buffer1,dnn_buffer1,layer_shapes_CNN_FE.conv2d_1_relu_shape);
	PerformSetEndTime(64);
	PerformSetStartTime(65);
  pooling_without_pad(dnn_buffer1,dnn_buffer2,layer_shapes_CNN_FE.max_pooling2d_1_shape);
	PerformSetEndTime(65);

	PerformSetStartTime(66);
  innerproduct(dnn_buffer2,dense_weights,dense_biases,dnn_buffer1,layer_shapes_CNN_FE.dense_shape,errorcode);
	PerformSetEndTime(66);
	PerformSetStartTime(67);
  relu(dnn_buffer1,dnn_buffer1,layer_shapes_CNN_FE.dense_relu_shape);
	PerformSetEndTime(67);

	PerformSetStartTime(68);
  innerproduct(dnn_buffer1,dense_1_weights,dense_1_biases,dnn_buffer2,layer_shapes_CNN_FE.dense_1_shape,errorcode);
	PerformSetEndTime(68);
	PerformSetStartTime(69);
  softmax(dnn_buffer2,dnn_buffer2,layer_shapes_CNN_FE.dense_1_softmax_shape);
	PerformSetEndTime(69);

  return(dnn_buffer2);
}
