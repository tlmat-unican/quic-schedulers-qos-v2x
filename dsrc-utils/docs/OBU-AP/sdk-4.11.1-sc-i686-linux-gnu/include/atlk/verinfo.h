/* Copyright (C) 2012-2015 Autotalks Ltd. */
#ifndef _ATLK_VERINFO_H
#define _ATLK_VERINFO_H

/**
   @file
   Autotalks SDK version information
*/

/* Constructor of an SDK version code */
#define ATLK_SDK_VER_TUPLE(major, minor, patch) \
  ( (10000000 * (major)) + (1000 * (minor)) + (patch) )

/* Current SDK version code */
#define ATLK_SDK_VER \
  ATLK_SDK_VER_TUPLE(ATLK_SDK_VER_MAJOR, \
                     ATLK_SDK_VER_MINOR, \
                     ATLK_SDK_VER_PATCH)

/* This version's major number */
#define ATLK_SDK_VER_MAJOR 4

/* This version's minor number */
#define ATLK_SDK_VER_MINOR 11

/* This version's patch number */
#define ATLK_SDK_VER_PATCH 1

#endif /* _ATLK_VERINFO_H */
