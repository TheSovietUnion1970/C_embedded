################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_usb_basic/src/hw/r_usb_creg_abs.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_creg_access.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_dma.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_hostelectrical.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_hreg_abs.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_hreg_access.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_preg_abs.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_preg_access.c \
../src/smc_gen/r_usb_basic/src/hw/r_usb_rx_mcu.c 

COMPILER_OBJS += \
src/smc_gen/r_usb_basic/src/hw/r_usb_creg_abs.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_creg_access.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_dma.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_hostelectrical.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_hreg_abs.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_hreg_access.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_preg_abs.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_preg_access.obj \
src/smc_gen/r_usb_basic/src/hw/r_usb_rx_mcu.obj 

C_DEPS += \
src/smc_gen/r_usb_basic/src/hw/r_usb_creg_abs.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_creg_access.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_dma.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_hostelectrical.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_hreg_abs.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_hreg_access.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_preg_abs.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_preg_access.d \
src/smc_gen/r_usb_basic/src/hw/r_usb_rx_mcu.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_usb_basic/src/hw/%.obj: ../src/smc_gen/r_usb_basic/src/hw/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_usb_basic\src\hw\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_usb_basic\src\hw\cSubCommand.tmp" "$<"


