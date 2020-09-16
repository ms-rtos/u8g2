/*
 *
 * u8x8_d_st75263.c
 *
 * Copyright (c) 2015-2020 ACOINFO Co., Ltd.
 * All rights reserved.
 *
 * Author: Yu.Kangzhi <yukangzhi@acoinfo.com>
 *
 * ST75263: provides 2 kinds of display mode 4-Gray/Monochrome sacle mode
 * The built-in DDRAM is an addressable memory array with 256 columns by
 * 81 rows (256 x 81 x 2 bit). For example, in 4-Gray scale mode setting
 * the page address to "00h" means that upcoming 8 bits data is addressing
 * to COM0~COM3. The column address is directly related to the SEG output number.
 *
 * 256-segment / 80-common + 1-icon-common
 *
 */

#include "u8x8.h"

/* not a real power down for the st75263... just a display off */
static const uint8_t u8x8_d_st75263_256x80_powersave0_seq[] = {
  U8X8_START_TRANSFER(),                /* enable chip, delay is part of the transfer start */
  U8X8_C( 0x030 ),                      /* select 00 commands */
  U8X8_C( 0x94 ),                       /* sleep out */
  U8X8_DLY(10),                         /* delay */
  U8X8_C( 0xaf ),                       /* display on */
  U8X8_END_TRANSFER(),                  /* disable chip */
  U8X8_END()                            /* end of sequence */
};

static const uint8_t u8x8_d_st75263_256x80_powersave1_seq[] = {
  U8X8_START_TRANSFER(),                /* enable chip, delay is part of the transfer start */
  U8X8_C( 0x030 ),                      /* select 00 commands */
  U8X8_C( 0xae ),                       /* display off */
  U8X8_C( 0x95 ),                       /* sleep in */
  U8X8_END_TRANSFER(),                  /* disable chip */
  U8X8_END()                            /* end of sequence */
};

static const uint8_t u8x8_d_st75263_jlx25680_flip0_seq[] = {
  U8X8_START_TRANSFER(),                /* enable chip, delay is part of the transfer start */
  U8X8_C( 0x030 ),                      /* select 00 commands */
  U8X8_CA( 0xbc, 0x00 ),                /* data scan dir 'Z' type */
  U8X8_A( 0xa6 ),                       /* INV=0: Normal display */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_C( 0x00c ),                      /* data format LSB top */
  U8X8_END_TRANSFER(),                  /* disable chip */
  U8X8_END()                            /* end of sequence */
};

static const uint8_t u8x8_d_st75263_jlx25680_flip1_seq[] = {
  U8X8_START_TRANSFER(),                /* enable chip, delay is part of the transfer start */
  U8X8_C( 0x030 ),                      /* select 00 commands */
  U8X8_CA( 0xbc, 0x02 ),                /* data scan dir against 'Z' type */
  U8X8_A( 0xa7 ),                       /* INV=1: Inverse display */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_C( 0x008 ),                      /* data format MSB top */
  U8X8_END_TRANSFER(),                  /* disable chip */
  U8X8_END()                            /* end of sequence */
};

/* generic handler of st75263 */
uint8_t u8x8_d_st75263_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg) {
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
        if ( arg_int == 0 ) {
            u8x8_cad_SendSequence(u8x8, u8x8_d_st75263_256x80_powersave0_seq);
        } else {
            u8x8_cad_SendSequence(u8x8, u8x8_d_st75263_256x80_powersave1_seq);
        }
        return 1;

    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
        if ( arg_int == 0 ) {
            u8x8_cad_SendSequence(u8x8, u8x8_d_st75263_jlx25680_flip0_seq);
            u8x8->x_offset = u8x8->display_info->default_x_offset;
        } else {
            u8x8_cad_SendSequence(u8x8, u8x8_d_st75263_jlx25680_flip1_seq);
            u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
        }
        return 1;

    default:
        return 0;
    }

    return 0;
}

/* displayer info of module GWMS15496I(R) */
static const u8x8_display_info_t u8x8_st75263_208x80_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,

  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 5,
  /* post_reset_wait_ms = */ 5,         /* */
  /* sda_setup_time_ns = */ 20,         /* */
  /* sck_pulse_width_ns = */ 40,        /* */
  /* sck_clock_hz = */ 4000000UL,       /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,                   /* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,       /* 400KHz */
  /* data_setup_time_ns = */ 15,
  /* write_pulse_width_ns = */ 70,
  /* tile_width = */ 26,                /* 208 = 26 * 8 */
  /* tile_hight = */ 10,                /* 80 = 10 * 8 */
  /* default_x_offset = */ 24,          /* [0 .... 255] ===(208*80)===> [(0+24) .... (255-24)] */
  /* flipmode_x_offset = */ 0,          /* x offset is used as y offset in flipmode */
  /* pixel_width = */ 208,
  /* pixel_height = */ 80
};

/* displayer initialization sequence of module GWMS15496I(R) */
static const uint8_t u8x8_d_st75263_208x80_init_seq[] = {
  U8X8_START_TRANSFER(),                /* enable chip, delay is part of the transfer start */

  U8X8_DLY(20),

  U8X8_C( 0x030 ),                      /* select 00 commands */
  U8X8_C( 0x094 ),                      /* sleep out */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_C( 0x0ae ),                      /* display off */
  U8X8_DLY(20),

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_CA( 0x020, 0x00b ),              /* VB, VR,VF All ON */
  U8X8_DLY(100),
  U8X8_CAA( 0x81, 0x37, 0x03 ),         /* Vop[5:0]  13.48V */
  U8X8_DLY(10),

  U8X8_C( 0x031 ),                      /* select 01 commands */
  U8X8_CA( 0x0d7, 0x09f ),              /* disable auto read */

  //U8X8_C( 0x031 ),                    /* select 01 commands */
  U8X8_CA( 0x0e0, 0x000 ),              /* enable OTP read */
  U8X8_DLY(20),
  U8X8_C( 0x0e3 ),                      /* OTP Up-Load */
  U8X8_DLY(20),
  U8X8_C( 0x0e1 ),                      /* OTP Control Out */

  //U8X8_C( 0x031 ),                    /* select 01 commands */
  U8X8_C( 0x032 ),                      /* analog circuit set */
  U8X8_A( 0x000 ),                      /* code example: OSC Frequency adjustment */
  U8X8_A( 0x001 ),                      /* Frequency on booster capacitors 1 = 6KHz? */
  U8X8_A( 0x004 ),                      /* Bias: 1: 1/13, 2: 1/12, 3: 1/11, 4:1/10, 5:1/9 */

  //U8X8_C( 0x031 ),                    /* select 01 commands */
  U8X8_CA( 0x051, 0x0fb ),              /* booster level set x10 */

  //U8X8_C( 0x031 ),                    /* select 01 commands */
  //U8X8_C( 0x020 ),                    /* gray levels */
  //U8X8_A( 0x01 ),
  //U8X8_A( 0x03 ),
  //U8X8_A( 0x05 ),
  //U8X8_A( 0x07 ),
  //U8X8_A( 0x09),
  //U8X8_A( 0x0b ),
  //U8X8_A( 0x0d ),
  //U8X8_A( 0x10 ),
  //U8X8_A( 0x11 ),
  //U8X8_A( 0x13 ),
  //U8X8_A( 0x15 ),
  //U8X8_A( 0x17 ),
  //U8X8_A( 0x19 ),
  //U8X8_A( 0x1b ),
  //U8X8_A( 0x1d ),
  //U8X8_A( 0x1f ),

  U8X8_C( 0x030 ),                      /* select 00 commands */
  U8X8_CAA(0x75, 0, 0x4f),              /* row range */
  U8X8_CAA(0x15, 0, 0xFF),              /* col range */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_CA( 0xbc, 0x02 ),                /* data scan dir */
  U8X8_A( 0xa6 ),                       /* INV=0: Normal display */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_C( 0x00c ),                      /* data format LSB top */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_C( 0xca ),                       /* display control, 3 args follow  */
  U8X8_A( 0x00 ),                       /* 0x00: no clock division, 0x04: devide clock */
  U8X8_A( 0x4f ),                       /* 1/160 duty value from the DS example code */
  U8X8_A( 0x00 ),                       /* nline off */

  //U8X8_C( 0x030 ),                    /* select 00 commands */
  U8X8_CA( 0x0f0, 0x010 ),              /* monochrome mode = 0x010, 4Gray level mode = 0x011 */

  U8X8_END_TRANSFER(),                  /* disable chip */
  U8X8_END()                            /* end of sequence */
};

/* u8x8 transaction handler of module GWMS15496I(R) */
uint8_t u8x8_d_st75263_gw20880(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t x, c;
    uint8_t *ptr;

    switch (msg) {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
        u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st75263_208x80_display_info);
        return 1;

    case U8X8_MSG_DISPLAY_INIT:
        u8x8_d_helper_display_init(u8x8);
        u8x8_cad_SendSequence(u8x8, u8x8_d_st75263_208x80_init_seq);
        return 1;

#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
        u8x8_cad_StartTransfer(u8x8);
        u8x8_cad_SendCmd(u8x8, 0x030 );
        u8x8_cad_SendCmd(u8x8, 0x081 );                     /* there are 9 bit for the volume control   */
        u8x8_cad_SendArg(u8x8, (arg_int & 0x1f)<<1 );       /* lower 6 bit                              */
        u8x8_cad_SendArg(u8x8, (arg_int>>5));               /* upper 3 bit                              */
        u8x8_cad_EndTransfer(u8x8);
        return 1;
#endif

    case U8X8_MSG_DISPLAY_DRAW_TILE:
        u8x8_cad_StartTransfer(u8x8);

        x  = ((u8x8_tile_t *)arg_ptr)->x_pos;
        x *= 8;

        u8x8_cad_SendCmd(u8x8, 0x030 );                     /* select command set                       */

        u8x8_cad_SendCmd(u8x8, 0x075 );                     /* set row parameter                        */
        if ( u8x8->x_offset == 1 ) {                        /* 1 means flip mode 1                      */
            u8x8_cad_SendArg(u8x8, 1 + (((u8x8_tile_t *)arg_ptr)->y_pos));
        } else {
            u8x8_cad_SendArg(u8x8, (((u8x8_tile_t *)arg_ptr)->y_pos));
        }
        u8x8_cad_SendArg(u8x8, 0x04f);

        u8x8_cad_SendCmd(u8x8, 0x015 );                     /* set col parameter                        */
        u8x8_cad_SendArg(u8x8, x + u8x8->display_info->default_x_offset);
        u8x8_cad_SendArg(u8x8, 255);

        u8x8_cad_SendCmd(u8x8, 0x05c );

        do {
            c   = ((u8x8_tile_t *)arg_ptr)->cnt;
            ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
            c  *= 8;

            if ( c + x > 208 ) {
                c = 208;
                c -= x;
            }

            u8x8_cad_SendData(u8x8, c, ptr);
            x += c;
            arg_int--;
        } while( arg_int > 0 );

        u8x8_cad_EndTransfer(u8x8);
        return 1;

    default:
        return u8x8_d_st75263_generic(u8x8, msg, arg_int, arg_ptr);
    }

    return 0;
}
