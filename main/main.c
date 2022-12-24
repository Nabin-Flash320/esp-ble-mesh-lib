

#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gap_bt_api.h"
#include "esp_gatts_api.h"
#include "string.h"
#include "esp_gatt_common_api.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_sensor_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"


#define TAG                                             "CUSTOM MODEL"
#define CID_ESP                                         0x02E5
#define ESP_BLE_CUSTOM_SERVER_ID                        0x0001
#define PROPERTY_ID                                     0x0056

// Custom OP code definitions
#define ESP_BLE_MESH_MODEL_OP_APP_GET                   ESP_BLE_MESH_MODEL_OP_3(0x00, CID_ESP)
#define ESP_BLE_MESH_MODEL_OP_APP_SET                   ESP_BLE_MESH_MODEL_OP_3(0x01, CID_ESP)
#define ESP_BLE_MESH_MODEL_OP_APP_STATUS                ESP_BLE_MESH_MODEL_OP_3(0x02, CID_ESP)

// OP code definition to the custom model
static esp_ble_mesh_model_op_t custom_op_code[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_MODEL_OP_APP_GET, 0),
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_MODEL_OP_APP_SET, 1),
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_MODEL_OP_APP_STATUS, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};

const char* NAME = "CUSTOM_MODEL";
static uint8_t custom_uuid[16] = {0xdd, 0xdd};

// sensor data structure
typedef struct
{
    const char *device_name;
    float temperature;
    float pressure;
} model_sensor_data_t;

static model_sensor_data_t _server_model_state = {
    .device_name = "BMP",
    .temperature = 0,
    .pressure = 0,
};

static void update_server_state()
{
    _server_model_state.pressure++;
    _server_model_state.temperature++;
    ESP_LOGI(TAG, "_server_model_state updated.");
}

// Configuration server context.
static esp_ble_mesh_cfg_srv_t config_server = {
    .relay              =     ESP_BLE_MESH_RELAY_DISABLED,
    .beacon             =     ESP_BLE_MESH_RELAY_ENABLED,
    .friend_state       =     ESP_BLE_MESH_FRIEND_ENABLED,
    .gatt_proxy         =     ESP_BLE_MESH_GATT_PROXY_ENABLED,
    .default_ttl        =     7,
    .net_transmit       =     ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay_retransmit   =     ESP_BLE_MESH_TRANSMIT(2, 20)
};


// Configuration model definition.
static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
};

// Definition of model publication instance.
ESP_BLE_MESH_MODEL_PUB_DEFINE(custom_srv_pub, 20, ROLE_NODE);
// Creating a mesh model using vendoro instance
static esp_ble_mesh_model_t custom_models[] = {
    ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, 
                                ESP_BLE_CUSTOM_SERVER_ID, 
                                custom_op_code, 
                                &custom_srv_pub, 
                                &_server_model_state),
};


// mesh element instance
static esp_ble_mesh_elem_t elements[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, custom_models),
    // ESP_BLE_MESH_ELEMENT(1, ESP_BLE_MESH_MODEL_NONE, custom_models),
};

static esp_ble_mesh_comp_t composition = {
    .cid = CID_ESP,
    .elements = elements,
    .element_count = ARRAY_SIZE(elements),
};

static esp_ble_mesh_prov_t provision = {
    .uuid = custom_uuid,
};


static void custom_provision_callback(esp_ble_mesh_prov_cb_event_t event,
                                esp_ble_mesh_prov_cb_param_t* param)
{
    switch (event)
    {
    case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROV_REGISTER_COMP_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_RESET_EVT:
    {
        ESP_LOGE(TAG, "ESP_BLE_MESH_NODE_PROV_RESET_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT, (error code: %s)", esp_err_to_name(param->node_set_unprov_dev_name_comp.err_code));
        break;
    }
    case ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT");
        break;
    }
    case ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT:
    {
        ESP_LOGE(TAG, "ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT");
        break;
    }
    case ESP_BLE_MESH_MODEL_SUBSCRIBE_GROUP_ADDR_COMP_EVT:
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_SUBSCRIBE_GROUP_ADDR_COMP_EVT");
        ESP_LOGI(TAG, "error code: %d, element_addr: 0x%x, company_id: 0x%x, model id: 0x%x, group_address: 0x%x", param->model_sub_group_addr_comp.err_code,
                                            param->model_sub_group_addr_comp.element_addr,
                                            param->model_sub_group_addr_comp.company_id,
                                            param->model_sub_group_addr_comp.model_id,
                                            param->model_sub_group_addr_comp.group_addr);
        break;
    }
    default:
    {
        ESP_LOGE(TAG, "Default provisioning callback event. (event: %d)", event);
    }
    }
}

static void custom_config_srv_callback(esp_ble_mesh_cfg_server_cb_event_t event,
                                esp_ble_mesh_cfg_server_cb_param_t *param)
{

    if(event == ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT)
    {
        ESP_LOGI(TAG, "ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT");
        switch(param->ctx.recv_op)
        {
            case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD:
            {
                ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD");
                ESP_LOGI(TAG, "net_idx: 0x%04x, app_idx: 0x%04x", param->value.state_change.netkey_add.net_idx,
                         param->value.state_change.appkey_add.app_idx);
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND:
            {
                ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND");
                ESP_LOGI(TAG, "elem_addr 0x%04x, app_idx 0x%04x, cid 0x%04x, mod_id 0x%04x",
                        param->value.state_change.mod_app_bind.element_addr,
                        param->value.state_change.mod_app_bind.app_idx,
                        param->value.state_change.mod_app_bind.company_id,
                        param->value.state_change.mod_app_bind.model_id);
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_MODEL_APP_UNBIND:
            {
                ESP_LOGE(TAG, "ESP_BLE_MESH_MODEL_OP_MODEL_APP_UNBIND");
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET:
            {
                ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET");
                ESP_LOGI(TAG, "elem_addr 0x%04x, pub_addr 0x%x, app_idx 0x%04x, cid 0x%04x, mod_id 0x%04x",
                         param->value.state_change.mod_pub_set.element_addr,
                         param->value.state_change.mod_pub_set.pub_addr,
                         param->value.state_change.mod_pub_set.app_idx,
                         param->value.state_change.mod_pub_set.company_id,
                         param->value.state_change.mod_pub_set.model_id);
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD:
            {
                ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD");
                ESP_LOGI(TAG, "company id: 0x%x, element addr: 0x%x, model id: 0x%x, sub addr: 0x%x", param->value.state_change.mod_sub_add.company_id,
                         param->value.state_change.mod_sub_add.element_addr,
                         param->value.state_change.mod_sub_add.model_id,
                         param->value.state_change.mod_sub_add.sub_addr);
                esp_ble_mesh_model_subscribe_group_addr(param->value.state_change.mod_sub_add.element_addr,
                            param->value.state_change.mod_sub_add.company_id,
                            param->value.state_change.mod_sub_add.model_id,
                            param->value.state_change.mod_sub_add.sub_addr);
                break;
            }
            default:
            {
                ESP_LOGE(TAG, "Default OP rcv_code.(param->ctx.recv_op: %d)", param->ctx.recv_op);
                break;
            }
        }
    }
    
}

void custom_server_callback(esp_ble_mesh_model_cb_event_t event,
                            esp_ble_mesh_model_cb_param_t *param)
{
    switch(event)
    {
        case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        {
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OPERATION_EVT");
            switch(param->model_operation.opcode)
            {
                case ESP_BLE_MESH_MODEL_OP_APP_GET:
                {
                    ESP_LOGI(TAG, "The opcode is: %x", param->model_operation.opcode);
                    ESP_LOGI(TAG, "ESP ble mesh custom vendor model data, %x", *param->model_operation.msg);
                    esp_ble_mesh_server_model_send_msg(&custom_models[0], 
                                    param->model_operation.ctx, ESP_BLE_MESH_MODEL_OP_APP_STATUS,
                                    strlen(_server_model_state.device_name), (uint8_t*)_server_model_state.device_name);
                    update_server_state();
                    break;
                    }
                case ESP_BLE_MESH_MODEL_OP_APP_SET:
                {
                    ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_APP_SET");
                    ESP_LOGI(TAG, "ESP ble mesh custom vendor model data, %x", *param->model_operation.msg);
                    break;
                }
                case ESP_BLE_MESH_MODEL_OP_APP_STATUS:
                {
                    ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_APP_STATUS");
                    ESP_LOGI(TAG, "param->model_operation.msg, %x\n", *param->model_operation.msg);
                    break;
                }
                default:
                {
                    ESP_LOGE(TAG, "Unknown OP code received!! %d", param->model_operation.opcode);
                    break;
                }
            }
            
            break;
        }
        case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
        {
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_SEND_COMP_EVT");
            ESP_LOGI(TAG, "The error code returned is %s", esp_err_to_name(param->model_send_comp.err_code));
            break;
        }
        case ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT:
        {   
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT");
            // model_sensor_data_t data = *(model_sensor_data_t*)param->model_operation.model->user_data;
            custom_srv_pub.msg->len = sizeof(_server_model_state);
            custom_srv_pub.msg->data = (uint8_t*)&_server_model_state.device_name;
            ESP_LOGI(TAG, "custom_srv_pub.msg->len: %d", custom_srv_pub.msg->len);
            esp_ble_mesh_model_publish(custom_srv_pub.model, ESP_BLE_MESH_MODEL_OP_APP_STATUS,
                                       custom_srv_pub.msg->len, (uint8_t*)&_server_model_state.temperature, ROLE_NODE);
            break;
        }
        case ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT:
        {
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT");
            ESP_LOGI(TAG, "Returned error code is: %s", esp_err_to_name(param->model_publish_comp.err_code));
            break;
        }
        default:
        {
            ESP_LOGE(TAG, "Default custom model event. (event: %d)", event);
            break;
        }
    }
}

esp_err_t ble_mesh_initialize()
{
    esp_err_t err = ESP_OK;
    esp_ble_mesh_register_prov_callback(custom_provision_callback);
    esp_ble_mesh_register_config_server_callback(custom_config_srv_callback);
    esp_ble_mesh_register_custom_model_callback(custom_server_callback);

    err = esp_ble_mesh_init(&provision, &composition);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize mesh stack");
        return err;
    }

    err = esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to enable mesh node");
        return err;
    }

    err = esp_ble_mesh_set_unprovisioned_device_name(NAME);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set unprovisioned node's name");
        return err;
    }

    return err;

}


int app_main()
{

    esp_err_t error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }
    else
    {
        ESP_LOGI(TAG, "NVS flash initialized.");
    }

    // Releasing memory previously occupied.
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    ESP_LOGI(TAG, "Controller memory released successfully");

    // Configuring the controller and initializing the controller.
    esp_bt_controller_config_t controller_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&controller_cfg));
    ESP_LOGI(TAG, "Controller initialization completed successfully.");

    // enabing the bluetooth controller.
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_LOGI(TAG, "Bluetooth controller enabled successfully.");

    // Initializing bluedroid
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_LOGI(TAG, "Bluedroid initialized successfully.");

    // Enabling bluedroid
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    ESP_LOGI(TAG, "Bluedroid enabled successfully.");

    ESP_LOGI(TAG, "Initializing BLE mesh...");
    ESP_ERROR_CHECK(ble_mesh_initialize());
    ESP_LOGI(TAG, "BLE mesh initialized.");
    return 0;
}