################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/Lepton/Src/CCI.c \
../Core/BSP/Lepton/Src/CCI_AGC.c \
../Core/BSP/Lepton/Src/CCI_OEM.c \
../Core/BSP/Lepton/Src/CCI_RAD.c \
../Core/BSP/Lepton/Src/CCI_SYS.c \
../Core/BSP/Lepton/Src/CCI_VID.c \
../Core/BSP/Lepton/Src/Lepton.c \
../Core/BSP/Lepton/Src/Telemetry.c \
../Core/BSP/Lepton/Src/VoSPI.c 

OBJS += \
./Core/BSP/Lepton/Src/CCI.o \
./Core/BSP/Lepton/Src/CCI_AGC.o \
./Core/BSP/Lepton/Src/CCI_OEM.o \
./Core/BSP/Lepton/Src/CCI_RAD.o \
./Core/BSP/Lepton/Src/CCI_SYS.o \
./Core/BSP/Lepton/Src/CCI_VID.o \
./Core/BSP/Lepton/Src/Lepton.o \
./Core/BSP/Lepton/Src/Telemetry.o \
./Core/BSP/Lepton/Src/VoSPI.o 

C_DEPS += \
./Core/BSP/Lepton/Src/CCI.d \
./Core/BSP/Lepton/Src/CCI_AGC.d \
./Core/BSP/Lepton/Src/CCI_OEM.d \
./Core/BSP/Lepton/Src/CCI_RAD.d \
./Core/BSP/Lepton/Src/CCI_SYS.d \
./Core/BSP/Lepton/Src/CCI_VID.d \
./Core/BSP/Lepton/Src/Lepton.d \
./Core/BSP/Lepton/Src/Telemetry.d \
./Core/BSP/Lepton/Src/VoSPI.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/Lepton/Src/%.o Core/BSP/Lepton/Src/%.su Core/BSP/Lepton/Src/%.cyclo: ../Core/BSP/Lepton/Src/%.c Core/BSP/Lepton/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DDATA_IN_D2_SRAM -DSTM32H723xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/Lepton/Inc" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/Inc" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi/inc" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi/src" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi/inc/scpi" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-Lepton-2f-Src

clean-Core-2f-BSP-2f-Lepton-2f-Src:
	-$(RM) ./Core/BSP/Lepton/Src/CCI.cyclo ./Core/BSP/Lepton/Src/CCI.d ./Core/BSP/Lepton/Src/CCI.o ./Core/BSP/Lepton/Src/CCI.su ./Core/BSP/Lepton/Src/CCI_AGC.cyclo ./Core/BSP/Lepton/Src/CCI_AGC.d ./Core/BSP/Lepton/Src/CCI_AGC.o ./Core/BSP/Lepton/Src/CCI_AGC.su ./Core/BSP/Lepton/Src/CCI_OEM.cyclo ./Core/BSP/Lepton/Src/CCI_OEM.d ./Core/BSP/Lepton/Src/CCI_OEM.o ./Core/BSP/Lepton/Src/CCI_OEM.su ./Core/BSP/Lepton/Src/CCI_RAD.cyclo ./Core/BSP/Lepton/Src/CCI_RAD.d ./Core/BSP/Lepton/Src/CCI_RAD.o ./Core/BSP/Lepton/Src/CCI_RAD.su ./Core/BSP/Lepton/Src/CCI_SYS.cyclo ./Core/BSP/Lepton/Src/CCI_SYS.d ./Core/BSP/Lepton/Src/CCI_SYS.o ./Core/BSP/Lepton/Src/CCI_SYS.su ./Core/BSP/Lepton/Src/CCI_VID.cyclo ./Core/BSP/Lepton/Src/CCI_VID.d ./Core/BSP/Lepton/Src/CCI_VID.o ./Core/BSP/Lepton/Src/CCI_VID.su ./Core/BSP/Lepton/Src/Lepton.cyclo ./Core/BSP/Lepton/Src/Lepton.d ./Core/BSP/Lepton/Src/Lepton.o ./Core/BSP/Lepton/Src/Lepton.su ./Core/BSP/Lepton/Src/Telemetry.cyclo ./Core/BSP/Lepton/Src/Telemetry.d ./Core/BSP/Lepton/Src/Telemetry.o ./Core/BSP/Lepton/Src/Telemetry.su ./Core/BSP/Lepton/Src/VoSPI.cyclo ./Core/BSP/Lepton/Src/VoSPI.d ./Core/BSP/Lepton/Src/VoSPI.o ./Core/BSP/Lepton/Src/VoSPI.su

.PHONY: clean-Core-2f-BSP-2f-Lepton-2f-Src

