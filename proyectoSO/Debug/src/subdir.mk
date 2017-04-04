################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Consola.c \
../src/Cpu.c \
../src/FIleSystem.c \
../src/Kernel.c \
../src/Memoria.c \
../src/proyectoSO.c 

OBJS += \
./src/Consola.o \
./src/Cpu.o \
./src/FIleSystem.o \
./src/Kernel.o \
./src/Memoria.o \
./src/proyectoSO.o 

C_DEPS += \
./src/Consola.d \
./src/Cpu.d \
./src/FIleSystem.d \
./src/Kernel.d \
./src/Memoria.d \
./src/proyectoSO.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


