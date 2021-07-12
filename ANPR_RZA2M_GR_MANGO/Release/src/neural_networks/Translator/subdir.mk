################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/neural_networks/Translator/dnnInit.cpp \
../src/neural_networks/Translator/dnn_compute_CNN_FE.cpp \
../src/neural_networks/Translator/network_CNN_FE.cpp 

OBJS += \
./src/neural_networks/Translator/dnnInit.o \
./src/neural_networks/Translator/dnn_compute_CNN_FE.o \
./src/neural_networks/Translator/network_CNN_FE.o 

CPP_DEPS += \
./src/neural_networks/Translator/dnnInit.d \
./src/neural_networks/Translator/dnn_compute_CNN_FE.d \
./src/neural_networks/Translator/network_CNN_FE.d 


# Each subdirectory must supply rules for building sources it contributes
src/neural_networks/Translator/%.o: ../src/neural_networks/Translator/%.cpp
	$(file > $@.in,-mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wstack-usage=100 -DTF_LITE_STRIP_ERROR_STRINGS -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include/private" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/portable/GCC/ARM_CA9_RZA2M" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/config_files" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/common/console/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/GR_MANGO/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/hwsetup/octabus_setup/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/compiler/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/configuration" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_adc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_cbuffer/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ceu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/src" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/drp_lib" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_mipi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_octabus/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ostm/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_riic/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_rvapi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_scifa/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_vdc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2lcrop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2l_gaussian_blur/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb_color_correction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_find_contours_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_resize_bilinear_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_stb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_gpio/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_intc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cache/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_mmu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cpg/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/os_abstraction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/iodefine" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/Core/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/DSP/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/NN/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/flatbuffers/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/gemmlowp" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/ruy" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/Translator" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_float" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_post_training_quantization" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_quantization_aware_training" -fabi-version=0 -ffunction-sections -fdata-sections -O3 -fdump-tree-vect-details -Wl,--strip-all -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<")
	@echo 'arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wstack-usage=100 -DTF_LITE_STRIP_ERROR_STRINGS -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/include/private" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/FreeRTOS/portable/GCC/ARM_CA9_RZA2M" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/config_files" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/common/console/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/application/GR_MANGO/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/renesas/hwsetup/octabus_setup/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/compiler/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/configuration" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_adc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_cbuffer/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ceu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/src" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/drp_lib" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_mipi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_octabus/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_ostm/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_riic/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_rvapi/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_scifa/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_vdc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2lcrop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_lrgb2l_gaussian_blur/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb_color_correction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_find_contours_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_resize_bilinear_crop/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_stb/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_gpio/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_intc/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cache/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_mmu/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/drivers/r_cpg/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/os_abstraction/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/inc" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/system/iodefine" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/Core/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/DSP/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/cmsis/CMSIS/NN/Include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/flatbuffers/include" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/gemmlowp" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/tensorflow_lite/third_party/ruy" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/Translator" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_float" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_post_training_quantization" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/src/neural_networks/tensorflow_lite/TF_lite_quantization_aware_training" -fabi-version=0 -ffunction-sections -fdata-sections -O3 -fdump-tree-vect-details -Wl,--strip-all -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"'

	@arm-none-eabi-gcc @"$@.in"

