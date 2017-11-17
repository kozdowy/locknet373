################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../contact_switch.c \
../main.c \
../neopixel.c \
../neopixel_test.c \
../servo.c 

OBJS += \
./contact_switch.o \
./main.o \
./neopixel.o \
./neopixel_test.o \
./servo.o 

C_DEPS += \
./contact_switch.d \
./main.d \
./neopixel.d \
./neopixel_test.d \
./servo.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\CMSIS -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IN:\PortableGit\locknet373\smartfusion\lockNET_SF\SoftConsole\lockNET_SF_MSS_MSS_CM3_0\lockNET_SF_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


