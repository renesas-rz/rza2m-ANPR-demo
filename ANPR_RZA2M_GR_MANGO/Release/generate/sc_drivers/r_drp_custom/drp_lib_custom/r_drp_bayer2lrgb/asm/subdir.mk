################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/asm/r_drp_bayer2lrgb_t3.asm 

OBJS += \
./generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/asm/r_drp_bayer2lrgb_t3.o 

ASM_DEPS += \
./generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/asm/r_drp_bayer2lrgb_t3.d 


# Each subdirectory must supply rules for building sources it contributes
generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/asm/%.o: ../generate/sc_drivers/r_drp_custom/drp_lib_custom/r_drp_bayer2lrgb/asm/%.asm
	$(file > $@.in,-mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wstack-usage=100 -x assembler-with-cpp -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/drp_lib" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/os_abstraction/inc" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<")
	@echo 'arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wstack-usage=100 -x assembler-with-cpp -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp/drp_lib" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/sc_drivers/r_drp_custom/drp_lib_custom" -I"C:/Renesas/e2studio_2021_01_workspace_RZA2M_ANPR/ANPR_RZA2M_GR_MANGO/generate/os_abstraction/inc" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"'

	@arm-none-eabi-gcc @"$@.in"

