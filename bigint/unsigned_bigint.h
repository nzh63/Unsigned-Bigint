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
#ifndef UNSIGNED_BIGINT_H
#define UNSIGNED_BIGINT_H

#include <complex>
#include <iostream>
#include <string>

#include "./overload_if.hpp"

typedef unsigned long long QWORD;

template <int LEN>
class unsigned_bigint {
   private:
    static constexpr void fft(std::complex<double> a[], const int n);

    static constexpr void idft(std::complex<double> a[], const int n);

    static constexpr void transform(std::complex<double> a[], const int n,
                                    const bool flag);

    bool Miller_Rabin(const int times) const;

    template <int LEN2>
    constexpr int cmp_with_offset(const unsigned_bigint<LEN2>& a,
                                  const unsigned_bigint<LEN2>& b,
                                  const int offset,
                                  const int all_zero = 0) const;

   public:
    QWORD data[LEN];

    class bitEditor;

    constexpr unsigned_bigint();
    constexpr unsigned_bigint(const QWORD a);
    explicit constexpr unsigned_bigint(const std::string a);

    constexpr bool bit(const int i) const;
    constexpr bitEditor bit(const int i);

    constexpr unsigned_bigint operator+(const QWORD addend) const;
    constexpr unsigned_bigint operator+(const unsigned_bigint& addend) const;
    constexpr unsigned_bigint operator+=(const QWORD addend);
    constexpr unsigned_bigint operator+=(const unsigned_bigint& addend);

    constexpr unsigned_bigint operator-(const QWORD subtrahend) const;
    constexpr unsigned_bigint operator-(
        const unsigned_bigint& subtrahend) const;
    constexpr unsigned_bigint operator-=(const QWORD subtrahend);
    constexpr unsigned_bigint operator-=(const unsigned_bigint& subtrahend);

    constexpr unsigned_bigint operator*(const QWORD multiplier) const;
    constexpr unsigned_bigint operator*(
        const unsigned_bigint& multiplier) const;
    // 使用传统方法计算(*this) * a
    constexpr unsigned_bigint mul_classic(
        const unsigned_bigint& multiplier) const;
    // 使用fft计算(*this) * a
    // 然而常数很大，仅仅在LEN较大时才使用
    constexpr unsigned_bigint mul_fft(const unsigned_bigint& multiplier) const;
    constexpr unsigned_bigint operator*=(const QWORD multiplier);
    constexpr unsigned_bigint operator*=(const unsigned_bigint& multiplier);

    constexpr unsigned_bigint operator/(const unsigned_bigint& divisor) const;
    constexpr unsigned_bigint operator/=(const unsigned_bigint& divisor);

    constexpr QWORD operator%(const QWORD divisor) const;
    constexpr unsigned_bigint operator%(const unsigned_bigint& divisor) const;
    constexpr unsigned_bigint operator%=(const QWORD divisor);
    constexpr unsigned_bigint operator%=(const unsigned_bigint& divisor);

    constexpr unsigned_bigint& operator++();
    constexpr unsigned_bigint operator++(int);
    constexpr unsigned_bigint& operator--();
    constexpr unsigned_bigint operator--(int);

    constexpr unsigned_bigint operator>>(const int a) const;
    constexpr unsigned_bigint operator>>=(const int a);
    constexpr unsigned_bigint operator<<(const int a) const;
    constexpr unsigned_bigint operator<<=(const int a);

    constexpr bool operator==(const unsigned_bigint& a) const;
    constexpr bool operator!=(const unsigned_bigint& a) const;
    constexpr bool operator>(const unsigned_bigint& a) const;
    constexpr bool operator<(const unsigned_bigint& a) const;
    constexpr bool operator>=(const unsigned_bigint& a) const;
    constexpr bool operator<=(const unsigned_bigint& a) const;

    constexpr bool operator==(const QWORD a) const;
    constexpr bool operator!=(const QWORD a) const;
    constexpr bool operator>(const QWORD a) const;
    constexpr bool operator<(const QWORD a) const;
    constexpr bool operator>=(const QWORD a) const;
    constexpr bool operator<=(const QWORD a) const;

    constexpr unsigned_bigint operator&(const unsigned_bigint& a) const;
    constexpr unsigned_bigint operator&=(const unsigned_bigint& a);
    constexpr unsigned_bigint operator|(const unsigned_bigint& a) const;
    constexpr unsigned_bigint operator|=(const unsigned_bigint& a);
    constexpr unsigned_bigint operator^(const unsigned_bigint& a) const;
    constexpr unsigned_bigint operator^=(const unsigned_bigint& a);

    template <int LEN2>
    explicit constexpr operator unsigned_bigint<LEN2>() const;

    template <int LEN2>
    constexpr unsigned_bigint(
        const unsigned_bigint<LEN2>& a,
        const typename overload_if<(LEN2 < LEN), int>::type dummy = 0);

    explicit constexpr operator unsigned long long() const;

    constexpr unsigned_bigint toMontgomery(const unsigned_bigint& n) const;

    /**
     * @brief 判断是否为质数
     *
     * @param times Miller-Rabin算法执行次数
     * @param pre_sieve 在Miller-Rabin前使用预设定的质数集筛一次
     * @return true 是质数
     * @return false 不是质数
     */
    bool is_prime_number(const int times = 10, bool pre_sieve = true);

    // 产生随机数，随机源是rand()函数
    static unsigned_bigint rand();

    // 产生随机数，随机源是rdrand，仅Intel CPU可用
    static unsigned_bigint randIntel();
};

template <int LEN>
class unsigned_bigint<LEN>::bitEditor {
   private:
    unsigned_bigint<LEN>* p;
    int i;

   public:
    bitEditor(unsigned_bigint<LEN>* _p, int _i);
    bool operator=(const bool a);
    constexpr operator bool() const;
};

template <int LEN>
constexpr unsigned_bigint<LEN> operator+(const QWORD a,
                                         const unsigned_bigint<LEN>& b);
template <int LEN>
constexpr unsigned_bigint<LEN> operator-(const QWORD a,
                                         const unsigned_bigint<LEN>& b);
template <int LEN>
constexpr unsigned_bigint<LEN> operator*(const QWORD a,
                                         const unsigned_bigint<LEN>& b);

template <int LEN>
constexpr std::istream& operator>>(std::istream& in,
                                   unsigned_bigint<LEN>& input_number);

template <int LEN>
constexpr std::ostream& operator<<(std::ostream& out,
                                   unsigned_bigint<LEN> output_number);

// 扩展欧几里得算法
// 对于给定的a,b找到一组x,y使得：ax-by=1
template <int LEN>
void exgcd(const unsigned_bigint<LEN>& a, const unsigned_bigint<LEN>& b,
           unsigned_bigint<LEN>& x, unsigned_bigint<LEN>& y);

// 二进制扩展欧几里得算法
// 对于给定的a,b找到一组x,y使得：ax-by=1
// 与exgcd()不同，这个函数返回的x,y可能是负值，因此x和y多了一位符号位
// 请根据最高位判断正负，负数表示方法位补码表示法
template <int LEN>
void binexgcd(const unsigned_bigint<LEN>& a, const unsigned_bigint<LEN>& b,
              unsigned_bigint<LEN + 1>& x, unsigned_bigint<LEN + 1>& y);

// (a * b) % m
template <int LEN>
constexpr unsigned_bigint<LEN> multiply_mod(
    const unsigned_bigint<LEN>& multiplicand,
    const unsigned_bigint<LEN>& multiplier, const unsigned_bigint<LEN>& mod);

/**
 * @brief 使用蒙哥马利乘模计算(a ^ n) % m
 *
 * @tparam LEN 大整数长度
 * @param base 底数
 * @param exponent 指数
 * @param mod 模数
 * @param modInv 可选，模数对2^64的逆元，可以使用如下代码生成
 *                exgcd(unsigned_bigint<LEN>(1) << 64, mod, y, modInv);
 * @param Rmod 可选，2^(64*LEN) % m，可以使用如下代码生成
 *                unsigned_bigint<LEN> Rmod(1);
 *                Rmod = Rmod.toMontgomery(mod);
 * @return constexpr unsigned_bigint<LEN>
 */
template <int LEN>
constexpr unsigned_bigint<LEN> fast_pow_mod(
    unsigned_bigint<LEN> base, const unsigned_bigint<LEN>& exponent,
    const unsigned_bigint<LEN>& mod, const unsigned_bigint<LEN>& modInv,
    const unsigned_bigint<LEN>& Rmod);

template <int LEN>
constexpr unsigned_bigint<LEN> fast_pow_mod(
    const unsigned_bigint<LEN>& base, const unsigned_bigint<LEN>& exponent,
    const unsigned_bigint<LEN>& mod, const unsigned_bigint<LEN>& modInv);

template <int LEN>
constexpr unsigned_bigint<LEN> fast_pow_mod(
    const unsigned_bigint<LEN>& base, const unsigned_bigint<LEN>& exponent,
    const unsigned_bigint<LEN>& mod);

/**
 * @brief 使用蒙哥马利乘模计算(a * b) % m
 *
 * @tparam LEN 大整数长度
 * @param multiplicand Montgomery域中的一个数，被乘数
 * @param multiplier Montgomery域中的一个数，乘数
 * @param mod 模数
 * @param modInv 2^64对模数的逆元，可以使用如下代码生成
 *                   exgcd(unsigned_bigint<LEN>(1) << 64, mod, y, modInv);
 * @return constexpr unsigned_bigint<LEN> Montgomery域中的结果
 */
template <int LEN>
constexpr unsigned_bigint<LEN> multiply_mod_Montgomery(
    const unsigned_bigint<LEN>& multiplicand,
    const unsigned_bigint<LEN>& multiplier, const unsigned_bigint<LEN>& mod,
    const unsigned_bigint<LEN>& modInv);

#include "./unsigned_bigint.hpp"

#endif
