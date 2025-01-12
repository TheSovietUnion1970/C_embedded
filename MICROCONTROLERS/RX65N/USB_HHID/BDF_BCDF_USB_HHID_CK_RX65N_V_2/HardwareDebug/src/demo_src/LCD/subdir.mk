################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/demo_src/LCD/ascii.c \
../src/demo_src/LCD/lcd.c \
../src/demo_src/LCD/r_cg_sci.c \
../src/demo_src/LCD/r_cg_sci_user.c 

COMPILER_OBJS += \
src/demo_src/LCD/ascii.obj \
src/demo_src/LCD/lcd.obj \
src/demo_src/LCD/r_cg_sci.obj \
src/demo_src/LCD/r_cg_sci_user.obj 

C_DEPS += \
src/demo_src/LCD/ascii.d \
src/demo_src/LCD/lcd.d \
src/demo_src/LCD/r_cg_sci.d \
src/demo_src/LCD/r_cg_sci_user.d 

# Each subdirectory must supply rules for building sources it contributes
src/demo_src/LCD/%.obj: ../src/demo_src/LCD/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\demo_src\LCD\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\demo_src\LCD\cSubCommand.tmp" "$<"


