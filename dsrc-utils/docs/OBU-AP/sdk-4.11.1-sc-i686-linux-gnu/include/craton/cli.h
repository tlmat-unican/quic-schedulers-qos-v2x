/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_CLI_H
#define _CRATON_CLI_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON CLI instances API
*/

/** CLI instance */
typedef struct cli_def cli_instance_t;

/** CLI instance type */
typedef enum {
  /** CLI is accessible over UART */
  CLI_INSTANCE_TYPE_UART = 0,

  /** CLI is accessible over Telnet, using well-known port 23 */
  CLI_INSTANCE_TYPE_TELNET = 1,

  /** CLI is accessible over Telnet, using port 1123 */
  CLI_INSTANCE_TYPE_TELNET2 = 2,

  /** Minimum CLI instance */
  CLI_INSTANCE_TYPE_MIN = CLI_INSTANCE_TYPE_UART,

  /** Maximum CLI instance */
  CLI_INSTANCE_TYPE_MAX = CLI_INSTANCE_TYPE_TELNET2,

} cli_instance_type_t;

/**
   Get pointer to CLI instance.

   @param[out] cli_ptr CLI instance
   @param[in] type CLI instance type

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
cli_instance_get(cli_instance_t **cli_ptr,
            cli_instance_type_t type);

/**
   Suspend CLI instance.

   @param[in] type CLI instance type

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
cli_suspend(cli_instance_type_t type);

/**
   Resume CLI instance.

   @param[in] type CLI instance type

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
cli_resume(cli_instance_type_t type);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_CLI_H */
