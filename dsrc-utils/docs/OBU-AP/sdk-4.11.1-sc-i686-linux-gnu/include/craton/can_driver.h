/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_CAN_DRIVER_H
#define _CRATON_CAN_DRIVER_H

#include <atlk/sdk.h>
#include <atlk/can.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CAN driver API

   Reference:

     - IPextreme Controller Area Network (CAN) Controller User Guide v1.1.0
*/

/** Number of CAN HW devices */
#define CAN_HW_NUM_DEVICES 2

/**
   CAN HW ID.
  
   Memory registers `ID1` (MSB) and `ID0` (LSB), see Appendix A, Table 25.
*/
typedef uint32_t can_hw_id_t;

/** CAN HW buffer direction */
typedef enum {
  /** Value indicating that CAN HW buffer direction is N/A */
  CAN_HW_BUFFER_DIRECTION_NA = 0,

  /** Ingress CAN HW buffer */
  CAN_HW_BUFFER_DIRECTION_RX = 1,

  /** Egress CAN HW buffer */
  CAN_HW_BUFFER_DIRECTION_TX = 2,

} can_hw_buffer_direction_t;

/** CAN HW buffer (direction + ID) */
typedef struct {
  /** CAN HW buffer direction */
  can_hw_buffer_direction_t direction;

  /** Buffer ID */
  can_hw_id_t id;

} can_hw_buffer_t;

/** CAN HW buffer default initializer */
#define CAN_HW_BUFFER_INIT {                \
  .direction = CAN_HW_BUFFER_DIRECTION_NA,  \
  .id = 0                                   \
}

/** Number of CAN HW buffers */
#define CAN_HW_NUM_BUFFERS 15

/** CAN HW buffer configuration */
typedef struct {
  /** Global mask for buffers 0-13 */
  can_hw_id_t gmask;

  /** Basic mask for buffer 14 */
  can_hw_id_t bmask;

  /** CAN HW buffers */
  can_hw_buffer_t buffers[CAN_HW_NUM_BUFFERS];

} can_hw_buffer_config_t;

/** CAN HW buffer configuration default initializer */
#define CAN_HW_BUFFER_CONFIG_INIT {                                 \
  .gmask = 0,                                                       \
  .bmask = 0,                                                       \
  .buffers = {                                                      \
    CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT,     \
    CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT,     \
    CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT,     \
    CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT,     \
    CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT, CAN_HW_BUFFER_INIT      \
  }                                                                 \
}

/**
   Set CAN HW buffer configuration.

   @note In masks, 0s denote 'care' and 1s denote 'dont care'.

   @param[in] device_id CAN device ID
   @param[in] config CAN HW configuration

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_hw_buffer_config_set(can_device_id_t device_id,
            const can_hw_buffer_config_t *config);

/**
   Get CAN HW buffer configuration.

   @note In masks, 0s denote 'care' and 1s denote 'dont care'.

   @param[in] device_id CAN device ID
   @param[out] config CAN HW configuration

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_hw_buffer_config_get(can_device_id_t device_id,
            can_hw_buffer_config_t *config);

/**
   Convert CAN ID to CAN HW ID.

   @param[in] id CAN ID in ::can_id_t format

   @return CAN HW ID in ::can_hw_id_t format
*/
#define CAN_HW_ID(id)                                                       \
  (((id) & (1 << CAN_ID_EXTENDED_BIT)) ?                                    \
  ((((id) & 0x1ffc0000) << (21 - 18)) | (1 << 20) | (1 << 19) |             \
  (((id) & 0x3ffff) << 1) |                                                 \
  (((id) & (1 << CAN_ID_RTR_BIT) ? 1 << 20 : 0))) :                         \
  ((((id) & 0x7ff) << 21) | ((id) & (1 << CAN_ID_RTR_BIT) ? 1 << 20 : 0)))

/**
   Convert CAN ID mask to CAN HW ID mask.

   @param[in] type Base format (0) or extended format (1)
   @param[in] mask CAN ID mask in ::can_id_t format

   @return CAN HW ID mask in ::can_hw_id_t format
 */
#define CAN_HW_MASK(type, mask)                                             \
  ((type) ? ((((mask) & 0x1ffc0000) << (21 - 18)) |                         \
  (((mask) & 0x3ffff) << 1) |                                               \
  (((mask) & (1 << CAN_ID_RTR_BIT) ? 1 : 0)) |                              \
  (((mask) & (1 << CAN_ID_EXTENDED_BIT) ? 1 << 19: 0))) :                   \
  ((((mask) & 0x7ff) << 21) |                                               \
  (((mask) & (1 << CAN_ID_RTR_BIT) ? 1 << 20 : 0)) |                        \
  (((mask) & (1 << CAN_ID_EXTENDED_BIT) ? 1 << 19: 0))))

/**
   CAN HW ID mask used to select all.

   Use this mask to specify that all bits in CAN HW ID should be considered
   when filtering.
*/
#define CAN_HW_MASK_SELECT_ALL 0x0

/**
   CAN SW filter callback.

   @param[in] device_id CAN device ID
   @param[in] id CAN ID of received frame

   @retval 0 indicates frame should be discarded
   @retval 1 indicates frame should be accepted
*/
typedef int (*can_filter_callback_t)(can_device_id_t device_id, can_id_t id);


/**
   Set CAN SW filter callback.

   Setting callback to NULL means accept all.

   @warning User callback is called at ISR context and should be handled
   as such.

   @param[in] device_id CAN device ID
   @param[in] callback CAN SW filter callback

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_isr_filter_callback_set(can_device_id_t device_id,
            can_filter_callback_t callback);

/** CAN driver speed */
typedef enum {
  /** Speed of 33 Kbps */
  CAN_SPEED_33_KBPS = 0,

  /** Speed of 250 Kbps */
  CAN_SPEED_250_KBPS = 1,

  /** Speed of 500 Kbps */
  CAN_SPEED_500_KBPS = 2,

  /** Speed of 1 Mbps */
  CAN_SPEED_1_MBPS = 3,

  /** Speed of 125 Kbps */
  CAN_SPEED_125_KBPS = 4

} can_speed_t;

/**
   Set CAN device speed.

   @param[in] device_id CAN device ID
   @param[in] speed CAN speed

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_hw_speed_set(can_device_id_t device_id,
            can_speed_t speed);

/**
   Get CAN device speed.

   @param[in] device_id CAN device ID
   @param[out] speed CAN speed

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_hw_speed_get(can_device_id_t device_id, 
            can_speed_t *speed);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_CAN_DRIVER_H */
