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
    .text
    .align 2
.globl invokeNative
    .type    invokeNative, @function
invokeNative:
    /*  rdi - function ptr */
    /*  rsi - argv */
    /*  rdx - n_stacks */

    push %rbp
    mov %rsp, %rbp

    mov %rdx, %r10
    mov %rsp, %r11      /* Check that stack is aligned on */
    and $8, %r11        /* 16 bytes. This code may be removed */
    je check_stack_succ /* when we are sure that compiler always */
    int3                /* calls us with aligned stack */
check_stack_succ:
    mov %r10, %r11      /* Align stack on 16 bytes before pushing */
    and $1, %r11        /* stack arguments in case we have an odd */
    shl $3, %r11        /* number of stack arguments */
    sub %r11, %rsp
    /* store memory args */
    movq %rdi, %r11     /* func ptr */
    movq %r10, %rcx     /* counter */
    lea 64+48-8(%rsi,%rcx,8), %r10
    sub %rsp, %r10
    cmpq $0, %rcx
    je push_args_end
push_args:
    push 0(%rsp,%r10)
    loop push_args
push_args_end:
    /* fill all fp args */
    movq 0x00(%rsi), %xmm0
    movq 0x08(%rsi), %xmm1
    movq 0x10(%rsi), %xmm2
    movq 0x18(%rsi), %xmm3
    movq 0x20(%rsi), %xmm4
    movq 0x28(%rsi), %xmm5
    movq 0x30(%rsi), %xmm6
    movq 0x38(%rsi), %xmm7

    /* fill all int args */
    movq 0x40(%rsi), %rdi
    movq 0x50(%rsi), %rdx
    movq 0x58(%rsi), %rcx
    movq 0x60(%rsi), %r8
    movq 0x68(%rsi), %r9
    movq 0x48(%rsi), %rsi

    call *%r11
    leave
    ret

