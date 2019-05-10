# Licence
***
**MIT License**

Copyright (c) [2019] [https://github.com/maxthon147532]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

# Introduction
- Author: https://github.com/maxthon147532
- Brief: cli is a simple command line interface for C++ without any other dependencies (use c++ std libs)

## Definition
app -s singleArgument --longOption argument1 argument2 ... --optionWithoutArgument -- {parameter list, like file list etc.}
- option
    required: option which must be set
    optional: 


- short option
- long option
- argument
- option group
- option constraint

## Usage
1. 

# Design
## Requirment

## Architectural [BNF]
Option
    |-- OptionArgument
    <attr: required, optional>
    <attr: >
OptionGroup
    |-- OptionConstrant
    |-- OptionUsage

Command
    |-- CommandParser
    [Method: AddOptionGroup]
    [Method: ParseCmd]
    [Method: Operator[$ArgumentName] ]

InteractiveCommand
    [Method: Wait(timeout) ]
    []
    交互式命令解析：提供将一个字符串看作一个命令行参数，进行解析
    提供历史命令功能
    提供命令补全功能

架构设计
    全局基类
        Version
            SetMajorVer
            SetMinorVer
            SetS？？？Ver
            SetBuildDate
            SetComment
        Object
            SetVersion/GetVersion
    接口类
    实现类
## Functional

### 实现设计
    接口

    实现
        CommandImpl
        InteractiveCommandImpl

### 功能设计
    Option: 单一的选项定义
        SetComment/GetComment
    OptionArgument: 选项参数值定义；参数值解析方法定义
        SetComment()
    OptionConstraint: 选项约束：依赖，互斥
        SetComment()
    OptionGroup: 包含一组 Option，它们对应的 OptionConstraint
        SetComment
    OptionCommentFormatter: 格式化选项文本帮助信息
    Command: 可以包含多组 OptionGroup，运行时动态判断使用哪个 OptionGroup 解析命令行参数
        OptionGroupFilter:
        HasOption
        GetOptionArg

        * 重载 operator[] ，提供直接按选项名获取参数值的方法 *
    SubCommand

# TODO
* OptionGroup 
    a) 实现选项组合/互斥检查
    b) 实例使用示例功能

* ArgumentParser
    提供一些选项参数的通用解析器。并允许使用者定制自己的解析器。
    【应该和 Option 关联吗？】

* Template

    从模板加载配置，简化选项定义方式。比如根据

    ```bash
    -t [-v [vArg]] file1 ...
    ```

    

![Sample Pic][test_img]

[test_img]: data:image/png;base64,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


