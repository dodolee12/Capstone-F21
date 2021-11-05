################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Do/Desktop/UVA/Sem 5/Capstone/project_zero_MSP_EXP432E401Y_tirtos_ccs" --include_path="C:/ti/simplelink_sdk_ble_plugin_3_20_00_24/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_20_00_10/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_20_00_10/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432e4_sdk_3_20_00_10/source/ti/posix/ccs" --include_path="C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --advice:power=none --define=DeviceFamily_MSP432E401x --define=__MSP432E401Y__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

cc2640r2lp_simple_np_uart_pm_sbl.obj: C:/ti/simplelink_sdk_ble_plugin_3_20_00_24/source/ti/snp/blestack/c-array/cc2640r2lp_simple_np_uart_pm_sbl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Do/Desktop/UVA/Sem 5/Capstone/project_zero_MSP_EXP432E401Y_tirtos_ccs" --include_path="C:/ti/simplelink_sdk_ble_plugin_3_20_00_24/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_20_00_10/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_20_00_10/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432e4_sdk_3_20_00_10/source/ti/posix/ccs" --include_path="C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --advice:power=none --define=DeviceFamily_MSP432E401x --define=__MSP432E401Y__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="cc2640r2lp_simple_np_uart_pm_sbl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


