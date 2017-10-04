#include "gatts_server.h"
#include "data_handle.h"
/*******************************************************************/
uint16_t gatts_if_1;
uint16_t connet_id;
uint16_t whandle;


uint8_t char1_str[] = {0x11,0x22,0x33};
esp_attr_value_t gatts_demo_char1_val = 
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len     = sizeof(char1_str),
    .attr_value   = char1_str,
};
/*******************************************************************
	广播数据-默认广播
********************************************************************/
#ifdef CONFIG_SET_RAW_ADV_DATA
static uint8_t raw_adv_data[]
= {
        0x02, 0x01, 0x06, 0x0f, 0x09, 0x45, 0x53, 0x50, 0x5f,
	0x47, 0x41, 0x54, 0x54, 0x53, 0x5f, 0x44,0x45, 0x4d, 
	0x4f, 0x02, 0x0a, 0xeb, 0x03, 0x03, 0xab, 0xcd
  };
/*******************************************************************
	广播数据-扫描回应
********************************************************************/
static uint8_t raw_scan_rsp_data[] 
= {
        0x02, 0x01, 0x06, 0x0f, 0x09, 0x45, 0x53, 0x50, 0x5f,
	0x47, 0x41, 0x54, 0x54, 0x53, 0x5f, 0x44,0x45, 0x4d,
	0x4f, 0x02, 0x0a, 0xeb, 0x03, 0x03, 0xab, 0xcd
  };
#else
/*******************************************************************
	测试-uuid-128位
********************************************************************/
static uint8_t test_service_uuid128[32] 
= {
    /* LSB <-------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 
    0x10, 0x00, 	0x00, 0xAB,	 0xCD, 0x00, 0x00,
    //second uuid, 32bit, [12], [13], [14], [15] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00,
    0x10, 0x00, 	0x00, 0xAB, 0xCD, 0xAB,		 0xCD,
  };
/*******************************************************************
	测试-广播数据设置体
********************************************************************/
/*static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN]
=  {0x12, 0x23, 0x45, 0x56};*/
static esp_ble_adv_data_t test_adv_data 
= {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x20,
    .max_interval = 0x40,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 32,
    .p_service_uuid = test_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC 
    	| ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
  };
#endif /* CONFIG_SET_RAW_ADV_DATA */
/*******************************************************************
	测试-广播参数设置体
********************************************************************/
static esp_ble_adv_params_t test_adv_params 
= {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
  };

struct gatts_profile_inst 
{
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

/* One gatt-based profile one app_id and one gatts_if, this array will 
	store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] 
= {
    [PROFILE_A_APP_ID] 
	= {
	    .gatts_cb = gatts_profile_a_event_handler,
	    .gatts_if = 22,/* Not get the gatt_if, 
	    so initial is ESP_GATT_IF_NONE */
	  },
  };

/*******************************************************************
	gap事件处理器
********************************************************************/
void gap_event_handler(esp_gap_ble_cb_event_t event,
	esp_ble_gap_cb_param_t *param)                 
{
    switch (event) 
	{
	    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
		esp_ble_gap_start_advertising(&test_adv_params);
		break;
	    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
		esp_ble_gap_start_advertising(&test_adv_params);
		break;
	    case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
		esp_ble_gap_start_advertising(&test_adv_params);
		break;
	    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
		/*advertising start complete event to indicate 
		advertising start successfully or failed*/
		if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) 
		    {
			ESP_LOGE(GATTS_TAG, "Advertising start failed\n");
		    }
		break;
	    default:
		break;
	}
}

/*******************************************************************
	gatts-a-事件处理器
********************************************************************/
void gatts_profile_a_event_handler(esp_gatts_cb_event_t event,
	esp_gatt_if_t gatts_if,
	esp_ble_gatts_cb_param_t *param) 
{
        switch (event) 
	{
	    case ESP_GATTS_REG_EVT:
		ESP_LOGI(GATTS_TAG, 
			"REGISTER_APP_EVT, status %d, app_id %d\n", 
			param->reg.status,
			param->reg.app_id);
		gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
		gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x02;
		gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = \
			ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = 
			GATTS_SERVICE_UUID_TEST_A;

		esp_ble_gap_set_device_name(TEST_DEVICE_NAME);
#ifdef CONFIG_SET_RAW_ADV_DATA
		esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
		esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data,
			sizeof(raw_scan_rsp_data));
#else
		esp_ble_gap_config_adv_data(&test_adv_data);
#endif
		esp_ble_gatts_create_service(gatts_if,
			&gl_profile_tab[PROFILE_A_APP_ID].service_id, 
			GATTS_NUM_HANDLE_TEST_A);
		break;
	    case ESP_GATTS_READ_EVT:
	    {
		ESP_LOGI(GATTS_TAG,
			"GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", 
			param->read.conn_id,
			param->read.trans_id,
			param->read.handle);
		esp_gatt_rsp_t rsp;
	
		memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
		rsp.attr_value.handle = param->read.handle;
		rsp.attr_value.len = 4;
		rsp.attr_value.value[0] = 12;
		rsp.attr_value.value[1] = 23;
		rsp.attr_value.value[2] = 34;
		rsp.attr_value.value[3] = 45;
		esp_ble_gatts_send_response(gatts_if,
			param->read.conn_id, 
			param->read.trans_id,
			ESP_GATT_OK, &rsp);
		gatts_if_1=gatts_if;
		connet_id=param->read.conn_id;
		whandle=param->write.handle;
		printf("gatts_if_1 %d\n",gatts_if);
		break;
	    }
	    case ESP_GATTS_WRITE_EVT:
	    {
		ESP_LOGI(GATTS_TAG, 
			"GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", 
			param->write.conn_id,
			param->write.trans_id,
			param->write.handle);
		/**
		uint8_t  rwrite[255];
		uint16_t rw_len;
		uint16_t w_i;
		rw_len=param->write.len;
		memset(rwrite, 0,rw_len);
		memcpy(rwrite,(param->write.value),rw_len);
		for(w_i=0;w_i<rw_len;w_i++)
		{    
		   // *(rwrite+w_i)=*(param->write.value+w_i);
		    printf(
			    "bobo_recv_,len %d, value %02x\n", 
			    param->write.len,
			    *(rwrite+w_i));
		 	}   		    
		    esp_ble_gatts_send_indicate(gatts_if, 
		    	param->write.conn_id,
			param->write.handle, 
			rw_len, 
			rwrite, false);
		**/
		/**/
		msg_handle_(param->write.value,param->write.len);
		/**/
		esp_ble_gatts_send_response(gatts_if,
			param->write.conn_id, 
			param->write.trans_id,
			ESP_GATT_OK, NULL);
		break;
	    }
	    case ESP_GATTS_EXEC_WRITE_EVT:
	    	
	    case ESP_GATTS_MTU_EVT:
	    case ESP_GATTS_CONF_EVT:
	    case ESP_GATTS_UNREG_EVT:
		break;
	    case ESP_GATTS_CREATE_EVT:
		ESP_LOGI(GATTS_TAG,
			"CREATE_SERVICE_EVT, status %d,  service_handle %d\n", 
			param->create.status, 
			param->create.service_handle);
		gl_profile_tab[PROFILE_A_APP_ID].service_handle = 
			param->create.service_handle;
		gl_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = 
			GATTS_CHAR_UUID_TEST_A;

		esp_ble_gatts_start_service( \
			gl_profile_tab[PROFILE_A_APP_ID].service_handle);

		esp_ble_gatts_add_char( \
			gl_profile_tab[PROFILE_A_APP_ID].service_handle, 
			&gl_profile_tab[PROFILE_A_APP_ID].char_uuid,
		       ESP_GATT_PERM_READ |
		       ESP_GATT_PERM_WRITE,
		       ESP_GATT_CHAR_PROP_BIT_READ |
		       ESP_GATT_CHAR_PROP_BIT_WRITE | 
		       ESP_GATT_CHAR_PROP_BIT_NOTIFY, 
		       &gatts_demo_char1_val, NULL);
		break;
	    case ESP_GATTS_ADD_INCL_SRVC_EVT:
		break;
	    case ESP_GATTS_ADD_CHAR_EVT:
	    {
		uint16_t length = 0;
		const uint8_t *prf_char;

		ESP_LOGI(GATTS_TAG,
			"ADD_CHAR_EVT, status %d,  attr_handle %d, \
			service_handle %d\n",
			param->add_char.status, 
			param->add_char.attr_handle, 
			param->add_char.service_handle);
		gl_profile_tab[PROFILE_A_APP_ID].char_handle = \
			param->add_char.attr_handle;
		gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 = 
			ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
		esp_ble_gatts_get_attr_value(param->add_char.attr_handle,
			&length, &prf_char);

		ESP_LOGI(GATTS_TAG, "the gatts demo char length = %x\n", length);
		for(int i = 0; i < length; i++){
		    ESP_LOGI(GATTS_TAG, "prf_char[%x] =%x\n",i,prf_char[i]);
		}
		esp_ble_gatts_add_char_descr( \
			gl_profile_tab[PROFILE_A_APP_ID].service_handle,
			&gl_profile_tab[PROFILE_A_APP_ID].descr_uuid,
			ESP_GATT_PERM_READ |
			ESP_GATT_PERM_WRITE, NULL, NULL);
		break;
	    }
	    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
		ESP_LOGI(GATTS_TAG,
			"ADD_DESCR_EVT, status %d, attr_handle %d, \
			service_handle %d\n",
			 param->add_char.status,
			 param->add_char.attr_handle, 
			 param->add_char.service_handle);
		break;
	    case ESP_GATTS_DELETE_EVT:
		break;
	    case ESP_GATTS_START_EVT:
		ESP_LOGI(GATTS_TAG,
			"SERVICE_START_EVT, status %d, service_handle %d\n",
			 param->start.status, 
			 param->start.service_handle);
		break;
	    case ESP_GATTS_STOP_EVT:
		break;
	    case ESP_GATTS_CONNECT_EVT:
		ESP_LOGI(GATTS_TAG, 
			"SERVICE_START_EVT, conn_id %d, remote %02x:%02x: \
			%02x:%02x:%02x:%02x:, is_conn %d\n",
			 param->connect.conn_id,
			 param->connect.remote_bda[0],
			 param->connect.remote_bda[1], 
			 param->connect.remote_bda[2],
			 param->connect.remote_bda[3],
			 param->connect.remote_bda[4],
			 param->connect.remote_bda[5],
			 param->connect.is_connected);
		gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
		break;
	    case ESP_GATTS_DISCONNECT_EVT:
		esp_ble_gap_start_advertising(&test_adv_params);
		break;
	    case ESP_GATTS_OPEN_EVT:
	    case ESP_GATTS_CANCEL_OPEN_EVT:
	    case ESP_GATTS_CLOSE_EVT:
	    case ESP_GATTS_LISTEN_EVT:
	    case ESP_GATTS_CONGEST_EVT:
	 default:
		break;
	}
}
/*******************************************************************
	gatts事件处理器

********************************************************************/
void gatts_event_handler(esp_gatts_cb_event_t event, 
	esp_gatt_if_t gatts_if, 
	esp_ble_gatts_cb_param_t *param)
{
uint8_t v[10]
={	
    0x010,0x01,0x02,
    0x03,0x04,0x05,
    0x06,0x07,0x08,
    0x09
    
    };              

    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT) 
    {
        if (param->reg.status == ESP_GATT_OK)
	{
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
	    printf("gatts_if %d\n",gatts_if);
	   // ESP_LOGI(GATTS_TAG,"param",param);
        } 
	else
	{
            ESP_LOGI(GATTS_TAG,
	    	"Reg app failed, app_id %04x, status %d\n",
                param->reg.app_id, 
                param->reg.status);
            return;
        }
    }

    /* If the gatts_if equal to profile A, call profile A cb handler,
     * so here call each profile's callback */
    do 
    {
        int idx;
        for (idx = 0; idx < PROFILE_NUM; idx++)
	{
            if (gatts_if == ESP_GATT_IF_NONE ||
		gatts_if == gl_profile_tab[idx].gatts_if)  
		/* ESP_GATT_IF_NONE, not specify a certain gatt_if, 
		need to call every profile cb function */

		{
                if (gl_profile_tab[idx].gatts_cb) 
		{
                    gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
		
	                }
                }
        }
    } while (0);
}

/*******************************************************************

********************************************************************/
void ble_init_(void)
{
    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) 
    {
        ESP_LOGE(GATTS_TAG, "%s initialize controller failed\n", __func__);
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (ret) 
    {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed\n", __func__);
        return;
    }

/*******************************************************************/
 ret = esp_bluedroid_init();
 if (ret) 
     {
	 ESP_LOGE(GATTS_TAG, "%s init bluetooth failed\n", __func__);
	 return;
     }
 ret = esp_bluedroid_enable();
     if (ret) 
     {
	 ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed\n", __func__);
	 return;
     }
     
 esp_ble_gatts_register_callback(gatts_event_handler);
 esp_ble_gap_register_callback(gap_event_handler);
 esp_ble_gatts_app_register(PROFILE_A_APP_ID);
 esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL0,ESP_PWR_LVL_N14);
 return;
}
