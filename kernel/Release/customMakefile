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

# Tool invocations
kernel: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/esther2017/dc-commons/Release" -o "kernel" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) kernel
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
