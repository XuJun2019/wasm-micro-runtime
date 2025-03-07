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

#ifndef LIB_BASE_RUNTIME_TIMER_H_
#define LIB_BASE_RUNTIME_TIMER_H_

#include "bh_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32 bh_get_elpased_ms(uint32 * last_system_clock);

struct _timer_ctx;
typedef struct _timer_ctx * timer_ctx_t;
typedef void (*timer_callback_f)(uint32 id, unsigned int owner);
typedef void (*check_timer_expiry_f)(timer_ctx_t ctx);

timer_ctx_t create_timer_ctx(timer_callback_f timer_handler,
        check_timer_expiry_f, int prealloc_num, unsigned int owner);
void destroy_timer_ctx(timer_ctx_t);
void timer_ctx_set_lock(timer_ctx_t ctx, bool lock);
void * timer_ctx_get_lock(timer_ctx_t ctx);
unsigned int timer_ctx_get_owner(timer_ctx_t ctx);

uint32 sys_create_timer(timer_ctx_t ctx, int interval, bool is_period,
        bool auto_start);
bool sys_timer_destroy(timer_ctx_t ctx, uint32 timer_id);
bool sys_timer_cancel(timer_ctx_t ctx, uint32 timer_id);
bool sys_timer_restart(timer_ctx_t ctx, uint32 timer_id, int interval);
void cleanup_app_timers(timer_ctx_t ctx);
int check_app_timers(timer_ctx_t ctx);
int get_expiry_ms(timer_ctx_t ctx);

void wakeup_timer_thread(timer_ctx_t ctx);
void * thread_timer_check(void * arg);

#ifdef __cplusplus
}
#endif
#endif /* LIB_BASE_RUNTIME_TIMER_H_ */
