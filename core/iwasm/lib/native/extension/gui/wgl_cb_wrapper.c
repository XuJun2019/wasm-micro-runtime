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

#include "lvgl.h"
#include "wasm_export.h"
#include "native_interface.h"
#include "module_wasm_app.h"
#include "wgl_native_utils.h"

/* -------------------------------------------------------------------------
 * Label widget native function wrappers
 * -------------------------------------------------------------------------*/
static int32
_cb_create(lv_obj_t *par, lv_obj_t *copy)
{
    return wgl_native_wigdet_create(WIDGET_TYPE_CB, par, copy);
}

static int32
_cb_get_text_length(lv_obj_t *cb)
{
    const char *text = lv_cb_get_text(cb);

    if (text == NULL)
        return 0;

    return strlen(text);
}

static char *
_cb_get_text(lv_obj_t *cb, char *buffer, int buffer_len)
{
    const char *text = lv_cb_get_text(cb);

    if (text == NULL)
        return 0;

    strncpy(buffer, text, buffer_len - 1);
    buffer[buffer_len - 1] = '\0';

    return buffer;
}

static WGLNativeFuncDef cb_native_func_defs[] = {
        { CB_FUNC_ID_CREATE, _cb_create, HAS_RET, 2, {0 | NULL_OK, 1 | NULL_OK, -1}, {-1} },
        { CB_FUNC_ID_SET_TEXT, lv_cb_set_text, NO_RET, 2, {0, -1}, {1, -1} },
        { CB_FUNC_ID_SET_STATIC_TEXT, lv_cb_set_static_text, NO_RET, 2, {0, -1}, {1, -1} },
        { CB_FUNC_ID_GET_TEXT_LENGTH, _cb_get_text_length, HAS_RET, 1, {0, -1}, {-1} },
        { CB_FUNC_ID_GET_TEXT, _cb_get_text, RET_PTR, 3, {0, -1}, {1, -1} },
};

/*************** Native Interface to Wasm App ***********/
void
wasm_cb_native_call(wasm_module_inst_t module_inst,
                    int32 func_id, uint32 argv_offset, uint32 argc)
{
    uint32 size = sizeof(cb_native_func_defs) / sizeof(WGLNativeFuncDef);

    wgl_native_func_call(module_inst,
                         cb_native_func_defs,
                         size,
                         func_id,
                         argv_offset,
                         argc);
}
