#include <stdio.h>
#include "driver/i2c.h"

#define I2C_MODE I2C_MODE_MASTER
#define I2C_NUM_SDA 33
#define I2C_NUM_SDL 34
#define I2C_HZ 100000

i2c_config_t i2c_init() {
    
}

void app_main() {
    i2c_config_t i2c_congfig = i2c_init();

    const i2c_config_t i2c_congfig = {
        .mode = I2C_MODE,
        .sda_io_num = I2C_NUM_SDA,
        .scl_io_num = I2C_NUM_SDL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_HZ,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MODE, &i2c_congfig));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MODE, i2c_congfig.mode, 0, 0, 0));
}