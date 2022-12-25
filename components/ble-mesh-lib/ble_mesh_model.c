
#include "ble_mesh_model.h"
#include "sdkconfig.h"
#include "string.h"
#include "ble_mesh_definitions.h"


esp_ble_mesh_model_t create_config_server_model(esp_ble_mesh_cfg_srv_t* config_server_data)
{
    esp_ble_mesh_model_t config_server_model = {
        .model_id = ESP_BLE_MESH_MODEL_ID_CONFIG_SRV,
        .op = NULL,
        .keys = { [0 ... (CONFIG_BLE_MESH_MODEL_KEY_COUNT - 1)] = ESP_BLE_MESH_KEY_UNUSED },                                  
        .pub = NULL,                                                        
        .groups = { [0 ... (CONFIG_BLE_MESH_MODEL_GROUP_COUNT - 1)] = ESP_BLE_MESH_ADDR_UNASSIGNED },     
        .user_data = config_server_data, 
    };
    return config_server_model;
}


esp_ble_mesh_model_t create_custom_model(uint16_t model_id, esp_ble_mesh_model_pub_t *const publication, 
                                                esp_ble_mesh_model_op_t* opcode, void* user_data)
{
    esp_ble_mesh_model_t custom_model = {
        .vnd.company_id = ESP_CID,
        .vnd.model_id  = model_id,
        .op = opcode,
        .pub = publication,
        .keys = { [0 ... (CONFIG_BLE_MESH_MODEL_KEY_COUNT - 1)] = ESP_BLE_MESH_KEY_UNUSED },
        .groups = { [0 ... (CONFIG_BLE_MESH_MODEL_GROUP_COUNT - 1)] = ESP_BLE_MESH_ADDR_UNASSIGNED }, 
        .user_data = user_data,
    };
    return custom_model;
}


esp_ble_mesh_model_pub_t create_model_publication(uint16_t size, uint8_t role, esp_ble_mesh_cb_t update)
{
    uint8_t net_buffer_data[size];
    struct net_buf_simple network_buffer = {
        .__buf = net_buffer_data,
        .len = 0,
        .size = size,
        .data = net_buffer_data
    };
    esp_ble_mesh_model_pub_t mesh_model_publication = {
        .update = update,
        .msg = &network_buffer,
        .dev_role = role
    };
    return mesh_model_publication;
}



