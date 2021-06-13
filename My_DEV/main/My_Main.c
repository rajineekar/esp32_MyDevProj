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
#include "global.h"
#include "ControlFun.h"
#include "Wifi_Dev.h"
#include "mailbox_pub.h"
#include "WIFI_pub.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif


void Wifi_InitProcess(void)
{
  esp_chip_info_t chip_info;
  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

    /* Print chip information */

    esp_chip_info(&chip_info);
    printf("\nThis is %s chip with %d CPU cores, WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("\nsilicon revision %d, ", chip_info.revision);

    printf("\n%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

}


static void Main_Process(void *pvParameters)
{
  u8 lu8_Satus = 1;
  while(1)
  {
    switch(lu8_Satus)
    {
      case 1:
          execute_Control_Fun();
          lu8_Satus = 2;
        break;
      case 2:
            vTaskDelay(100);
        break;
      default:
        break;
    }

  }
}

void app_main(void)
{

    Wifi_InitProcess();
    mailBox_Init();
    printf("\r\n app_main:mailBox_Init executed");
    Wifi_PreCheckingInfo();

    xTaskCreate(&Main_Process, "Main_Process", 4096, NULL, 5, NULL);
         printf("\r\n Main_Process:created Succesfully\n");
    while(1)
    {
        ;
    }
}
