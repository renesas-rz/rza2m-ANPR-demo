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
* File Name    : layer_graph_CNN_FE.h
* Version      : 1.00
* Description  : Declarations of all functions
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.06.2017 1.00     First Release
***********************************************************************************************************************/

#ifndef LAYER_GRAPH_CNN_FE_H_
#define LAYER_GRAPH_CNN_FE_H_

void padding(TPrecision *, TPrecision *, TsInt *);
void convolution_without_pad(TPrecision *,const TPrecision *,const TPrecision *,TPrecision *,TsInt *,TsInt *);
void avgpool_padding(TPrecision *, TPrecision *, TsInt *);
void max_pooling_without_pad(TPrecision *, TPrecision *, TsInt *);
void average_pooling_without_pad(TPrecision *, TPrecision *, TsInt *);
void pooling_without_pad(TPrecision *, TPrecision *, TsInt *);
void innerproduct(TPrecision *,const TPrecision *,const TPrecision *,TPrecision *,TsInt *,TsInt *);
void relu(TPrecision *, TPrecision *, TsInt);
void softmax(TPrecision *, TPrecision *, TsInt);

#endif
