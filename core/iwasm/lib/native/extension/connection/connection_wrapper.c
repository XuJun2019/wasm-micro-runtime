/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "connection_lib.h"
#include "wasm_export.h"
#include "native_interface.h"

/* Note:
 *
 * This file is the consumer of connection lib which is implemented by different platforms
 */

uint32
wasm_open_connection(wasm_module_inst_t module_inst,
                     int32 name_offset, int32 args_offset, uint32 len)
{
    attr_container_t *args;
    char *name, *args_buf;

    if (!validate_app_addr(name_offset, 1) ||
        !validate_app_addr(args_offset, len) ||
        !(name = addr_app_to_native(name_offset)) ||
        !(args_buf = addr_app_to_native(args_offset)))
        return -1;

    args = (attr_container_t *)args_buf;

    if (connection_impl._open != NULL)
        return connection_impl._open(name, args);

    return -1;
}

void
wasm_close_connection(wasm_module_inst_t module_inst, uint32 handle)
{
    if (connection_impl._close != NULL)
        connection_impl._close(handle);
}

int
wasm_send_on_connection(wasm_module_inst_t module_inst,
                        uint32 handle, int32 data_offset, uint32 len)
{
    char *data;

    if (!validate_app_addr(data_offset, len) ||
        !(data = addr_app_to_native(data_offset)))
     return -1;

    if (connection_impl._send != NULL)
        return connection_impl._send(handle, data, len);

    return -1;
}

bool
wasm_config_connection(wasm_module_inst_t module_inst,
                       uint32 handle, int32 cfg_offset, uint32 len)
{
    char *cfg_buf;
    attr_container_t *cfg;

    if (!validate_app_addr(cfg_offset, len) ||
        !(cfg_buf = addr_app_to_native(cfg_offset)))
     return false;

    cfg = (attr_container_t *)cfg_buf;

    if (connection_impl._config != NULL)
        return connection_impl._config(handle, cfg);

    return false;
}
