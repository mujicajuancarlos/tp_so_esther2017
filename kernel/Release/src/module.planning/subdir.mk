################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.planning/handler-planning.c \
../src/module.planning/longTermScheduler.c \
../src/module.planning/processLifeCycle.c \
../src/module.planning/scheduler.c \
../src/module.planning/shortTermScheduler.c \
../src/module.planning/stateConverter.c 

OBJS += \
./src/module.planning/handler-planning.o \
./src/module.planning/longTermScheduler.o \
./src/module.planning/processLifeCycle.o \
./src/module.planning/scheduler.o \
./src/module.planning/shortTermScheduler.o \
./src/module.planning/stateConverter.o 

C_DEPS += \
./src/module.planning/handler-planning.d \
./src/module.planning/longTermScheduler.d \
./src/module.planning/processLifeCycle.d \
./src/module.planning/scheduler.d \
./src/module.planning/shortTermScheduler.d \
./src/module.planning/stateConverter.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.planning/%.o: ../src/module.planning/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


