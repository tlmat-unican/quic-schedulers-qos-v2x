/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_GPIO_H
#define _CRATON_GPIO_H

#include <atlk/sdk.h>

#include <craton/check.h>

/**
   @file
   CRATON GPIO definitions
*/

/** GPIO number */
typedef uint8_t gpio_num_t;

/** Minimum GPIO number */
#define GPIO_NUM_MIN 0

/** Maximum GPIO number */
#define GPIO_NUM_MAX 31

/** Value indicating that GPIO number is N/A */
#define GPIO_NUM_NA 0xff

#endif /* _CRATON_GPIO_H */
