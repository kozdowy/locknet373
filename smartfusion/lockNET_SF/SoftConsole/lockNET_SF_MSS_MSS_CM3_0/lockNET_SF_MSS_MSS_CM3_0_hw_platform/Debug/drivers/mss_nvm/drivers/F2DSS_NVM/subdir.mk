################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_nvm/drivers/F2DSS_NVM/mss_nvm.c 

OBJS += \
./drivers/mss_nvm/drivers/F2DSS_NVM/mss_nvm.o 

C_DEPS += \
./drivers/mss_nvm/drivers/F2DSS_NVM/mss_nvm.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_nvm/drivers/F2DSS_NVM/%.o: ../drivers/mss_nvm/drivers/F2DSS_NVM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -DACTEL_STDIO_THRU_UART -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\CMSIS -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O1 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


