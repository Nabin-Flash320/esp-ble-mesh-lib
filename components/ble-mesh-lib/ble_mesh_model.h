#ifndef _BLE_MESH_MODEL_H_
#define _BLE_MESH_MODEL_H_

#include "esp_ble_mesh_defs.h"


#define BLE_MESH_CONFIG_SERVER_MODEL(config_server_data)                              \
        ESP_BLE_MESH_SIG_MODEL(ESP_BLE_MESH_MODEL_ID_CONFIG_SRV,          \
                  NULL, NULL, srv_data)

#endif //_BLE_MESH_MODEL_H_