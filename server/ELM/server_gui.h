#ifndef _SERVER
#define _SERVER

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <sys/types.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write



void socket_server();
int input_user(int input);
char *info;
int cancel_trig;
int data_paket;
void send_info();

int trig;
void send_message(char *pesan);

struct Server
{
    int run;
    int listenfd;
    int connfd;
    char receiveBuff[1025];
    int numrv;
    int fd;
    int cmd_len;
    int reply_len;
    unsigned char status[4];
    unsigned char *cmd;
    unsigned char *reply;
};

struct Reader
{
    int run;
    int listenfd;
    int connfd;
    char receiveBuff[1025];
    int numrv;
    int fd;
    int cmd_len;
    int reply_len;
    unsigned char status[4];
    unsigned char *cmd;
    unsigned char *reply;
};

GThread *SERVERThread;
GMutex *mutexelm1;
GCond *condelm1;

GThread *READERthread;
GMutex *mutex_reader;
GCond *cond_reader;

void* server_thread(void *data);
int server_start(void);
int server_stop(void);
int reader_thread(int data);

#endif /* _SERVER */
