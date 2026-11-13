/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_CC3100_DRIVER_H
#define _CRATON_CC3100_DRIVER_H

#include <atlk/sdk.h>
#include <atlk/os.h>

#include <craton/spi_driver.h>
#include <craton/hdmac_driver.h>
#include <craton/gpio.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   TI CC3100 Wi-Fi driver configuration
*/

/** CC3100 configuration parameters */
typedef struct {
  /** Ready signal from CC3100 towards CRATON */
  gpio_num_t spi_gpio_srdy;

  /** Hibernate signal from CRATON towards CC3100 */
  gpio_num_t spi_gpio_hibernate;

  /** SPI RX DMA channel */
  hdmac_channel_id_t spi_dma_rx_channel;

  /** SPI TX DMA channel */
  hdmac_channel_id_t spi_dma_tx_channel;

  /** SPI device number which interacts with CC3100 */
  spi_device_id_t spi_device;

  /** Work queue thread scheduling parameters */ 
  atlk_thread_sched_t workqueue_sched;

  /** Simple Link task thread scheduling parameters */ 
  atlk_thread_sched_t sl_task_sched;

} cc3100_config_t;

/** CC3100 configuration parameters default init */
#define CC3100_CONFIG_INIT {                    \
  .spi_gpio_srdy = GPIO_NUM_MIN,                \
  .spi_gpio_hibernate = GPIO_NUM_MIN,           \
  .spi_dma_rx_channel = HDMAC_CHANNEL_ID_NA,    \
  .spi_dma_tx_channel = HDMAC_CHANNEL_ID_NA,    \
  .spi_device = SPI_DEVICE_ID_MIN,              \
  .workqueue_sched = ATLK_THREAD_SCHED_INIT,    \
  .sl_task_sched = ATLK_THREAD_SCHED_INIT,      \
}

/** 
   Initialize CC3100.

   @param[in] config CC3100 configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/ 
atlk_rc_t atlk_must_check
cc3100_init(const cc3100_config_t *config); 

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_CC3100_DRIVER_H */ 
