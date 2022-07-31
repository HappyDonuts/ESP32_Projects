/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

/* Deep sleep includes */
#include <time.h>
#include <sys/time.h>
#include "soc/soc_caps.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"

#include "wifi_sta.h"
#include "http_request.h"
#include "http_parser.h"

void nvs_setup(void);

uint8_t connected;

static const char *TAG = "main app";
static RTC_DATA_ATTR struct timeval sleep_enter_time;

const char* web_server = "api.openweathermap.org";
const char* web_port = "80";
const char* web_path_forecast_24h = "http://api.openweathermap.org/data/2.5/forecast?q=Toulouse,FR&current.temp&units=metric&cnt=8&appid=e1bf9b3eaf3e5f441b6264d879cf4aa4";
const char* web_path_weather = "http://api.openweathermap.org/data/2.5/weather?q=Toulouse,FR&current.temp&units=metric&appid=e1bf9b3eaf3e5f441b6264d879cf4aa4";



void app_main(void)
{
    uint8_t connected = 1;
    uint32_t sleep_period_s = 60 * 60;
    http_handle_t* http_handle = http_handle_new(web_server);

    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            /* Secury delay */
            // vTaskDelay(30000 / portTICK_PERIOD_MS);
            printf("Not a deep sleep reset\n");
    }

    /* Initialize NVS */
    nvs_setup();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    /* Wifi station connection to the AP */
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    connected = wifi_init_sta();


    if (connected == 0){
        /* http request */
        http_make_request(http_handle, web_server, web_port, web_path_forecast_24h);

        /* Response parsing */
        http_rec_parser(http_handle->recv_buf, http_handle->rec_strlen, 1024);

            }

    printf("Enabling timer wakeup, %ds\n", sleep_period_s);
    // esp_sleep_enable_timer_wakeup(sleep_period_s * 1000000);


#if CONFIG_IDF_TARGET_ESP32
// Isolate GPIO12 pin from external circuits. This is needed for modules
// which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
// to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    ESP_LOGI(TAG, "Entering deep sleep for %d seconds...\n", sleep_period_s);
    gettimeofday(&sleep_enter_time, NULL);

    esp_deep_sleep_start();
    
}

void nvs_setup(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}
