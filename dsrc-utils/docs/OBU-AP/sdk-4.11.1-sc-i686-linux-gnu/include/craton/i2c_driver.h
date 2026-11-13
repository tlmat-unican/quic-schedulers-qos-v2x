/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_I2C_DRIVER_H
#define _CRATON_I2C_DRIVER_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON I2C driver API
*/

/** I2C slave address */
typedef uint16_t i2c_slave_address_t;

/** I2C initialization configuration descriptor */
typedef struct {
  /**
     Ratio between the system clock frequency and the desired bus
     clock frequency.

     Must be a multiple of I2C_CLOCK_DIVISOR_STEP.

     E.g. clock divisor value 400 means 100 kHz bus clock frequency.
  */
  uint16_t clock_divisor;

} i2c_driver_config_t;

/** Value indicating that I2C clock divisor is N/A */
#define I2C_CLOCK_DIVISOR_NA 0

/** I2C clock divisor minimum value */
#define I2C_CLOCK_DIVISOR_MIN 32

/** I2C clock divisor maximum value */
#define I2C_CLOCK_DIVISOR_MAX 2044

/** I2C clock divisor step */
#define I2C_CLOCK_DIVISOR_STEP 4

/** I2C driver configuration default initializer */
#define I2C_DRIVER_CONFIG_INIT {                \
  .clock_divisor = I2C_CLOCK_DIVISOR_NA         \
}

/** I2C flag: no stop condition */
#define I2C_F_NO_STOP (1 << 0)

/**
   Initialize I2C driver.

   @remark This is the first function to be called prior to using I2C.

   @param[in] config I2C driver configuration

   @retval ::ATLK_OK for successful operation
   @return  Error code if failed
*/
atlk_rc_t atlk_must_check
i2c_driver_init(const i2c_driver_config_t *config);

/**
   Send data to a slave device.

   @remark This function doesn't utilize DMA ("PIO" stands for processor I/O).
   @remark This function returns only after all data has been transferred.

   @param[in] address Slave device address
   @param[in] data_ptr Data to be sent
   @param[in] data_size Size in bytes of data to be sent
   @param[in] flags I2C flags

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
i2c_pio_send(i2c_slave_address_t address,
            const void *data_ptr,
            size_t data_size,
            unsigned int flags);

/**
   Receive data from a slave device.

   @remark This function doesn't utilize DMA ("PIO" stands for processor I/O).
   @remark This function returns only after all data has been transferred.

   @param[in] address Slave device address
   @param[out] data_ptr Buffer for data to be received
   @param[in] data_size Size in bytes of receive buffer
   @param[in] flags I2C flags

   @retval ::ATLK_OK for successful operation
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
i2c_pio_receive(i2c_slave_address_t address,
            void *data_ptr,
            size_t data_size,
            unsigned int flags);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_I2C_DRIVER_H */
