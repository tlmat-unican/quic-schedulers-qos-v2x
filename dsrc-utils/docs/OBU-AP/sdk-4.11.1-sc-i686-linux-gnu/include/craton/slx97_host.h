/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _CRATON_SLX97_HOST_H
#define _CRATON_SLX97_HOST_H

#include <atlk/sdk.h>
#include <atlk/slx97.h>

#include <craton/spi_driver.h>
#include <craton/gpio.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON SLx97 host API
*/

/** SLx97 host I/O configuration parameters */
typedef struct {
  /** SPI device ID */
  spi_device_id_t spi_device_id;

  /** Tx DMA channel */
  hdmac_channel_id_t tx_dma_channel;

  /** Rx DMA channel */
  hdmac_channel_id_t rx_dma_channel;

  /** GPIO number */
  gpio_num_t gpio_num;

} slx97_host_io_config_t;

/** SLx97 host I/O configuration default initializer */
#define SLX97_HOST_IO_CONFIG_INIT {      \
  .spi_device_id = SPI_DEVICE_ID_NA,     \
  .tx_dma_channel = HDMAC_CHANNEL_ID_NA, \
  .rx_dma_channel = HDMAC_CHANNEL_ID_NA, \
  .gpio_num = GPIO_NUM_NA,               \
}

/**
   Create SLx97 host.

   @warning If this function failed, calling it again may result in
   undefined behavior.

   @param[in] io_config SLx97 host I/O configuration parameters
   @param[in] sec_config SLx97 host communication security parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_host_init(const slx97_host_io_config_t *io_config,
            const slx97_host_sec_config_t *sec_config);

/**
   Initialize SLx97 communication security.

   @p master_key is required if and only if
   slx97_host_sec_config_t::sec_master_key_external was nonzero
   when provided as argument to ::slx97_host_init.

   @param[in] master_key Master key (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_host_sec_init(const slx97_host_sec_key_t *master_key);

/**
   Initialize SLx97 server.

   @warning If this function failed, calling it again may result in
   undefined behavior.

   @param[in] io_config SLx97 host I/O configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_server_init(const slx97_host_io_config_t *io_config);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_SLX97_HOST_H */
