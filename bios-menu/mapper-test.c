#include "mapper-test.h"
#include <sms/console.h>
#include <sms/sms.h>
#include <stdint.h>

void mapper_test(){
    uint16_t i;
    uint8_t test_ok;
    
    uint8_t* slot0_base = (void*) SMS_SLOT0_BASE_ADDRESS;
    uint8_t* slot1_base = (void*) SMS_SLOT1_BASE_ADDRESS;
    uint8_t* slot2_base = (void*) SMS_SLOT2_BASE_ADDRESS;
    
    con_clear();
    con_gotoxy(0,2);
    con_put("Mapper self-test:\n\n");
    
    /* Test SLOT2 */
    test_ok = 1;
    con_put("SLOT2... ");
    SMS_MAPPER_SLOT2(0); /*<-- SLOT2 == ROM Page 0*/
    for( i = 0; i<SMS_SLOT_SIZE; i++){
        test_ok = test_ok && (slot0_base[i] == slot2_base[i]);
    }
    if(test_ok){
        test_ok = 0;
        /*Change page and spot at least one difference*/
        SMS_MAPPER_SLOT2(1);
        for(i=0; i<SMS_SLOT_SIZE; i++){
            test_ok = test_ok || (slot0_base[i] != slot2_base[i]);
        }
    }
    if(test_ok){
        con_put("OK\n");
    }
    else{
        con_put("Unavailable\n");
    }
    SMS_MAPPER_SLOT2(2);
    
    /* Test SLOT1 */
    test_ok = 1;
    con_put("SLOT1... ");
    SMS_MAPPER_SLOT1(0); /*<-- SLOT1 == ROM Page 0*/
    for( i = 0; i<SMS_SLOT_SIZE; i++){
        test_ok = test_ok && (slot0_base[i] == slot1_base[i]);
    }
    if(test_ok){
        test_ok = 0;
        /*Change page and spot at least one difference*/
        SMS_MAPPER_SLOT1(1);
        for(i=0; i<SMS_SLOT_SIZE; i++){
            test_ok = test_ok || (slot0_base[i] != slot1_base[i]);
        }
    }
    if(test_ok){
        con_put("OK\n");
    }
    else{
        con_put("Unavailable\n");
    }
    SMS_MAPPER_SLOT1(1);
    
    con_put("\nDone.");
}
