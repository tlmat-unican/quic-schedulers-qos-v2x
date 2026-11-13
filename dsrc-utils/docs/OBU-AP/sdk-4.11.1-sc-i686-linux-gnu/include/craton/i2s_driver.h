/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_I2S_DRIVER_H
#define _CRATON_I2S_DRIVER_H

#include <atlk/sdk.h>

#include <craton/hdmac.h>
#include <craton/gpio.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON I2S driver API
*/

/** Maximum size of audio sample buffer in bytes */
#define I2S_MAX_SAMPLE_BUFFER_SIZE 262080

/** Playback status codes */
typedef enum {
  /** Playback not started */
  I2S_PLAYBACK_NOT_STARTED = 0,

  /** Playback in progress */
  I2S_PLAYBACK_IN_PROGRESS = 1,

  /** Playback complete */
  I2S_PLAYBACK_COMPLETE = 2,

  /** Playback interrupted by user */
  I2S_PLAYBACK_INTERRUPTED = 3,

  /** Playback failed */
  I2S_PLAYBACK_FAILED = 4,

} i2s_playback_status_t;

/** I2S Driver configuration */
typedef struct {
  /** I2S DMA left channel ID */
  hdmac_channel_id_t left_dma_channel;

  /** I2S DMA right channel ID */
  hdmac_channel_id_t right_dma_channel;
  
  /** I2S mute audio GPIO */
  gpio_num_t mute_gpio;

} i2s_driver_config_t;

/** I2S Driver configuration default initializer */
#define I2S_DRIVER_CONFIG_INIT {            \
  .left_dma_channel = HDMAC_CHANNEL_ID_NA,  \
  .right_dma_channel = HDMAC_CHANNEL_ID_NA, \
  .mute_gpio = GPIO_NUM_NA                  \
}

/**
   Initialize the I2S driver.
   
   @param[in] config Driver configuration parameters
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check 
i2s_driver_init(const i2s_driver_config_t *config);

/** I2S DMA playback descriptor */
typedef struct i2s_dma_playback {
  /**
     Pointer to sample buffer for left channel playback

     @remark Must be aligned to cache line size.
  */
  const void *left_sample_buffer_ptr;

  /**
     Pointer to sample buffer for right channel playback

     @remark Must be aligned to cache line size.
     @remark May be equal to i2s_dma_playback_t::left_sample_buffer_ptr.
  */
  const void *right_sample_buffer_ptr;

  /**
     Both sample buffers' size in bytes

     @remark Must be a multiple of cache line size.
  */
  size_t sample_buffer_size;

  /**
     User context

     Not used by the driver.
  */
  void *context;

  /** DMA playback completion handler */
  void (*completion_handler)(struct i2s_dma_playback *playback);

  /**
     Current playback status
  */
  i2s_playback_status_t playback_status;

} i2s_dma_playback_t;

/** I2S DMA playback descriptor default initializer */
#define I2S_DMA_PLAYBACK_INIT {                 \
  .left_sample_buffer_ptr = NULL,               \
  .right_sample_buffer_ptr = NULL,              \
  .sample_buffer_size = 0,                      \
  .context = NULL,                              \
  .completion_handler = NULL,                   \
  .playback_status = I2S_PLAYBACK_NOT_STARTED   \
}

/**
   Start DMA playback of audio sample buffers.

   If playback is already in progress, it will be stopped.

   The audio samples should be two's complement signed 8-bit PCM
   with 20 kHz sample rate. 

   @param[in,out] DMA playback descriptor
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
i2s_dma_playback_start(i2s_dma_playback_t *playback);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_I2S_DRIVER_H */
