/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <ESP8266SAM.h>

extern "C" {
    #include "sound.h"
}

bool output_audio(void *cbdata, int16_t* b) {
    size_t bytes_written;
    i2s_write((i2s_port_t)0, b, 2, &bytes_written, portMAX_DELAY);
    return true;
}

extern "C" void app_main()
{

    size_t bytes_written;
    int i2s_read_len = EXAMPLE_I2S_READ_LEN;
    uint8_t* i2s_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));

    example_i2s_init();
    example_set_file_play_mode();

    ESP8266SAM *sam = new ESP8266SAM(output_audio);
    sam->SetSpeed(120);
    sam->SetPitch(100);
    sam->SetThroat(100);
    sam->SetMouth(200);
    sam->Say("I am Dalek, you are the Doctor, you must be exterminated!");
    printf("Done\n");
    vTaskDelay(500 / portTICK_RATE_MS);
    sam->Say("EXTERMINATE!");
    delete sam;
}
