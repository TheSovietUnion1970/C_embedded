################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_tfat_driver_rx/src/device/usb_mini/r_tfat_drv_if_usb_mini.c 

COMPILER_OBJS += \
src/smc_gen/r_tfat_driver_rx/src/device/usb_mini/r_tfat_drv_if_usb_mini.obj 

C_DEPS += \
src/smc_gen/r_tfat_driver_rx/src/device/usb_mini/r_tfat_drv_if_usb_mini.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_tfat_driver_rx/src/device/usb_mini/%.obj: ../src/smc_gen/r_tfat_driver_rx/src/device/usb_mini/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_tfat_driver_rx\src\device\usb_mini\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_tfat_driver_rx\src\device\usb_mini\cSubCommand.tmp" "$<"


