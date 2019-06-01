/*
 * Copyright 2019 nzh63
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#ifndef UNSIGNED_BIGINT_ASM_HPP
#define UNSIGNED_BIGINT_ASM_HPP

#include <intrin.h>
#include "./unsigned_bigint.h"

// 以下函数由汇编语言实现
// 详见unsigned_bigint.asm
extern "C" {
// 计算两个e位大数a+b，结果在ans中
constexpr void __stdcall add_proc(int e, const QWORD* a, const QWORD* b,
                                  QWORD* ans);
// 计算一个e位大数a加上一个64位数b，结果在ans中
constexpr void __stdcall add_mix(int e, const QWORD* a, const QWORD b,
                                 QWORD* ans);
// 计算两个e位大数a-b，结果在ans中
constexpr void __stdcall sub_proc(int e, const QWORD* a, const QWORD* b,
                                  QWORD* ans);
// 计算一个e位大数a减去一个64位数b，结果在ans中
constexpr void __stdcall sub_mix(int e, const QWORD* a, const QWORD b,
                                 QWORD* ans);
// 计算一个e位大数a乘以一个64位数b，结果在ans中
constexpr void __stdcall mul_mix(int e, const QWORD* a, const QWORD b,
                                 QWORD* ans);
// 计算一个e位大数a乘以一个64位数b加上ans，结果在ans中
constexpr void __stdcall mul_mix_add(int e, const QWORD* a, const QWORD b,
                                     QWORD* ans);
// 计算一个e位大数a模一个64位数m，结果在rax（返回值）中
constexpr QWORD __stdcall mod_mix(int e, const QWORD* a, const QWORD m);

constexpr QWORD __stdcall div_128(const QWORD ax, QWORD dx, const QWORD div);

// 计算3个e位大数a+b+ans，结果在ans中
constexpr void __stdcall add_three_this(int e, QWORD* ans, const QWORD* a,
                                        const QWORD* b);

// Intel硬件随机数的辅助函数
void __stdcall real_rand(int e, const QWORD* a);
}
// 以上函数由汇编语言实现

#endif
