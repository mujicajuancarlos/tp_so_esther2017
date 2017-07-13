################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.userInterface/handler-user.c 

OBJS += \
./src/module.userInterface/handler-user.o 

C_DEPS += \
./src/module.userInterface/handler-user.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.userInterface/%.o: ../src/module.userInterface/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


