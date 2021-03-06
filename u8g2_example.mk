#*********************************************************************************************************
#
#                                 北京翼辉信息技术有限公司
#
#                                   微型安全实时操作系统
#
#                                       MS-RTOS(TM)
#
#                               Copyright All Rights Reserved
#
#--------------文件信息--------------------------------------------------------------------------------
#
# 文   件   名: u8g2_example.mk
#
# 创   建   人: IoT Studio
#
# 文件创建日期: 2016 年 10 月 08 日
#
# 描        述: 本文件由 IoT Studio 生成，用于配置 Makefile 功能，请勿手动修改
#*********************************************************************************************************

#*********************************************************************************************************
# Clear setting
#*********************************************************************************************************
include $(CLEAR_VARS_MK)

#*********************************************************************************************************
# Target
#*********************************************************************************************************
LOCAL_TARGET_NAME := u8g2_example

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
./src/ms_u8g2_example.c

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your hearder files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := \
-I"$(MSRTOS_BASE_PATH)/u8g2/src/u8g2/csrc"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := 

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB      := -lu8g2
LOCAL_DEPEND_LIB_PATH := \
-L"$(MSRTOS_BASE_PATH)/u8g2/$(OUTDIR)"

#*********************************************************************************************************
# C++ config
#*********************************************************************************************************
LOCAL_USE_CXX        := no
LOCAL_USE_CXX_EXCEPT := no

#*********************************************************************************************************
# Code coverage config
#*********************************************************************************************************
LOCAL_USE_GCOV := no

#*********************************************************************************************************
# Depend target
#*********************************************************************************************************
LOCAL_DEPEND_TARGET := $(OUTDIR)/libu8g2.a

include $(APPLICATION_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************
