# Patch development for the SIPODEV SP1064 touchpad

known to be used in:
* Trekstor Primebook C11, C13
* Teclast F6 Pro, F7
* Direkt-Tek DTLAPY116-2
* Mediacom Flexbook edge 11


Note: This patch is upstream since Linux v4.19, so please ignore the
contents of this repo.

Commit: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit?id=9ee3e06610fdb8a601cde59c92089fb6c1deb4aa

Since people are still finding notebooks with this touchpad in the
wild, a few quick hints on how to check if your notebook model is
affected too:
* If your error message in dmesg is something like that, it's a strong
  indicator that you are affected too:
    i2c_hid i2c-SYNA3602:00: unexpected HID descriptor bcdVersion (0x00ff)
* Get yourself some kernel sources, e.g. from your distro or the
  official ones from kernel.org
* Edit the file drivers/hid/i2c-hid/i2c-hid-dmi-quirks.c in the kernel
  source and look for the i2c_hid_dmi_desc_override_table.
* Insert an entry similar to the already exisiting ones, where .ident
  is a human readable name for your notebook and the two strings in
  .matches are the outputs of
    cat /sys/class/dmi/id/sys_vendor
  and
    cat /sys/class/dmi/id/product_name
* Compile the kernel with your preferred method
* If it works, send me an email or open an issue or something like
  that, and I will send your entry to the mainline kernel
* If you are feeling adventurous, you can also try compiling only the
  i2c-hid module and insmod -f it, but make sure if you have the same
  kernel source as you are currently running
