################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuration/configuration.c 

OBJS += \
./src/configuration/configuration.o 

C_DEPS += \
./src/configuration/configuration.d 


# Each subdirectory must supply rules for building sources it contributes
src/configuration/%.o: ../src/configuration/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


