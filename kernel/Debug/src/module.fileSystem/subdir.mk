################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/module.fileSystem/fileSystemManager.c \
../src/module.fileSystem/fileSystemRequest.c 

OBJS += \
./src/module.fileSystem/fileSystemManager.o \
./src/module.fileSystem/fileSystemRequest.o 

C_DEPS += \
./src/module.fileSystem/fileSystemManager.d \
./src/module.fileSystem/fileSystemRequest.d 


# Each subdirectory must supply rules for building sources it contributes
src/module.fileSystem/%.o: ../src/module.fileSystem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2017-1c-Los-5-Fant-sticos/dc-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


