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

#ifndef TENSORFLOW_LITE_MICRO_MODEL_SETTINGS_H_
#define TENSORFLOW_LITE_MICRO_MODEL_SETTINGS_H_

#include <stdio.h>
#include "r_neural_networks.h"

// Keeping these as constant expressions allow us to allocate fixed-sized arrays
// on the stack for our working memory.

// All of these values are derived from the values used during model training,
// if you change your model you'll need to update these constants.
constexpr unsigned int kNumCols      = NN_IN_HEIGHT;
constexpr unsigned int kNumRows      = NN_IN_WIDTH;
constexpr unsigned int kNumChannels  = NN_IN_CHANNEL;

constexpr unsigned int kMaxImageSize = kNumCols * kNumRows * kNumChannels;

constexpr int kCategoryCount = 39;
extern const char* kCategoryLabels[kCategoryCount];


extern float   CNN_result[kCategoryCount];
extern int8_t  CNN_result_int8[kCategoryCount];


#endif  // TENSORFLOW_LITE_MICRO_MODEL_SETTINGS_H_
