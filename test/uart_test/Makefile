C_COMPILE ?= arm-linux-gnueabihf-gcc
CC_COMPILE ?= arm-linux-gnueabihf-g++
INCLUDES ?= /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include

c_objects = 
cpp_objects = uart_test.o

buildout = uart_test

run_dist = /home/lrq/linux/nfs/qtrootfs/home/module_test

ENV_PATH_ROOT := /home/lrq/Desktop/IMXControl

include $(ENV_PATH_ROOT)/scripts/build/build_exe.mk

