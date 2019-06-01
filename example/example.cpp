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

#include "../bigint/unsigned_bigint.h"

int main() {
    unsigned_bigint<16> a, b;
    std::cin >> std::hex >> a >> b;
    std::cout << std::hex << "a = " << a << std::endl
              << "b = " << b << std::endl
              << "a + b = " << a + b << std::endl
              << "a - b = " << a - b << std::endl
              << "a * b = " << a * b << std::endl
              << "a / b = " << a / b << std::endl
              << "a % b = " << a % b << std::endl
              << "(a > 0) = " << (a > 0) << std::endl
              << "(a == b) = " << (a == b) << std::endl
              << "(a != b) = " << (a != b) << std::endl
              << "(a < b) = " << (a < b) << std::endl
              << "(a <= b) = " << (a <= b) << std::endl
              << "(a > b) = " << (a > b) << std::endl
              << "(a >= b) = " << (a >= b) << std::endl
              << "(a | b) = " << (a | b) << std::endl
              << "(a & b) = " << (a & b) << std::endl
              << "(a ^ b) = " << (a ^ b) << std::endl
              << "unsigned_bigint<16>::randIntel() = "
              << unsigned_bigint<16>::randIntel() << std::endl
              << "a " << (a.is_prime_number() ? "is" : "isn't")
              << " a prime number" << std::endl;

    std::cin.get();
}
