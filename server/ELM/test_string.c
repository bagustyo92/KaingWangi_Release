#include <stdio.h>
#include <stdlib.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include "server_gui.h"
#include "elm-enco.h"
#include "lcd_i2c.h"

char pesan;
void test_s(){
    puts("Input = ");
    scanf("%c\n",pesan);
    printf("Pesan : %c\n",pesan );
}
