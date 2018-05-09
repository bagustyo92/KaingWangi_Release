#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <unistd.h>
#include <errno.h>
#include "server_gui.h"
#include "usb-printer.h"
#include "elm_membercard.h"
#include "elm-enco.h"




void printer (){
    int fd = 0;
    int iOut = 0;
    char pesan[] = "TERIMA KASIH";

    //scanf(pesan,"TESSS");
   // sprintf(E)
    char *e = "dsfsd\n";
    char *ComPort = "/dev/usb/lp0";

    fd=open(ComPort, O_WRONLY);

    if(fd == -1);
    iOut = write(fd,"\x1B\x61\x1",3);
    iOut = write(fd,"\x1C\x70\x1\x0",4);  //gambar
    iOut = write(fd,"\x1B\x61\x0",3);      // ratakiri
    iOut = write(fd,"\x1B\x21\x2",3);    //size
    iOut = write(fd, info_print, strlen(info_print));
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, resi_print, strlen(resi_print));
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
     iOut = write(fd, nama_paket, strlen(nama_paket));
     iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
     iOut = write(fd, tarif_print, strlen(tarif_print));
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, saldo_print, strlen(saldo_print));
    iOut = write(fd,"\n",1);
    iOut = write(fd,"\x1B\x61\x1",3);
      iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, pesan, strlen(pesan));
    iOut = write(fd,"\n\n\n\n\n",5);


    close (fd);
    return 0;
}
