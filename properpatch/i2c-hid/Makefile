#
# Makefile for the I2C input drivers
#

obj-$(CONFIG_I2C_HID)	+= i2c-hid-temp.o 

i2c-hid-temp-objs	=  i2c-hid.o

ifeq ($(ARCH),x86)
i2c-hid-temp-objs	+= i2c-hid-dmi-quirks.o
endif
