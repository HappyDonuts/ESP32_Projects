#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <string.h>

#include "wifi_ap_credentials.h"

#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       4

void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data);

void wifi_init_softap(void);