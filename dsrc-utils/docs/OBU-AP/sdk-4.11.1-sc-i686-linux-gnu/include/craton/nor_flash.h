/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_NOR_FLASH_H
#define _CRATON_NOR_FLASH_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifndef __CRATON_ARM
#error "CRATON NOR Flash API should be used only on the ARM CPU"
#endif /* __CRATON_ARM */

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   NOR Flash API
*/

/** Partition number */
typedef uint8_t norfl_part_num_t;

/** Main firmware image partition number */
#define NORFL_PART_FIRMWARE_MAIN 0

/** Rescue firmware image partition number */
#define NORFL_PART_FIRMWARE_RESCUE 1

/** Backup partition number */
#define NORFL_PART_RAM_FS_BACKUP 14

/* Private usage by AutoTalks */
#define NORFL_PART_ATLK_PRIVATE  15

/** Total number of partitions */
#define NORFL_NUM_PARTS 16

/** Partition type "unused" */
#define NORFL_PART_TYPE_UNUSED 0

/** Partition type "firmware" */
#define NORFL_PART_TYPE_FIRMWARE 1

/** Partition type "SafeFAT file system" */
#define NORFL_PART_TYPE_SAFEFAT 2

/** Smallest user-defined partition type */
#define NORFL_PART_TYPE_USER_MIN 0x80

/** Largest user-defined partition type */
#define NORFL_PART_TYPE_USER_MAX 0xff

/** Partition can be locked against modification */
#define NORFL_PART_TYPE_F_LOCKABLE 0x100

/** Partition is locked against modification */
#define NORFL_PART_TYPE_F_LOCKED 0x200

/** Partition table entry */
typedef struct {
  /** Offset of partition start from flash start */
  uint32_t part_offset;

  /** Partition size in bytes */
  uint32_t part_size;

  /** Partition type */
  uint32_t part_type;

} norfl_part_info_t;

/** Partition table entry default initializer */
#define NORFL_PART_INFO_INIT {                      \
  .part_offset = UINT32_MAX,                        \
  .part_size = 0,                                   \
  .part_type = NORFL_PART_TYPE_UNUSED               \
}

/** Partition table */
typedef struct {
  /** Partition table entry array */
  norfl_part_info_t part_info[NORFL_NUM_PARTS];

} norfl_part_table_t;

/** Partition table default initializer */
#define NORFL_PART_TABLE_INIT {                     \
  .part_info = {                                    \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT,     \
    NORFL_PART_INFO_INIT, NORFL_PART_INFO_INIT      \
  }                                                 \
}

/**
   Initialize NOR Flash Interface

   @retval ::ATLK_OK if succeeded
   @return Error code if failed

*/
atlk_rc_t atlk_must_check
norfl_init(void);

/**
   Read partition table from flash.

   @param[in] table Partition table pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed

   @todo Currently only ::NORFL_PART_FIRMWARE_MAIN is supported.
*/
atlk_rc_t atlk_must_check
norfl_part_table_read(norfl_part_table_t *table);

/**
   Read bytes from partition.

   @param[in] part_num Partition number
   @param[in] offset Offset from partition start
   @param[out] data_ptr Pointer to output buffer
   @param[in] data_size Size of output buffer in bytes

   @retval ::ATLK_OK if succeeded
   @return Error code if failed

   @todo Currently only ::NORFL_PART_FIRMWARE_MAIN is supported.
*/
atlk_rc_t atlk_must_check
norfl_part_read(norfl_part_num_t part_num,
            uint32_t offset,
            void *data_ptr,
            size_t data_size);

/**
   Erase and program entire partition.

   @warning Doesn't check validity of written data or that it has been
   written correctly.

   @param[in] part_num Partition number
   @param[in] data_ptr Pointer to input buffer
   @param[in] data_size Size of input buffer in bytes

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
norfl_part_rewrite(norfl_part_num_t part_num,
            const void *data_ptr,
            size_t data_size);

/**
   Lock partition against further modification.

   @param[in] part_num Partition number

   @pre A partition must have the ::NORFL_PART_TYPE_F_LOCKABLE flag set in
   the norfl_part_info_t::part_type field.

   @post A partition will have the ::NORFL_PART_TYPE_F_LOCKED flag set in
   the norfl_part_info_t::part_type field, and will have the
   ::NORFL_PART_TYPE_F_LOCKABLE flag unset.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
norfl_part_lock(norfl_part_num_t part_num);

/** Source of next warm boot */
typedef enum {
  /** Try to boot from main firmware image */
  NORFL_NEXT_BOOT_MAIN = 0,

  /** Try to boot from rescue firmware image */
  NORFL_NEXT_BOOT_RESCUE = 1,

  /** Boot into ROM-based flashing agent */
  NORFL_NEXT_BOOT_FLASHER = 2

} norfl_next_boot_t;

/**
   Set source of next warm boot.

   @param[in] next_boot Source of next boot

   @retval ::ATLK_OK if succeeded
   @return Error code if failed

   @todo Currently unsupported.
*/
atlk_rc_t atlk_must_check
norfl_next_boot_set(norfl_next_boot_t next_boot);

/**
   Get source of next warm boot.

   @param[out] next_boot Source of next boot

   @retval ::ATLK_OK if succeeded
   @return Error code if failed

   @todo Currently unsupported.
*/
atlk_rc_t atlk_must_check
norfl_next_boot_get(norfl_next_boot_t *next_boot);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_NOR_FLASH_H */
