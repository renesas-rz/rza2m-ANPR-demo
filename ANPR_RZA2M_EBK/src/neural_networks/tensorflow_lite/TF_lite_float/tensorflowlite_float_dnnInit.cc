/*
 * tensorflowlite_dnnInit.cc
 *
 */

#include "r_typedefs.h"
#include "tensorflowlite_model_settings.h"
#include "tensorflowlite_float_dnnInit.h"


// floating point CNN model :

void tfl_float_dnn_setup() {
	tfl_float_dnn_setupPre();
}

float* tfl_float_dnn_compute(float* inPointer) {
	tfl_float_dnn_computePre(inPointer, &CNN_result[0]);
	return(&CNN_result[0]);
}

