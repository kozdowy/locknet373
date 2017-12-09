################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_spi/drivers/mss_spi/mss_spi.c 

OBJS += \
./drivers/mss_spi/drivers/mss_spi/mss_spi.o 

C_DEPS += \
./drivers/mss_spi/drivers/mss_spi/mss_spi.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_spi/drivers/mss_spi/%.o: ../drivers/mss_spi/drivers/mss_spi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -DACTEL_STDIO_THRU_UART -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\CMSIS -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IN:\Git\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O1 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


