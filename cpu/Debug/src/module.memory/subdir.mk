################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.memory/memoryRequest.c \
../src/module.memory/memoryResponse.c 

OBJS += \
./src/module.memory/memoryRequest.o \
./src/module.memory/memoryResponse.o 

C_DEPS += \
./src/module.memory/memoryRequest.d \
./src/module.memory/memoryResponse.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.memory/%.o: ../src/module.memory/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Desarrollo/workspace/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


