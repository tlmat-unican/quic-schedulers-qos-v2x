/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_SPI_DRIVER_H
#define _CRATON_SPI_DRIVER_H

#include <atlk/sdk.h>

#include <craton/hdmac.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON SPI driver API

   References:

     1. ARM PrimeCell(r) Synchronous Serial Port (PL022); revision r1p3.
     2. ATK4100A1 – ATK4100A0 (CRATON) Datasheet; version 1.9.
*/

/** SPI device ID */
typedef uint8_t spi_device_id_t;

/** First SPI device ID */
#define SPI_DEVICE_ID_MIN 0

/** Last SPI device ID */
#define SPI_DEVICE_ID_MAX 4

/** Invalid SPI device ID */
#define SPI_DEVICE_ID_NA 0xff

/** Minimum SPI data size in bits */
#define SPI_DATA_BITS_MIN 4

/** Maximum SPI data size in bits */
#define SPI_DATA_BITS_MAX 16

/** SPI device mode */
typedef enum {
  /** SPI device is master */
  SPI_MODE_MASTER = 0,

  /** SPI device is slave */
  SPI_MODE_SLAVE = 1,

  /** Invalid SPI device mode */
  SPI_MODE_NA = 0xff

} spi_mode_t;

/** SPI clock polarity */
typedef enum {
  /** SPI clock signal is idle when low */
  SPI_CLOCK_POLARITY_IDLE_LOW = 0,

  /** SPI clock signal is idle when high */
  SPI_CLOCK_POLARITY_IDLE_HIGH = 1

} spi_clock_polarity_t;

/** SPI clock phase */
typedef enum {
  /** Data signal is sampled at clock first edge */
  SPI_CLOCK_PHASE_1ST_EDGE = 0,

  /** Data signal is sampled at clock second edge */
  SPI_CLOCK_PHASE_2ND_EDGE = 1

} spi_clock_phase_t;

/**
   SPI device configuration

   Effective SPI clock rate (in Hz) is:

     base_rate / (clock_rate_divisor1 * (1 + clock_rate_divisor2))
*/
typedef struct {
  /** SPI device ID */
  spi_device_id_t device_id;

  /** SPI device mode */
  spi_mode_t device_mode;

  /** SPI data size in bits */
  uint8_t data_bits;

  /** Clock polarity */
  spi_clock_polarity_t clock_polarity;

  /** Clock phase */
  spi_clock_phase_t clock_phase;

  /**
     Clock rate divisor #1

     @remark Must be even and in the range [2, 254].
     Reference: [1] clause 3.3.1.
  */
  uint8_t clock_rate_divisor1;

  /**
     Clock rate divisor #2

     @remark Can be any value in the range [0, 255].
     Reference: [1] clause 3.3.5.
  */
  uint8_t clock_rate_divisor2;

  /** Tx DMA channel */
  hdmac_channel_id_t tx_dma_channel;

  /** Rx DMA channel */
  hdmac_channel_id_t rx_dma_channel;

} spi_config_t;

/** SPI device configuration default initializer */
#define SPI_CONFIG_INIT {                               \
  .device_id = SPI_DEVICE_ID_NA,                        \
  .device_mode = SPI_MODE_NA,                           \
  .data_bits = 8,                                       \
  .clock_polarity = SPI_CLOCK_POLARITY_IDLE_LOW,        \
  .clock_phase = SPI_CLOCK_PHASE_1ST_EDGE,              \
  .clock_rate_divisor1 = 8,                             \
  .clock_rate_divisor2 = 2,                             \
  .tx_dma_channel = HDMAC_CHANNEL_ID_NA,                \
  .rx_dma_channel = HDMAC_CHANNEL_ID_NA                 \
}

/**
   Get base clock rate (in Hz) of SPI device.

   @param[in] device_id SPI device ID
   @param[out] sspclk SPI SSP clock in Hz

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
spi_sspclk_get(spi_device_id_t device_id, 
            uint32_t *sspclk);

/** SPI device object */
typedef struct spi_device spi_device_t;

/**
   Initialize SPI driver.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
spi_driver_init(void);

/**
   Initialize SPI device according to user configuration.

   @param[in] config SPI device configuration
   @param[out] device_ptr SPI device object pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
spi_device_init(const spi_config_t *config,
            spi_device_t **device_ptr);

/** SPI DMA transfer descriptor */
typedef struct spi_dma_transfer {
  /** Pointer to SPI device object */
  spi_device_t *device_ptr;

  /**
     Pointer to Tx buffer

     @remark Set to NULL to disable Tx.
     @remark Must be aligned to cache line size.
     @remark If ::spi_config_t::data_bits of used SPI device is in [4, 7]
     then (8 - data_bits) most significant bits of each data byte will be
     ignored. If ::spi_config_t::data_bits of used SPI device is in [9, 15]
     then (16 - data_bits) most significant bits of each data byte with odd
     offset (1, 3, 5, etc) will be ignored.
  */
  const void *tx_buffer_ptr;

  /**
     Tx buffer size in bytes

     @remark Ignored if ::spi_dma_transfer_t::tx_buffer_ptr is NULL.
     @remark Must be equal or greater than ::spi_dma_transfer_t::data_size.
     @remark Must be a multiple of cache line size.
     @warning The area marked with asterisks below shouldn't overlap with any
     DMA Rx buffer.

     @verbatim
                                        tx_buffer_size
                     |<------------------------------------------------>|
                     |<--tx_data_offset-->|<---data_size--->|<---***--->|
     tx_buffer_ptr   ^
     
     @endverbatim
  */
  size_t tx_buffer_size;

  /**
     Pointer to Rx buffer

     @remark Set to NULL to disable Rx.
     @remark Must be aligned to cache line size.
     @remark If ::spi_config_t::data_bits of used SPI device is in [4, 7]
     then (8 - data_bits) most significant bits of each data byte will be
     zeroed. If ::spi_config_t::data_bits of used SPI device is in [9, 15]
     then (16 - data_bits) most significant bits of each data byte with odd
     offset (1, 3, 5, etc) will be zeroed.
  */
  void *rx_buffer_ptr;

  /**
     Rx buffer size in bytes

     @remark Ignored if ::spi_dma_transfer_t::rx_buffer_ptr is NULL.
     @remark Must be equal or greater than ::spi_dma_transfer_t::data_size.
     @remark Must be a multiple of cache line size.
     @warning The area marked with asterisks below may be modified in an
     arbitrary fashion by the DMA transfer.
     @verbatim
                                       rx_buffer_size
                     |<------------------------------------------------>|
                     |<--rx_data_offset-->|<---data_size--->|<---***--->|
     rx_buffer_ptr   ^

     @endverbatim
  */
  size_t rx_buffer_size;

  /**
     Offset (in bytes) from 'tx_buffer_ptr' to start of data to transmit
     
     @remark Must be divisible by 2 if ::spi_config_t::data_bits of used
     SPI device is set to 9 or more.
  */
  size_t tx_data_offset;

  /**
     Offset (in bytes) from 'rx_buffer_ptr' to start of data to receive

     @remark Must be divisible by 2 if ::spi_config_t::data_bits of used
     SPI device is set to 9 or more.
  */
  size_t rx_data_offset;
 
  /**
     Size (in bytes) of data to receive and/or transmit

     @remark Must be divisible by 2 if ::spi_config_t::data_bits of used
     SPI device is set to 9 or more.
  */
  size_t data_size;

  /**
     User context

     Not used by the driver.
  */
  void *context;

  /** 
     DMA transfer completion handler.
     Important note: This function will be executed in ISR context.
  */
  void (*completion_handler)(struct spi_dma_transfer *transfer);

} spi_dma_transfer_t;

/** SPI DMA transfer descriptor default initializer */
#define SPI_DMA_TRANSFER_INIT {      \
  .device_ptr = NULL,                \
  .tx_buffer_ptr = NULL,             \
  .tx_buffer_size = 0,               \
  .rx_buffer_ptr = NULL,             \
  .rx_buffer_size = 0,               \
  .tx_data_offset = 0,               \
  .rx_data_offset = 0,               \
  .data_size = 0,                    \
  .context = NULL,                   \
  .completion_handler = NULL         \
}

/**
   Start Tx and/or Rx of data on SPI using DMA.

   @param[in] transfer SPI DMA transfer descriptor

   @remark At least one of ::spi_dma_transfer_t::tx_buffer_ptr and
   ::spi_dma_transfer_t::rx_buffer_ptr must be non-NULL.
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
spi_dma_transfer_start(spi_dma_transfer_t *transfer);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_SPI_DRIVER_H */
