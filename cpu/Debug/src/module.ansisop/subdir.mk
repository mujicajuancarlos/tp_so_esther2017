################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.ansisop/primitiveFunctions.c \
../src/module.ansisop/primitiveKernelFuncions.c 

OBJS += \
./src/module.ansisop/primitiveFunctions.o \
./src/module.ansisop/primitiveKernelFuncions.o 

C_DEPS += \
./src/module.ansisop/primitiveFunctions.d \
./src/module.ansisop/primitiveKernelFuncions.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.ansisop/%.o: ../src/module.ansisop/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-1c-Los-5-Fant-sticos/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


