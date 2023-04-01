################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MCAL/Clock_Control.c \
../src/MCAL/GPIO.c \
../src/MCAL/NVIC.c \
../src/MCAL/RCC.c \
../src/MCAL/SCB.c \
../src/MCAL/SysTick.c \
../src/MCAL/USART.c 

OBJS += \
./src/MCAL/Clock_Control.o \
./src/MCAL/GPIO.o \
./src/MCAL/NVIC.o \
./src/MCAL/RCC.o \
./src/MCAL/SCB.o \
./src/MCAL/SysTick.o \
./src/MCAL/USART.o 

C_DEPS += \
./src/MCAL/Clock_Control.d \
./src/MCAL/GPIO.d \
./src/MCAL/NVIC.d \
./src/MCAL/RCC.d \
./src/MCAL/SCB.d \
./src/MCAL/SysTick.d \
./src/MCAL/USART.d 


# Each subdirectory must supply rules for building sources it contributes
src/MCAL/%.o: ../src/MCAL/%.c src/MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"D:\ITI\Tourkey\Projects\STM32F401\include\Sched" -I"D:\ITI\Tourkey\Projects\STM32F401\include\HAL" -I"D:\ITI\Tourkey\Projects\STM32F401\include\LIB" -I"D:\ITI\Tourkey\Projects\STM32F401\include\MCAL" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


