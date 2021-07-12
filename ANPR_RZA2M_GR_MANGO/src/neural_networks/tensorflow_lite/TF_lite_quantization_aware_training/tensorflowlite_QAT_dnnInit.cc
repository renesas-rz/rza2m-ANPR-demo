/*
 * tensorflowlite_dnnInit.cc
 *
 */

#include "r_typedefs.h"
#include "tensorflowlite_model_settings.h"
#include "tensorflowlite_QAT_dnnInit.h"


// 'quantized during training' CNN model :

void tfl_QAT_dnn_setup() {
	tfl_QAT_dnn_setupPre();
}

int8_t* tfl_QAT_dnn_compute(int8_t* inPointer) {
	tfl_QAT_dnn_computePre(inPointer, &CNN_result_int8[0]);
	return(&CNN_result_int8[0]);
}

