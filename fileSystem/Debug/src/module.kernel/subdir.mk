################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.kernel/handler-kernel.c \
../src/module.kernel/kernelRequest.c 

OBJS += \
./src/module.kernel/handler-kernel.o \
./src/module.kernel/kernelRequest.o 

C_DEPS += \
./src/module.kernel/handler-kernel.d \
./src/module.kernel/kernelRequest.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.kernel/%.o: ../src/module.kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


