################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/Src/BSP.c \
../Core/BSP/Src/EE24.c \
../Core/BSP/Src/EEPROM.c \
../Core/BSP/Src/LED.c \
../Core/BSP/Src/RTP.c \
../Core/BSP/Src/SCPI_Def.c \
../Core/BSP/Src/SCPI_LEP_IMG.c \
../Core/BSP/Src/SCPI_LEP_OEM.c \
../Core/BSP/Src/SCPI_LEP_SYS.c \
../Core/BSP/Src/SCPI_LEP_VID.c \
../Core/BSP/Src/SCPI_Server.c \
../Core/BSP/Src/SCPI_System.c \
../Core/BSP/Src/utility.c 

OBJS += \
./Core/BSP/Src/BSP.o \
./Core/BSP/Src/EE24.o \
./Core/BSP/Src/EEPROM.o \
./Core/BSP/Src/LED.o \
./Core/BSP/Src/RTP.o \
./Core/BSP/Src/SCPI_Def.o \
./Core/BSP/Src/SCPI_LEP_IMG.o \
./Core/BSP/Src/SCPI_LEP_OEM.o \
./Core/BSP/Src/SCPI_LEP_SYS.o \
./Core/BSP/Src/SCPI_LEP_VID.o \
./Core/BSP/Src/SCPI_Server.o \
./Core/BSP/Src/SCPI_System.o \
./Core/BSP/Src/utility.o 

C_DEPS += \
./Core/BSP/Src/BSP.d \
./Core/BSP/Src/EE24.d \
./Core/BSP/Src/EEPROM.d \
./Core/BSP/Src/LED.d \
./Core/BSP/Src/RTP.d \
./Core/BSP/Src/SCPI_Def.d \
./Core/BSP/Src/SCPI_LEP_IMG.d \
./Core/BSP/Src/SCPI_LEP_OEM.d \
./Core/BSP/Src/SCPI_LEP_SYS.d \
./Core/BSP/Src/SCPI_LEP_VID.d \
./Core/BSP/Src/SCPI_Server.d \
./Core/BSP/Src/SCPI_System.d \
./Core/BSP/Src/utility.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/Src/%.o Core/BSP/Src/%.su Core/BSP/Src/%.cyclo: ../Core/BSP/Src/%.c Core/BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DDATA_IN_D2_SRAM -DUSE_HAL_DRIVER -DSTM32H723xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/Lepton/Inc" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/Inc" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi/inc" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi/src" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi" -I"/home/grzegorz/git/ETH1IRCAM1/SOFTWARE/Core/BSP/SCPI/libscpi/inc/scpi" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-Src

clean-Core-2f-BSP-2f-Src:
	-$(RM) ./Core/BSP/Src/BSP.cyclo ./Core/BSP/Src/BSP.d ./Core/BSP/Src/BSP.o ./Core/BSP/Src/BSP.su ./Core/BSP/Src/EE24.cyclo ./Core/BSP/Src/EE24.d ./Core/BSP/Src/EE24.o ./Core/BSP/Src/EE24.su ./Core/BSP/Src/EEPROM.cyclo ./Core/BSP/Src/EEPROM.d ./Core/BSP/Src/EEPROM.o ./Core/BSP/Src/EEPROM.su ./Core/BSP/Src/LED.cyclo ./Core/BSP/Src/LED.d ./Core/BSP/Src/LED.o ./Core/BSP/Src/LED.su ./Core/BSP/Src/RTP.cyclo ./Core/BSP/Src/RTP.d ./Core/BSP/Src/RTP.o ./Core/BSP/Src/RTP.su ./Core/BSP/Src/SCPI_Def.cyclo ./Core/BSP/Src/SCPI_Def.d ./Core/BSP/Src/SCPI_Def.o ./Core/BSP/Src/SCPI_Def.su ./Core/BSP/Src/SCPI_LEP_IMG.cyclo ./Core/BSP/Src/SCPI_LEP_IMG.d ./Core/BSP/Src/SCPI_LEP_IMG.o ./Core/BSP/Src/SCPI_LEP_IMG.su ./Core/BSP/Src/SCPI_LEP_OEM.cyclo ./Core/BSP/Src/SCPI_LEP_OEM.d ./Core/BSP/Src/SCPI_LEP_OEM.o ./Core/BSP/Src/SCPI_LEP_OEM.su ./Core/BSP/Src/SCPI_LEP_SYS.cyclo ./Core/BSP/Src/SCPI_LEP_SYS.d ./Core/BSP/Src/SCPI_LEP_SYS.o ./Core/BSP/Src/SCPI_LEP_SYS.su ./Core/BSP/Src/SCPI_LEP_VID.cyclo ./Core/BSP/Src/SCPI_LEP_VID.d ./Core/BSP/Src/SCPI_LEP_VID.o ./Core/BSP/Src/SCPI_LEP_VID.su ./Core/BSP/Src/SCPI_Server.cyclo ./Core/BSP/Src/SCPI_Server.d ./Core/BSP/Src/SCPI_Server.o ./Core/BSP/Src/SCPI_Server.su ./Core/BSP/Src/SCPI_System.cyclo ./Core/BSP/Src/SCPI_System.d ./Core/BSP/Src/SCPI_System.o ./Core/BSP/Src/SCPI_System.su ./Core/BSP/Src/utility.cyclo ./Core/BSP/Src/utility.d ./Core/BSP/Src/utility.o ./Core/BSP/Src/utility.su

.PHONY: clean-Core-2f-BSP-2f-Src

