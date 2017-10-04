#ifndef _GATTS_SERVER__H_
#define _GATTS_SERVER__H_
/*******************************************************************
Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD

     Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0
	李波 -学习蓝牙-gatts-server技术
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "bt.h"
#include "bta_api.h"
/*******************************************************************/
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_main.h"
/*******************************************************************/
#include "sdkconfig.h"
/*******************************************************************/
#define GATTS_TAG "bobo_"

///Declare the static function 
void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, 
	esp_gatt_if_t gatts_if,
	esp_ble_gatts_cb_param_t *param);
void gatts_event_handler(esp_gatts_cb_event_t event,                                                     
        esp_gatt_if_t gatts_if,                                                                          
        esp_ble_gatts_cb_param_t *param);
void gap_event_handler(esp_gap_ble_cb_event_t event,
	esp_ble_gap_cb_param_t *param);                 
/*******************************************************************
	宏区
********************************************************************/

#define GATTS_SERVICE_UUID_TEST_A   0xFFF0
#define GATTS_CHAR_UUID_TEST_A      0xFFF1
#define GATTS_DESCR_UUID_TEST_A     0x3333
#define GATTS_NUM_HANDLE_TEST_A     20

#define TEST_DEVICE_NAME            "bobo"
#define TEST_MANUFACTURER_DATA_LEN  17

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

/*******************************************************************/
#define PROFILE_NUM 1
#define PROFILE_A_APP_ID 0
/*******************************************************************/
void ble_init_(void);
#endif

