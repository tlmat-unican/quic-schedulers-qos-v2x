#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>

#include "minihdlc.h"

int fd;

void send_char(uint8_t c) 
{
    write(fd, &c, 1);
    return;
}

void frame_handler(const uint8_t *data, uint16_t len)
{
    for(uint16_t i = 0;i < len;i++) {
        printf("%02X ", data[i]);
        if (i != 0 && i % 8 == 7)
            printf(" ");
        if (i != 0 && i % 16 == 15)
            printf("\n");
    }
    printf("\n");

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

int main(int argc, void *argv[])
{
    uint8_t buf[2048] = {0};
    int pkt_num;
    int peroid_ms;
    size_t pkt_size;

    if (argc != 5) {
        printf("Usage: ./uart_sender dev_name pkt_num priod_ms pkt_size\n");
        printf("Example: sudo ./uart_sender /dev/ttyS0 10 100 1450\n");
        return 0;
    }

    fd = open(argv[1], O_RDWR | O_NOCTTY);
	if (fd < 0) {
		printf("Open UART fail\n");
		return -1;
	}
	
	if (uart_init(fd) != 0) {
        printf("Set uart fail\n");
        close(fd);
        return -1;		
	}

	/* Max size is 1460 */
    minihdlc_init(send_char, frame_handler);

    pkt_num = atoi(argv[2]);
    peroid_ms = atoi(argv[3]);
    pkt_size = atoi(argv[4]);

    while(pkt_num) {
        if (minihdlc_send_frame(buf, pkt_size) != 0) {
			printf("Size is over\n");			
		}
        usleep(peroid_ms * 1000);
        pkt_num--;
    }

    return 0;

}
