#include "aux.h"
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <getopt.h>
#include <time.h>

// Define the flag_interrupted variable in this source file
volatile int flag_interrupted = 0;
// Signal handler function for Ctrl+C (SIGINT)
void handle_interrupt(int signum) {
    printf("\nReceived Ctrl+C. Cleaning up and exiting...\n");
    // Perform any cleanup tasks here
    flag_interrupted = 1; // Set the flag to indicate interruption
}

// Function for initialization
void initialize() {
    // Register the signal handler for Ctrl+C
    if (signal(SIGINT, handle_interrupt) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
}

long long CurrentTimeMicro() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

long long CurrentTimeNano() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // CLOCK_MONOTONIC para medir intervalos
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

CmdlineParams file_in_commandline_tx(int argc, char *argv[]){
    // Parse command-line options using getopt
    int opt;
    CmdlineParams params = {
        .file_size = 0,
        .rate_mbps = 0,
        .pkt_size = CHUNK_SIZE
    };
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        if (opt == 'f') {
            params.file_size = atof(optarg);
        } else {
            fprintf(stderr, "Usage: ./udp_sender <remote_ip> <remote_port> <local_port> <filename> <rate_mbps> <pkt_size> [-f file_size]\n");
            exit(1);
        }
    }

    if (argc - optind != 6) {
        fprintf(stderr, "Usage: ./udp_sender <remote_ip> <remote_port> <local_port> <filename> <rate_mbps> <pkt_size> [-f file_size]\n");
        exit(1);
    }

    params.rate_mbps = atof(argv[optind + 4]);
    params.pkt_size = atoi(argv[optind + 5]);

    return params;
}

float file_in_commandline_rx(int argc, char *argv[]){

    // Parse command-line options using getopt
    int opt;
    float file_size = 0; // Initialize file size to 0
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                file_size = atof(optarg); // Convert optarg to an integer
                break;
            default:
                printf("Usage: ./udp_receiver <local_port> <filename> [-f file_size]\n");
                exit(1);
        }
    }

    if (argc - optind != 2) {
        printf("Usage: ./udp_receiver <local_port> <filename> [-f file_size]\n");
        exit(1);
    }
   
    return file_size;


}


int min(int a, int b) {
    return (a < b) ? a : b;
}