/* Copyright (C) Unex Technology Corporation - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * This implementation provides a simple malloc/free architecture to replace POSIX malloc/free.
 * Because ThreadX doesn't provide POSIX compatible memory APIs.
 */

#include <stdlib.h>
#include <tx_api.h>


TX_BYTE_POOL simple_mem_pool;
#define MEM_POOL_SIZE 1*1024*1024
static unsigned char mem_pool_buf[MEM_POOL_SIZE];
static int flag_init = 0;

void *simple_malloc(size_t size);
void simple_free(void *ptr);
void *simple_calloc(size_t nmemb, size_t size);


void *simple_malloc(size_t size)
{
	void *ptr;

	if (flag_init == 0) {
		flag_init = 1;
		tx_byte_pool_create(&simple_mem_pool, "simple_mem_pool", (void *) mem_pool_buf, MEM_POOL_SIZE);
	}

	tx_byte_allocate(&simple_mem_pool, (void **) &ptr, size, TX_WAIT_FOREVER);

	return ptr;
}

void simple_free(void *ptr)
{
	tx_byte_release(ptr);
	return;
}

void *simple_calloc(unsigned int nmemb, unsigned int size)
{
	void *ptr = NULL;

	if (flag_init == 0) {
		flag_init = 1;
		tx_byte_pool_create(&simple_mem_pool, "simple_mem_pool", (void *) mem_pool_buf, MEM_POOL_SIZE);
	}

	tx_byte_allocate(&simple_mem_pool, (void **) &ptr, nmemb * size, TX_WAIT_FOREVER);
	(void) memset(ptr, 0, nmemb * size);
	return ptr;
}


