/*
 * Copyright (c) 2015-2020 ACOINFO Co., Ltd.
 * All rights reserved.
 *
 * Detailed license information can be found in the LICENSE file.
 *
 * File: ms_u8g2_example.c u8g2 example.
 *
 * Author: Jiao.jinxing <jiaojixing@acoinfo.com>
 *
 */

#include <ms_rtos.h>
#include "u8x8.h"
#include "ms_u8g2_porting.h"

static u8x8_t u8x8;                    // u8x8 object
static ms_uint8_t u8x8_x, u8x8_y;      // current position on the screen

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
