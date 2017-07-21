################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.model/cpu.c \
../src/module.model/kernel.c \
../src/module.model/memoryClient.c 

OBJS += \
./src/module.model/cpu.o \
./src/module.model/kernel.o \
./src/module.model/memoryClient.o 

C_DEPS += \
./src/module.model/cpu.d \
./src/module.model/kernel.d \
./src/module.model/memoryClient.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.model/%.o: ../src/module.model/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-1c-Los-5-Fant-sticos/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


