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

#ifndef TENSORFLOW_LITE_MICRO_FLOAT_DNNINIT_H_
#define TENSORFLOW_LITE_MICRO_FLOAT_DNNINIT_H_

#include "r_typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif


void      tfl_float_dnn_setup();                   // setup floating point CNN model
float*    tfl_float_dnn_compute(float*);           // inference with floating point CNN model


#ifdef __cplusplus
}
#endif


void tfl_float_dnn_setupPre();
void tfl_float_dnn_computePre(float* inPointer, float* outPointer);


#endif  // TENSORFLOW_LITE_MICRO_FLOAT_DNNINIT_H_
