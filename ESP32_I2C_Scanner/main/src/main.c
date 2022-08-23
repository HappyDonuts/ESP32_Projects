/* i2c - Simple example
   Simple I2C example that shows how to initialize I2C
   as well as reading and writing from and to registers for a sensor connected over I2C.
   The sensor used in this example is a MPU9250 inertial measurement unit.
   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples
   See README.md file to get detailed usage of this example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_system.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/i2c.h"

#include "inc/esp32_i2c_handler/esp32_i2c_handler.h"

static const char *TAG = "main";



void app_main(void)
{   
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    if (i2c_master_init() != ESP_OK){
        ESP_LOGE(TAG, "i2c init failed\n");
        return;
    }        
    ESP_LOGI(TAG, "I2C initialized successfully");

    for (uint8_t i = 1; i < 127; i++){
        int ret;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK){
            printf("Found device at: 0x%2x\n", i);
        }
    }
    printf("I2C scan finished\n");
}