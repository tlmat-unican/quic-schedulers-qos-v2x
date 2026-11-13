/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_CACHE_H
#define _CRATON_CACHE_H

#include <atlk/sdk.h>

#include <craton/check.h>

/**
   @file
   CRATON cache-related definitions
*/

#if defined __arm__
/** Size of cache line in bytes on CRATON's ARM CPU */
#define CACHE_LINE_SIZE 32
#elif defined __arc__
/** Size of cache line in bytes on CRATON's ARC CPUs */
#define CACHE_LINE_SIZE 64
#else
#error "Neither __arm__ nor __arc__ are defined"
#endif /* __arm__, __arc__ */

/** Attribute to specify a cache-line alignment for variables */
#if defined __GNUC__
#define cache_line_aligned __attribute__ ((aligned(CACHE_LINE_SIZE)))
#else
#error "Only GCC compiler is supported"
#endif /* __GNUC__ */

#endif /* _CRATON_CACHE_H */
