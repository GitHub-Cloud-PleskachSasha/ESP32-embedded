#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_PORT I2C_NUM_0  // чому він і я зробив на початку без нього 
    // ESP_ERROR_CHECK(i2c_param_config(I2C_MODE, &i2c_congfig));
    // ESP_ERROR_CHECK(i2c_driver_install(I2C_MODE, i2c_congfig.mode, 0, 0, 0));
#define I2C_MODE I2C_MODE_MASTER
#define I2C_NUM_SDA 33
#define I2C_NUM_SDL 34
#define I2C_HZ 100000

#define AHT10_I2C_ADDR 0x38

static const char* TAG = "AHT10";

esp_err_t i2c_init(i2c_config_t *i2c_config_out) {
    const i2c_config_t i2c_congfig = {
        .mode = I2C_MODE,
        .sda_io_num = I2C_NUM_SDA,
        .scl_io_num = I2C_NUM_SDL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_HZ,
    };

    esp_err_t err = i2c_param_config(I2C_NUM_0, &i2c_congfig);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "i2c_param_config failed: %s", esp_err_to_name(err));
        return err;
    }
    
    err = i2c_driver_install(I2C_NUM_0, i2c_congfig.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2c_driver_install failed: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t aht10_init() {

    esp_err_t err = i2c_master_write_to_device(I2C_NUM_0, AHT10_I2C_ADDR)
    
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C init failed: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

void app_main() {
    i2c_config_t i2c_congfig;
    i2c_init(&i2c_congfig);
    aht10_init();
}