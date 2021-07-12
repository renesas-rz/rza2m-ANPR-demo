
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
	const tflite::Model*      model          = nullptr;
	tflite::ErrorReporter*    error_reporter = nullptr;
	tflite::MicroInterpreter* interpreter    = nullptr;
	tflite::MicroProfiler*    profiler       = nullptr;
	TfLiteTensor*             input          = nullptr;
	TfLiteTensor*             output         = nullptr;

	// An area of memory to use for input, output, and intermediate arrays.
	constexpr int kTensorArenaSize = 30 * 1024;   // 30 ok  (30 for AllOpsResolver)
	static uint8_t tensor_arena[kTensorArenaSize];
	//static uint8_t tensor_arena[kTensorArenaSize]  __attribute__ ((section("ImageWork_RAM")));

}  // namespace


void tfl_float_dnn_setupPre() {
	// tflite::InitializeTarget();    // originally empty; not modified and therefore not needed for this demo

	// Set up logging. Google style is to avoid globals or statics because of
	// lifetime uncertainty, but since this has a trivial destructor it's okay.
	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroErrorReporter micro_error_reporter;
	error_reporter = &micro_error_reporter;

	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroProfiler micro_profiler;
	profiler = &micro_profiler;

	// Map the model into a usable data structure. This doesn't involve any
	// copying or parsing, it's a very lightweight operation.
	model = tflite::GetModel(g_CNN_FE_Schrift_float_data);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
	  TF_LITE_REPORT_ERROR(error_reporter,
	                       "Model provided is schema version %d not equal "
	                     "to supported version %d.",
	                      model->version(), TFLITE_SCHEMA_VERSION);
	  return;
	} else {
	  TF_LITE_REPORT_ERROR(error_reporter,
	                     "   Model provided is schema version %d "
		                 "(equal to supported version %d).",
		                 model->version(), TFLITE_SCHEMA_VERSION);
	  TF_LITE_REPORT_ERROR(error_reporter,
	                       "   => floating point model loaded without error.");
	}

	// Pull in only the operation implementations we need.
	// This relies on a complete list of all the ops needed by this graph.
	// An easier approach is to just use the AllOpsResolver, but this will
	// incur some penalty in code space for op implementations that are not
	// needed by this graph.
	//
	//#-#// tflite::AllOpsResolver resolver;

	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroMutableOpResolver<5> micro_op_resolver(error_reporter);
	micro_op_resolver.AddConv2D();
	micro_op_resolver.AddMaxPool2D();
	micro_op_resolver.AddReshape();
	micro_op_resolver.AddFullyConnected();
	micro_op_resolver.AddSoftmax();

	//#### list of supported operations given in tensorflow/lite/micro/all_ops_resolver.cpp


	// Build an interpreter to run the model with.

	//#-#//// NOLINTNEXTLINE(runtime-global-variables)
	//#-#// static tflite::MicroInterpreter static_interpreter(
	//#-#//         model, resolver, tensor_arena, kTensorArenaSize, error_reporter, profiler);

	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroInterpreter static_interpreter(
		  model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter, profiler);

	interpreter = &static_interpreter;

	error_reporter->Report("   start allocating tensors");

	// Allocate memory from the tensor_arena for the model's tensors.
	TfLiteStatus allocate_status = interpreter->AllocateTensors();
	if (allocate_status != kTfLiteOk) {
	  TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
	  return;
	} else {
	TF_LITE_REPORT_ERROR(error_reporter, "   allocation finished; status: ok");
	}

	// Get information about the memory area to use for the model's input.
	input  = interpreter->input(0);

	error_reporter->Report("   input  dimensions : %d", input->dims->size);
	error_reporter->Report("   input  shape      : %d, %d, %d, %d", input->dims->data[0], input->dims->data[1], input->dims->data[2], input->dims->data[3]);
	if ( kTfLiteFloat32 == input->type ) {
	  error_reporter->Report("   input  type       : %s", "tTfLiteFloat32");
	} else {
	  error_reporter->Report("   ATTENTION: input type is not %s", "tTfLiteFloat32");
	}

	output = interpreter->output(0);
	error_reporter->Report("   output dimensions : %d", output->dims->size);
	error_reporter->Report("   output shape      : %d, %d", output->dims->data[0], output->dims->data[1]);
	if ( kTfLiteFloat32 == output->type ) {
	  error_reporter->Report("   output type       : %s", "tTfLiteFloat32");
	} else {
	  error_reporter->Report("   ATTENTION: output type is not %s", "tTfLiteFloat32");
	}

}

void tfl_float_dnn_computePre(float* inPointer, float* outPointer) {

	// input->data.f = inPointer;
	memcpy(input->data.f, inPointer, sizeof(float)*kMaxImageSize);

	// Run the model on this input and make sure it succeeds.
	if (kTfLiteOk != interpreter->Invoke()) {
		error_reporter->Report("Invoke failed.");
	}

	output = interpreter->output(0);

	float* result = output->data.f;

	memcpy(outPointer, result,  sizeof(float)*kCategoryCount);
}
