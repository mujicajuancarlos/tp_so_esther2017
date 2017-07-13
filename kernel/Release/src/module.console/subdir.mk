################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.console/consoleRequests.c \
../src/module.console/consoleResponse.c \
../src/module.console/handler-console.c 

OBJS += \
./src/module.console/consoleRequests.o \
./src/module.console/consoleResponse.o \
./src/module.console/handler-console.o 

C_DEPS += \
./src/module.console/consoleRequests.d \
./src/module.console/consoleResponse.d \
./src/module.console/handler-console.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.console/%.o: ../src/module.console/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


