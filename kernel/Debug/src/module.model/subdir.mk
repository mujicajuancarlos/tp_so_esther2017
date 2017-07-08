################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.model/cpu.c \
../src/module.model/fileDescriptor.c \
../src/module.model/process.c \
../src/module.model/processFinder.c \
../src/module.model/sharedSemaphore.c \
../src/module.model/sharedVariables.c 

OBJS += \
./src/module.model/cpu.o \
./src/module.model/fileDescriptor.o \
./src/module.model/process.o \
./src/module.model/processFinder.o \
./src/module.model/sharedSemaphore.o \
./src/module.model/sharedVariables.o 

C_DEPS += \
./src/module.model/cpu.d \
./src/module.model/fileDescriptor.d \
./src/module.model/process.d \
./src/module.model/processFinder.d \
./src/module.model/sharedSemaphore.d \
./src/module.model/sharedVariables.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.model/%.o: ../src/module.model/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


