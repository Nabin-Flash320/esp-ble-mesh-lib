
#include "ble_mesh_op_code.h"
#include "esp_ble_mesh_defs.h"
#include "ble_mesh_definitions.h"


esp_ble_mesh_model_op_t ble_mesh_get_op_code(const uint32_t op_code, const size_t min_len, uint32_t param_cb)
{
    esp_ble_mesh_model_op_t mesh_op_code = {
        .opcode = op_code,
        .min_len = min_len,
        .param_cb = param_cb,
    };
    return mesh_op_code;
}