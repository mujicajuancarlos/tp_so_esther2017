################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.core/cacheAdministrator.c \
../src/module.core/hashing.c \
../src/module.core/pageAdministrator.c 

OBJS += \
./src/module.core/cacheAdministrator.o \
./src/module.core/hashing.o \
./src/module.core/pageAdministrator.o 

C_DEPS += \
./src/module.core/cacheAdministrator.d \
./src/module.core/hashing.d \
./src/module.core/pageAdministrator.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.core/%.o: ../src/module.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


