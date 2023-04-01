################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HAL/LCD.c \
../src/HAL/LCD_Cfg.c \
../src/HAL/LED.c \
../src/HAL/LED_Cfg.c \
../src/HAL/SSegment.c \
../src/HAL/SSegment_Cfg.c \
../src/HAL/Switch.c \
../src/HAL/Switch_Cfg.c 

OBJS += \
./src/HAL/LCD.o \
./src/HAL/LCD_Cfg.o \
./src/HAL/LED.o \
./src/HAL/LED_Cfg.o \
./src/HAL/SSegment.o \
./src/HAL/SSegment_Cfg.o \
./src/HAL/Switch.o \
./src/HAL/Switch_Cfg.o 

C_DEPS += \
./src/HAL/LCD.d \
./src/HAL/LCD_Cfg.d \
./src/HAL/LED.d \
./src/HAL/LED_Cfg.d \
./src/HAL/SSegment.d \
./src/HAL/SSegment_Cfg.d \
./src/HAL/Switch.d \
./src/HAL/Switch_Cfg.d 


# Each subdirectory must supply rules for building sources it contributes
src/HAL/%.o: ../src/HAL/%.c src/HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"D:\ITI\Tourkey\Projects\STM32F401\include\Sched" -I"D:\ITI\Tourkey\Projects\STM32F401\include\HAL" -I"D:\ITI\Tourkey\Projects\STM32F401\include\LIB" -I"D:\ITI\Tourkey\Projects\STM32F401\include\MCAL" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


