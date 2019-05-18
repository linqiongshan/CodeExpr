# LICENSE

* **Author**: github.com/linqiongshan

* **本作品采用 <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议</a> 进行许可**

* ![CC Claim Picture](assets/by-nc-sa 4.0.png)

# 系统命令行工具

## mklink

* 创建符号或硬链接

### 基本使用

* 快速使用示例

  

* 基本使用格式

  ```powershell
  MKLINK [[/D] | [/H] | [/J]] Link Target
  
          /D      创建目录符号链接。默认为文件
                  符号链接。
          /H      创建硬链接而非符号链接。
          /J      创建目录联接。
          Link    指定新的符号链接名称。
          Target  指定新链接引用的路径
                  (相对或绝对)。
  ```

