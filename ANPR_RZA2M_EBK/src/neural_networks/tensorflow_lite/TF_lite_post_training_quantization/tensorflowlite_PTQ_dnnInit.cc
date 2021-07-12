/*
 * tensorflowlite_dnnInit.cc
 *
 */

#include "r_typedefs.h"
#include "tensorflowlite_model_settings.h"
#include "tensorflowlite_PTQ_dnnInit.h"


// 'post-training' quantized CNN model :

void tfl_PTQ_dnn_setup() {
	tfl_PTQ_dnn_setupPre();
}

int8_t* tfl_PTQ_dnn_compute(int8_t* inPointer) {
	tfl_PTQ_dnn_computePre(inPointer, &CNN_result_int8[0]);
	return(&CNN_result_int8[0]);
}
