/*
 * Copyright (c) 2015-2020 ACOINFO Co., Ltd.
 * All rights reserved.
 *
 * Detailed license information can be found in the LICENSE file.
 *
 * File: ms_u8g2_example.c u8g2 example.
 *
 * Author: Jiao.jinxing <jiaojinxing@acoinfo.com>
 *
 */

#define EXAMPLE_WITH_HW_I2C         (0)
#define EXAMPLE_WITH_HW_4WIRE_SPI   (1)
#define EXAMPLE_ITEM_SELECT         EXAMPLE_WITH_HW_I2C

#include <ms_rtos.h>
#include "u8x8.h"
#include "ms_u8g2_porting.h"

static u8x8_t u8x8;                 // u8x8 object
static ms_uint8_t u8x8_x, u8x8_y;   // current position on the screen

#if EXAMPLE_ITEM_SELECT == EXAMPLE_WITH_HW_I2C
static void initDisplay(void)
{
    ms_u8x8_i2c_dev_set("/dev/i2c2");

    u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c,
               ms_u8x8_byte_hw_i2c, ms_u8x8_gpio_and_delay_hw_i2c);

    u8x8_InitDisplay(&u8x8);
    u8x8_ClearDisplay(&u8x8);
    u8x8_SetPowerSave(&u8x8, 0);
    u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_r);

    u8x8_x = 0;
    u8x8_y = 0;
}
#endif

#if EXAMPLE_ITEM_SELECT == EXAMPLE_WITH_HW_4WIRE_SPI
static void initDisplay(void)
{
    ms_u8x8_spi_dev_set("/dev/gpio_spi2", "/dev/gpio_st75263_cs", "/dev/gpio_st75263_dc");
    ms_u8x8_spi_chip_select("/dev/st75263");

    u8x8_Setup(&u8x8, u8x8_d_st75263_gw20880, u8x8_cad_011,
               ms_u8x8_byte_4wire_hw_spi, ms_u8x8_gpio_and_delay_hw_spi);

    u8x8_InitDisplay(&u8x8);
    u8x8_ClearDisplay(&u8x8);
    u8x8_SetPowerSave(&u8x8, 0);
    u8x8_SetFont(&u8x8, u8x8_font_victoriabold8_r);

    u8x8_x = 0;
    u8x8_y = 0;
}
#endif

static void outChar(ms_uint8_t c)
{
    if (u8x8_x >= u8x8_GetCols(&u8x8)) {
        u8x8_x = 0;
        u8x8_y++;
        if (u8x8_y >= u8x8_GetRows(&u8x8)) {
            u8x8_y = 0;
        }
    }

    u8x8_DrawGlyph(&u8x8, u8x8_x, u8x8_y, c);
    u8x8_x++;
}

static void outStr(const char *s)
{
    while (*s != '\0') {
        if (*s == '\n') {
            u8x8_x = 0;
            u8x8_y++;
            if (u8x8_y >= u8x8_GetRows(&u8x8)) {
                u8x8_y = 0;
            }
            break;
        }
        outChar(*s++);
    }
}

static void setPos(ms_uint8_t x, ms_uint8_t y)
{
    u8x8_x = x;
    u8x8_y = y;
}

int main(int argc, char *argv[])
{
    initDisplay();

    setPos(0, 0);

    while (1) {
        outStr("Hello World!");
        ms_thread_sleep_s(3);
    }

    return 0;
}
