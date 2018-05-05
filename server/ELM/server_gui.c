
#include<stdio.h>
#include<stdlib.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdbool.h>

#include "server_gui.h"
#include "elm-enco.h"
#include "lcd_i2c.h"
#include "usb-printer.h"

char pesan;
long time_reboot;
struct Reader reader;
struct Server server;
int  data_paket;


//=============Make thread for running enco purchasedebit============

int reader_start(int data){
    reader.run= TRUE;
    mutex_reader = g_mutex_new();
    cond_reader = g_cond_new();
    puts("reader");
    READERthread = g_thread_create(reader_thread, NULL, TRUE, NULL);

    g_mutex_lock (mutex_reader);
    g_cond_wait (cond_reader, mutex_reader);
    g_mutex_unlock (mutex_reader);

    return 0;

}

int reader_stop()
{
    reader.run= FALSE;
    g_thread_join(READERthread);

    g_mutex_free(mutex_reader);
    g_cond_free(cond_reader);
    return 0;
}

int reader_thread(int data){


    g_mutex_lock(mutex_reader);
    g_cond_broadcast(cond_reader);
    g_mutex_unlock(mutex_reader);

    enco_purchase_debit();

    return NULL;
}


//=============END Make thread for running enco purchasedebit============



//=============Declaration Thread for start and stop=================

int server_start(void){
    server.run= TRUE;
    mutexelm1 = g_mutex_new();
    condelm1 = g_cond_new();

    SERVERThread = g_thread_create(server_thread, NULL, TRUE, NULL);

    g_mutex_lock (mutexelm1);
    g_cond_wait (condelm1, mutexelm1);
    g_mutex_unlock (mutexelm1);

    return 1;

}

int server_stop(void)
{
    server.run= FALSE;
    g_thread_join(SERVERThread);

    g_mutex_free(mutexelm1);
    g_cond_free(condelm1);
    return 1;
}

//============= END Declaration Thread for start and stop=================



//=============Name of thread who start or stop======================
int socket_desc , client_sock , c , read_size, read_size2, read_size3;
char client_message[2000];
char cancel_message[2000];
char server_message[2000] = "OK";
//    char *server_message = "OK";
char server_message2[2000] = "CANCELING FROM SERVER";
char pesan_client;
struct sockaddr_in server_sock , client;
bool status = false;


void read_cancel(){
    for(;;){
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

        //Receive a message from client
        read_size2 = read(client_sock , cancel_message , sizeof(cancel_message));
        //        data = respon(&pesan);
        while( read_size2 > 0 )
        {
            if (get_out==1){
// //                cancel_trig = 1;
                return 0;
            }
            puts("oke");
            int cancel;
            cancel = strcmp(cancel_message, "cancel");


            if(cancel == 0){
                puts ("CANCELING CHOOSING PACKAGE");
                cancel_trig = 1;
                break;
            }

        }
        memset( &cancel_message, 0, sizeof(cancel_message));
        read_size2 = 0;
        break;
    }

}


void send_info(){
    write(client_sock , info , strlen(info));
 //    printer();
	// sleep(4);
	// printer();
}


void* server_thread(void *data)
{




    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server_sock.sin_family = AF_INET;
    server_sock.sin_addr.s_addr = INADDR_ANY;
    server_sock.sin_port = htons( 8888 );

    //Bind
    // setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)
    if( bind(socket_desc,(struct sockaddr *)&server_sock, sizeof(struct sockaddr)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen

    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //char text[60];
    g_mutex_lock(mutexelm1);
    g_cond_broadcast(condelm1);
    g_mutex_unlock(mutexelm1);

    for(;;){
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

        //Receive a message from client
        read_size = read(client_sock , client_message , sizeof(client_message));
        //        data = respon(&pesan);


        while( read_size > 0 )
        {


            //Send the message back to client
            puts("Client Said : ");

            puts(client_message);
            write(client_sock , server_message , strlen(server_message));

            int paket1, paket2, paket3, cancel;
            paket1 = strcmp(client_message, "1");
            paket2 = strcmp(client_message, "2");
            paket3 = strcmp(client_message, "3");
            cancel = strcmp(client_message, "cancel");


            if(cancel == 0){
                break;
            }

            if (paket1 == 0){
                //                status = false;
                //                write(client_sock , paket_message , strlen(paket_message));
                data_paket = 1;

                reader_start(1);
                read_cancel();

                reader_stop();
                puts("thread_reader_finished");

                if(get_out == 1){
                    get_out = 0;
                    break;
                }

                if(cancel_out == 1){
                    cancel_out = 0;
                    break;
                }


            }else if(paket2 == 0 ){
                
                data_paket = 2;
                reader_start(2);
                read_cancel();
                reader_stop();
                puts("thread reader finished");
                
                if(get_out == 1){
                    get_out = 0;
                    break;
                }

                if(cancel_out == 1){
                    cancel_out = 0;
                    break;
                }

                //                write(client_sock , info , strlen(info));
                //                printer();
            }else if(paket3 == 0){
               

                //STARTING THREAD FOR SCAN CARD
                data_paket = 3;
                reader_start(3);
                read_cancel();
                reader_stop();
                puts("thread reader finished");

                if(get_out == 1){
                    get_out = 0;
                    break;
                }

                if(cancel_out == 1){
                    cancel_out = 0;
                    break;
                }

                //                if (reads == 1){
                //                    write(client_sock , info , strlen(info));
                //                }

            }

        }

        memset( &client_message, 0, sizeof(client_message));
        read_size = 0;
        //        }

    }

    sleep(2);
    ///	bank_sam_init();
    sleep(2);
}
//=============END Name of thread who start or stop======================
