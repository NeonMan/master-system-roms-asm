#include <sms/console.h>
#include <sms/sms.h>
#include <sms/intv-dummy.h>

#define CURSOR_COUNT 8

#define KEY_UP     0
#define KEY_DOWN   1
#define KEY_LEFT   2
#define KEY_RIGHT  3
#define KEY_ONE    4
#define KEY_NONE   127

static uint8_t  last_key;
static int8_t   cursor_index;
static uint8_t  io_control_copy;

void setmode(uint8_t index){
    index = index & 0b00000111;
    io_control_copy = io_control_copy ^ (1<<index);
    sms_io_control_port = io_control_copy;
}

void main(){

    sms_io_control_port = 0xFF; /*Set peripheral ports all to inputs*/
    io_control_copy = 0xFF;
    con_init();
    con_clear();
    
    con_gotoxy(0,0);
    con_put("Peripheral port test");
    
    cursor_index = 0;
    last_key = KEY_NONE;
    
    while(1){
        /*Show registers*/
        {
            uint8_t ab;
            uint8_t bm;
            ab = sms_ab_port;
            bm = sms_b_misc_port;
            
            /*Show raw port registers*/
            con_gotoxy(0,3);
            con_put("Port 0xDC: 0x");
            con_puth(ab);
            con_put("\nPort 0xDD: 0x");
            con_puth(bm);
            con_put("\n\n");
            
            /*Show decoded bits*/
            con_put("       PORT-A    PORT-B\n");
            con_put("   Up:   ");
                (ab & (1<<0)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (ab & (1<<6)) ? con_put("HI") : con_put("LO");
                con_put("\n");
                
            con_put(" Down:   ");
                (ab & (1<<1)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (ab & (1<<7)) ? con_put("HI") : con_put("LO");
                con_put("\n");
                
            con_put(" Left:   ");
                (ab & (1<<2)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (bm & (1<<0)) ? con_put("HI") : con_put("LO");
                con_put("\n");
                
            con_put("Right:   ");
                (ab & (1<<3)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (bm & (1<<1)) ? con_put("HI") : con_put("LO");
                con_put("\n");
                
            con_put("(1)TL:   ");
                (ab & (1<<4)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (bm & (1<<2)) ? con_put("HI") : con_put("LO");
                con_put("\n");
                
            con_put("(2)TR:   ");
                (ab & (1<<5)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (bm & (1<<3)) ? con_put("HI") : con_put("LO");
                con_put("\n");
                
            con_put("   TH:   ");
                (bm & (1<<6)) ? con_put("HI") : con_put("LO");
                con_put("        ");
                (bm & (1<<7)) ? con_put("HI") : con_put("LO");
                con_put("\n");
            
            con_put("[Cart:   ");
            (bm & (1<<5)) ? con_put("HI") : con_put("LO");
            con_put(" Reset: ");
            (bm & (1<<4)) ? con_put("HI") : con_put("LO");
            con_put(" ]\n");
        }
        
        /*Show menu*/
        {
            con_gotoxy(0, 18);
            con_put("        Mode      Level\n");
            con_put(" TR-A:\n");
            con_put(" TH-A:\n");
            con_put(" TR-B:\n");
            con_put(" TH-B:\n");
            con_relxy(9,-4);
                (io_control_copy & (1<<0)) ? con_put("IN ") : con_put("OUT"); 
            con_relxy(7,0);
                (io_control_copy & (1<<4)) ? con_put("HI ") : con_put("LO "); 
            
            con_relxy(-13,1);
                (io_control_copy & (1<<1)) ? con_put("IN ") : con_put("OUT");
            
            con_relxy(7,0);
                (io_control_copy & (1<<5)) ? con_put("HI ") : con_put("LO "); 
            
            con_relxy(-13,1);
                (io_control_copy & (1<<2)) ? con_put("IN ") : con_put("OUT");
            
            con_relxy(7,0);
                (io_control_copy & (1<<6)) ? con_put("HI ") : con_put("LO "); 
            
            con_relxy(-13,1);
                (io_control_copy & (1<<3)) ? con_put("IN ") : con_put("OUT");
            
            con_relxy(7,0);
                (io_control_copy & (1<<7)) ? con_put("HI ") : con_put("LO "); 
        }
        
        /*Update cursor*/
        {
            uint8_t ab;
            uint8_t key;
            
            ab = sms_ab_port;
            
            if     ((ab & (1<<0))==0) key = KEY_UP;
            else if((ab & (1<<1))==0) key = KEY_DOWN;
            else if((ab & (1<<2))==0) key = KEY_LEFT;
            else if((ab & (1<<3))==0) key = KEY_RIGHT;
            else if((ab & (1<<4))==0) key = KEY_ONE;
            else                 key = KEY_NONE;
            if((last_key == KEY_NONE) && (key != KEY_NONE)){
                switch(key){
                    case KEY_UP:
                    cursor_index--;
                    break;
                    
                    case KEY_DOWN:
                    cursor_index++;
                    break;
                    
                    case KEY_LEFT:
                    cursor_index = cursor_index - 4;
                    break;
                    
                    case KEY_RIGHT:
                    cursor_index = cursor_index + 4;
                    break;
                    
                    case KEY_ONE:
                    setmode(cursor_index);
                    break;
                    
                    default:
                    break;
                }
                if(cursor_index < 0){
                    cursor_index = cursor_index + CURSOR_COUNT;
                }
                else{
                    cursor_index = cursor_index % CURSOR_COUNT;
                }
            }
            
            last_key = key;
        }
        
        /*Print cursor*/
        {
            uint8_t row;
            uint8_t col;
            int8_t i;
            
            /*Clean cursor spots*/
            for(i=0; i<CURSOR_COUNT; i++){
                row = i % (CURSOR_COUNT / 2);
                col = (i / (CURSOR_COUNT / 2));
                con_gotoxy(8 + (10*col), 19 + row);
                if(i == cursor_index) con_putc('>');
                else                  con_putc(' ');
            }
        }
    }
}