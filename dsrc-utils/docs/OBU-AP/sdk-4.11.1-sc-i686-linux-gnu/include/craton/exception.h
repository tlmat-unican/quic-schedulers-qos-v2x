/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_EXCEPTION_H
#define _CRATON_EXCEPTION_H

#include <atlk/sdk.h>

#include <craton/exception_arc.h>
#include <craton/exception_arm.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON CPU exception API
*/

/**
   Exception information structure

   @exception_info_t is CPU dependent
*/
#if defined __arm__
typedef exception_arm_info_t exception_info_t;
#elif defined __arc__
typedef exception_arc_info_t exception_info_t;
#else
#error Unknown CRATON CPU
#endif

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_EXCEPTION_H */
