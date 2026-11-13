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
#include <sys/time.h>
#include "aux.h"


struct udp_receiver
{
    uint64_t timestamp_micro;
    uint8_t payload[992];
};


int main(int argc, char *argv[])
{
    int fd, new_socket;   /* file description into transport */
    char buf[3000000];
    struct UDPpacket packet;
    int count = 0;
    socklen_t length;
    int pkt_len;    
    struct sockaddr_in local_sk_addr;
    struct sockaddr_in remote_sk_addr;
    char folderPath[] = "./tmp_rx/";
    int rem = 0;
    int maxfd;
    fd_set read_fds;
    float file_size = file_in_commandline_rx(argc,(char **)argv);
   
    // Initialize the program
    initialize();
  
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror ("socket failed");
        exit(EXIT_FAILURE);
    }

    strcat(folderPath,argv[optind+1]);
    FILE *fp;
    fp = fopen(folderPath,"w");

    // Set receive timeout to 100 milliseconds (100,000 microseconds)
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt");
        close(fd);
        exit(1);
    }

    memset(&local_sk_addr, 0, sizeof(local_sk_addr));
    local_sk_addr.sin_family = AF_INET;
    local_sk_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_sk_addr.sin_port = htons(atoi(argv[optind]));
    if (bind(fd, (struct sockaddr *)&local_sk_addr, sizeof(local_sk_addr)) <0) {
        perror ("bind failed\n");
        exit(1);
    }
    printf(" + Binding succesfully done: %s:%s\n", inet_ntoa(local_sk_addr.sin_addr), (char *)argv[optind]);
    long long server_timestamp;
    char buffer[256];
    if (file_size != 0){
        printf(" + FileSize is set to %0.2f MB\n",file_size);
    }

    int progress = 0;
    // Your simulation or program logic here
    printf("[Progress] Receiving file...\n");
    while (!flag_interrupted && rem < (int) (file_size * 1e6)){
       
        FD_ZERO(&read_fds);
        FD_SET(fd, &read_fds);
        maxfd = fd + 1;
        int ready = select(maxfd, &read_fds, NULL, NULL, &timeout);
        if (ready < 0) {
            perror("select");
            break;
        } else if (ready == 0) {
            // Timeout occurred, no data received
            // printf("Received data: 0\n");
            server_timestamp = CurrentTimeMicro();
            // snprintf(buffer,sizeof(buffer),"%lld\t%i\n", server_timestamp, 0);
            // fprintf(fp,"%s",buffer);
        } else {
           
            if (FD_ISSET(fd, &read_fds)) {
                // Data is available to read
                length = sizeof(remote_sk_addr);
                pkt_len = recvfrom(fd, &packet, sizeof(packet), 0, (struct sockaddr *)&remote_sk_addr, &length);
                if (pkt_len == -1) {
                    perror("recvfrom");
                }else{
                    count ++;
                    // printf("count = %d, len = %d\n", count, pkt_len);
                    rem += pkt_len;
                    server_timestamp = CurrentTimeMicro();
                    snprintf(buffer, sizeof(buffer),"%lld\t%i\t%i\n", server_timestamp, pkt_len, packet.packetID);
                    fprintf(fp,"%s",buffer);
                }
                
            }
        
        }
          
            int percent_received = (int)(((double)rem / (file_size * 1e6)) * 100);
            if (percent_received >= progress + 10) {
                progress = percent_received - (percent_received % 10);  // Snap to the nearest lower 10
                printf(" >> %3d%% completed\n", progress);
              
            }
     
    }
    printf(" + %0.2f MB has been received\n",rem/1e6);
    printf(" + Total pkt received: %i || Pkt size: %i\n", count, (int)rem/count);
    // Cleanup and exit
    printf("Simulation or program ended.\n");
    // Perform any additional cleanup tasks as needed

    return 0;
       
}

