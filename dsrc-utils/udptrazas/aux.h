#ifndef AUX_H
#define AUX_H
#define CHUNK_SIZE 8000

struct UDPpacket {
    int packetID;
    char buffpayload[1400]; 
};


typedef struct {
    float file_size;
    float rate_mbps;
    int pkt_size;
} CmdlineParams;


// Global variable to track whether the user has pressed Ctrl+C
// Declare the flag_interrupted variable without defining it
extern volatile int flag_interrupted;
// Function declaration for initialization
void initialize();
long long CurrentTimeMicro();
long long CurrentTimeNano();
void handle_interrupt(int signum);
CmdlineParams file_in_commandline_tx(int argc, char *argv[]);
float file_in_commandline_rx(int argc, char *argv[]);
int min(int a, int b);
#endif