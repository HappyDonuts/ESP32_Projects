#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"


typedef struct http_handle_t {
    char* web_server;
    char recv_buf[4096];
    uint16_t rec_strlen;
} http_handle_t;


http_handle_t* http_handle_new(char* web_server);
void http_make_request(http_handle_t* http_handle, char* web_server, char* web_port, char* web_path);
void http_request_composer(char* request_str, char* web_server, char* web_port, char* web_path);
