################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/module.userInterface/subdir.mk
-include src/module.planning/subdir.mk
-include src/module.model/subdir.mk
-include src/module.memory/subdir.mk
-include src/module.fileSystem/subdir.mk
-include src/module.cpu/subdir.mk
-include src/module.console/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: kernel

dependents:
	-cd /home/utnso/tp-2017-1c-Los-5-Fant-sticos/dc-commons/Debug && $(MAKE) all

# Tool invocations
kernel: $(OBJS) $(USER_OBJS) /home/utnso/tp-2017-1c-Los-5-Fant-sticos/dc-commons/Debug/libdc-commons.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2017-1c-Los-5-Fant-sticos/dc-commons/Debug" -o "kernel" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) kernel
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2017-1c-Los-5-Fant-sticos/dc-commons/Debug/libdc-commons.so:

-include ../makefile.targets
