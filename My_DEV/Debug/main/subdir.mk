################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main/Ble.c \
../main/Connections.c \
../main/ControlFun.c \
../main/My_Main.c \
../main/TCP_Server.c \
../main/Tcp_Client.c \
../main/UDP_Server.c \
../main/mailbox_main.c \
../main/wifi.c 

OBJS += \
./main/Ble.o \
./main/Connections.o \
./main/ControlFun.o \
./main/My_Main.o \
./main/TCP_Server.o \
./main/Tcp_Client.o \
./main/UDP_Server.o \
./main/mailbox_main.o \
./main/wifi.o 

C_DEPS += \
./main/Ble.d \
./main/Connections.d \
./main/ControlFun.d \
./main/My_Main.d \
./main/TCP_Server.d \
./main/Tcp_Client.d \
./main/UDP_Server.d \
./main/mailbox_main.d \
./main/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
main/%.o: ../main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


