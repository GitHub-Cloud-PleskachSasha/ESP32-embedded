#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_PORT I2C_NUM_0  //
#define I2C_MODE I2C_MODE_MASTER
#define I2C_NUM_SDA 33
#define I2C_NUM_SDL 34
#define I2C_HZ 100000

#define AHT10_I2C_ADDR 0x38
#define AHT10_I2C_INIT 0xE1 
#define AHT10_I2C_CALIBR 0x08 //
#define AHT10_I2C_MEASURE 0xAC  

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
    ESP_LOGI(TAG, "i2c_param_config is ok");
    
    err = i2c_driver_install(I2C_NUM_0, i2c_congfig.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2c_driver_install failed: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t aht10_init() {
    uint8_t init_cmd[] = {AHT10_I2C_INIT, AHT10_I2C_CALIBR, 0x00}; //
    esp_err_t err = i2c_master_write_to_device(
        I2C_NUM_0,
        AHT10_I2C_ADDR,
        init_cmd,
        sizeof(init_cmd),
        1000 / portTICK_PERIOD_MS
    );
    
    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "aht10 init failed: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t aht10_measure() {
    uint8_t init_cmd[] = {AHT10_I2C_MEASURE, 0x33, 0x00}; //
    esp_err_t err = i2c_master_write_to_device(
        I2C_NUM_0,
        AHT10_I2C_ADDR,
        init_cmd,
        sizeof(init_cmd),
        1000 / portTICK_PERIOD_MS
    );
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "aht10 measure init failed: %s", esp_err_to_name(err));
        return err;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);


    uint8_t data[6];
    esp_err_t data_err = i2c_master_read_from_device(
        I2C_NUM_0,
        AHT10_I2C_ADDR,
        data,
        sizeof(datai2c_master_write_to_deviceESP_OK) {
        ESP_LOGE(TAG, "aht10 get data failed: %s", esp_err_to_name(data_err));
        return data_err;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
 
    uint32_t raw_humidity = (data[1] << 12) | (data[2] << 4) | (data[3] >> 4); //
    uint32_t raw_temperature = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]; //

    float humidity = ((float)raw_humidity / 1048576.0) * 100.0;
    float temperature = ((float)raw_temperature / 1048576.0) * 200.0 - 50.0;

    ESP_LOGI(TAG, "Temperature: %.2f °C, Humidity: %.2f %%", temperature, humidity);

    return ESP_OK;
}

void app_main() {
    i2c_config_t i2c_congfig;
    esp_err_t i2c_err = i2c_init(&i2c_congfig);
    if (i2c_err != ESP_OK) {
        ESP_LOGE(TAG, "i2c init failed");
        return;
    }

    esp_err_t aht10_err = aht10_init();
    if (aht10_err != ESP_OK) {
        ESP_LOGE(TAG, "i2c init failed: %s", esp_err_to_name(aht10_err));
        return;
    }

    ESP_LOGI(TAG, "Connect is ok");
    while (true)
    {
        aht10_measure();
    }
}