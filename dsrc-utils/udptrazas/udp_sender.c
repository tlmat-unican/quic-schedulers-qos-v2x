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
#include <sys/time.h>
#include "aux.h"
#include <stdlib.h>


struct UDPPacket {
    uint64_t timestamp_micro; // Timestamp in microseconds since Unix epoch
    uint8_t payload[992];    // Payload data (total size = 1000 bytes)
};



int main(int argc, void *argv[])
{
    int fd;
    struct UDPpacket packet;
    int length;
    int nbytes;
    int send_buffer_size;
    socklen_t optlen = sizeof(int);
    char buf[CHUNK_SIZE];
    uint64_t timestamp;
    double time_ia;
    char buffer[256]; // Adjust the buffer size to print as needed
    struct sockaddr_in local_sk_addr;
    struct sockaddr_in remote_sk_addr;
    char folderPath[] = "./tmp_send/";
    int pkt_size_param, rem = 0, pkt_size=0; // Initialize file size to 0
    float file_size = 0; 
    CmdlineParams params = file_in_commandline_tx(argc,(char **)argv);
    
    
    initialize();
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror ("socket failed");
        exit(EXIT_FAILURE);
    }
        
    printf(" + Results file name: %s\n",(char *)argv[optind+3]);
    strcat(folderPath,argv[optind+3]);
    FILE *fp;
    fp = fopen(folderPath,"w");

    memset(&local_sk_addr, 0, sizeof(local_sk_addr));
    local_sk_addr.sin_family = AF_INET;
    local_sk_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_sk_addr.sin_port = htons(atoi(argv[optind+2]));
    if (bind(fd, (struct sockaddr *)&local_sk_addr, sizeof(local_sk_addr)) <0) {
        perror ("bind failed\n");
        exit(1);
    }
    printf(" + Binding succesfully done: %s:%s\n",inet_ntoa(local_sk_addr.sin_addr), (char *)argv[optind+2]);
    memset(&remote_sk_addr, 0, sizeof(remote_sk_addr));
    remote_sk_addr.sin_family = AF_INET;
    remote_sk_addr.sin_addr.s_addr = inet_addr(argv[optind]);
    remote_sk_addr.sin_port = htons(atoi(argv[optind+1]));
    length = sizeof(remote_sk_addr); 
    printf(" + Conifguration succesfully done: %s:%s\n",(char *)argv[optind],(char *)argv[optind+1]);
    
  
   
    
    
    if (params.file_size!=0){
        file_size = params.file_size;
        rem = params.file_size*1e6;
        printf(" + FileSize is set to %0.2f MB\n",params.file_size);
    }

    pkt_size_param = params.pkt_size;
    time_ia = (pkt_size_param * 8.0f) / (params.rate_mbps * 1e6);

    // Print result
    printf(" + Time between packets is: %.3f ms\n", time_ia*1000);

    // long long init_time = CurrentTimeMicro(); 
    // // pkt_num = 10;
    // int counter = 0;
   
    // while(!flag_interrupted && (file_size != 0 && rem > 0.0)) {

        
    //     // Sending a packet with a timestamp
    //     long long timestamp = CurrentTimeMicro(); // Get current timestamp
    //     // memcpy(&buf.timestamp_micro, &timestamp, sizeof(long long)); // Include timestamp in payload
    //     if (file_size>0){
    //         pkt_size = min(pkt_size_param, rem);
    //         packet.packetID = counter;
    //         int payload_size = pkt_size - sizeof(packet.packetID);
    //         if (payload_size > sizeof(packet.buffpayload)) {
    //             payload_size = sizeof(packet.buffpayload);
    //         }

    //         memcpy(packet.buffpayload, buf, payload_size);

    //         if (sendto(fd, &packet, sizeof(packet.packetID) + payload_size, 0, (struct sockaddr *) &remote_sk_addr, length) < 0) {
    //             perror("Could not send datagram!!\n");
    //             continue;
    //         }
    //         rem -= pkt_size;
    //     }else{
    //         if (sendto(fd, &buf, (size_t)pkt_size , 0, (struct sockaddr *) &remote_sk_addr, length) < 0) {
    //             perror("Could not send datagram!!\n");
    //             continue;
    //         }
    //     }

         
      
    
    //     // fprintf(fp,"%lld\t%i\n",CurrentTimeMicro(),CHUNK_SIZE);
    //     snprintf(buffer, sizeof(buffer), "%lld\t%i\t%i\n", timestamp, pkt_size, packet.packetID);
    //     // Write the contents of the buffer to the file
    //     fprintf(fp, "%s", buffer);
     
    //     usleep(useconds_t (time_ia * 1e6));
    //     // printf("Time_inter_arrival: %f\n", time_ia*1e6);
    
       
    //     counter++;
    // }




    // tiempo inicial en µs
    long long init_time = CurrentTimeMicro(); 
    long long next_time = init_time;  

    int counter = 0;

while (!flag_interrupted && (file_size != 0 && rem > 0.0)) {

    long long timestamp = CurrentTimeMicro(); // instante real de envío

    if (file_size > 0) {
        pkt_size = min(pkt_size_param, rem);
        packet.packetID = counter;
        int payload_size = pkt_size - sizeof(packet.packetID);
        if (payload_size > sizeof(packet.buffpayload)) {
            payload_size = sizeof(packet.buffpayload);
        }

        memcpy(packet.buffpayload, buf, payload_size);

        if (sendto(fd, &packet, sizeof(packet.packetID) + payload_size, 0,
                   (struct sockaddr *)&remote_sk_addr, length) < 0) {
            perror("Could not send datagram!!\n");
            continue;
        }
        rem -= pkt_size;
    } else {
        if (sendto(fd, &buf, (size_t)pkt_size, 0,
                   (struct sockaddr *)&remote_sk_addr, length) < 0) {
            perror("Could not send datagram!!\n");
            continue;
        }
    }

    // log a fichero
    snprintf(buffer, sizeof(buffer), "%lld\t%i\t%i\n", timestamp, pkt_size, packet.packetID);
    fprintf(fp, "%s", buffer);

    // programar siguiente envío en base al schedule absoluto
    next_time += (long long)(time_ia * 1e6);
    long long now = CurrentTimeMicro();
    long long sleep_time = next_time - now;

    if (sleep_time > 0) {
        usleep((useconds_t)sleep_time);
    } else {
        // si estamos retrasados, no dormir
        // (opcional: printf("Warning: behind schedule!\n"); )
    }

    counter++;
    }
    if (file_size != 0) {
        printf("FileSize %0.2f MB has been sent with a rem %d\n",file_size,rem);
        printf(" - Total pkts sent: %i || Pkt Size: %i\n", counter, pkt_size_param);   
        printf(" - Program duration: %0.2f s\n", (CurrentTimeMicro()-init_time)*1e-6);
    }
    // Cleanup and exit
    printf("Program ended.\n");
    // Perform any additional cleanup tasks as needed
    close(fd);
    return 0;
}

