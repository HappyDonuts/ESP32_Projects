#include <stdio.h>

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define GPIO_LED GPIO_NUM_2

void init_gpio(void);

void app_main(void)
{
    init_gpio();

    while(true) {
        gpio_set_level(GPIO_LED, 0);
        vTaskDelay(200 / portTICK_RATE_MS);
        gpio_set_level(GPIO_LED, 1);
        vTaskDelay(200 / portTICK_RATE_MS);
    }
}

void init_gpio(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << GPIO_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}