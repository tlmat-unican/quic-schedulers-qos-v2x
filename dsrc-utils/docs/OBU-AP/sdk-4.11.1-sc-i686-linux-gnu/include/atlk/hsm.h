/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_HSM_H
#define _ATLK_HSM_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   HSM API declarations
*/

/** HSM secure storage cell index */
typedef uint32_t hsm_cell_index_t;

/** Value indicating that HSM cell index is N/A */
#define HSM_CELL_INDEX_NA 0xffffffffUL

/** HSM secure storage cell range */
typedef struct {
  /** First cell index in range */
  hsm_cell_index_t first_cell_index;

  /** Number of cells in range */
  uint32_t num_of_cells;

} hsm_cell_range_t;

/** HSM cell range default initializer */
#define HSM_CELL_RANGE_INIT {               \
  .first_cell_index = HSM_CELL_INDEX_NA,    \
  .num_of_cells = 0,                        \
}

/** HSM service instance */
typedef struct hsm_service hsm_service_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_HSM_H */
