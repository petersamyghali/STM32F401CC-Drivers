################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/APP2.c \
../src/Init.c \
../src/LCD_Task.c \
../src/Switch_App.c \
../src/Switch_Task.c \
../src/USART.c \
../src/main.c 

OBJS += \
./src/APP2.o \
./src/Init.o \
./src/LCD_Task.o \
./src/Switch_App.o \
./src/Switch_Task.o \
./src/USART.o \
./src/main.o 

C_DEPS += \
./src/APP2.d \
./src/Init.d \
./src/LCD_Task.d \
./src/Switch_App.d \
./src/Switch_Task.d \
./src/USART.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"D:\ITI\Tourkey\Projects\STM32F401\include\Sched" -I"D:\ITI\Tourkey\Projects\STM32F401\include\HAL" -I"D:\ITI\Tourkey\Projects\STM32F401\include\LIB" -I"D:\ITI\Tourkey\Projects\STM32F401\include\MCAL" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


