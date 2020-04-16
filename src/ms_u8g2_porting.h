/*
 * Copyright (c) 2019 MS-RTOS Team.
 * All rights reserved.
 *
 * Detailed license information can be found in the LICENSE file.
 *
 * File: ms_u8g2_porting.h u8g2 porting.
 *
 * Author: Jiao.jinxing <jiaojixing@acoinfo.com>
 *
 */

#ifndef MS_U8G2_PORTING_H
#define MS_U8G2_PORTING_H

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ms_u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t ms_u8x8_gpio_and_delay_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void    ms_u8x8_i2c_dev_set(const char *i2c_dev);
void    ms_u8x8_i2c_address_set(ms_uint16_t i2c_addr);

#ifdef __cplusplus
}
#endif

#endif /* MS_U8G2_PORTING_H */
