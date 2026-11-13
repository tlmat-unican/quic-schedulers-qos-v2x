/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_COMPILER_H
#define _ATLK_COMPILER_H

/**
   @file
   Compiler specific attributes, declarations and macros
*/

/** Attribute to mark functions whose return code must be checked */
#if !defined __CC_ARM && \
  ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
#define atlk_must_check __attribute__((warn_unused_result))
#else
#define atlk_must_check
#endif

/** Compiler branch hint support */
#if defined __CC_ARM || defined __GNUC__
#define atlk_likely(x) __builtin_expect(!!(x), 1)
#define atlk_unlikely(x) __builtin_expect(!!(x), 0)
#else
#warning "Unknown compiler; branch hints disabled"
#define atlk_likely(x) (x)
#define atlk_unlikely(x) (x)
#endif

/** Explicit function inlining support */
#if defined __CC_ARM || defined __GNUC__
#define atlk_inline static __inline
#else
#warning "Unknown compiler; explicit function inlining disabled"
#define atlk_inline static
#endif

/** Format string checking support */
#if defined __CC_ARM || defined __GNUC__
#define atlk_format_printf(format_index, value_index) \
  __attribute__ ((format (printf, format_index, value_index)))
#else
#define atlk_format_printf(format_index, value_index)
#endif

/** No-return function attribute */
#if defined __CC_ARM || defined __GNUC__
#define atlk_no_return __attribute__ ((noreturn))
#else
#define atlk_no_return
#endif

#endif /* _ATLK_COMPILER_H */
