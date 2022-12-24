# ESP BLE Mesh


1. OP-code creation.

    a. First define message OP-codes. Use *ESP_BLE_MESH_MODEL_OP_3* macro to define the message OP-codes.

    b. Create and array of structure *esp_ble_mesh_model_op_t* and populate it with structure type or use predefined macro in header *esp_ble_mesh_defs.h* as below:
    ```c
    static esp_ble_mesh_model_op_t custom_op_code[] = {
        /* Mesh model opcodes */
        ESP_BLE_MESH_MODEL_OP_END, // This marks the end of the opcode.
    };
    ```
    - Use following struct to define the OP-codes
        ```c
        typedef struct {
            const uint32_t    opcode;   /*!< Message opcode */
            const size_t      min_len;  /*!< Message minimum length */
            esp_ble_mesh_cb_t param_cb; /*!< Callback used to handle message. Initialized by the stack. */
        } esp_ble_mesh_model_op_t;
        ```
    - Following macro can also be used to define OP-codes
        ```c
        #define ESP_BLE_MESH_MODEL_OP(_opcode, _min_len) \
        { \
            .opcode = _opcode, \
            .min_len = _min_len, \
            .param_cb = (uint32_t)NULL, \
        }
        ```

2. Creating configuration server context.
    
    a. Use following struct to create configuration server context.(More on the struct will be available in *esp_ble_mesh_config_model_api.h* header file.)
    ```c
    typedef struct esp_ble_mesh_cfg_srv {
        esp_ble_mesh_model_t *model;    /*!< Pointer to Configuration Server Model */

        uint8_t net_transmit;           /*!< Network Transmit state */
        uint8_t relay;                  /*!< Relay Mode state */
        uint8_t relay_retransmit;       /*!< Relay Retransmit state */
        uint8_t beacon;                 /*!< Secure Network Beacon state */
        uint8_t gatt_proxy;             /*!< GATT Proxy state */
        uint8_t friend_state;           /*!< Friend state */
        uint8_t default_ttl;            /*!< Default TTL */
    } esp_ble_mesh_cfg_srv_t;
    ```

3. Define the configuration server model.

    a. Use following struct to create configuration server context.
    ```c
    struct esp_ble_mesh_model {
        /** Model ID */
        union {
            const uint16_t model_id; /*!< 16-bit model identifier */
            struct {
                uint16_t company_id; /*!< 16-bit company identifier */
                uint16_t model_id; /*!< 16-bit model identifier */
            } vnd; /*!< Structure encapsulating a model ID with a company ID */
        };

        /** Internal information, mainly for persistent storage */
        uint8_t  element_idx;   /*!< Belongs to Nth element */
        uint8_t  model_idx;     /*!< Is the Nth model in the element */
        uint16_t flags;         /*!< Information about what has changed */

        /** The Element to which this Model belongs */
        esp_ble_mesh_elem_t *element;

        /** Model Publication */
        esp_ble_mesh_model_pub_t *const pub;

        /** AppKey List */
        uint16_t keys[CONFIG_BLE_MESH_MODEL_KEY_COUNT];

        /** Subscription List (group or virtual addresses) */
        uint16_t groups[CONFIG_BLE_MESH_MODEL_GROUP_COUNT];

        /** Model operation context */
        esp_ble_mesh_model_op_t *op;

        /** Model callback structure */
        esp_ble_mesh_model_cbs_t *cb;

        /** Model-specific user data */
        void *user_data;
    };
    ```

    b. However more efficient and shortcut method to create configuration server model would be using the following macro.
    ```c
    #define ESP_BLE_MESH_MODEL_CFG_SRV(srv_data)                              \
        ESP_BLE_MESH_SIG_MODEL(ESP_BLE_MESH_MODEL_ID_CONFIG_SRV,          \
                  NULL, NULL, srv_data)
    ```
4. Definition of model publication instance.
