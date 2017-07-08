################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.core/cpu-core.c \
../src/module.core/signalHandler.c 

OBJS += \
./src/module.core/cpu-core.o \
./src/module.core/signalHandler.o 

C_DEPS += \
./src/module.core/cpu-core.d \
./src/module.core/signalHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.core/%.o: ../src/module.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Desarrollo/workspace/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


