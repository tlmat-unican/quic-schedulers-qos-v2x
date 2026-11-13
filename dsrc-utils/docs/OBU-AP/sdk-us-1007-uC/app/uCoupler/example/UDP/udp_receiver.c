#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int fd, new_socket;   /* file description into transport */
    char buf[2048];
    int count;
    int length;
    int pkt_len;    
    struct sockaddr_in local_sk_addr;
    struct sockaddr_in remote_sk_addr;

    if (argc != 2) {
        printf("Usage: ./udp_sender local_port\n");
        exit(1);
    }

  
   

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror ("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&local_sk_addr, 0, sizeof(local_sk_addr));
    local_sk_addr.sin_family = AF_INET;
    local_sk_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_sk_addr.sin_port = htons(atoi(argv[1]));
    if (bind(fd, (struct sockaddr *)&local_sk_addr, sizeof(local_sk_addr)) <0) {
        perror ("bind failed\n");
        exit(1);
    }

    count = 0;

    while(1) {
        length = sizeof(remote_sk_addr);
        if ((pkt_len = recvfrom(fd, &buf, 2048, 0, (struct sockaddr *) &remote_sk_addr, &length)) < 0) {
            perror("Could not recv datagram!!\n");
            continue;
        }
        count++;
        printf("count = %d, len = %d\n", count, pkt_len);
    }

    return 0;
    
        
        // if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        //     perror ("socket failed");
        //     exit(EXIT_FAILURE);
        // }

        // memset(&local_sk_addr, 0, sizeof(local_sk_addr));
        // local_sk_addr.sin_family = AF_INET;
        // local_sk_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        // local_sk_addr.sin_port = htons(atoi(argv[1]));
        // if (bind(fd, (struct sockaddr *)&local_sk_addr, sizeof(local_sk_addr)) !=0) {
        //     perror ("bind failed\n");
        //     exit(1);
        // }

        // count = 0;
        // if (listen(fd, 3) < 0) {
        //     perror("listen");
        //     exit(EXIT_FAILURE);
        // }
           
        // length = sizeof(remote_sk_addr);
        // if ((new_socket = accept(fd, (struct sockaddr*)&remote_sk_addr,(socklen_t*)&length))< 0) {
        //     perror("accept");
        //     exit(EXIT_FAILURE);
        // }else{
        //      printf("TCP - conn\n");
        // }
        
   
        // while(1) {
        //    printf("Here\n");
        //     if ((pkt_len = read(new_socket, &buf, 2048)) < 0){
        //         perror("Could not recv datagram!!\n");
        //         continue;
        //     }
           
        //     // send(new_socket, hello, strlen(hello), 0);
        //     // printf("Hello message sent\n");
        //     count++;
        //     printf("count = %d, len = %d\n", count, pkt_len);
        // }
        // return 0;
    // }else{
    //     printf("Error\n");
    // }
   
}

