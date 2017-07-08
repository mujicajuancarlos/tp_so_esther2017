################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.cpu/cpuRequest.c \
../src/module.cpu/cpuResponse.c \
../src/module.cpu/handler-cpu.c 

OBJS += \
./src/module.cpu/cpuRequest.o \
./src/module.cpu/cpuResponse.o \
./src/module.cpu/handler-cpu.o 

C_DEPS += \
./src/module.cpu/cpuRequest.d \
./src/module.cpu/cpuResponse.d \
./src/module.cpu/handler-cpu.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.cpu/%.o: ../src/module.cpu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Desarrollo/workspace/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


