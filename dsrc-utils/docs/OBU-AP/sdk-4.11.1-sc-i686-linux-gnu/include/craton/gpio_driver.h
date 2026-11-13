/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_GPIO_DRIVER_H
#define _CRATON_GPIO_DRIVER_H

#include <atlk/sdk.h>

#include <craton/gpio.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON GPIO driver API
*/

/** GPIO direction */
typedef enum {
  /** Treat pin as input */
  GPIO_INPUT = 0,

  /** Drive pin as output */
  GPIO_OUTPUT = 1,

} gpio_dir_t;

/**
   Set GPIO direction.

   @param[in] gpio_num GPIO number
   @param[in] direction GPIO direction

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_dir_set(gpio_num_t gpio_num,
            gpio_dir_t direction);

/**
   Get GPIO direction.

   @param[in] gpio_num GPIO number
   @param[out] direction GPIO direction

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_dir_get(gpio_num_t gpio_num,
            gpio_dir_t *direction);

/** GPIO IRQ sense mode */
typedef enum {
  /** Detect edge */
  GPIO_SENSE_EDGE = 0,

  /** Detect level */
  GPIO_SENSE_LEVEL = 1,

} gpio_sense_t;

/**
   Set interrupt sense.

   @note If @p sense is set to ::GPIO_SENSE_LEVEL then ::gpio_irq_event_set
   should be used to configure the desired level.

   @param[in] gpio_num GPIO number
   @param[in] sense IRQ sense mode

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_sense_set(gpio_num_t gpio_num,
            gpio_sense_t sense);

/** GPIO edge mode */
typedef enum {
  /** Detect one edge */
  GPIO_EDGE_ONE = 0,

  /** Detect both edges */
  GPIO_EDGE_BOTH = 1,

} gpio_edge_t;

/**
   Set interrupt both-edges register.

   @note If @p edge is set to ::GPIO_EDGE_ONE then ::gpio_irq_event_set
   should be used to configure the desired edge.

   @param[in] gpio_num gpio number
   @param[in] edge GPIO edge mode

   @todo Currently function is not supported

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_edge_set(gpio_num_t gpio_num,
            gpio_edge_t edge);

/** GPIO IRQ event mode */
typedef enum {
  /** Detect falling edge or low level */
  GPIO_EVENT_FALLING_OR_LOW = 0,

  /** Detect rising edge or high level */
  GPIO_EVENT_RISING_OR_HIGH = 1,

} gpio_event_t;

/**
   Set event register.

   @param[in] gpio_num GPIO number
   @param[in] event IRQ event mode

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_event_set(gpio_num_t gpio_num,
            gpio_event_t event);

/** GPIO IRQ mask status */
typedef enum {
  /** Masked */
  GPIO_MASKED = 0,

  /** Unmasked */
  GPIO_UNMASKED = 1,

} gpio_mask_t;

/**
   Set interrupt mask register.

   @param[in] gpio_num GPIO number
   @param[in] mask IRQ mask status

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_mask_set(gpio_num_t gpio_num,
            gpio_mask_t mask);

/**
   Clear IRQ register.

   @param[in] gpio_num GPIO number

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_clear(gpio_num_t gpio_num);

/**
   Set GPIO level.

   @note This function should only be used if GPIO is configured as output.

   @param[in] gpio_num GPIO number
   @param[in] data 0 or 1

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
gpio_data_set(gpio_num_t gpio_num,
            int data);

/**
   Get GPIO level.

   @note This function should only be used if GPIO is configured as input.

   @param[in] gpio_num GPIO number
   @param[out] data 0 or 1

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_data_get(gpio_num_t gpio_num,
            int *data);

/**
   Get IRQ status.

   @param[in] gpio_num GPIO number
   @param[out] status IRQ status (0 or 1)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_status_get(gpio_num_t gpio_num,
            int *status);

/** GPIO IRQ handler */
typedef void
(*gpio_irq_handler_t)(gpio_num_t gpio_num, void *context);

/**
   Set GPIO IRQ handler.

   @param[in] gpio_num GPIO number
   @param[in] handler GPIO IRQ handler
   @param[in] context GPIO handler context
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gpio_irq_handler_set(gpio_num_t gpio_num,
            gpio_irq_handler_t handler,
            void *context);

#ifdef __cplusplus
}
#endif

#endif /*_CRATON_GPIO_DRIVER_H */
