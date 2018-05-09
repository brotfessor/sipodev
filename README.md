# Work in progress patch for the SIPODEV SP1064 touchpad

known to be used in:
* Trekstor Primebook C11, C13
* Teclast F6 Pro, F7

i2ctest.c was an attempt to decode the message without using a kernel driver

In 'b' there is a modified i2c-hid.c that simply overrides the descriptors to make the touchpad work  
sipodev-quirk.patch is a patch with the modifications__
This patch does not check for the touchpad or laptop or anything.

The development for the upstream patch is occuring in properpatch; this driver does already check for laptops known to use the touchpad.


