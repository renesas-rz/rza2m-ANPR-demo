/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_LITE_QAT_MICRO_DNNINIT_H_
#define TENSORFLOW_LITE_QAT_MICRO_DNNINIT_H_

#include "r_typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif


void     tfl_QAT_dnn_setup();             // setup 'quantized during training' CNN model (input and output data format: uint8)
int8_t*  tfl_QAT_dnn_compute(int8_t*);   // inference with 'quantized during training' CNN model (input and output data format: uint8)


#ifdef __cplusplus
}
#endif


void tfl_QAT_dnn_setupPre();
void tfl_QAT_dnn_computePre(int8_t* inPointer, int8_t* outPointer);


#endif  // TENSORFLOW_LITE_QAT_MICRO_DNNINIT_H_
