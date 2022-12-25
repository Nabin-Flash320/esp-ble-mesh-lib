

#ifndef _BLE_MESH_DEFINITIONS_H_
#define _BLE_MESH_DEFINITIONS_H_


#define TAG                                             "CUSTOM MODEL"
#define ESP_CID                                         0x02E5
#define ESP_BLE_CUSTOM_SERVER_ID                        0x0001
#define PROPERTY_ID                                     0x0056

// Custom OP code definitions
#define ESP_BLE_MESH_MODEL_OP_APP_GET                       ESP_BLE_MESH_MODEL_OP_3(0x00, ESP_CID)
#define ESP_BLE_MESH_MODEL_OP_APP_SET                       ESP_BLE_MESH_MODEL_OP_3(0x01, ESP_CID)
#define ESP_BLE_MESH_MODEL_OP_APP_STATUS                    ESP_BLE_MESH_MODEL_OP_3(0x02, ESP_CID)

#endif //_BLE_MESH_DEFINITIONS_H_



