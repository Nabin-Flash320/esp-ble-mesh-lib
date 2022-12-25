

#ifndef _BLE_MESH_OP_CODE_H_
#define _BLE_MESH_OP_CODE_H_

#include "esp_ble_mesh_defs.h"


#define BLE_MESH_GET_OP_CODE(_opcode, _min_len, _param_cb)\
{ \
    .opcode = _opcode, \
    .min_len = _min_len, \
    .param_cb = _param_cb \
}


#endif //_BLE_MESH_OP_CODE_H_

