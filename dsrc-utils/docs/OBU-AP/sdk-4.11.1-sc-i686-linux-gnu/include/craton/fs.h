/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_FS_H
#define _CRATON_FS_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   File system
*/

/** Maximum number of open files */
#if !CONFIG_SECTONM
#define FS_OPEN_FILES_MAX 127
#else 
#define FS_OPEN_FILES_MAX 63
#endif /* !CONFIG_SECTONM */

/** Maximum directory traversal depth */
#define FS_DIRSTAT_DEPTH_MAX 8

/** Write buffer size */
#define FS_WRITE_BUFFER_SIZE 1024

/** Maximum data size for read/write operations */
#define FS_READ_WRITE_SIZE_MAX 0x7FFFFFFF

/** Directory statistics structure */
struct fs_dirstat {

  /** Traversal depth */
  unsigned int traversal_depth;

  /** Total size of files in traversed directory in bytes */
  uint64_t size_bytes;

  /** Total number of directories in traversed directory */
  unsigned int num_of_dirs;

  /** Total number of files in traversed directory */
  unsigned int num_of_files;
};

/**
   Obtain directory statistics.

   @param[in] dirpath Directory path
   @param[in] dirstat Directory statistics structure
   @param[in] depth_limit Maximun depth of traversal, must not exceed
              ::FS_DIRSTAT_DEPTH_MAX

   @retval 0 if succeeded
   @return -1 if failed and set errno appropriately

   @remark: Supported on ARM core only.
*/
int
fs_dirstat(const char *dirpath,
            struct fs_dirstat *dirstat,
            unsigned int depth_limit);

/**
   Sync file system.

   Flushes all open files in file system to media (Flash or microSD).

   @remark Supported on ARM core only.
*/
void
fs_sync(void);

/**
   Enable thread to work with file system.

   This function should be called in every thread created via
   tx_thread_create which requires file system access. There is no need
   to call it for threads created via pthread_create.

   Calling this function more than once is safe.

   @retval 0 if succeeded
   @return -1 if failed and set errno appropriately

   @remark: Supported on ARM core only.
*/
int
fs_thread_enable(void);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_FS_H */
