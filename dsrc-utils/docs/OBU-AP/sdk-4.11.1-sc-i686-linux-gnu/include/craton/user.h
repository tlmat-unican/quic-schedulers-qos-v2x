/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_USER_H
#define _CRATON_USER_H

#include <atlk/sdk.h>

#include <craton/exception.h>
#include <craton/check.h>
#include <craton/v2x_config.h>
#include <craton/slx97_host.h>
#include <craton/uart_driver.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON user API

   All functions declared in this file are for the user to define.
*/

/**
   CRATON user abort handler.

   @warning New implementation of this handler will override default
   exception handling (defined as a weak symbol).

   @param[in] info Exception information
*/
void
craton_user_abort_handler(const exception_info_t *info);

/**
   Craton user code initialization function.

   This function is the user application entry point and is defined per
   CRATON CPU.

   @warning Default implementation is defined as a weak symbol, but
   requires linking with libvca.
 */
void 
craton_user_init(void);

/**
   CRATON user V2X configuration.

   This structure defines V2X memory allocation sizes.

   @remark Default definition is defined as a weak symbol.
 */
extern const v2x_config_t craton_user_v2x_config;

/**
   SLx97 communication security configuration.

   @remark Default definition is defined as a weak symbol.
 */
extern const slx97_host_sec_config_t craton_user_slx97_host_sec_config;

/**
    UART0 configuration.

    @remark Default definition is defined as a weak symbol.
*/
extern const uart_config_t craton_user_uart0_config;

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_USER_H */
