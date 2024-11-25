################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_tfat_driver_rx/src/device/usb/r_tfat_drv_if_usb.c 

COMPILER_OBJS += \
src/smc_gen/r_tfat_driver_rx/src/device/usb/r_tfat_drv_if_usb.obj 

C_DEPS += \
src/smc_gen/r_tfat_driver_rx/src/device/usb/r_tfat_drv_if_usb.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_tfat_driver_rx/src/device/usb/%.obj: ../src/smc_gen/r_tfat_driver_rx/src/device/usb/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_tfat_driver_rx\src\device\usb\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_tfat_driver_rx\src\device\usb\cSubCommand.tmp" "$<"


