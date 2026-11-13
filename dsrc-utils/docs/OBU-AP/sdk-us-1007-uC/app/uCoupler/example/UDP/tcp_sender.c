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

#define MAX 1400

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
 


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
    struct sockaddr_in server_sk_addr;

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

    memset(&server_sk_addr, 0, sizeof(server_sk_addr));
    server_sk_addr.sin_family = AF_INET;
    server_sk_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_sk_addr.sin_port = htons(atoi(argv[2]));

    // connect the client socket to server socket
    if (connect(fd, (struct sockaddr*)&server_sk_addr, sizeof(server_sk_addr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    func(fd);
 
    // close the socket
    close(fd);

    return 0;
}

