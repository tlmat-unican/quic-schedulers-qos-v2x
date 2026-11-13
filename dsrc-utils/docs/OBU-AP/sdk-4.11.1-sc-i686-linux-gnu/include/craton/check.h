/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_CHECK_H
#define _CRATON_CHECK_H

/**
   @file
   CRATON build environment check
*/

#if (defined __CRATON__ || defined __SECTONM__)

#if ! (defined __arm__ || defined __arc__)

#error "__arm__ or __arc__ must be defined when compiling CRATON firmware"

#endif /* __arm__ || __arc__ */

#else /* __CRATON__  || __SECTONM__*/

#error "__CRATON__ or __SECTONM__are not defined. CRATON API should be used \
only for compiling CRATON or SECTONM firmware"

#endif /* __CRATON__  || __SECTONM__ */

#endif /* _CRATON_CHECK_H */
