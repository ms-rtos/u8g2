/*
 * Copyright (c) 2015-2020 ACOINFO Co., Ltd.
 * All rights reserved.
 *
 * Detailed license information can be found in the LICENSE file.
 *
 * File: ms_u8g2_porting.c u8g2 porting.
 *
 * Author: Jiao.jinxing <jiaojinxing@acoinfo.com>
 *
 */

#include <ms_rtos.h>
#include "u8x8.h"

/*********************************************************************************************************
  Hardware I2C Communication
*********************************************************************************************************/

static const char *ms_u8g2_i2c_dev = "/dev/i2c0";
static ms_uint16_t ms_u8g2_i2c_addr = 0;

void ms_u8x8_i2c_dev_set(const char *i2c_dev)
{
    ms_u8g2_i2c_dev = i2c_dev;
}

void ms_u8x8_i2c_address_set(ms_uint16_t i2c_addr)
{
    ms_u8g2_i2c_addr = i2c_addr;
}

ms_uint8_t ms_u8x8_byte_hw_i2c(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr)
{
    static int fd = -1;
    static ms_uint8_t fifo_buf[32];
    static ms_fifo_t  fifo;

    switch (msg) {
    case U8X8_MSG_BYTE_SEND:
        ms_fifo_put(&fifo, arg_ptr, arg_int);
        break;

    case U8X8_MSG_BYTE_INIT:
        if (fd < 0) {
            fd = ms_io_open(ms_u8g2_i2c_dev, O_RDWR, 0666);
            ms_fifo_init(&fifo, fifo_buf, sizeof(fifo_buf));
        }
        break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        ms_fifo_reset(&fifo);
        break;

    case U8X8_MSG_BYTE_END_TRANSFER: {
           ms_i2c_msg_t msg;

           msg.addr  = (ms_u8g2_i2c_addr != 0) ? ms_u8g2_i2c_addr : u8x8_GetI2CAddress(u8x8);
           msg.flags = 0;
           msg.len   = ms_fifo_len(&fifo);
           msg.buf   = fifo_buf;

           (void)ms_io_write(fd, &msg, sizeof(msg));
        }
        break;

    default:
        return 0;
    }

    return 1;
}

ms_uint8_t ms_u8x8_gpio_and_delay_hw_i2c(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr)
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

/*********************************************************************************************************
  Hardware SPI Communication
*********************************************************************************************************/

typedef struct {
    int         fd_spi;
    const char *dev_name;
    int         dev_fd;
    int         fd_cs;
    int         fd_dc;
    int         inited;
} ms_u8x8_spi_ctx_t;

static ms_u8x8_spi_ctx_t __u8x8_spi_ctx;

void ms_u8x8_spi_dev_set(const char *spi_dev, const char *gpio_cs, const char *gpio_dc)
{
    int fd_spi;
    int fd_cs;
    int fd_dc;
    ms_gpio_param_t param;

    fd_spi = ms_io_open(spi_dev, O_RDWR, 0666);
    fd_cs  = ms_io_open(gpio_cs, O_RDWR, 0666);
    fd_dc  = ms_io_open(gpio_dc, O_RDWR, 0666);

    param.mode  = MS_GPIO_MODE_OUTPUT_PP;
    param.pull  = MS_GPIO_PULL_UP;
    param.speed = MS_GPIO_SPEED_HIGH;
    ms_io_ioctl(fd_cs, MS_GPIO_CMD_SET_PARAM, &param);
    ms_io_ioctl(fd_dc, MS_GPIO_CMD_SET_PARAM, &param);

    __u8x8_spi_ctx.inited = MS_FALSE;
    __u8x8_spi_ctx.fd_spi = fd_spi;
    __u8x8_spi_ctx.fd_cs  = fd_cs;
    __u8x8_spi_ctx.fd_dc  = fd_dc;
}

void ms_u8x8_spi_chip_select(const char *device_name)
{
    int ret;
    int dev_fd;
    int fd_spi = __u8x8_spi_ctx.fd_spi;
    const char *node_name = MS_NULL;

    dev_fd = ms_io_open(device_name, O_RDWR, 0666);
    if (dev_fd > 0) {
        node_name = device_name + strlen("/dev/");
        ret = ms_io_ioctl(fd_spi, MS_SPI_CMD_SEL_DEV, (ms_ptr_t)node_name);
    } else {
        ret = -1;
    }

    if (ret == 0) {
        __u8x8_spi_ctx.inited = MS_TRUE;
    } else {
        __u8x8_spi_ctx.inited = MS_FALSE;
    }

    __u8x8_spi_ctx.dev_name = device_name;
}

ms_uint8_t ms_u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr)
{
    static int fd_spi = -1;
    static int fd_cs  = -1;
    static int fd_dc  = -1;

    if (__u8x8_spi_ctx.inited == MS_FALSE) {
        return 0;
    }

    if (fd_spi < 0) {
        fd_spi = __u8x8_spi_ctx.fd_spi;
        fd_cs  = __u8x8_spi_ctx.fd_cs ;
        fd_dc  = __u8x8_spi_ctx.fd_dc ;
    }

    switch (msg) {
    case U8X8_MSG_BYTE_SEND: {
        ms_spi_msg_t msg;
        ms_uint8_t   dump;

        msg.len    = arg_int;
        msg.tx_buf = arg_ptr;
        msg.rx_buf = &dump;
        msg.flags  = MS_SPI_M_ONCE | MS_SPI_M_RX_FIX | MS_SPI_M_WRITE;

        (void)ms_io_write(fd_spi, &msg, sizeof(msg));
    }
        break;

    case U8X8_MSG_BYTE_INIT: {
        ms_uint8_t status;
        status = u8x8->display_info->chip_disable_level;
        ms_io_write(fd_cs, &status, sizeof(status));
    }
        break;

    case U8X8_MSG_BYTE_SET_DC: {
        ms_uint8_t status = arg_int;
        ms_io_write(fd_dc, &status, sizeof(status));
    }
        break;

    case U8X8_MSG_BYTE_START_TRANSFER: {
        ms_uint8_t status;
        ms_spi_param_t param;

        ms_io_ioctl(fd_spi, MS_SPI_CMD_GET_PARAM, &param);
        param.frame_mode &= ~MS_SPI_CLK_POLARITY_HIGH;
        param.frame_mode &= ~MS_SPI_CLK_PHASE_2EDGE;

        switch (u8x8->display_info->spi_mode) {
            case 0: param.frame_mode |= MS_SPI_CLK_POLARITY_LOW  | MS_SPI_CLK_PHASE_1EDGE; break;
            case 1: param.frame_mode |= MS_SPI_CLK_POLARITY_LOW  | MS_SPI_CLK_PHASE_2EDGE; break;
            case 2: param.frame_mode |= MS_SPI_CLK_POLARITY_HIGH | MS_SPI_CLK_PHASE_1EDGE; break;
            case 3: param.frame_mode |= MS_SPI_CLK_POLARITY_HIGH | MS_SPI_CLK_PHASE_2EDGE; break;
        }
        ms_io_ioctl(fd_spi, MS_SPI_CMD_SET_PARAM, &param);

        status = u8x8->display_info->chip_enable_level;
        ms_io_write(fd_cs, &status, sizeof(status));

        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
    }
        break;

    case U8X8_MSG_BYTE_END_TRANSFER: {
        ms_uint8_t status;

        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);

        status = u8x8->display_info->chip_disable_level;
        ms_io_write(fd_cs, &status, sizeof(status));
    }
        break;

    default:
        return 0;
    }

    return 1;
}

ms_uint8_t ms_u8x8_gpio_and_delay_hw_spi(u8x8_t *u8x8, ms_uint8_t msg, ms_uint8_t arg_int, ms_ptr_t arg_ptr)
{
    switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        /* init gpio and delay timer */
        break;

    case U8X8_MSG_DELAY_MILLI:
        /* delay_ms(arg_int); */
        break;

    case U8X8_MSG_GPIO_CS:
        /* chip select */
        break;

    case U8X8_MSG_GPIO_DC:
        /* indicate whether data or command */
        break;

    case U8X8_MSG_GPIO_RESET:
        /* reset gpio */
        break;

    case U8X8_MSG_GPIO_SPI_CLOCK:
        /* configure spi baud rate */
        break;

    case U8X8_MSG_GPIO_SPI_DATA:
        /* send data by using gpio-spi */
        break;

    default:
        break;
    }

    return 1;
}
