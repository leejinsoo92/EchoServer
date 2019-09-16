################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Mng/EpollMng/UserMng/UserMng.cpp 

OBJS += \
./src/Mng/EpollMng/UserMng/UserMng.o 

CPP_DEPS += \
./src/Mng/EpollMng/UserMng/UserMng.d 


# Each subdirectory must supply rules for building sources it contributes
src/Mng/EpollMng/UserMng/%.o: ../src/Mng/EpollMng/UserMng/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


