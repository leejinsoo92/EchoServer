################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Mng/UserMng/User/User.cpp 

OBJS += \
./src/Mng/UserMng/User/User.o 

CPP_DEPS += \
./src/Mng/UserMng/User/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/Mng/UserMng/User/%.o: ../src/Mng/UserMng/User/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


