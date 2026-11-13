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

arm/bin/wave-sdk: arm/obj/main.o $(SDK_DIR)/board/$(BOARD)/arm/lib/ref-sys.o
