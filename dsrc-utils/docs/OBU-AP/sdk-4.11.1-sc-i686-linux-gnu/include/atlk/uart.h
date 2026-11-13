/* Copyright (C) 2015-2016 Autotalks Ltd. */
#ifndef _ATLK_UART_H
#define _ATLK_UART_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   UART API
*/

/* UART speed in bits/s */
typedef uint32_t uart_speed_bps_t;

/** Speed of 4800 bits/s */
#define UART_SPEED_4800_BPS 4800

/** Speed of 9600 bits/s */
#define UART_SPEED_9600_BPS 9600

/** Speed of 14400 bits/s */
#define UART_SPEED_14400_BPS 14400

/** Speed of 19200 bits/s */
#define UART_SPEED_19200_BPS 19200

/** Speed of 38400 bits/s */
#define UART_SPEED_38400_BPS 38400

/** Speed of 57600 bits/s */
#define UART_SPEED_57600_BPS 57600

/** Speed of 115200 bits/s */
#define UART_SPEED_115200_BPS 115200

/** Speed of 230400 bits/s */
#define UART_SPEED_230400_BPS 230400

/** Speed of 460800 bits/s */
#define UART_SPEED_460800_BPS 460800

/** Speed of 921600 bits/s */
#define UART_SPEED_921600_BPS 921600

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_UART_H */
