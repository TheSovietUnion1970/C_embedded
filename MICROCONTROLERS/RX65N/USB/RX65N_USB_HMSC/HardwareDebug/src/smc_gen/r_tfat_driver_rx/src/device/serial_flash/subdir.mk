################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_tfat_driver_rx/src/device/serial_flash/r_tfat_drv_if_flash.c 

COMPILER_OBJS += \
src/smc_gen/r_tfat_driver_rx/src/device/serial_flash/r_tfat_drv_if_flash.obj 

C_DEPS += \
src/smc_gen/r_tfat_driver_rx/src/device/serial_flash/r_tfat_drv_if_flash.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_tfat_driver_rx/src/device/serial_flash/%.obj: ../src/smc_gen/r_tfat_driver_rx/src/device/serial_flash/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_tfat_driver_rx\src\device\serial_flash\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_tfat_driver_rx\src\device\serial_flash\cSubCommand.tmp" "$<"


