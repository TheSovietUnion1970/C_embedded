################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/demo_src/main.c \
../src/demo_src/r_usb_hmsc_apl.c 

COMPILER_OBJS += \
src/demo_src/main.obj \
src/demo_src/r_usb_hmsc_apl.obj 

C_DEPS += \
src/demo_src/main.d \
src/demo_src/r_usb_hmsc_apl.d 

# Each subdirectory must supply rules for building sources it contributes
src/demo_src/%.obj: ../src/demo_src/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\demo_src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\demo_src\cSubCommand.tmp" "$<"


