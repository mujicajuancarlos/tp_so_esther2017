################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/dc-commons/config.c \
../src/dc-commons/logger.c \
../src/dc-commons/package.c \
../src/dc-commons/protocol-fileSystem-kernel.c \
../src/dc-commons/protocol-kernel-cpu.c \
../src/dc-commons/protocol-memory-core.c \
../src/dc-commons/protocol-memory-kernel.c \
../src/dc-commons/serialization.c \
../src/dc-commons/socket-client.c \
../src/dc-commons/socket-message.c \
../src/dc-commons/socket-server.c \
../src/dc-commons/stdout-printer.c \
../src/dc-commons/utils.c 

OBJS += \
./src/dc-commons/config.o \
./src/dc-commons/logger.o \
./src/dc-commons/package.o \
./src/dc-commons/protocol-fileSystem-kernel.o \
./src/dc-commons/protocol-kernel-cpu.o \
./src/dc-commons/protocol-memory-core.o \
./src/dc-commons/protocol-memory-kernel.o \
./src/dc-commons/serialization.o \
./src/dc-commons/socket-client.o \
./src/dc-commons/socket-message.o \
./src/dc-commons/socket-server.o \
./src/dc-commons/stdout-printer.o \
./src/dc-commons/utils.o 

C_DEPS += \
./src/dc-commons/config.d \
./src/dc-commons/logger.d \
./src/dc-commons/package.d \
./src/dc-commons/protocol-fileSystem-kernel.d \
./src/dc-commons/protocol-kernel-cpu.d \
./src/dc-commons/protocol-memory-core.d \
./src/dc-commons/protocol-memory-kernel.d \
./src/dc-commons/serialization.d \
./src/dc-commons/socket-client.d \
./src/dc-commons/socket-message.d \
./src/dc-commons/socket-server.d \
./src/dc-commons/stdout-printer.d \
./src/dc-commons/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/dc-commons/%.o: ../src/dc-commons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


