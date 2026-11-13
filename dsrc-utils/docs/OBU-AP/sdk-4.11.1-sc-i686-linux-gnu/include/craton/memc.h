/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_MEMC_H
#define _CRATON_MEMC_H

#include <atlk/sdk.h>

#include <craton/io.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   MEMC API
*/

/** MEMC chip select number */
typedef enum {
  /** CS number 0 */
  MEMC_CHIP_SELECT_0 = 0,

  /** CS number 1 */
  MEMC_CHIP_SELECT_1 = 1,

  /** CS number 2 */
  MEMC_CHIP_SELECT_2 = 2,

  /** CS number 3 */
  MEMC_CHIP_SELECT_3 = 3,

  /** CS number 4 */
  MEMC_CHIP_SELECT_4 = 4,

  /** CS number 5 */
  MEMC_CHIP_SELECT_5 = 5,

  /** CS number 6 */
  MEMC_CHIP_SELECT_6 = 6,

  /** CS number 7 */
  MEMC_CHIP_SELECT_7 = 7,

  /** CS maximum value */
  MEMC_CHIP_SELECT_MAX = MEMC_CHIP_SELECT_7

} memc_chip_select_t;

/** MEMC chip-select information */
typedef struct {
  /** Pointer to start of mapped region */
  void *region_ptr;

  /** Size of mapped region in bytes */
  size_t region_size;

} memc_chip_select_info_t;

/**
   Get chip select memory region information.

   @param[in] chip_select Chip select number
   @param[out] chip_select_info_ptr Pointer to chip select information

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
memc_chip_select_info_get(memc_chip_select_t chip_select,
            memc_chip_select_info_t *chip_select_info_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_MEMC_H */
