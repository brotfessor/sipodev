#ifndef I2C_HID_H
#define I2C_HID_H


#ifdef CONFIG_X86
struct i2c_hid_desc *i2c_hid_get_dmi_i2c_hid_desc_override(void);
char *i2c_hid_get_dmi_hid_report_desc_override(unsigned int *size);
#else
static inline struct i2c_hid_desc *i2c_hid_get_dmi_i2c_hid_desc_override(void)
{ return NULL; }
static inline char *i2c_hid_get_dmi_hid_report_desc_override(unsigned int *size)
{ return NULL; }
#endif

#endif
