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
#ifndef UNSIGNED_BIGINT_IO_HPP
#define UNSIGNED_BIGINT_IO_HPP

#include <iostream>
#include "./unsigned_bigint.h"

template <int LEN>
constexpr std::istream& operator>>(std::istream& in, unsigned_bigint<LEN>& input_number) {
    if ((std::ios_base::fmtflags)(std::ios_base::dec &
                                  std::ios_base::basefield) ==
        (std::ios_base::fmtflags)(in.flags() &
                                  std::ios_base::basefield)) {  // 十进制
        char c;
        unsigned_bigint<LEN> ans(0);
        while (1) {
            c = static_cast<char>(in.get());
            if (c < '0' || c > '9') {
                input_number = ans;
                return in;
            } else {
                ans *= 10;
                ans += c - '0';
            }
        }
    } else if ((std::ios_base::fmtflags)(std::ios_base::oct &
                                         std::ios_base::basefield) ==
               (std::ios_base::fmtflags)(in.flags() &
                                         std::ios_base::basefield)) {  // 八进制
        char c;
        unsigned_bigint<LEN> ans(0);
        while (1) {
            c = static_cast<char>(in.get());
            if (c < '0' || c > '8') {
                input_number = ans;
                return in;
            } else {
                int num = c - '0';
                ans <<= 3;
                ans += num;
            }
        }
    } else if ((std::ios_base::fmtflags)(std::ios_base::hex &
                                         std::ios_base::basefield) ==
               (std::ios_base::fmtflags)(
                   in.flags() & std::ios_base::basefield)) {  // 十六进制
        char c;
        unsigned_bigint<LEN> ans(0);

        while (1) {
            c = static_cast<char>(in.get());
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
                  (c >= 'a' && c <= 'f'))) {
                input_number = ans;
                return in;
            } else {
                int num = 0;
                if (c >= '0' && c <= '9')
                    num = c - '0';
                else if (c >= 'A' && c <= 'F')
                    num = c - 'A' + 10;
                else if (c >= 'a' && c <= 'f')
                    num = c - 'a' + 10;
                ans <<= 4;
                ans += num;
            }
        }
    }
    return in;
}

template <int LEN>
constexpr std::ostream& operator<<(std::ostream& out,
                         unsigned_bigint<LEN> output_number) {
    if (output_number == 0) {
        out << '0';
        return out;
    }
    if ((std::ios_base::fmtflags)(std::ios_base::dec &
                                  std::ios_base::basefield) ==
        (std::ios_base::fmtflags)(out.flags() &
                                  std::ios_base::basefield)) {  // 十进制
        std::stack<short> s;
        unsigned_bigint<LEN> residue;
        while (output_number != 0) {
            residue = output_number % 10;
            output_number /= 10;
            s.push((short)((unsigned long long)residue));
        }
        while (!s.empty()) {
            out << s.top();
            s.pop();
        }
    } else if ((std::ios_base::fmtflags)(std::ios_base::oct &
                                         std::ios_base::basefield) ==
               (std::ios_base::fmtflags)(out.flags() &
                                         std::ios_base::basefield)) {  // 八进制
        int cur = 0;
        bool outputted = 0;
        if ((LEN * 64) % 3 != 0) {
            short o = 0;
            for (; cur < (LEN * 64) % 3; cur++) {
                o *= 2;
                o += output_number.bit(LEN * 64 - cur - 1);
            }
            if (outputted == 1 || o != 0) {
                out << o;
                outputted = 1;
            }
        }
        while (cur != LEN * 64) {
            short o = 0;
            for (int i = 0; i < 3; i++, cur++) {
                o *= 2;
                o += output_number.bit(LEN * 64 - cur - 1);
            }
            if (outputted == 1 || o != 0) {
                out << o;
                outputted = 1;
            }
        }
    } else if ((std::ios_base::fmtflags)(std::ios_base::hex &
                                         std::ios_base::basefield) ==
               (std::ios_base::fmtflags)(
                   out.flags() & std::ios_base::basefield)) {  // 十六进制
        int cur = 0;
        bool outputted = 0;
        if ((LEN * 64) % 4 != 0) {
            short o = 0;
            for (; cur < (LEN * 64) % 4; cur++) {
                o *= 2;
                o += output_number.bit(LEN * 64 - cur - 1);
            }
            if (outputted == 1 || o != 0) {
                outputted = 1;
                if (o < 10) {
                    out << o;
                } else {
                    out << (char)('A' - 10 + o);
                }
            }
        }
        while (cur != LEN * 64) {
            short o = 0;
            for (int i = 0; i < 4; i++, cur++) {
                o *= 2;
                o += output_number.bit(LEN * 64 - cur - 1);
            }
            if (outputted == 1 || o != 0) {
                outputted = 1;
                if (o < 10) {
                    out << o;
                } else {
                    out << (char)('A' - 10 + o);
                }
            }
        }
    }
    return out;
}

#endif
