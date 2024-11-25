################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_usb_hmsc/src/r_usb_hmsc_api.c \
../src/smc_gen/r_usb_hmsc/src/r_usb_hmsc_driver.c \
../src/smc_gen/r_usb_hmsc/src/r_usb_hstorage_driver.c 

COMPILER_OBJS += \
src/smc_gen/r_usb_hmsc/src/r_usb_hmsc_api.obj \
src/smc_gen/r_usb_hmsc/src/r_usb_hmsc_driver.obj \
src/smc_gen/r_usb_hmsc/src/r_usb_hstorage_driver.obj 

C_DEPS += \
src/smc_gen/r_usb_hmsc/src/r_usb_hmsc_api.d \
src/smc_gen/r_usb_hmsc/src/r_usb_hmsc_driver.d \
src/smc_gen/r_usb_hmsc/src/r_usb_hstorage_driver.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_usb_hmsc/src/%.obj: ../src/smc_gen/r_usb_hmsc/src/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_usb_hmsc\src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_usb_hmsc\src\cSubCommand.tmp" "$<"


