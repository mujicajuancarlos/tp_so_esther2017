################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuration.c \
../src/console.c \
../src/handler-kernel.c \
../src/handler-program.c \
../src/handler-user.c \
../src/program.c 

OBJS += \
./src/configuration.o \
./src/console.o \
./src/handler-kernel.o \
./src/handler-program.o \
./src/handler-user.o \
./src/program.o 

C_DEPS += \
./src/configuration.d \
./src/console.d \
./src/handler-kernel.d \
./src/handler-program.d \
./src/handler-user.d \
./src/program.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Desarrollo/workspace/esther2017/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


