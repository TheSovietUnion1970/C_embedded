################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_tfat_rx/src/ff.c \
../src/smc_gen/r_tfat_rx/src/ffsystem.c \
../src/smc_gen/r_tfat_rx/src/ffunicode.c 

COMPILER_OBJS += \
src/smc_gen/r_tfat_rx/src/ff.obj \
src/smc_gen/r_tfat_rx/src/ffsystem.obj \
src/smc_gen/r_tfat_rx/src/ffunicode.obj 

C_DEPS += \
src/smc_gen/r_tfat_rx/src/ff.d \
src/smc_gen/r_tfat_rx/src/ffsystem.d \
src/smc_gen/r_tfat_rx/src/ffunicode.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_tfat_rx/src/%.obj: ../src/smc_gen/r_tfat_rx/src/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_tfat_rx\src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_tfat_rx\src\cSubCommand.tmp" "$<"


