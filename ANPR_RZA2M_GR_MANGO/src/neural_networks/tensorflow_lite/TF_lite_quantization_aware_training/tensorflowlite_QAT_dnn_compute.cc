
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
/* Copyright 2021 Renesas Electronics

This file is based on TensorFlow Lite example project file
	person_detection.ino

Adjustments to ANPR demo by Renesas.

==============================================================================*/

#include "r_neural_networks.h"
#include "tensorflowlite_model_settings.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
//#-#// #include "tensorflow/lite/micro/all_ops_resolver.h"


// Globals, used for compatibility with Arduino-style sketches.
namespace {
	const tflite::Model*      model_QAT          = nullptr;
	tflite::ErrorReporter*    error_reporter_QAT = nullptr;
	tflite::MicroInterpreter* interpreter_QAT    = nullptr;
	tflite::MicroProfiler*    profiler_QAT       = nullptr;
	TfLiteTensor*             input_QAT          = nullptr;
	TfLiteTensor*             output_QAT         = nullptr;

	// In order to use optimized tensorflow lite kernels, a signed int8_t quantized
	// model is preferred over the legacy unsigned model format. This means that
	// throughout this project, input images must be converted from unisgned to
	// signed format. The easiest and quickest way to convert from unsigned to
	// signed 8-bit integers is to subtract 128 from the unsigned value to get a
	// signed value.

	// An area of memory to use for input, output, and intermediate arrays.
	constexpr int kTensorArenaSize_QAT = 10 * 1024;  // 10 ok (10 for AllOpsResolver)
	static uint8_t tensor_arena_QAT[kTensorArenaSize_QAT];
	//static uint8_t tensor_arena_QAT[kTensorArenaSize_QAT]  __attribute__ ((section("ImageWork_RAM")));

}  // namespace


void tfl_QAT_dnn_setupPre() {
	// tflite::InitializeTarget();    // originally empty; not modified and therefore not needed for this demo

	// Set up logging. Google style is to avoid globals or statics because of
	// lifetime uncertainty, but since this has a trivial destructor it's okay.
	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroErrorReporter micro_error_reporter_QAT;
	error_reporter_QAT = &micro_error_reporter_QAT;

	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroProfiler micro_profiler_QAT;
	profiler_QAT = &micro_profiler_QAT;

	// Map the model into a usable data structure. This doesn't involve any
	// copying or parsing, it's a very lightweight operation.
	model_QAT = tflite::GetModel(g_CNN_FE_Schrift_QAT_data);
	if (model_QAT->version() != TFLITE_SCHEMA_VERSION) {
	  TF_LITE_REPORT_ERROR(error_reporter_QAT,
	    	             "QAT model provided is schema version %d not equal "
	    		         "to supported version %d.",
						 model_QAT->version(), TFLITE_SCHEMA_VERSION);
	  return;
	} else {
	  TF_LITE_REPORT_ERROR(error_reporter_QAT,
		                     "   Model provided is schema version %d "
			                 "(equal to supported version %d).",
							 model_QAT->version(), TFLITE_SCHEMA_VERSION);
	  TF_LITE_REPORT_ERROR(error_reporter_QAT,
	                     "   => quantized model (QAT) loaded without error.");
	}

	// Pull in only the operation implementations we need.
	// This relies on a complete list of all the ops needed by this graph.
	// An easier approach is to just use the AllOpsResolver, but this will
	// incur some penalty in code space for op implementations that are not
	// needed by this graph.
	//
	//#-#// tflite::AllOpsResolver resolver_QAT;

	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroMutableOpResolver<5> micro_op_resolver_QAT(error_reporter_QAT);
	micro_op_resolver_QAT.AddConv2D();
	micro_op_resolver_QAT.AddMaxPool2D();
	micro_op_resolver_QAT.AddReshape();
	micro_op_resolver_QAT.AddFullyConnected();
	micro_op_resolver_QAT.AddSoftmax();

	//#### list of supported operations given in tensorflow/lite/micro/all_ops_resolver.cpp


	// Build an interpreter to run the model with.

	//#-#//// NOLINTNEXTLINE(runtime-global-variables)
	//#-#// static tflite::MicroInterpreter static_interpreter_QAT(
	//#-#//         model_QAT, resolver_QAT, tensor_arena_QAT, kTensorArenaSize_QAT, error_reporter_QAT, profiler_QAT);

	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroInterpreter static_interpreter_QAT(
		  model_QAT, micro_op_resolver_QAT, tensor_arena_QAT, kTensorArenaSize_QAT, error_reporter_QAT, profiler_QAT);

	interpreter_QAT = &static_interpreter_QAT;

	error_reporter_QAT->Report("   start allocating tensors");

	// Allocate memory from the tensor_arena for the model's tensors.
	TfLiteStatus allocate_status_QAT = interpreter_QAT->AllocateTensors();
	if (allocate_status_QAT != kTfLiteOk) {
	  TF_LITE_REPORT_ERROR(error_reporter_QAT, "AllocateTensors() failed");
	  return;
	} else {
	  TF_LITE_REPORT_ERROR(error_reporter_QAT, "   allocation finished; status: ok");
	}

	// Get information about the memory area to use for the model's input.
	input_QAT = interpreter_QAT->input(0);

	error_reporter_QAT->Report("   input  dimensions : %d", input_QAT->dims->size);
	error_reporter_QAT->Report("   input  shape      : %d, %d, %d, %d", input_QAT->dims->data[0], input_QAT->dims->data[1], input_QAT->dims->data[2], input_QAT->dims->data[3]);
	if ( kTfLiteInt8 == input_QAT->type ) {
	  error_reporter_QAT->Report("   input  type       : %s", "kTfLiteInt8");
	} else {
	  error_reporter_QAT->Report("   ATTENTION: input type is not %s", "kTfLiteInt8");
	}

	TfLiteTensor* output_QAT = interpreter_QAT->output(0);
	error_reporter_QAT->Report("   output dimensions : %d", output_QAT->dims->size);
	error_reporter_QAT->Report("   output shape      : %d, %d", output_QAT->dims->data[0], output_QAT->dims->data[1]);
	if ( kTfLiteInt8 == output_QAT->type ) {
	  error_reporter_QAT->Report("   output type       : %s", "kTfLiteInt8");
	} else {
	  error_reporter_QAT->Report("   ATTENTION: output type is not %s", "kTfLiteInt8");
	}

}


void tfl_QAT_dnn_computePre(int8_t* inPointer, int8_t* outPointer) {

	// input_QAT->data.int8 = (int8_t*)inPointer;
	memcpy(input_QAT->data.int8, inPointer, sizeof(int8_t)*kMaxImageSize);

	// Run the model on this input and make sure it succeeds.
	if (kTfLiteOk != interpreter_QAT->Invoke()) {
		error_reporter_QAT->Report("Invoke failed.");
	}

	output_QAT = interpreter_QAT->output(0);

	int8_t*  result = output_QAT->data.int8;

	memcpy(outPointer, result, sizeof(int8_t)*kCategoryCount);
}
