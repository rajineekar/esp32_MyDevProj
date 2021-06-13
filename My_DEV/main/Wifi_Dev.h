#ifndef _WIFI_DEV_INIT_H
#define _WIFI_DEV_INIT_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define ADHOC_SSID 			"MyDevPROCESS"
#define ADHOC_PWD 			"password"
#define ADHOC_SSID_LEN 		strlen(ADHOC_SSID)+1
#define ADHOC_CHANNEL		1
#define ADHOC_AUTHMODE 	WIFI_AUTH_OPEN

#define SCAN_METHOD 		WIFI_ALL_CHANNEL_SCAN
#define SORT_METHOD 		WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_RSSI 		-127
#define DEFAULT_AUTHMODE 	WIFI_AUTH_OPEN

#define EXAMPLE_ESP_WIFI_SSID      "Rajineekar"
#define EXAMPLE_ESP_WIFI_PASS      "lalitha@321"
#define EXAMPLE_ESP_MAXIMUM_RETRY  4

#endif
