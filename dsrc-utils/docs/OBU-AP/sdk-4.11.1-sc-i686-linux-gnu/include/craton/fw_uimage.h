/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_FW_UIMAGE_H
#define _CRATON_FW_UIMAGE_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON firmware validation
*/

/**
   Check whether CRATON firmware uimage is valid.

   @note Doesn't verify cryptographic signature!

   @retval :: 1 if firmware uimage is valid, 0 otherwise
*/
int
fw_uimage_valid(const void *uimage,
            size_t uimage_size);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_FW_UIMAGE_H */
