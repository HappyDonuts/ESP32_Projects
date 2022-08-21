/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <nvs_flash.h>

#include "inc//wifi_sta/wifi_sta.h"
#include "inc/mqtt_handler/mqtt_handler.h"
#include "cJSON.h"

#define BROKER_IP "mqtt://192.168.1.48"
#define TOPIC "home/humi_1"

static const char *TAG = "main";



char* json_encap(double humi, double temp)
{
    cJSON* json_struct = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_struct, "humi", humi);
    cJSON_AddNumberToObject(json_struct, "temp", temp);

    char* json_str = cJSON_Print(json_struct);
    cJSON_Delete(json_struct);
    return json_str;
}

void app_main(void)
{   
    /* MQTT URI */
    esp_mqtt_client_config_t mqtt_cfg = {
            .uri = BROKER_IP,
        };

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    /* Start WiFi in STA mode */
    wifi_init_sta();

    /* MQTT client handler */
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    char* sensor_json;
    double humi = 45.7;
    double temp = 29.0;

    sensor_json = json_encap(humi, temp);
    printf("%s\n", sensor_json);
 

    while(1) {

        esp_mqtt_client_publish(client, TOPIC, sensor_json, 0, 1, 0);
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}

