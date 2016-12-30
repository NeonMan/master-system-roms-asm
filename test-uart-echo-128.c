#include <sms/intv-dummy-rst.h>
#include <sms/console.h>
#include <sms/uart.h>
#include <stdint.h>

/*Interrupt vectors. we want to catch NMI (Pause button)*/
void nmi(){
    /*Reset ROM on button press*/
    void (*rv)(void) = (void*)0x0000;
    rv();
}

void int1(){
    /*Do nothing*/
}

uint8_t buff[128];

void main(){
    uint8_t i;
    con_init();
    con_put("UART echo test:\n\n");
    con_put("Connect an UART transceiver on\n");
    con_put("the SMS CONTROL 2 port as\n");
    con_put("Shown below (SMS front):\n\n");
    con_put("     CONTROL 2                \n");
    con_put("     o o o o o                \n");
    con_put("      o o o o                 \n");
    con_put("        | | `---PC TX (SMS RX)\n");
    con_put("        | `-----Ground        \n");
    con_put("        `-------PC RX (SMS TX)\n");
    con_put("                              \n");
    con_put("Serial port parameters are:   \n");
    con_put(" 4800bps      8 data bits     \n");
    con_put(" 1 start bit  1 stop bit      \n");
    con_put("");
    con_put("This program will read 128\n");
    con_put("characters then echo them back");

    do{
        UART_BUFF_GETC(buff,   0);
        UART_BUFF_GETC(buff,   1);
        UART_BUFF_GETC(buff,   2);
        UART_BUFF_GETC(buff,   3);
        UART_BUFF_GETC(buff,   4);
        UART_BUFF_GETC(buff,   5);
        UART_BUFF_GETC(buff,   6);
        UART_BUFF_GETC(buff,   7);
        UART_BUFF_GETC(buff,   8);
        UART_BUFF_GETC(buff,   9);
        UART_BUFF_GETC(buff,  10);
        UART_BUFF_GETC(buff,  11);
        UART_BUFF_GETC(buff,  12);
        UART_BUFF_GETC(buff,  13);
        UART_BUFF_GETC(buff,  14);
        UART_BUFF_GETC(buff,  15);
        UART_BUFF_GETC(buff,  16);
        UART_BUFF_GETC(buff,  17);
        UART_BUFF_GETC(buff,  18);
        UART_BUFF_GETC(buff,  19);
        UART_BUFF_GETC(buff,  20);
        UART_BUFF_GETC(buff,  21);
        UART_BUFF_GETC(buff,  22);
        UART_BUFF_GETC(buff,  23);
        UART_BUFF_GETC(buff,  24);
        UART_BUFF_GETC(buff,  25);
        UART_BUFF_GETC(buff,  26);
        UART_BUFF_GETC(buff,  27);
        UART_BUFF_GETC(buff,  28);
        UART_BUFF_GETC(buff,  29);
        UART_BUFF_GETC(buff,  30);
        UART_BUFF_GETC(buff,  31);
        UART_BUFF_GETC(buff,  32);
        UART_BUFF_GETC(buff,  33);
        UART_BUFF_GETC(buff,  34);
        UART_BUFF_GETC(buff,  35);
        UART_BUFF_GETC(buff,  36);
        UART_BUFF_GETC(buff,  37);
        UART_BUFF_GETC(buff,  38);
        UART_BUFF_GETC(buff,  39);
        UART_BUFF_GETC(buff,  40);
        UART_BUFF_GETC(buff,  41);
        UART_BUFF_GETC(buff,  42);
        UART_BUFF_GETC(buff,  43);
        UART_BUFF_GETC(buff,  44);
        UART_BUFF_GETC(buff,  45);
        UART_BUFF_GETC(buff,  46);
        UART_BUFF_GETC(buff,  47);
        UART_BUFF_GETC(buff,  48);
        UART_BUFF_GETC(buff,  49);
        UART_BUFF_GETC(buff,  50);
        UART_BUFF_GETC(buff,  51);
        UART_BUFF_GETC(buff,  52);
        UART_BUFF_GETC(buff,  53);
        UART_BUFF_GETC(buff,  54);
        UART_BUFF_GETC(buff,  55);
        UART_BUFF_GETC(buff,  56);
        UART_BUFF_GETC(buff,  57);
        UART_BUFF_GETC(buff,  58);
        UART_BUFF_GETC(buff,  59);
        UART_BUFF_GETC(buff,  60);
        UART_BUFF_GETC(buff,  61);
        UART_BUFF_GETC(buff,  62);
        UART_BUFF_GETC(buff,  63);
        UART_BUFF_GETC(buff,  64);
        UART_BUFF_GETC(buff,  65);
        UART_BUFF_GETC(buff,  66);
        UART_BUFF_GETC(buff,  67);
        UART_BUFF_GETC(buff,  68);
        UART_BUFF_GETC(buff,  69);
        UART_BUFF_GETC(buff,  70);
        UART_BUFF_GETC(buff,  71);
        UART_BUFF_GETC(buff,  72);
        UART_BUFF_GETC(buff,  73);
        UART_BUFF_GETC(buff,  74);
        UART_BUFF_GETC(buff,  75);
        UART_BUFF_GETC(buff,  76);
        UART_BUFF_GETC(buff,  77);
        UART_BUFF_GETC(buff,  78);
        UART_BUFF_GETC(buff,  79);
        UART_BUFF_GETC(buff,  80);
        UART_BUFF_GETC(buff,  81);
        UART_BUFF_GETC(buff,  82);
        UART_BUFF_GETC(buff,  83);
        UART_BUFF_GETC(buff,  84);
        UART_BUFF_GETC(buff,  85);
        UART_BUFF_GETC(buff,  86);
        UART_BUFF_GETC(buff,  87);
        UART_BUFF_GETC(buff,  88);
        UART_BUFF_GETC(buff,  89);
        UART_BUFF_GETC(buff,  90);
        UART_BUFF_GETC(buff,  91);
        UART_BUFF_GETC(buff,  92);
        UART_BUFF_GETC(buff,  93);
        UART_BUFF_GETC(buff,  94);
        UART_BUFF_GETC(buff,  95);
        UART_BUFF_GETC(buff,  96);
        UART_BUFF_GETC(buff,  97);
        UART_BUFF_GETC(buff,  98);
        UART_BUFF_GETC(buff,  99);
        UART_BUFF_GETC(buff, 100);
        UART_BUFF_GETC(buff, 101);
        UART_BUFF_GETC(buff, 102);
        UART_BUFF_GETC(buff, 103);
        UART_BUFF_GETC(buff, 104);
        UART_BUFF_GETC(buff, 105);
        UART_BUFF_GETC(buff, 106);
        UART_BUFF_GETC(buff, 107);
        UART_BUFF_GETC(buff, 108);
        UART_BUFF_GETC(buff, 109);
        UART_BUFF_GETC(buff, 110);
        UART_BUFF_GETC(buff, 111);
        UART_BUFF_GETC(buff, 112);
        UART_BUFF_GETC(buff, 113);
        UART_BUFF_GETC(buff, 114);
        UART_BUFF_GETC(buff, 115);
        UART_BUFF_GETC(buff, 116);
        UART_BUFF_GETC(buff, 117);
        UART_BUFF_GETC(buff, 118);
        UART_BUFF_GETC(buff, 119);
        UART_BUFF_GETC(buff, 120);
        UART_BUFF_GETC(buff, 121);
        UART_BUFF_GETC(buff, 122);
        UART_BUFF_GETC(buff, 123);
        UART_BUFF_GETC(buff, 124);
        UART_BUFF_GETC(buff, 125);
        UART_BUFF_GETC(buff, 126);
        UART_BUFF_GETC(buff, 127);
        
        for(i=0; i<128; i++){
            uart_putc(buff[i]);
            buff[i] = 0;
        }
    }while(1);
}