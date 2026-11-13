## Copyright (C) 2015-2017 Unex Technology Corporation.

# To build the example outside of the SDK directory please change SDK_DIR
# to the path of the SDK directory

SDK_DIR = /tools/autotalks/sdk-4.11.1-sc-i686-linux-gnu
CFLAGS := -std=c99 -Wall -Wextra -Os -I../../include
LDFLAGS :=-Wl,--start-group ../../lib/* -Wl,--end-group -lcraton -lm

BOARD := pcb201v1

include $(SDK_DIR)/build/rules.mk
include $(SDK_DIR)/build/craton.mk
include $(SDK_DIR)/build/u-boot.mk

.DEFAULT_GOAL := arm/img/wave-sdk.img

TEST_FILES = $(shell ls cmd_*.c)

TEST_OBJ = $(TEST_FILES:%.c=arm/obj/%.o)

arm/bin/wave-sdk: arm/obj/main.o $(TEST_OBJ) $(SDK_DIR)/board/$(BOARD)/arm/lib/ref-sys.o
