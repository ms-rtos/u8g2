/*
 * Copyright (c) 2015-2020 ACOINFO Co., Ltd.
 * All rights reserved.
 *
 * Detailed license information can be found in the LICENSE file.
 *
 * File: ms_u8g2_porting.h u8g2 porting.
 *
 * Author: Jiao.jinxing <jiaojinxing@acoinfo.com>
 *
 */

#ifndef MS_U8G2_PORTING_H
#define MS_U8G2_PORTING_H

#ifdef __cplusplus
extern "C" {
#endif

ms_uint8_t ms_u8x8_byte_hw_i2c(ms_uint8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr);
ms_uint8_t ms_u8x8_gpio_and_delay_hw_i2c(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr);
void       ms_u8x8_i2c_dev_set(const char *i2c_dev);
void       ms_u8x8_i2c_address_set(ms_uint16_t i2c_addr);

ms_uint8_t ms_u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr);
ms_uint8_t ms_u8x8_gpio_and_delay_hw_spi(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr);
void       ms_u8x8_spi_dev_set(const char *spi_dev, const char *gpio_cs, const char *gpio_dc);
void       ms_u8x8_spi_chip_select(const char *device_name);

#ifdef __cplusplus
}
#endif

#endif /* MS_U8G2_PORTING_H */
