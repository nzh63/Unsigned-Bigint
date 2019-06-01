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
#ifndef UNSIGNED_BIGINT_HPP
#define UNSIGNED_BIGINT_HPP

#include "./unsigned_bigint.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <limits>
#include <sstream>
#include <stack>
#include <vector>

#include "./unsigned_bigint_asm.hpp"
#include "./unsigned_bigint_io.hpp"

template <int LEN>
constexpr void unsigned_bigint<LEN>::fft(std::complex<double> a[],
                                         const int n) {
    transform(a, n, 1);
}

template <int LEN>
constexpr void unsigned_bigint<LEN>::idft(std::complex<double> a[],
                                          const int n) {
    transform(a, n, 0);
    for (int i = 0; i < n; ++i) a[i] /= n;
}

template <int LEN>
constexpr void unsigned_bigint<LEN>::transform(std::complex<double> a[],
                                               const int n, const bool flag) {
    const double PI = 3.14159265358979323846;
    using namespace std::complex_literals;
    for (int i = 0, j = 0; i < n; ++i) {
        if (i > j) std ::swap(a[i], a[j]);
        for (int l = n >> 1; (j ^= l) < l; l >>= 1)
            ;
    }

    for (int l = 2; l <= n; l <<= 1) {
        int m = l / 2;
        std::complex<double> wn;
        if (flag)
            wn = cos(2 * PI / l) + sin(2 * PI / l) * 1i;
        else
            wn = cos(2 * PI / l) - sin(2 * PI / l) * 1i;
        for (std::complex<double>* p = a; p != a + n; p += l) {
            std::complex<double> w = 1;
            for (int k = 0; k < m; k++) {
                std::complex<double> t = w * p[k + m];
                p[k + m] = p[k] - t;
                p[k] += t;
                w *= wn;
            }
        }
    }
}

template <int LEN>
bool unsigned_bigint<LEN>::Miller_Rabin(const int times) const {
    const unsigned_bigint<LEN>& n = *this;
    if (n == 0 || n == 1) {
        return false;
    } else if (n == 2) {
        return true;
    } else {
        unsigned_bigint<LEN> u = n - 1;
        int t = 0;
        while (u.bit(0) == 0) {
            t++;
            u >>= 1;
        }
        for (int i = 0; i < times; i++) {
            unsigned_bigint<LEN> a = unsigned_bigint<LEN>::rand();
            a = a % (n - 1) + 1;
            a = fast_pow_mod(a, u, n);
            for (int j = 0; j < t; j++) {
                unsigned_bigint<LEN> y = multiply_mod(a, a, n);
                if (y == 1 && a != 1 && a != n - 1) return false;
                a = y;
            }
            if (a != 1) {
                return false;
            }
        }
        return true;
    }
}

template <int LEN>
template <int LEN2>
constexpr int unsigned_bigint<LEN>::cmp_with_offset(
    const unsigned_bigint<LEN2>& a, const unsigned_bigint<LEN2>& b, const int n,
    const int all_zero) const {
    for (int i = LEN2 - 1 - all_zero; i >= n; i--) {
        if (a.data[i] != b.data[i - n]) {
            if (a.data[i] > b.data[i - n]) {
                return 1;
            } else {
                return -1;
            }
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        if (a.data[i] != 0) return 1;
    }
    return 0;
}

template <int LEN>
constexpr unsigned_bigint<LEN>::unsigned_bigint() {}

template <int LEN>
constexpr unsigned_bigint<LEN>::unsigned_bigint(const QWORD a) {
    memset(this->data, 0, sizeof this->data);
    this->data[0] = a;
}

template <int LEN>
constexpr unsigned_bigint<LEN>::unsigned_bigint(const std::string a) {
    std::istringstream is(a);
    if (a[0] == '0' && a[1] == 'x') {
        is.seekg(2);
        is >> std::hex >> *this;
    } else {
        is >> std::dec >> *this;
    }
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::bit(const int i) const {
    return (this->data[i / 64] >> (i % 64) & 1);
}

template <int LEN>
constexpr typename unsigned_bigint<LEN>::bitEditor unsigned_bigint<LEN>::bit(
    const int i) {
    return bitEditor(this, i);
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator+(
    const QWORD addend) const {
    unsigned_bigint ans = *this;
    add_mix(LEN, &(this->data[0]), addend, &(ans.data[0]));
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator+(
    const unsigned_bigint& addend) const {
    unsigned_bigint ans = *this;
    add_proc(LEN, &(this->data[0]), &(addend.data[0]), &(ans.data[0]));
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator+=(
    const QWORD addend) {
    add_mix(LEN, &(this->data[0]), addend, &(this->data[0]));
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator+=(
    const unsigned_bigint& addend) {
    add_proc(LEN, &(this->data[0]), &(addend.data[0]), &(this->data[0]));
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator-(
    const QWORD subtrahend) const {
    unsigned_bigint ans = *this;
    sub_mix(LEN, &(this->data[0]), subtrahend, &(ans.data[0]));
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator-(
    const unsigned_bigint& subtrahend) const {
    unsigned_bigint ans = *this;
    sub_proc(LEN, &(this->data[0]), &(subtrahend.data[0]), &(ans.data[0]));
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator-=(
    const QWORD subtrahend) {
    sub_mix(LEN, &(this->data[0]), subtrahend, &(this->data[0]));
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator-=(
    const unsigned_bigint& subtrahend) {
    sub_proc(LEN, &(this->data[0]), &(subtrahend.data[0]), &(this->data[0]));
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator*(
    const QWORD multiplier) const {
    unsigned_bigint ans;
    mul_mix(LEN, &(this->data[0]), multiplier, &(ans.data[0]));
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator*(
    const unsigned_bigint& multiplier) const {
    if (LEN >= 128)
        return this->mul_fft(multiplier);
    else
        return this->mul_classic(multiplier);
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::mul_classic(
    const unsigned_bigint& multiplier) const {
    unsigned_bigint ans(0);
    for (int i = 0; i < LEN; i++) {
        // ans := ans + (((*this) * multiplier[i]) << (64 * i));
        mul_mix_add(LEN - i, &(this->data[0]), multiplier.data[i],
                    &(ans.data[i]));
    }
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::mul_fft(
    const unsigned_bigint& multiplier) const {
    unsigned_bigint ans;
    int n = 1;
    while (n < LEN) n <<= 1;
    n *= 8;
    std::complex<double>*multiplicand_complex = new std::complex<double>[n],
    *multiplie_complex = new std::complex<double>[n];
    const unsigned __int8 *multiplicand_char_ptr =
                              reinterpret_cast<const unsigned __int8*>(
                                  &(this->data[0])),
                          *multiplier_char_ptr =
                              reinterpret_cast<const unsigned __int8*>(
                                  &(multiplier.data[0]));
    for (int i = 0; i < n; i++) {
        if (i < LEN * 8) {
            multiplicand_complex[i] = multiplicand_char_ptr[i];
            multiplie_complex[i] = multiplier_char_ptr[i];
        } else {
            multiplicand_complex[i] = 0;
            multiplie_complex[i] = 0;
        }
    }
    fft(multiplicand_complex, n);
    fft(multiplie_complex, n);
    for (int i = 0; i < n; i++) {
        multiplicand_complex[i] *= multiplie_complex[i];
    }
    idft(multiplicand_complex, n);
    for (int i = 0; i < LEN * 8; i++) {
        unsigned int tmp =
            (unsigned int)std::round(multiplicand_complex[i].real());
        multiplicand_complex[i + 1].real(multiplicand_complex[i + 1].real() +
                                         tmp / 256);
        *(reinterpret_cast<unsigned __int8*>(ans.data) + i) = tmp % 256;
    }
    delete[] multiplicand_complex;
    delete[] multiplie_complex;

    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator*=(
    const QWORD multiplier) {
    *this = *this * multiplier;
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator*=(
    const unsigned_bigint& multiplier) {
    *this = *this * multiplier;
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator/(
    const unsigned_bigint& divisor) const {
    // 来自Handbook of Applied Cryptography
    // http://cacr.uwaterloo.ca/hac/about/chap14.pdf
    // 14.20 Algorithm Multiple-precision division
    assert(divisor != 0);
    unsigned_bigint<LEN + 1> x = *this, q(0);
    const unsigned_bigint<LEN + 1> y = divisor;
    int n = LEN - 1, t = LEN - 1;
    while (x.data[n] == 0) n--;
    while (y.data[t] == 0) t--;
    if (n < t) return unsigned_bigint(0);

    if (cmp_with_offset(x, y, n - t, LEN - 1 - n) >= 0) {
        unsigned_bigint<LEN + 1> yl[16];
        yl[0] = y;
        for (int i = 1; i < 16; i++) {
            yl[i] = y << (4 * i);
        }
        while (cmp_with_offset(x, y, n - t, LEN - 1 - n) >= 0) {
            for (int i = 15; i >= 0; i--) {
                while (cmp_with_offset(x, yl[i], n - t, LEN - 1 - n) >= 0) {
                    q.data[n - t] += 1i64 << (i * 4);
                    // x := x - (y << ((n - t) * 64 + i));
                    sub_proc(LEN - (n - t), &(x.data[n - t]), &(yl[i].data[0]),
                             &(x.data[n - t]));
                }
            }
        }
    }

    for (int i = n; i > t; i--) {
        if (x.data[i] == y.data[t]) {
            q.data[i - t - 1] = 0xffffffffffffffff;
        } else {
            q.data[i - t - 1] = div_128(x.data[i - 1], x.data[i], y.data[t]);
        }
        unsigned_bigint<3> tmp1, tmp2;
        tmp1.data[0] = t == 0 ? 0 : y.data[t - 1];
        tmp1.data[1] = y.data[t];
        tmp1.data[2] = 0;

        tmp2.data[0] = x.data[i - 2];
        tmp2.data[1] = x.data[i - 1];
        tmp2.data[2] = x.data[i];

        if (tmp1 * q.data[i - t - 1] > tmp2) {
            for (int k = 63; k >= 0; k--) {
                QWORD z = 1i64 << k;
                if (q.data[i - t - 1] > z) {
                    if (tmp1 * (q.data[i - t - 1] - z) > tmp2) {
                        q.data[i - t - 1] -= z;
                    }
                }
            }
            if (tmp1 * q.data[i - t - 1] > tmp2) {
                q.data[i - t - 1]--;
            }
        }

        // x = x - ((y * q.data[i - t - 1]) << ((i - t - 1) * 64));
        unsigned_bigint<LEN + 1> tmp = y * q.data[i - t - 1];
        sub_proc(LEN - (i - t - 1), &(x.data[i - t - 1]), &(tmp.data[0]),
                 &(x.data[i - t - 1]));
        if (x.data[LEN - 1] != 0) {
            // x := x + y << ((i - t - 1) * 64);
            add_proc(LEN - (i - t - 1), &(x.data[i - t - 1]), &(y.data[0]),
                     &(x.data[i - t - 1]));
            q.data[i - t - 1]--;
        }
    }
    return (unsigned_bigint<LEN>)q;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator/=(
    const unsigned_bigint& divisor) {
    *this = *this / divisor;
    return *this;
}

template <int LEN>
constexpr QWORD unsigned_bigint<LEN>::operator%(const QWORD divisor) const {
    assert((divisor != 1 && divisor != 0));
    return mod_mix(LEN, &(this->data[0]), divisor);
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator%(
    const unsigned_bigint& divisor) const {
    // 来自Handbook of Applied Cryptography
    // http://cacr.uwaterloo.ca/hac/about/chap14.pdf
    // 14.20 Algorithm Multiple-precision division
    assert(divisor != 0);
    unsigned_bigint<LEN + 1> x = *this, q(0);
    const unsigned_bigint<LEN + 1> y = divisor;
    int n = LEN - 1, t = LEN - 1;
    while (x.data[n] == 0) n--;
    while (y.data[t] == 0) t--;
    if (n < t) return *this;

    if (cmp_with_offset(x, y, n - t, LEN - 1 - n) >= 0) {
        unsigned_bigint<LEN + 1> yl[16];
        yl[0] = y;
        for (int i = 1; i < 16; i++) {
            yl[i] = y << (4 * i);
        }
        while (cmp_with_offset(x, y, n - t, LEN - 1 - n) >= 0) {
            for (int i = 15; i >= 0; i--) {
                while (cmp_with_offset(x, yl[i], n - t, LEN - 1 - n) >= 0) {
                    q.data[n - t] += 1i64 << (i * 4);
                    // x := x - (y << ((n - t) * 64 + i));
                    sub_proc(LEN - (n - t), &(x.data[n - t]), &(yl[i].data[0]),
                             &(x.data[n - t]));
                }
            }
        }
    }

    for (int i = n; i > t; i--) {
        if (x.data[i] == y.data[t]) {
            q.data[i - t - 1] = 0xffffffffffffffff;
        } else {
            q.data[i - t - 1] = div_128(x.data[i - 1], x.data[i], y.data[t]);
        }
        unsigned_bigint<3> tmp1, tmp2;
        tmp1.data[0] = t == 0 ? 0 : y.data[t - 1];
        tmp1.data[1] = y.data[t];
        tmp1.data[2] = 0;

        tmp2.data[0] = x.data[i - 2];
        tmp2.data[1] = x.data[i - 1];
        tmp2.data[2] = x.data[i];

        if (tmp1 * q.data[i - t - 1] > tmp2) {
            for (int k = 63; k >= 0; k--) {
                QWORD z = 1i64 << k;
                if (q.data[i - t - 1] > z) {
                    if (tmp1 * (q.data[i - t - 1] - z) > tmp2) {
                        q.data[i - t - 1] -= z;
                    }
                }
            }
            if (tmp1 * q.data[i - t - 1] > tmp2) {
                q.data[i - t - 1]--;
            }
        }

        // x := x - ((y * q.data[i - t - 1]) << ((i - t - 1) * 64));
        unsigned_bigint<LEN + 1> tmp = y * q.data[i - t - 1];
        sub_proc(LEN - (i - t - 1), &(x.data[i - t - 1]), &(tmp.data[0]),
                 &(x.data[i - t - 1]));
        if (x.data[LEN - 1] != 0) {
            // x := x + y << ((i - t - 1) * 64);
            add_proc(LEN - (i - t - 1), &(x.data[i - t - 1]), &(y.data[0]),
                     &(x.data[i - t - 1]));
            q.data[i - t - 1]--;
        }
    }
    return (unsigned_bigint<LEN>)x;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator%=(
    const QWORD divisor) {
    *this = *this % divisor;
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator%=(
    const unsigned_bigint& divisor) {
    *this = *this % divisor;
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN>& unsigned_bigint<LEN>::operator++() {
    *this += 1;
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator++(int) {
    unsigned_bigint<LEN> ans = *this;
    *this += 1;
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN>& unsigned_bigint<LEN>::operator--() {
    *this -= 1;
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator--(int) {
    unsigned_bigint<LEN> ans = *this;
    *this -= 1;
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator>>(
    const int a) const {
    unsigned_bigint ans;
    if (a == 0) {
        ans = *this;
    } else if (a == 1) {
        QWORD tmp = 0;
        for (int i = LEN - 1; i >= 0; i--) {
            ans.data[i] = this->data[i] >> 1;
            ans.data[i] |= tmp;
            tmp = this->data[i] << 63;
        }
    } else if (a % 8 == 0) {
        QWORD b = a / 8;
        std::memcpy((unsigned char*)ans.data, (unsigned char*)this->data + b,
                    (LEN * 8 - b) * sizeof(QWORD) / 8);
        std::memset((unsigned char*)ans.data + LEN * 8 - b, 0,
                    b * sizeof(QWORD) / 8);
    } else {
        int b = a / 64;
        int c = a % 64;
        std::memset(&(ans.data[LEN - b]), 0, b * sizeof(QWORD));
        if (c == 0) {
            std::memcpy(&(ans.data[0]), &(this->data[b]),
                        (LEN - b) * sizeof(QWORD));
        } else {
            QWORD tmp = 0;
            for (int i = LEN - b - 1; i >= 0; i--) {
                ans.data[i] = this->data[i + b] >> c;
                ans.data[i] |= tmp;
                tmp = this->data[i + b] << (64 - c);
            }
        }
    }
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator>>=(const int a) {
    if (a == 0) {
        // donothing
    } else if (a == 1) {
        QWORD tmp = 0;
        for (int i = LEN - 1; i >= 0; i--) {
            QWORD tmp_new = 0;
            tmp_new = this->data[i] << 63;
            this->data[i] >>= 1;
            this->data[i] |= tmp;
            tmp = tmp_new;
        }
    } else if (a % 8 == 0) {
        QWORD b = a / 8;
        std::memmove((unsigned char*)(this->data),
                     (unsigned char*)this->data + b,
                     (LEN * 8 - b) * sizeof(QWORD) / 8);
        std::memset((unsigned char*)(this->data) + LEN * 8 - b, 0,
                    b * sizeof(QWORD) / 8);
    } else {
        *this = *this >> a;
    }
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator<<(
    const int a) const {
    unsigned_bigint ans;
    if (a == 0) {
        ans = *this;
    } else if (a == 1) {
        QWORD tmp = 0;
        for (int i = 0; i < LEN; i++) {
            ans.data[i] = this->data[i] << 1;
            ans.data[i] |= tmp;
            tmp = this->data[i] >> 63;
        }
    } else if (a % 8 == 0) {
        QWORD b = a / 8;
        std::memcpy((unsigned char*)ans.data + b, (unsigned char*)this->data,
                    (LEN * 8 - b) * sizeof(QWORD) / 8);
        std::memset((unsigned char*)ans.data, 0, b * sizeof(QWORD) / 8);
    } else {
        int b = a / 64;
        int c = a % 64;
        std::memset(&(ans.data[0]), 0, b * sizeof(QWORD));
        if (c == 0) {
            std::memcpy(&(ans.data[b]), &(this->data[0]),
                        (LEN - b) * sizeof(QWORD));
        } else {
            QWORD tmp = 0;
            for (int i = 0; i < LEN - b; i++) {
                ans.data[i + b] = this->data[i] << c;
                ans.data[i + b] |= tmp;
                tmp = this->data[i] >> (64 - c);
            }
        }
    }
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator<<=(const int a) {
    if (a == 0) {
        // donothing
    } else if (a == 1) {
        QWORD tmp = 0;
        for (int i = 0; i < LEN; i++) {
            QWORD tmp_new = this->data[i] >> 63;
            this->data[i] <<= 1;
            this->data[i] |= tmp;
            tmp = tmp_new;
        }
    } else if (a % 8 == 0) {
        QWORD b = a / 8;
        std::memmove((unsigned char*)(this->data) + b,
                     (unsigned char*)this->data,
                     (LEN * 8 - b) * sizeof(QWORD) / 8);
        std::memset((unsigned char*)(this->data), 0, b * sizeof(QWORD) / 8);
    } else {
        *this = *this << a;
    }
    return *this;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator==(
    const unsigned_bigint& a) const {
    for (int i = 0; i < LEN; i++) {
        if (this->data[i] != a.data[i]) return false;
    }
    return true;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator!=(
    const unsigned_bigint& a) const {
    for (int i = 0; i < LEN; i++) {
        if (this->data[i] != a.data[i]) return true;
    }
    return false;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator>(const unsigned_bigint& a) const {
    for (int i = LEN - 1; i >= 0; i--) {
        if (this->data[i] != a.data[i]) return this->data[i] > a.data[i];
    }
    return false;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator<(const unsigned_bigint& a) const {
    for (int i = LEN - 1; i >= 0; i--) {
        if (this->data[i] != a.data[i]) return this->data[i] < a.data[i];
    }
    return false;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator>=(
    const unsigned_bigint& a) const {
    for (int i = LEN - 1; i >= 0; i--) {
        if (this->data[i] != a.data[i]) return this->data[i] > a.data[i];
    }
    return true;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator<=(
    const unsigned_bigint& a) const {
    for (int i = LEN - 1; i >= 0; i--) {
        if (this->data[i] != a.data[i]) return this->data[i] < a.data[i];
    }
    return true;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator==(const QWORD a) const {
    return *this == (unsigned_bigint)a;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator!=(const QWORD a) const {
    return *this != (unsigned_bigint)a;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator>(const QWORD a) const {
    return *this > (unsigned_bigint)a;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator<(const QWORD a) const {
    return *this < (unsigned_bigint)a;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator>=(const QWORD a) const {
    return *this >= (unsigned_bigint)a;
}

template <int LEN>
constexpr bool unsigned_bigint<LEN>::operator<=(const QWORD a) const {
    return *this <= (unsigned_bigint)a;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator&(
    const unsigned_bigint& a) const {
    unsigned_bigint ans;
    for (int i = 0; i < LEN; i++) ans.data[i] = this->data[i] & a.data[i];
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator&=(
    const unsigned_bigint& a) {
    for (int i = 0; i < LEN; i++) this->data[i] &= a.data[i];
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator|(
    const unsigned_bigint& a) const {
    unsigned_bigint ans;
    for (int i = 0; i < LEN; i++) ans.data[i] = this->data[i] | a.data[i];
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator|=(
    const unsigned_bigint& a) {
    for (int i = 0; i < LEN; i++) this->data[i] |= a.data[i];
    return *this;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator^(
    const unsigned_bigint& a) const {
    unsigned_bigint ans;
    for (int i = 0; i < LEN; i++) ans.data[i] = this->data[i] ^ a.data[i];
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::operator^=(
    const unsigned_bigint& a) {
    for (int i = 0; i < LEN; i++) this->data[i] ^= a.data[i];
    return *this;
}

template <int LEN>
template <int LEN2>
constexpr unsigned_bigint<LEN>::operator unsigned_bigint<LEN2>() const {
    unsigned_bigint<LEN2> ans;
    memcpy(ans.data, this->data, std::min(LEN, LEN2) * sizeof(QWORD));
    for (int i = LEN; i < LEN2; i++) {
        ans.data[i] = 0;
    }
    return ans;
}

template <int LEN>
constexpr unsigned_bigint<LEN>::operator unsigned long long() const {
    return this->data[0];
}

template <int LEN>
template <int LEN2>
constexpr unsigned_bigint<LEN>::unsigned_bigint(
    const unsigned_bigint<LEN2>& a,
    const typename overload_if<(LEN2 < LEN), int>::type dummy) {
    dummy;  // C4100
    memcpy(this->data, a.data, LEN2 * sizeof(QWORD));
    for (int i = LEN2; i < LEN; i++) {
        this->data[i] = 0;
    }
}

template <int LEN>
constexpr unsigned_bigint<LEN> unsigned_bigint<LEN>::toMontgomery(
    const unsigned_bigint& n) const {
    unsigned_bigint<LEN* 2> a = (*this), n2 = n;
    a <<= LEN * 64;
    if (n.bit(LEN * 64 - 1) == 1 && *this == 1) {
        a -= n2;
    } else {
        a %= n2;
    }
    return (unsigned_bigint<LEN>)a;
}

template <int LEN>
bool unsigned_bigint<LEN>::is_prime_number(const int times, bool pre_sieve) {
    const std::vector<QWORD> sieve = {3, 5, 7, 11, 13, 17};
    if (pre_sieve) {
        for (auto a : sieve) {
            if (*this != a && *this % a == 0) {
                return false;
            }
        }
    }
    return this->Miller_Rabin(times);
}

template <int LEN>
unsigned_bigint<LEN> unsigned_bigint<LEN>::rand() {
    unsigned_bigint<LEN> ans(0);
    const int times = (int)std::ceil(64.0 / std::floor(log2(RAND_MAX + 1)));
    for (int i = 0; i < LEN; i++) {
        QWORD a = 0;
        for (int j = 0; j < times; j++) {
            a <<= 15;
            a += std::rand();
        }
        ans.data[i] = a;
    }
    return ans;
}

template <int LEN>
unsigned_bigint<LEN> unsigned_bigint<LEN>::randIntel() {
    unsigned_bigint<LEN> ans;
    real_rand(LEN, &(ans.data[0]));
    return ans;
}

template <int LEN>
unsigned_bigint<LEN>::bitEditor::bitEditor(unsigned_bigint<LEN>* _p, int _i) {
    p = _p;
    i = _i;
}

template <int LEN>
bool unsigned_bigint<LEN>::bitEditor::operator=(const bool a) {
    if (a)
        p->data[i / 64] |= 1i64 << (i % 64);
    else
        p->data[i / 64] &= ~(1i64 << (i % 64));
    return a;
}

template <int LEN>
constexpr unsigned_bigint<LEN>::bitEditor::operator bool() const {
    return (p->data[i / 64] >> (i % 64) & 1);
}

template <int LEN>
constexpr unsigned_bigint<LEN> operator+(const QWORD a,
                                         const unsigned_bigint<LEN>& b) {
    return b + a;
}

template <int LEN>
constexpr unsigned_bigint<LEN> operator-(const QWORD a,
                                         const unsigned_bigint<LEN>& b) {
    return unsigned_bigint<LEN>(a) - b;
}

template <int LEN>
constexpr unsigned_bigint<LEN> operator*(const QWORD a,
                                         const unsigned_bigint<LEN>& b) {
    return b * a;
}

template <int LEN>
void exgcd(const unsigned_bigint<LEN>& a, const unsigned_bigint<LEN>& b,
           unsigned_bigint<LEN>& x,
           unsigned_bigint<LEN>& y) {  // ax - by = 1
    if (b == 0) {
        x = 1;
        y = 0;
    } else {
        unsigned_bigint<LEN> ty, tx;
        exgcd(b, a % b, tx, ty);
        x = b;
        y = a;
        auto tmp = tx + (a / b) * ty;
        while (y < tmp || x < ty) {
            x += b;
            y += a;
        }
        x -= ty;
        y -= tmp;
    }
}

template <int LEN>
void binexgcd(const unsigned_bigint<LEN>& a, const unsigned_bigint<LEN>& b,
              unsigned_bigint<LEN + 1>& x,
              unsigned_bigint<LEN + 1>& y) {  // ax - by = 1
    unsigned_bigint<LEN + 1> g = 1, a2 = a, b2 = b;
    while (a2.bit(0) == 0 && b2.bit(0) == 0) {
        a2 >>= 1;
        b2 >>= 1;
        g <<= 1;
    }
    unsigned_bigint<LEN + 1> u = a2, v = b2, A(1), B(0), C(0), D(1);
    do {
        while (u.bit(0) == 0) {
            u >>= 1;
            u.bit(LEN * 64 + 64 - 1) = (bool)u.bit(LEN * 64 + 64 - 2);
            if (A.bit(0) == 0 && B.bit(0) == 0) {
                A >>= 1;
                A.bit(LEN * 64 + 64 - 1) = (bool)A.bit(LEN * 64 + 64 - 2);
                B >>= 1;
                B.bit(LEN * 64 + 64 - 1) = (bool)B.bit(LEN * 64 + 64 - 2);
            } else {
                A = (A + b2) >> 1;
                A.bit(LEN * 64 + 64 - 1) = (bool)A.bit(LEN * 64 + 64 - 2);
                B = (B - a2) >> 1;
                B.bit(LEN * 64 + 64 - 1) = (bool)B.bit(LEN * 64 + 64 - 2);
            }
        }
        while (v.bit(0) == 0) {
            v >>= 1;
            v.bit(LEN * 64 + 64 - 1) = (bool)v.bit(LEN * 64 + 64 - 2);
            if (C.bit(0) == 0 && D.bit(0) == 0) {
                C >>= 1;
                C.bit(LEN * 64 + 64 - 1) = (bool)C.bit(LEN * 64 + 64 - 2);
                D >>= 1;
                D.bit(LEN * 64 + 64 - 1) = (bool)D.bit(LEN * 64 + 64 - 2);
            } else {
                C = (C + b2) >> 1;
                C.bit(LEN * 64 + 64 - 1) = (bool)C.bit(LEN * 64 + 64 - 2);
                D = (D - a2) >> 1;
                D.bit(LEN * 64 + 64 - 1) = (bool)D.bit(LEN * 64 + 64 - 2);
            }
        }
        if ((u.bit(LEN * 64 + 64 - 1) == 0 && v.bit(LEN * 64 + 64 - 1) == 0 &&
             u >= v) ||
            (u.bit(LEN * 64 + 64 - 1) == 0 && v.bit(LEN * 64 + 64 - 1) == 1) ||
            (u.bit(LEN * 64 + 64 - 1) == 1 && v.bit(LEN * 64 + 64 - 1) == 1 &&
             u <= v)) {
            u -= v;
            A -= C;
            B -= D;
        } else {
            v -= u;
            C -= A;
            D -= B;
        }
    } while (u != 0);
    D = 0 - D;
    x = C;
    y = D;
}

template <int LEN>
constexpr unsigned_bigint<LEN> multiply_mod(
    const unsigned_bigint<LEN>& multiplicand,
    const unsigned_bigint<LEN>& multiplier, const unsigned_bigint<LEN>& mod) {
    unsigned_bigint<LEN* 2> a2 = multiplicand;
    a2 *= multiplier;
    a2 %= mod;
    return (unsigned_bigint<LEN>)a2;
}

template <int LEN>
constexpr unsigned_bigint<LEN> multiply_mod_Montgomery(
    const unsigned_bigint<LEN>& multiplicand,
    const unsigned_bigint<LEN>& multiplier, const unsigned_bigint<LEN>& mod,
    const unsigned_bigint<LEN>& modInv) {
    // 这里LEN+1就不会溢出了，但为了执行更快，我们选择LEN+4
    unsigned_bigint<LEN + 4> mod2 = mod, multiplicand2 = multiplicand, rlt;
    unsigned_bigint<LEN * 2 + 4> ans(0);

    for (int i = 0; i < LEN; i++) {
        unsigned_bigint<LEN + 4> tmp1, tmp2;
        // tmp1 := multiplicand * multiplier[i]
        mul_mix_add(LEN + 4, &(multiplicand2.data[0]), multiplier.data[i],
                    &(ans.data[i]));
        // 一次蒙哥马利约简
        // 实际上，这个乘法运算会溢出
        // 但是我们利用C++无符号数溢出后会被截断的特性来加速运算
        QWORD u = ans.data[i] * modInv.data[0];
        // tmp := mod * u;
        mul_mix_add(LEN + 4, &(mod2.data[0]), u, &(ans.data[i]));
        // ans := ans + (tmp1 + tmp2) << (i * 64)
    }
    // rlt = ans >> LEN * 64;
    memcpy(&(rlt.data[0]), &(ans.data[LEN]), sizeof rlt.data);
    if (rlt > mod2) {
        rlt -= mod2;
    }
    return (unsigned_bigint<LEN>)rlt;
}

template <int LEN>
constexpr unsigned_bigint<LEN> fast_pow_mod(
    const unsigned_bigint<LEN>& base, const unsigned_bigint<LEN>& exponent,
    const unsigned_bigint<LEN>& mod) {
    assert(mod.bit(0) == 1);  // 模数必须与2^64互质，换言之必须是奇数
    unsigned_bigint<LEN> modInv, y;
    exgcd(unsigned_bigint<LEN>(1) << 64, mod, y, modInv);
    return fast_pow_mod(base, exponent, mod, modInv);
}
template <int LEN>
constexpr unsigned_bigint<LEN> fast_pow_mod(
    const unsigned_bigint<LEN>& base, const unsigned_bigint<LEN>& exponent,
    const unsigned_bigint<LEN>& mod, const unsigned_bigint<LEN>& modInv) {
    assert(mod.bit(0) == 1);  // 模数必须与2^64互质，换言之必须是奇数
    unsigned_bigint<LEN> Rmod(1);
    Rmod = Rmod.toMontgomery(mod);
    return fast_pow_mod(base, exponent, mod, modInv, Rmod);
}
template <int LEN>
constexpr unsigned_bigint<LEN> fast_pow_mod(
    unsigned_bigint<LEN> bsae, const unsigned_bigint<LEN>& exponent,
    const unsigned_bigint<LEN>& mod, const unsigned_bigint<LEN>& modInv,
    const unsigned_bigint<LEN>& Rmod) {
    assert(mod.bit(0) == 1);  // 模数必须与2^64互质，换言之必须是奇数
    unsigned_bigint<LEN> ans = Rmod;
    int frist_1_of_n = LEN * 64 - 1;
    while (exponent.bit(frist_1_of_n) == 0) frist_1_of_n--;
    bsae = bsae.toMontgomery(mod);
    // 转换完毕

    // 滑动窗口法
    const int k =  // 滑动窗口法的参数
        ((frist_1_of_n) > 671
             ? 6
             : (frist_1_of_n) > 239
                   ? 5
                   : (frist_1_of_n) > 79 ? 4 : (frist_1_of_n) > 23 ? 3 : 1);
    // a_pow_mod[i] means a^i mod m
    std::vector<unsigned_bigint<LEN>> a_pow_mod;
    a_pow_mod.resize(1i64 << k);
    unsigned_bigint<LEN> a_sqrt =
        multiply_mod_Montgomery(bsae, bsae, mod, modInv);
    a_pow_mod[1] = bsae;
    for (int i = 3; i < (1 << k); i += 2) {
        a_pow_mod[i] =
            multiply_mod_Montgomery(a_pow_mod[i - 2], a_sqrt, mod, modInv);
    }
    for (int i = frist_1_of_n; i >= 0; i--) {
        if (exponent.bit(i) == 0) {
            ans = multiply_mod_Montgomery(ans, ans, mod, modInv);
        } else {
            int s = std::max(i - k + 1, 0);
            while (exponent.bit(s) == 0) s++;
            int u = 0;
            for (int j = 0; j <= i - s; j++) {
                ans = multiply_mod_Montgomery(ans, ans, mod, modInv);
                u |= exponent.bit(s + j) << j;
            }
            ans = multiply_mod_Montgomery(ans, a_pow_mod[u], mod, modInv);
            i = s;
        }
    }
    ans = multiply_mod_Montgomery(ans, unsigned_bigint<LEN>(1), mod, modInv);
    return ans;
}
#endif
