/*
 * Copyright (c) 2019 MS-RTOS Team.
 * All rights reserved.
 *
 * Detailed license information can be found in the LICENSE file.
 *
 * File: ms_u8g2_porting.c u8g2 porting.
 *
 * Author: Jiao.jinxing <jiaojixing@acoinfo.com>
 *
 */

#include <ms_rtos.h>
#include "u8x8.h"

static const char *g_i2c_dev = "/dev/i2c0";
static ms_uint16_t g_i2c_addr = 0;

void ms_u8x8_i2c_dev_set(const char *i2c_dev)
{
    g_i2c_dev = i2c_dev;
}

void ms_u8x8_i2c_address_set(ms_uint16_t i2c_addr)
{
    g_i2c_addr = i2c_addr;
}

uint8_t ms_u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static int fd = -1;
    static ms_uint8_t data[32];
    static ms_fifo_t fifo;

    switch (msg) {
    case U8X8_MSG_BYTE_SEND:
        ms_fifo_put(&fifo, arg_ptr, arg_int);
        break;

    case U8X8_MSG_BYTE_INIT:
        if (fd < 0) {
            fd = ms_io_open(g_i2c_dev, O_RDWR, 0666);
            ms_fifo_init(&fifo, data, sizeof(data));
        }
        break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        ms_fifo_reset(&fifo);
        break;

    case U8X8_MSG_BYTE_END_TRANSFER: {
           ms_i2c_msg_t msg;

           msg.addr  = (g_i2c_addr != 0) ? g_i2c_addr : u8x8_GetI2CAddress(u8x8);
           msg.flags = 0;
           msg.len   = ms_fifo_len(&fifo);
           msg.buf   = data;

           (void)ms_io_write(fd, &msg, sizeof(msg));
       }
        break;

    default:
        return 0;
    }

    return 1;
}

uint8_t ms_u8x8_gpio_and_delay_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        /* only support for software I2C*/
        break;

    case U8X8_MSG_DELAY_NANO:
        /* not required for SW I2C */
        break;

    case U8X8_MSG_DELAY_10MICRO:
        /* not used at the moment */
        break;

    case U8X8_MSG_DELAY_100NANO:
        /* not used at the moment */
        break;

    case U8X8_MSG_DELAY_MILLI:
        break;

    case U8X8_MSG_DELAY_I2C:
        break;

    case U8X8_MSG_GPIO_I2C_CLOCK:
        break;

    case U8X8_MSG_GPIO_I2C_DATA:
        break;

    default:
        break;
    }

    return 1;
}
