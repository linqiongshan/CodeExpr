# LICENSE

* **Author**: github.com/linqiongshan

* **本作品采用 <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议</a> 进行许可**

* ![CC Claim Picture](assets/by-nc-sa 4.0.png)

# 快速查找手册

## 函数/类型速查

| 函数/类型定义                                                | 用途                                                         | 标准    | 依赖                                             |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------- | ------------------------------------------------ |
| std::ostringstream                                           |                                                              | C++     | <sstream>                                        |
| std::cout<br />std::endl                                     |                                                              | C++     | <iostream>                                       |
| struct timeval                                               |                                                              | POSIX   | <sys/time.h>                                     |
| ~~gettimeofday()~~                                           | 【**过时**：POSIX.1-2008 将此函数标记为过时（obsolete），替代函数，clock_gettime】 | POSIX   | <sys/time.h>                                     |
| mode_t umask(mode_t mask);                                   | 设置默认的文件创建权限掩码                                   | POSIX   | #include <sys/types.h><br/>#include <sys/stat.h> |
| unsigned int sleep(unsigned int seconds)                     |                                                              | POSIX   | <unistd.h>                                       |
| iconv_open, iconv, iconv_close, iconv_t                      | 字符集转换                                                   | POSIX   | <iconv.h>                                        |
| -- FILE （type name） <br />-- fopen，fclose，fread，fwrite，ffush <br />-- fdopen 【使用文件描述符生成 FILE 对象】<br />-- freopen<br />-- fileno 【获取 FILE 对象使用的文件描述符】 |                                                              | C       | <stdio.h>                                        |
| va_start<br/>va_end<br/>va_arg<br/>va_copy<br/><br/>va_list  | 变长参数                                                     | C       | <stdarg.h>                                       |
| strerror                                                     |                                                              | C/POSIX | <string.h>                                       |
| pid_t fork(void);                                            | 创建子进程                                                   | POSIX   | <unistd.h>                                       |
| int chdir(const char *path);<br/>int fchdir(int fd);         | change working directory                                     | POSIX   | <unistd.h>                                       |

* References
  * [gettimeofday](https://linux.die.net/man/2/gettimeofday)

## 宏速查

| 宏         | 范围 | 用途                                                         |
| ---------- | ---- | ------------------------------------------------------------ |
| `__GNUC__` | GCC  | GCC 编译的预定义宏<br />代码中可以通过该宏判断是否是使用 GCC 进行编译的<br />`__GNUC__`的值表示gcc 的版本 |
|            |      |                                                              |
|            |      |                                                              |



# C/C++ 标准库

## 线程模型：线程，锁，条件变量等

* 标准：C++ 11

* 实现：

  一些平台在 C++11 标准之前已经有相应的线程库，这些平台上的 C++ 标准库有时候会直接复用已有实现。比如 Linux 平台，C++11 的 thread 实质上是通过 pthread 实现的

* **依赖**：pthread

  Linux 平台，C++11 的 std::thread 实际上是通过 pthread 实现的。因此程序在链接时，需要链接 pthread 的库

### thread

* 头文件：\<thread>

#### 命名空间

* std::this_thread

#### 关键字

* thread_local

  thread_local 是 C++ 11 引入的新的关键字

#### 核心对象

##### std::thread

* 类型：类
* 绑定回调接口
* 方法：
  * join

##### std::thread::id

* 类型：类

* 序列化：该类型重载了 << 操作符，所以可以通过 << 将对象序列化为字符串

  序列化示例：

  ```cpp
  std::string getThreadId()
  {
      std::ostringstream s;
      s << std::this_thread::get_id();
      return s.str();
  }
  ```


##### std::this_thread::get_id

* 类型：函数
* 原型：std::thread::id get_id() noexcept;
* 功能：获取当前线程的 id

### mutex

## 智慧指针

### auto_ptr

* 通常不再建议使用

### unique_ptr

### smart_ptr

## stdarg 

### 依赖头文件

* stdarg.h  【C】

* cstdarg   【C++】

### 核心函数

stdarg 中的函数，在多数系统上，是通过宏实现的，不是真正意义上的函数

#### void va_start(va_list ap, last);

> * ap：va_start 根据 last 初始化 ap（va_list）对象。last 是变长参数列表前的一个参数，是函数所指导的最后一个类型可知的参数。
> 
> * last：注意，last 参数的地址可能被用在 ap 对象中，因此，**last** **不应该是以下几种类型**：
> 
>   1. 寄存器变量（register variable）
>   2. 函数（函数指针）
>   3. 数组

#### type va_arg(va_list ap, type);

> * va_arg 操作 va_start 初始化后的 va_list 对象。每次调用 va_arg 都会修改 va_list 对象，返回下一个参数。
>
> * va_arg 通过入参 type 确定返回值的类型。type 一般由使用 va_list 的函数，通过其它地方获取。
>
>       比如 vsnprintf 要提供一个 format 字符串，也可以函数事先约定好每个参数的类型。
> 
>       定义相应类型的变量保存 va_arg 返回的参数值，如：
>
>         ```c
>         char* value = va_arg(vaList,  char*)
>         ```

#### void va_end(va_list ap);

> 重置 va_list 对象。 va_start 和 va_end 总应该配对出现。
>
> va_end 调用之后，va_list 对象的值将是未定义的。如果想重新使用该对象，必须再次调用 va_start 

### 扩展函数

#### void va_copy(va_list dest,        va_list src);

> tips：并非所有的系统都包含此函数
>
> 1. 将 src 复制到 dest 。不同平台，va_list 的实际实现类型不同，va_copy 的实现也不同
> 2. 也可以通过使用同一个 va_list 对象，如果要用在多个地方，在每个使用的地方前后都使用 va_start 和 va_end 进行初始化和结束操作
> 3. **使用** **va_copy** **获取的** **dest** **对象，也必须使用** **va_end** **进行销毁**
> 4. e.g.：
>
>     ```c++
>     void PrintCorrectCase3(const char* fmt, ...)
>     {
>         va_list ap;
>         va_list anew;
>         va_start(ap, fmt);
>         va_copy(anew, ap);
>         
>         vprintf(fmt, ap);
>         va_end(ap);
>         
>         vprintf(fmt, anew);
>         va_end(anew);
>     }
>     ```

### 核心类型

* va_list

### 使用


- 函数变长参数列表处理，允许函数接受变长参数（参数类型和个数不定）

- 函数通过形如 void func(int a, …) 的方式接受变长参数，在函数内部如果要处理该变长参数列表，必须先调用 va_start

  > 1. 使用 va_start 初始化一个 va_list 对象
  > 
  > 2. 使用 va_arg 从 va_list 对象中，逐个读取参数
  > 
  > 3. 使用 va_end 重置 va_list 对象

### 注意事项

#### **禁止重复使用 va_list 对象**

> 如果一个 va_list 对象传递给某个函数使用，并且该函数对  va_list 对象调用了 va_arg（比如 vsnprintf），在函数返回后，va_list 对象的值将是未定义的
>
> 此时对该 va_list 的使用将是未定义行为（比如可能导致 core 等）
>
> 如果要将一个变长参数传递给多个函数使用，可以定义多个 va_list 对象，或者在每次 va_list 对象使用完后，调用 va_end 充值，再调用 va_start 重新初始化一次，再传递给其它地方使用

## IO

### C++ IO Stream

* 类派生关系

  ```mermaid
  graph TD
  
  linkStyle default interpolate basis
  
  title[<b>IO Stream 类派生视图</b>]
  style title fill:#FFF,stroke:#FFF
  
  base[ios_base] --> ios[ios]
  ios --> istream[istream]
  istream --> ifsream[ifstream]
  istream --> iss[istringstream]
  istream --> iostream[iostream]
  
  ios --> ostream[ostream]
  ostream --> iostream
  ostream --> ofstream[ofstream]
  ostream --> oss[ostringstream]
  ```



### FILE （stdio.h）

#### 接口

#### fopen 和 open 的 flags 对应关系

```
┌─────────────┬───────────────────────────────┐
│fopen() mode │ open() flags                  │
├─────────────┼───────────────────────────────┤
│     r       │ O_RDONLY                      │
├─────────────┼───────────────────────────────┤
│     w       │ O_WRONLY | O_CREAT | O_TRUNC  │
├─────────────┼───────────────────────────────┤
│     a       │ O_WRONLY | O_CREAT | O_APPEND │
├─────────────┼───────────────────────────────┤
│     r+      │ O_RDWR                        │
├─────────────┼───────────────────────────────┤
│     w+      │ O_RDWR | O_CREAT | O_TRUNC    │
├─────────────┼───────────────────────────────┤
│     a+      │ O_RDWR | O_CREAT | O_APPEND   │
└─────────────┴───────────────────────────────┘
```

# Linux 系统函数

非 C/C++ 标准库的一些 Unix/Linux 系统函数，和 POSIX 规范定义的函数

## iconv

[Manual Page Reference](http://man7.org/linux/man-pages/man3/iconv.3.html)

* 头文件： <iconv.h>
* 标准：POSIX.1-2001, POSIX.1-2008.

### 原型

  ```cpp
  #include <iconv.h>

  //创建字符集转换描述符，包含源字符集和目标字符集信息. 可通过 iconv -l 获取操作系统上支持的字符集
  iconv_t iconv_open(const char *tocode, const char *fromcode);
  
  //根据 iconv_t 对象，对 inbuf 数据进行字符集转换，输出到 outbuf
  size_t iconv(iconv_t cd,
           char **inbuf, size_t *inbytesleft,
           char **outbuf, size_t *outbytesleft);
  
  //关闭 iconv_open 返回的 iconv_t 对象
  int iconv_close(iconv_t cd);
  ```

#### 使用注意事项：

1. iconv_t

   iconv_open 返回的 iconv_t 是一个描述符，记录源字符集, 目标字符集等信息

   iconv_open 返回的 iconv_t 对象，一开始处于初始化状态，被用于 iconv 后，其内部状态信息会被修改。如果想重复使用同一个 iconv_t 对象，在每次使用完后，可以通过调用 iconv，入参 inbuf 设置为 NULL，将 iconv_t 对象重置为初始化状态

2. iconv 会修改 inbuf，outbuf 指向的缓冲区数据，同时会移动 inbuf 和 outbuf 指针。所以在实践时，不会直接将调用者准备的输入输出缓冲区地址直接传给 iconv，而是会在创建两个变量指向输入和输出缓冲区地址

3. inbuf, outbuf

   inbuf 和 outbuf 指向的是一个二进制数据缓冲区，不是 C 字符序列。一般情况下 inbuf 和 *inbuf 不应为 NULL

   在 iconv 调用完成后，应该将 inbuf 置为 NULL，以刷新（flush out）转换的输入数据。

   注意，**iconv 在运行过程中会修改 cd，inbuf，\*inbuf，outbuf，\*outbuf 的值，每次调用完后，如果要继续使用这些值，请确保理解这些值的状态【输入缓冲区的数据自身不会修改】**

4. inbytesleft，outbytesleft

   iconv 是逐字节处理数据的，在处理过程中，会不断的移动 inbuf，outbuf 的指针，并相应的减少 inbytesleft 和 outbytesleft 的值

   当 iconv 结束后，inbytesleft 表示的就是输入缓冲区中还有多少字节没有完成转换，正常情况下应该是0。outbytesleft 表示的输出缓冲区还剩余多少空间，调用者使用输出缓冲区大小减去 outbytesleft 得到的就是转换后输出的数据大小

#### 返回值

* iconv_open: 错误时返回 (iconv_t)-1，错误码设置到 errno

* iconv_close: 错误时返回  (size_t)-1，错误码设置到 errno。正确时返回 0

* 原则上，iconv 返回 -1 表示错误，并设置 errno。返回 >= 0 的值表示的是不可逆方式转换的字符（含义还不完全确定）。可逆的不会统计在内。

* 如向通过 iconv 将字符串从 UTF8 转成 GBK，但输入本身就是 GBK 编码时，实际会转换失败，而返回值是一个很大的数

* 应该配合 inbytesleft 进行判断，正常情况下，如果输入数据全部完成转换，inbytesleft 应该是0（当然，输出缓冲区要足够到），因此在输出缓冲区足够大的前提下，如果 inbytesleft 不为0，说明输入数据有部分转换不成功

### 代码示例

  ```cpp
virtual std::string conv(const std::string& msg, const std::string& srcCharset, const std::string& dstCharset)
{
    std::string convertedResult;

    iconv_t cd = iconv_open(dstCharset.c_str(), srcCharset.c_str());
    if ((iconv_t)-1 == cd)
    {
        return msg;
    }

    const char* msgData = msg.data();
    char** ppInbuf = (char**)&msgData;
    size_t inBytesLeft = msg.size();

    //1. iconv 要一次性转换数据, 如果 outbuf 不够大, iconv 会失败
    //2. iconv 会修改 pOutbuf 和 ppOutbuf, 所以如果想从缓冲区最初的位置将数据拷贝出来, 不能使用 pOutbuf 和 ppOutbuf
    //   可以单独使用一个指针记录原始地址 (例如此处的 pOutbufHead)
    size_t outbufSize = msg.size() * 4;
    char* pOutbuf = new char[msg.size()*4];
    char* const pOutbufHead = pOutbuf;
    char** ppOutbuf = &pOutbuf;
    size_t outBytesLeft = outbufSize;
    do
    {
        size_t n = iconv(cd, ppInbuf, &inBytesLeft, ppOutbuf, &outBytesLeft);
        if (-1 == n)
        {
            convertedResult = msg;
            break;
        }

        ppInbuf = NULL;
        convertedResult.append(pOutbufHead, outbufSize - outBytesLeft);
        if (0 != inBytesLeft)
        {
            convertedResult = msg;
        }

    } while (false);

    delete [] pOutbufHead;  //tips: 此处不要 delete [] pOutbuf, 因为 pOutbuf 已经不指向缓冲区的首地址了
    pOutbuf = NULL;
    iconv_close(cd);
    return convertedResult;
}
  ```

## getopt

tips：在 Linux 下，除了有一个名为 getopt 的函数，还有一个名字也是 getopt 的可执行程序。不要混淆

### 使用注意事项

getopt 的运行涉及到全局变量：optind, optarg

* optind：当前处理的选项的序号
* optarg：当前处理到的选项的参数

这也导致了：

* getopt 是多线程不安全的
* **如果多次调用 getopt，需要自己重置 optind 为 0**，否则每次 getopt 都是从上次结束的位置开始，导致无法获取期望的参数

## fork

### 注意事项

#### 父子进程资源继承

* 子进程不继承父进程的子线程：fork 后子进程只会有一个线程，即子进程自身

# Linux 系统编程

## 典型场景

### 守护进程创建方式



# 最佳实践

## 故障处理

### Linux 下的程序段错误（coredump）

#### 内存泄漏导致 OOM （out of memory）

#### 栈溢出

#### 堆溢出

#### 系统日志分析

* dmesg

## 易错场景

### 无符号整数作为循环终止判断条件导致的死循环

* 代码示例

  ```cpp
  for (unsigned int u = 10; u >= 0; --u)
  {
      std::cout << u << std::endl;
  }
  ```

* 错误分析
  * 每次循环，u 会先自减 1 
  * 当 u == 0 时，此时还满足循环条件，会输出 u 的值。然后走到 --u 。由于 u 是无符号整数，数据下溢，通常值会变成 unsigned int 的最大值（如 2^32-1）
  * 此时相当于死循环
* 修复方法

  * 方法一：修改终止条件，改成 u > 0 （或改成 u != 0）

# 基本知识点

## 数据类型

### 字面值

#### 字面值基本定义

* 指针字面值

  * nullptr ：表示空指针的字面值

* 布尔字面值

  * true
  * false

* 整型字面值

  * 八进制：0 开头的整型字面值，如 024

  * 十进制：1~9 开头的整型字面值，如 20

    负数：十进制字面值严格来说不会是负数，前面添加负号（-）表示负数时，负号自身并不是十进制字面值的一部分，其含义时对字面值取负值

  * 十六进制：0x 或 0X 开头的整型字面值，如 0x14

* 浮点型字面值

  * 可以用 e 或 E 指定指数部分
  * 几种示例
    * 3.14159
    * 3.14159E0
    * 0.
    * 0e0
    * .001
  * 精度：默认精度是 double

* 字符和字符串字面值

* 转义序列

  使用 \ 作为转义序列开始标识

  * C++ 标准： \r, \n, \t 等

  * 使用十六进制：\x 后跟一个或多个十六进制数字。根据字符集不同，理论上 \x 后面跟的十六进制数字个数不限

  * 使用八进制：\ 后面跟1到3个八进制数字。最多只使用3个八进制数字

    ​	e.g.： \1234 表示两个字符，\123 和 4 两个字符

#### 指定字面值类型（通过前缀或后缀）

* 字符和字符串字面值

  统一通过前缀指定字面值类型

  | 前缀 | 含义                       | 类型     |
  | ---- | -------------------------- | :------- |
  | u    | Unicode 16 字符            | char16_t |
  | U    | Unicode 32 字符            | char32_t |
  | L    | 宽字符                     | wchar_t  |
  | u8   | UTF-8 【只用于字符串字面】 | char     |

* 整型字面值

  注意，整型有多种长度的类型，如 int，long，long long

  通过后缀指定整型字面值类型时，实质的含义是指定最小匹配类型

  | 后缀  | 最小匹配类型 | 备注                                                         |
  | ----- | ------------ | ------------------------------------------------------------ |
  | u/U   | unsigned     |                                                              |
  | l/L   | long         | u 可以和 l 后缀一起使用<br />由于 I 和大写 i，以及数字1看起来比较相近，一般情况下应该使用大写字母 L |
  | ll/LL | long long    | u 可以和 ll 后缀一起使用<br />由于 I 和大写 i，以及数字1看起来比较相近，一般情况下应该使用大写字母 LL |

* 浮点型字面值

  | 后缀 | 类型                         |
  | ---- | ---------------------------- |
  | f/F  | float                        |
  | l/L  | long double 【建议用大写 L】 |

## 初始化对象

* 初始化：创建变量时，赋予其一个初始值 【C++ 中，初始化操作和赋值操作是两个不同的操作】

* 几种初始化方式

  * 列表初始化（list initialization）

    * 标准：

      C++ 11（不含）之前的标准，只有在有限的场景下，允许使用列表初始化。

      C++ 11（含）之后，初始化或部分赋值场景，都可以使用列表初始化

    * 特点（优点）

      对内置类型的变量使用列表初始化时，如果值存在丢失信息（如精度损失）的风险，编译器会报错

      e.g.：

      ```cpp