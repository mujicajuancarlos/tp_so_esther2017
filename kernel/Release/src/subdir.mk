################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/commonFunctions.c \
../src/handler-console.c \
../src/handler-cpu.c \
../src/handler-memory.c \
../src/kernel.c 

OBJS += \
./src/commonFunctions.o \
./src/handler-console.o \
./src/handler-cpu.o \
./src/handler-memory.o \
./src/kernel.o 

C_DEPS += \
./src/commonFunctions.d \
./src/handler-console.d \
./src/handler-cpu.d \
./src/handler-memory.d \
./src/kernel.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


