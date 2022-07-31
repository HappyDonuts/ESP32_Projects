#include "post_parser.h"

void parse_post_credentials(char* message, uint8_t len, char* ssid, char* psw)
{  
    uint8_t psw_index = 0;
    for (uint8_t i=0; i<len; i++) {
        if (message[i] == '&') {
            psw_index = i;
            break;
        }
    }
    
    for (uint8_t i=5; i<psw_index; i++) {
        ssid[i-5] = message[i];
    }

    for (uint8_t i=psw_index+10; i<len; i++) {
        psw[i-(psw_index+10)] = message[i];
    }
}