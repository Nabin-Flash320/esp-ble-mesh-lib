#ifndef _BLE_MESH_MODEL_H_
#define _BLE_MESH_MODEL_H_

#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_defs.h"

#define NET_BUFFER_DEFINE_STATIC(_name, _size)  \
    static uint8_t net_buf_data_##_name[_size];     \
    static struct net_buf_simple _name = {          \
        .data  = net_buf_data_##_name,              \
        .len   = 0,                                 \
        .size  = _size,                             \
        .__buf = net_buf_data_##_name,              \
    }


#define BLE_MESH_MODEL_DEFINE_PUBLICATION(_name, _msg_len, _role, _update) \
    NET_BUFFER_DEFINE_STATIC(bt_mesh_pub_msg_##_name, _msg_len); \
    static esp_ble_mesh_model_pub_t _name = { \
        .update = _update, \
        .msg = &bt_mesh_pub_msg_##_name, \
        .dev_role = _role, \
    }


#define BLE_MESH_CONFIG_SERVER_MODEL_CREATE(config_server_data)                \
{                                                                       \
    .model_id = ESP_BLE_MESH_MODEL_ID_CONFIG_SRV,                       \
    .op = NULL,                                                         \
    .keys = { [0 ... (CONFIG_BLE_MESH_MODEL_KEY_COUNT - 1)] =           \
            ESP_BLE_MESH_KEY_UNUSED },                                  \
    .pub = NULL,                                                        \
    .groups = { [0 ... (CONFIG_BLE_MESH_MODEL_GROUP_COUNT - 1)] =       \
            ESP_BLE_MESH_ADDR_UNASSIGNED },                             \
    .user_data = config_server_data,                                    \
}                                                                       

#define BLE_MESH_CUSTOM_MODEL_CREATE(_company, _id, _op, _pub, _user_data)                \
{                                                                       \
    .vnd.company_id = (_company),                                       \
    .vnd.model_id = (_id),                                              \
    .op = _op,                                                          \
    .pub = _pub,                                                        \
    .keys = { [0 ... (CONFIG_BLE_MESH_MODEL_KEY_COUNT - 1)] =           \
            ESP_BLE_MESH_KEY_UNUSED },                                  \
    .groups = { [0 ... (CONFIG_BLE_MESH_MODEL_GROUP_COUNT - 1)] =       \
            ESP_BLE_MESH_ADDR_UNASSIGNED },                             \
    .user_data = _user_data,                                            \
}                                                                       

esp_ble_mesh_model_t create_config_server_model(esp_ble_mesh_cfg_srv_t* config_server_data);
esp_ble_mesh_model_t create_custom_model(uint16_t model_id, esp_ble_mesh_model_pub_t *const publication, 
                                                esp_ble_mesh_model_op_t* opcode, void* user_data);
esp_ble_mesh_model_pub_t create_model_publication(uint16_t size, uint8_t role, esp_ble_mesh_cb_t update);

#endif //_BLE_MESH_MODEL_H_