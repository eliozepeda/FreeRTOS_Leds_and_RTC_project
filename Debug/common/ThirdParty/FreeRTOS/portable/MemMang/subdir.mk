################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.o: C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.c common/ThirdParty/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/FreeRTOS/include" -I"C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/SEGGER/Config" -I"C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/SEGGER/OS" -I"C:/Users/ezepedasanchez/Documents/Elio/Cursos/FREERTOS/workspace/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-common-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

clean-common-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.cyclo ./common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.d ./common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.o ./common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.su

.PHONY: clean-common-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

