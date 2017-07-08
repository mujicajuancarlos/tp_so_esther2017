################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.memory/dynamicMemoryManager.c \
../src/module.memory/heapMetadata.c \
../src/module.memory/memoryRequests.c 

OBJS += \
./src/module.memory/dynamicMemoryManager.o \
./src/module.memory/heapMetadata.o \
./src/module.memory/memoryRequests.o 

C_DEPS += \
./src/module.memory/dynamicMemoryManager.d \
./src/module.memory/heapMetadata.d \
./src/module.memory/memoryRequests.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.memory/%.o: ../src/module.memory/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


