#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "ESP32_I2C_Handler/esp32_i2c_handler.h"
#include "SSD1306/ssd1306_basic.h"

static const char *TAG = "i2c-simple-example";





void app_main(void)
{
    i2c_master_init();
    ESP_LOGI(TAG, "I2C initialized successfully");

    ssd1306_t* ssd1306_1;

	ssd1306_1 = ssd1306_new(ADDRESS); // 0x79

	SSD1306_GotoXY(ssd1306_1, 2, 0);
	SSD1306_Puts(ssd1306_1, "Hello ESP32", &Font_11x18, 1);
	SSD1306_UpdateScreen(ssd1306_1);
}

