# bigint
这是一个大数运算库。

## 要求
* 编译时要求Visual Studio 2017
* 运行时要求支持ADX、BMI2、RDRAND指令集
* 运行时支持Intel Haswell或更新，AMD Ryzen或更新

## 使用
### 声明
```C++
unsigned_bigint<16> foo;    // 16表示这个数有16*64=1024位二进制位
```
### 输入输出
```C++
unsigned_bigint<16> a, b;
std::cin >> std::hex >> a >> b;                     // 十六进制输入
std::cout << std::hex << "a = " << a << std::endl;  // 十六进制输出
std::cin >> std::dec >> a >> b;                     // 十进制输入
std::cout << std::dec << "a = " << a << std::endl;  // 十进制输出
std::cin >> std::oct >> a >> b;                     // 八进制输入
std::cout << std::oct << "a = " << a << std::endl;  // 八进制输出
```
### 赋值
```C++
unsigned_bigint<16> a;
a = 65535;
std::cout << std::hex << "a = " << a << std::endl;
```
但如果要赋值为大数，由于C++常量不支持大整数（要支持的话我还写这个库做什么），请使用字符串进行赋值：
```C++
unsigned_bigint<16> a;
a = unsigned_bigint<16>("0x0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
std::cout << std::hex << "a = " << a << std::endl;
```
### 四则运算等
```C++
unsigned_bigint<16> a, b;
std::cin >> std::hex >> a >> b;
std::cout << std::hex << "a + b = " << a + b << std::endl
                      << "a - b = " << a - b << std::endl
                      << "a * b = " << a * b << std::endl
                      << "a / b = " << a / b << std::endl
                      << "a % b = " << a % b << std::endl;
```
### 模运算
```C++
foo = fast_pow_mod(a, n, m); // foo = (a ^ n) % m
bar = multiply_mod(a, b, m); // foo = (b * b) % m
```
### 比较操作及位运算
```C++
unsigned_bigint<16> a, b;
std::cin >> std::hex >> a >> b;
std::cout << std::hex << "(a > 0) = " << (a > 0) << std::endl
                      << "(a == b) = " << (a == b) << std::endl
                      << "(a != b) = " << (a != b) << std::endl
                      << "(a < b) = " << (a < b) << std::endl
                      << "(a <= b) = " << (a <= b) << std::endl
                      << "(a > b) = " << (a > b) << std::endl
                      << "(a >= b) = " << (a >= b) << std::endl
                      << "(a | b) = " << (a | b) << std::endl
                      << "(a & b) = " << (a & b) << std::endl
                      << "(a ^ b) = " << (a ^ b) << std::endl;
```
### 获取及修改某一位的值
```C++
unsigned_bigint<16> a;
std::cin >> std::hex >> a;
a.bit(0) = 1;                        // 最低位置1
std::cout << a.bit(0) << std::endl;  // 输出最低位
```
### 随机数生成器
```C++
unsigned_bigint<16> a, b;
std::cin >> std::hex >> a >> b;
// 使用Intel CPU中的指令获得真随机数，仅在Intel CPU上可用
std::cout << std::hex << "unsigned_bigint<16>::randIntel() = "
                      << unsigned_bigint<16>::randIntel() << std::endl;
// 生成伪随机数
std::cout << std::hex << "unsigned_bigint<16>::rand() = "
                      << unsigned_bigint<16>::rand() << std::endl;
```
### 质数判断
```C++
unsigned_bigint<16> a;
std::cin >> std::hex >> a;
std::cout << "a " << (a.is_prime_number() ? "is" : "isn't")
          << " a prime number" << std::endl;
```

## 性能对比
与[Boost](https://www.boost.org)比较如下。（数据为运行时间，越小越好）

其中四则运算运算次数为500000次，模幂运算次数为1000次。

<table>
    <tr>
        <th rowspan=2 height="91">操作</th>
        <th colspan=2>256位</th>
        <th colspan=2>512位</th>
        <th colspan=2>1024位</th>
        <th colspan=2>2048位</th>
        <th colspan=2>4096位</th>
    </tr>
    <tr>
        <th>Unsigned Bigint</th>
        <th>boost</th>
        <th>Unsigned Bigint</th>
        <th>boost</th>
        <th>Unsigned Bigint</th>
        <th>boost</th>
        <th>Unsigned Bigint</th>
        <th>boost</th>
        <th>Unsigned Bigint</th>
        <th>boost</th>
    </tr>
    <tr>
        <td>加</td>
        <td>0.0060686</td>
        <td>0.0083953</td>
        <td>0.0064107</td>
        <td>0.0115023</td>
        <td>0.0091979</td>
        <td>0.0212813</td>
        <td>0.0151207</td>
        <td>0.0388485</td>
        <td>0.0315682</td>
        <td>0.0767778</td>
    </tr>
    <tr>
        <td>减</td>
        <td>0.0052497</td>
        <td>0.0079826</td>
        <td>0.0062715</td>
        <td>0.0123272</td>
        <td>0.0095903</td>
        <td>0.0280378</td>
        <td>0.0165817</td>
        <td>0.0494003</td>
        <td>0.0354999</td>
        <td>0.0785873</td>
    </tr>
    <tr>
        <td>乘</td>
        <td>0.019142</td>
        <td>0.0335125</td>
        <td>0.063854</td>
        <td>0.2700601</td>
        <td>0.111633</td>
        <td>0.3944791</td>
        <td>0.3698449</td>
        <td>1.4607164</td>
        <td>1.2972828</td>
        <td>5.5994093</td>
    </tr>
    <tr>
        <td>除</td>
        <td>0.9787682</td>
        <td>0.2175322</td>
        <td>1.6977481</td>
        <td>0.3206437</td>
        <td>2.571229</td>
        <td>0.6465876</td>
        <td>5.7703808</td>
        <td>4.6256008</td>
        <td>11.174972</td>
        <td>10.216111</td>
    </tr>
    <tr>
        <td>模幂</td>
        <td>0.0675292</td>
        <td>0.2627964</td>
        <td>0.2580181</td>
        <td>1.6192191</td>
        <td>1.2872345</td>
        <td>10.114824</td>
        <td>7.361546</td>
        <td>76.14449</td>
        <td>52.701681</td>
        <td>445.34939</td>
    </tr>
</table>

boost自己进行的性能测试可以在[这里](https://www.boost.org/doc/libs/1_70_0/libs/multiprecision/doc/html/boost_multiprecision/perf/integer_performance.html)找到。
