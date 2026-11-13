#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include <tx_api.h>
#include <tx_posix.h>

#include <libcli.h>

#include <atlk/sdk.h>

#include <craton/uart_driver.h>
#include <craton/cli.h>
#include <atlk/uart.h>

#define CLI_PRINT(fmt, ...) cli_print(cli, fmt, ## __VA_ARGS__)

/* Global */
/* File descriptor for BT device */
static int serial_fd;

/* BT device name */
static char *uart_name = "/dev/uart0";

void craton_user_init(void)
{
    ssize_t bytes;
    unsigned char buf[64] = {0};   
    unsigned char echo_buf[64] = "Echo:";
    cli_instance_t *cli = NULL;
    atlk_rc_t rc = ATLK_OK;

    /* CLI must be suspended before opening UART */
    /* CLI Suspend */
    rc = cli_suspend(CLI_INSTANCE_TYPE_UART);
    if (atlk_error(rc)) {
        CLI_PRINT("Error! Failed to suspend UART cli, rc = %d\n", rc);
        return;
    }

    /* Set uart speed */
    rc = uart_speed_set(UART_DEVICE_ID_0, UART_SPEED_115200_BPS);
    if (atlk_error(rc)) {
        CLI_PRINT("Error! Failed to change UART baudrate, rc = %d\n", rc);
        return;
    }

    /* Open serial port */
    serial_fd = open(uart_name, 0);
    if (serial_fd < 0) {
        fprintf(stderr, "Failed to open \"%s\", rc=%d\n", uart_name, serial_fd);
        return;
    }
  
    while(1) {
        bytes = read(serial_fd, buf, sizeof(buf) - 1);    
        if (bytes > 0) {
            CLI_PRINT("\nRead:%s, len=%d\n", buf, bytes);
        }
        else if (bytes < 0) {
            fprintf(stderr, "Error! Failed to read"); 
        }

        /* Echo to the user */ 
        memcpy(echo_buf + 5, buf, bytes);
        bytes = write(serial_fd, echo_buf, bytes + 5);

        if (bytes < 0) {
            fprintf(stderr, "Failed to write to BT\n");
        }
        else {
            CLI_PRINT("Write Success!! string=%s to UART, len=%d\n", echo_buf, bytes);
        }
    }
  
    return;
}


