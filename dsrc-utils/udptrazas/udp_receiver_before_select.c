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
    char buf[1400];
    int count;
    int length;
    int pkt_len;    
    struct sockaddr_in local_sk_addr;
    struct sockaddr_in remote_sk_addr;
    char folderPath[] = "./tmp_rx/";
    double rem = 0;

    float file_size = file_in_commandline_rx(argc,(char **)argv);
    printf(" + FileSize set to %f MB\n", file_size);
    // Initialize the program
    initialize();
    if (argc != 3) {
        printf("Usage: ./udp_receiver local_port filename\n");
        exit(1);
    }

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

    count = 0;
    // long long client_timestamp;
    long long server_timestamp;
    char buffer[256];
    if (file_size != 0){
        printf(" + FileSize is set to %f MB\n",file_size);
    }
       // Your simulation or program logic here
    while (!flag_interrupted || (file_size != 0 && rem == file_size*1e6)) {
        // Continue simulation or program execution
        
        length = sizeof(remote_sk_addr);
        if ((pkt_len = recvfrom(fd, &buf, CHUNK_SIZE, 0, (struct sockaddr *) &remote_sk_addr, &length)) <= 0) {
            if((errno == EWOULDBLOCK & count >= 1 ) || (errno == EAGAIN & count >= 1)){
                printf("TIMEOuT\n");
                server_timestamp = CurrentTimeMicro();
                snprintf(buffer,sizeof(buffer),"%lld\t%i\n", server_timestamp, 0);
                fprintf(fp,"%s",buffer);
                continue;
            }else{
                perror("Could not recv datagram!!\n");
                continue;
            }
        }
        count++;
        printf("count = %d, len = %d\n", count, pkt_len);
        rem += pkt_len;
        server_timestamp = CurrentTimeMicro();
        snprintf(buffer,sizeof(buffer),"%lld\t%i\n", server_timestamp, pkt_len);
        fprintf(fp,"%s",buffer);
        
        
        // memcpy(&client_timestamp,&buf.timestamp_micro,sizeof(uint64_t));
       
        // fprintf(fp,"%lld\t%lld\t%i\n",server_timestamp,client_timestamp,pkt_len);
 
    
    }
    printf("%f MB has been received\n",rem/1e6);
    // Cleanup and exit
    printf("Simulation or program ended.\n");
    // Perform any additional cleanup tasks as needed

    return 0;
       
}

