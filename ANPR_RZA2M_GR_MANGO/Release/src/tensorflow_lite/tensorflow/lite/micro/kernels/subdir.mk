################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/activations.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/add_n.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/arg_min_max.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/batch_to_space_nd.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/cast.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/ceil.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/circular_buffer.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/comparisons.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/concatenation.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/conv_common.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/cumsum.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/depth_to_space.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/depthwise_conv_common.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/dequantize.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/detection_postprocess.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/elementwise.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/elu.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/ethosu.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/exp.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/expand_dims.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/fill.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/floor.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/floor_div.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/floor_mod.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/fully_connected_common.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/gather.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/gather_nd.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/hard_swish.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/if.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/kernel_runner.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/kernel_util.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/l2_pool_2d.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/l2norm.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/leaky_relu.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/log_softmax.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/logical.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/logistic.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/maximum_minimum.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/neg.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/pack.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/pad.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/prelu.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/quantize.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/quantize_common.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/reduce.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/reshape.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/resize_bilinear.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/round.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/shape.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/softmax_common.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/space_to_batch_nd.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/split.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/split_v.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/squeeze.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/strided_slice.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/sub.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/svdf_common.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/tanh.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/transpose.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/transpose_conv.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/unpack.cc \
../src/tensorflow_lite/tensorflow/lite/micro/kernels/zeros_like.cc 

CC_DEPS += \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/activations.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/add_n.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/arg_min_max.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/batch_to_space_nd.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/cast.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/ceil.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/circular_buffer.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/comparisons.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/concatenation.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/conv_common.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/cumsum.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/depth_to_space.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/depthwise_conv_common.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/dequantize.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/detection_postprocess.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/elementwise.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/elu.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/ethosu.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/exp.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/expand_dims.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/fill.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/floor.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/floor_div.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/floor_mod.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/fully_connected_common.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/gather.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/gather_nd.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/hard_swish.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/if.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/kernel_runner.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/kernel_util.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/l2_pool_2d.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/l2norm.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/leaky_relu.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/log_softmax.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/logical.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/logistic.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/maximum_minimum.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/neg.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/pack.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/pad.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/prelu.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/quantize.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/quantize_common.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/reduce.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/reshape.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/resize_bilinear.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/resize_nearest_neighbor.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/round.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/shape.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/softmax_common.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/space_to_batch_nd.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/split.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/split_v.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/squeeze.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/strided_slice.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/sub.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/svdf_common.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/tanh.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/transpose.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/transpose_conv.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/unpack.d \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/zeros_like.d 

OBJS += \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/activations.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/add_n.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/arg_min_max.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/batch_to_space_nd.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/cast.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/ceil.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/circular_buffer.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/comparisons.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/concatenation.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/conv_common.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/cumsum.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/depth_to_space.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/depthwise_conv_common.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/dequantize.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/detection_postprocess.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/elementwise.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/elu.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/ethosu.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/exp.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/expand_dims.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/fill.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/floor.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/floor_div.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/floor_mod.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/fully_connected_common.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/gather.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/gather_nd.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/hard_swish.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/if.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/kernel_runner.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/kernel_util.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/l2_pool_2d.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/l2norm.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/leaky_relu.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/log_softmax.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/logical.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/logistic.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/maximum_minimum.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/neg.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/pack.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/pad.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/prelu.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/quantize.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/quantize_common.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/reduce.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/reshape.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/resize_bilinear.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/resize_nearest_neighbor.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/round.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/shape.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/softmax_common.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/space_to_batch_nd.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/split.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/split_v.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/squeeze.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/strided_slice.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/sub.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/svdf_common.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/tanh.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/transpose.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/transpose_conv.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/unpack.o \
./src/tensorflow_lite/tensorflow/lite/micro/kernels/zeros_like.o 


# Each subdirectory must supply rules for building sources it contributes
src/tensorflow_lite/tensorflow/lite/micro/kernels/%.o: ../src/tensorflow_lite/tensorflow/lite/micro/kernels/%.cc
	$(file > $@.in,-mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wstack-usage=100 -DTF_LITE_STRIP_ERROR_STRINGS -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include/private" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/portable/GCC/ARM_CA9_RZA2M" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/config_files" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/common/console/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/GR_MANGO/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/hwsetup/octabus_setup/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/compiler/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/configuration" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_adc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_cbuffer/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ceu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/src" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/drp_lib" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_mipi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_octabus/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ostm/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_riic/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_rvapi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_scifa/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_vdc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2lcrop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2l_gaussian_blur/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb_color_correction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_find_contours_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_resize_bilinear_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_stb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_gpio/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_intc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cache/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_mmu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cpg/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/os_abstraction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/iodefine" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/Core/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/DSP/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/NN/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/flatbuffers/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/gemmlowp" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/ruy" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/Translator" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_float" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_post_training_quantization" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_quantization_aware_training" -fabi-version=0 -ffunction-sections -fdata-sections -O3 -fdump-tree-vect-details -Wl,--strip-all -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<")
	@echo 'arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wstack-usage=100 -DTF_LITE_STRIP_ERROR_STRINGS -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include/private" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/portable/GCC/ARM_CA9_RZA2M" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/config_files" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/common/console/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/GR_MANGO/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/hwsetup/octabus_setup/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/compiler/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/configuration" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_adc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_cbuffer/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ceu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/src" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/drp_lib" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_mipi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_octabus/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ostm/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_riic/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_rvapi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_scifa/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_vdc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2lcrop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2l_gaussian_blur/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb_color_correction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_find_contours_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_resize_bilinear_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_stb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_gpio/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_intc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cache/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_mmu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cpg/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/os_abstraction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/iodefine" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/Core/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/DSP/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/NN/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/flatbuffers/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/gemmlowp" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/ruy" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/Translator" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_float" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_post_training_quantization" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_quantization_aware_training" -fabi-version=0 -ffunction-sections -fdata-sections -O3 -fdump-tree-vect-details -Wl,--strip-all -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"'

	@arm-none-eabi-gcc @"$@.in"

