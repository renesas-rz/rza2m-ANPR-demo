/*
 * dnnInit.cpp
 */

#include "Typedef.h"

TPrecision* dnn_compute_CNN_FEPre(TPrecision*, TsInt*) ;

extern "C" float* dnn_compute_CNN_FE(float*, TsInt*) ;

float* dnn_compute_CNN_FE(float* inPointer, TsInt* errorPointer) {
	return dnn_compute_CNN_FEPre((TPrecision*)inPointer, (TsInt*)errorPointer) ;
}

