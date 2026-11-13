/*
 * hw_if.h
 *
 *  Created on: Sep 29, 2014
 *      Author: Ido Reis <ido.reis@tandemg.com>
 */

#ifndef HW_IF_H_
#define HW_IF_H_

#include <tx_api.h>

typedef int Fd_t;
typedef void (*P_EVENT_HANDLER)(void* pValue);
typedef void (*cc3100_isr_t)(void*);

extern int at_craton_isr_workqueue_init(void);
extern void spi_master_sleep(void);
extern void spi_master_wake_up(void);

extern Fd_t at_craton_hw_if_cc3100_spi_open(char *ifName, unsigned long flags);
extern int at_craton_hw_if_cc3100_spi_close(Fd_t fd);
extern int at_craton_hw_if_cc3100_spi_read(Fd_t fd, unsigned char *pBuff,
		int len);
extern int at_craton_hw_if_cc3100_spi_write(Fd_t fd, unsigned char *pBuff,
		int len);
extern void at_craton_hw_if_cc3100_enable(void);
extern void at_craton_hw_if_cc3100_disable(void);
extern void at_craton_hw_if_cc3100_int_mask(void);
extern void at_craton_hw_if_cc3100_int_unmask(void);
extern int at_craton_hw_if_cc3100_register_isr(cc3100_isr_t handler,
		void* param);
extern void at_craton_hw_if_wdt_start(void);
extern void at_craton_hw_if_wdt_stop(void);
extern void at_craton_hw_if_init_clk(void);
extern void hw_if_init(void);
extern void hw_if_deinit(void);

extern int at_craton_os_if_sync_obj_create(TX_SEMAPHORE* pSyncObj, char* pName);
extern int at_craton_os_if_sync_obj_delete(TX_SEMAPHORE* pSyncObj);
extern int at_craton_os_if_sync_obj_put(TX_SEMAPHORE* pSyncObj);
extern int at_craton_os_if_sync_obj_get(TX_SEMAPHORE* pSyncObj, int Timeout);

extern int at_craton_os_if_lock_obj_create(TX_MUTEX* pLockObj,char* pName);
extern int at_craton_os_if_lock_obj_delete(TX_MUTEX* pLockObj);
extern int at_craton_os_if_lock_obj_lock(TX_MUTEX* pLockObj, int Timeout);
extern int at_craton_os_if_lock_obj_unlock(TX_MUTEX* pLockObj);

#endif /* HW_IF_H_ */
