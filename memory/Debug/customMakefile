################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/module.userInterface/subdir.mk
-include src/module.model/subdir.mk
-include src/module.kernel/subdir.mk
-include src/module.cpu/subdir.mk
-include src/module.core/subdir.mk
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
all: memory

dependents:
	-cd ../../dc-commons/Release && $(MAKE) clean all

# Tool invocations
memory: $(OBJS) $(USER_OBJS) ../../dc-commons/Release/libdc-commons.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"../../dc-commons/Debug" -o "memory" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) memory
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
../../dc-commons/Release/libdc-commons.so:

-include ../makefile.targets
