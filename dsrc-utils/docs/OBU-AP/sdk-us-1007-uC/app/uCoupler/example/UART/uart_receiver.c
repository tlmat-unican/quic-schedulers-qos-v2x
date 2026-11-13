#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>

#include "minihdlc.h"

int count = 1;

int fd;

void send_char(uint8_t c) 
{
    write(fd, &c, 1);
    return;
}

void frame_handler(const uint8_t *data, uint16_t len)
{
    printf("count = %d, len = %d\n", count, len);
    count++;

    //for(uint16_t i = 0;i < len;i++) {
    //    printf("%02X ", data[i]);
    //    if (i != 0 && i % 8 == 7)
    //        printf(" ");
    //    if (i != 0 && i % 16 == 15)
    //        printf("\n");
    //}
    //printf("\n");

    return;
}

/* The function is only for Linux environment. */
int uart_init(int fd)
{
    struct termios config;
    memset(&config, 0, sizeof(config));
    tcgetattr(fd, &config);

    /* input modes - clear indicated ones giving: no break, no CR to NL,
       no parity check, no strip char, no start/stop output (sic) control */
    config.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    /* output modes - clear giving: no post processing such as NL to CR+NL */
    config.c_oflag &= ~(OPOST);

    /* control modes - set 8 bit chars */
    config.c_cflag |= (CS8);

    /* local modes - clear giving: echoing off, canonical off (no erase with
       backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
    config.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    cfsetospeed(&config, B115200);
    cfsetispeed(&config, B115200);

    /* Timeouts configuration */
    config.c_cc[VTIME] = 0;
    config.c_cc[VMIN]  = 1;

    if (tcsetattr(fd, TCSANOW, &config) < 0) {
        close(fd);

        return -1;
    }
	
	return 0;
}

int main(int argc, void *argv[]) {
    int len;
    unsigned char buf[100];

    if (argc != 2) {
        printf("Usage: ./uart_receiver dev_name\n");
        printf("Example: ./uart_receiver /dev/ttyS2\n");
        exit(1);
    }
    
    fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Open UART fail\n");
		return -1;
	}

	if (uart_init(fd) != 0) {
        printf("Set uart fail\n");
        close(fd);
        return -1;		
	}

    minihdlc_init(send_char, frame_handler);

    unsigned char c;
    while (read(fd, &c, 1) > 0) {
        minihdlc_char_receiver(c);
    }

    return 0;

}
