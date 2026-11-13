#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, void *argv[])
{
    int fd;
    int length;
    int nbytes;
    char buf[2048];
    int pkt_num;
    int peroid_ms;
    size_t pkt_size;
    struct sockaddr_in local_sk_addr;
    struct sockaddr_in remote_sk_addr;

    if (argc != 7) {
        printf("Usage: ./udp_sender remote_ip remote_port local_port pkt_num priod_ms pkt_size\n");
        exit(1);
    }

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror ("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&local_sk_addr, 0, sizeof(local_sk_addr));
    local_sk_addr.sin_family = AF_INET;
    local_sk_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_sk_addr.sin_port = htons(atoi(argv[3]));
    if (bind(fd, (struct sockaddr *)&local_sk_addr, sizeof(local_sk_addr)) <0) {
        perror ("bind failed\n");
        exit(1);
    }

    memset(&remote_sk_addr, 0, sizeof(remote_sk_addr));
    remote_sk_addr.sin_family = AF_INET;
    remote_sk_addr.sin_addr.s_addr = inet_addr(argv[1]);
    remote_sk_addr.sin_port = htons(atoi(argv[2]));

    length = sizeof(remote_sk_addr);
    pkt_num = atoi(argv[4]);
    peroid_ms = atoi(argv[5]);
    pkt_size = atoi(argv[6]);

    while(pkt_num) {
        if (sendto(fd, &buf, pkt_size, 0, (struct sockaddr *) &remote_sk_addr, length) < 0) {
            perror("Could not send datagram!!\n");
            continue;
        }

        usleep(peroid_ms * 1000);

        pkt_num--;
    }

    return 0;
}

