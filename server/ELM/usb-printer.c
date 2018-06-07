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
#include "elm-setting.h"
#include "elm.h"


void printer (){
    int fd = 0;
    int iOut = 0;
    char pesan[] = "TERIMA KASIH";
    char ending[] = "Powerd By TRITON INTERNASIONAL";
    
    char nama_laundry[]= "KAINWANGI";
    char lokasi[] = "MANGGA DUA";
    char angka[]= "1234567890123456";
    

    //scanf(pesan,"TESSS");
   // sprintf(E)
    char *e = "dsfsd\n";
    char *ComPort = "/dev/usb/lp0";

    fd=open(ComPort, O_WRONLY);

    int jml_outlet;
    char *size;


    jml_outlet = strlen(elm_outlet[elm_setting.elm_outlet]);
    if (jml_outlet <= 16){
        size = "\x1B\x21\x30";
    }else{
        size = "\x1B\x21\x10";
    }

    if(fd == -1);
    iOut = write(fd,"\x1B\x61\x1",3);
    iOut = write(fd,size,3);
    iOut = write(fd,elm_outlet[elm_setting.elm_outlet], strlen(elm_outlet[elm_setting.elm_outlet]));
    iOut = write(fd,"\n",1);
    iOut = write(fd,"\x1B\x21\x2",3);
    iOut = write(fd,elm_location[elm_setting.elm_location], strlen(elm_location[elm_setting.elm_location]));
    iOut = write(fd,"\n",1);
    iOut = write(fd,elm_setting.elm_telp, strlen(elm_setting.elm_telp));
    iOut = write(fd,"\n\n\n",3);
    iOut = write(fd,"\x1B\x61\x0",3);      // ratakiri
    iOut = write(fd,"\x1B\x21\x2",3);    //size
    iOut = write(fd, date_print, strlen(date_print));
    iOut = write(fd,"\n",1);
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, nama_print, strlen(nama_print));
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, resi_print, strlen(resi_print));
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
     iOut = write(fd, nama_paket, strlen(nama_paket));
     iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
     iOut = write(fd, tarif_print, strlen(tarif_print));
    iOut = write(fd,"\n\n\n",3);
    iOut = write(fd,"\x1B\x61\x1",3);
      iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, pesan, strlen(pesan));
    iOut = write(fd,"\n",1);
    iOut = write(fd, ending, strlen(ending));
    iOut = write(fd,"\n\n\n\n\n",5);


    close (fd);
    return 0;
}

void printer_topup (){
    int fd = 0;
    int iOut = 0;
    char pesan[] = "TERIMA KASIH";
    char ending[] = "Powerd By TRITON INTERNASIONAL";
    char nama_laundry[]= "KAINWANGI";
    char lokasi[] = "MANGGA DUA";
    char telp[] = "No Telp";
    char nominal_print[] = "";

    //scanf(pesan,"TESSS");
   // sprintf(E)
    char *e = "dsfsd\n";
    char *ComPort = "/dev/usb/lp0";

    fd=open(ComPort, O_WRONLY);

    int jml_outlet;
    char *size;


    jml_outlet = strlen(elm_outlet[elm_setting.elm_outlet]);
    if (jml_outlet <= 16){
        size = "\x1B\x21\x30";
    }else{
        size = "\x1B\x21\x10";
    }

    if(fd == -1);
    iOut = write(fd,"\x1B\x61\x1",3);
    iOut = write(fd,size,3);
    iOut = write(fd,elm_outlet[elm_setting.elm_outlet], strlen(elm_outlet[elm_setting.elm_outlet]));
    // iOut = write(fd,"\n",1);
    // iOut = write(fd,angka, strlen(angka));
    iOut = write(fd,"\n",1);
    iOut = write(fd,"\x1B\x21\x2",3);
    iOut = write(fd,elm_location[elm_setting.elm_location], strlen(elm_location[elm_setting.elm_location]));
    iOut = write(fd,"\n",1);
    iOut = write(fd,elm_setting.elm_telp, strlen(elm_setting.elm_telp));
    iOut = write(fd,"\n\n\n",3);
    iOut = write(fd,"\x1B\x61\x0",3);      // ratakiri
    iOut = write(fd,"\x1B\x21\x2",3);    //size
    iOut = write(fd, date_print, strlen(date_print));
    iOut = write(fd,"\n",1);
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, nama_print, strlen(nama_print));
    iOut = write(fd,"\x1B\x61\x0",3);
     iOut = write(fd,"\x1B\x21\x02",3);
     // iOut = write(fd, nominal_print, strlen(nominal_print));
     // iOut = write(fd,"\x1B\x61\x0",3);
     // iOut = write(fd,"\x1B\x21\x02",3);
     iOut = write(fd, saldo_print, strlen(saldo_print));
    iOut = write(fd,"\n\n\n",3);
    iOut = write(fd,"\x1B\x61\x1",3);
      iOut = write(fd,"\x1B\x21\x02",3);
    iOut = write(fd, pesan, strlen(pesan));
    iOut = write(fd,"\n",1);
    iOut = write(fd, ending, strlen(ending));
    iOut = write(fd,"\n\n\n\n\n",5);
    get_updated_name_saldo(cardnum);
                        printf("E5 %llu %llu \n",List_data.saldo[indeks][2], cardnum);


    close (fd);
    return 0;
}
