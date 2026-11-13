/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_FTP_SERVER_H
#define _ATLK_FTP_SERVER_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   FTP server API
*/

/**
   Start FTP server.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ftp_server_start(void);


/**
   Stop FTP server.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ftp_server_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_FTP_SERVER_H */
