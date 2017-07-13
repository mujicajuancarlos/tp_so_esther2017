################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.core/sadicaBlock.c \
../src/module.core/sadicaCore.c \
../src/module.core/sadicaFile.c \
../src/module.core/sadicaManager.c 

OBJS += \
./src/module.core/sadicaBlock.o \
./src/module.core/sadicaCore.o \
./src/module.core/sadicaFile.o \
./src/module.core/sadicaManager.o 

C_DEPS += \
./src/module.core/sadicaBlock.d \
./src/module.core/sadicaCore.d \
./src/module.core/sadicaFile.d \
./src/module.core/sadicaManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.core/%.o: ../src/module.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


