# LICENSE

* **Author**: github.com/maxthon147532

* **本作品采用 <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议</a> 进行许可**

* ![CC Claim Picture](assets/by-nc-sa 4.0.png)

# Linux 实用程序

## 简单程序

| 程序  | 用途简述                                                     |
| ----- | ------------------------------------------------------------ |
| id    | 输出用户的 uid(uname), gid(group name)                       |
| umask | \> **umask -S**：适合人阅读的方式，展示用户掩码设置<br />\> **umask** ：数值方式查看用户掩码设置（如一般默认为 0022）<br />> **umask XXXX** ： 修改用户掩码 |
| uname |                                                              |

## ps

### 常用命令

| 命令                            | 用途简述                                                     |
| ------------------------------- | ------------------------------------------------------------ |
| ps axjf                         | 打印进程树信息（包含 sid，ppid，pgid，uid 等信息）           |
| ps                              | 查看属于当前 shell 的进程信息                                |
| ps -e / ps -A / ps -ef / ps -Af | 查看所有活动中的进程 <br />可以配合 -f 选项，控制进程信息输出格式 |

### 格式控制符

* 常用格式控制符

| **CODE**     | **HEADER** | **DESCRIPTION**                                              |
| :------------ | :---------- | :------------------------------------------------------------ |
| **%cpu**     | %CPU       |CPU 使用率 <br /><br />**- 格式**："##.#" （百分比） <br />**- 值含义**：程序使用的 CPU 时间占比：cputime/realtime ratio)<br />一般不会到 100%<br />**- 类似控制符**：pcpu |
| **%mem**     | %MEM       | ratio of the process's resident set size  to                                   the   physical memory on the machine, expressed                                   as a   percentage.  (alias **pmem**). |

* 全部格式控制符

| **CODE**     | **HEADER** | **DESCRIPTION**                                              |
| :------------ | :---------- | :------------------------------------------------------------ |
| **%cpu**     | %CPU       | cpu utilization of the process in   "##.#"                                     format.  Currently, it is the   CPU time used                                   divided   by the time the process has been                                   running   (cputime/realtime ratio), expressed as                                   a   percentage.  It will not add up to   100%                                   unless   you are lucky.  (alias **pcpu**). |
| **%mem**     | %MEM       | ratio of the process's resident set size  to                                   the   physical memory on the machine, expressed                                   as a   percentage.  (alias **pmem**). |
| **args**     | COMMAND    | command with all its arguments as a string.                                     Modifications to the arguments may be shown.                                   The   output in this column may contain spaces.                                   A   process marked <defunct> is partly dead,                                   waiting   to be fully destroyed by its parent.                                     Sometimes the process args will be unavailable;                                   when   this happens, **ps** will instead print the                                     executable name in brackets.    (alias **cmd**,                                   **command**).  See also the **comm** format   keyword,                                   the **-f** option,   and the **c** option.                                   When   specified last, this column will extend to                                   the edge   of the display.  If **ps** can   not                                     determine display width, as when output is                                     redirected (piped) into a file or another                                   command,   the output width is undefined (it may                                   be 80,   unlimited, determined by the **TERM**                                     variable, and so on).  The **COLUMNS** environment                                   variable   or **--cols** option may be used to                                   exactly   determine the width in this case.    The                                   **w** or   **-w** option may be also be used to adjust                                   width. |
| **blocked**  | BLOCKED    | mask of the blocked signals, see [signal(7)](http://man7.org/linux/man-pages/man7/signal.7.html).                                     According to the width of the field, a 32 or                                   64-bit   mask in hexadecimal format is displayed.                                   (alias **sig_block**, **sigmask**). |
| **bsdstart** | START      | time the command started.  If the process was                                   started   less than 24 hours ago, the output                                   format   is " HH:MM", else it is " Mmm:SS" (where                                   Mmm is   the three letters of the month).    See                                   also **lstart**,   **start**, **start_time**, and **stime**. |
| **bsdtime**  | TIME       | accumulated cpu time, user + system.  The                                   display   format is usually "MMM:SS", but can be                                   shifted   to the right if the process used more                                   than 999   minutes of cpu time. |
| **c**        | C          | processor utilization.    Currently, this is the                                   integer   value of the percent usage over the                                   lifetime   of the process.  (see **%cpu**). |
| **caught**   | CAUGHT     | mask of the caught signals, see [signal(7)](http://man7.org/linux/man-pages/man7/signal.7.html).                                     According to the width of the field, a 32 or 64                                   bits   mask in hexadecimal format is displayed.                                   (alias **sig_catch**, **sigcatch**). |
| **cgname**   | CGNAME     | display name of control   groups to which the                                   process   belongs. |
| **cgroup**   | CGROUP     | display control groups to which the process                                   belongs. |
| **class**    | CLS        | scheduling class of the process.  (alias                                   **policy**,   **cls**).  Field's   possible values are:   -   not reported                                              TS  SCHED_OTHER                                              FF  SCHED_FIFO                                              RR  SCHED_RR                                              B   SCHED_BATCH                                              ISO SCHED_ISO                                              IDL SCHED_IDLE                                              DLN SCHED_DEADLINE                                              ?   unknown value |
|              |            |                                                              |
| **cls**      | CLS        | scheduling class of the process.  (alias                                   **policy**,   **cls**).  Field's   possible values are:   -   not reported                                              TS  SCHED_OTHER                                              FF  SCHED_FIFO                                              RR  SCHED_RR                                              B   SCHED_BATCH                                              ISO SCHED_ISO                                              IDL SCHED_IDLE                                              DLN SCHED_DEADLINE                                              ?   unknown value |
|              |            |                                                              |
| **cmd**      | CMD        | see **args**.  (alias **args**,   **command**).              |
| **comm**     | COMMAND    | command name (only the executable name).                                     Modifications to the command name will not be                                   shown.  A process marked <defunct> is   partly                                   dead,   waiting to be fully destroyed by its                                     parent.  The output in this   column may contain                                     spaces.  (alias **ucmd**,   **ucomm**).  See also   the                                   **args format keyword,** the **-f**   option, and the **c**                                     option.                                   When   specified last, this column will extend to                                   the edge   of the display.  If **ps** can   not                                     determine display width, as when output is                                     redirected (piped) into a file or another                                   command,   the output width is undefined (it may                                   be 80,   unlimited, determined by the **TERM**                                     variable, and so on).  The **COLUMNS** environment                                   variable   or **--cols** option may be used to                                   exactly   determine the width in this case.    The                                   **w** or   **-w** option may be also be used to adjust                                   width. |
| **command**  | COMMAND    | See **args**.  (alias **args**,   **command**).              |
| **cp**       | CP         | per-mill (tenths of a percent) CPU usage.  (see                                   **%cpu**). |
| **cputime**  | TIME       | cumulative CPU time, "[DD-]hh:mm:ss"   format.                                   (alias **time**). |
| **cputimes** | TIME       | cumulative CPU time in seconds (alias **times**).            |
| **drs**      | DRS        | data resident set size, the amount of physical                                   memory   devoted to other than executable code. |
| **egid**     | EGID       | effective group ID number of the process as a                                   decimal   integer.  (alias **gid**). |
| **egroup**   | EGROUP     | effective group ID of the process.  This will                                   be the   textual group ID, if it can be obtained                                   and the   field width permits, or a decimal                                     representation otherwise.    (alias **group**). |
| **eip**      | EIP        | instruction pointer.                                         |

## nmon

* [NMON](http://nmon.sourceforge.net/pmwiki.php?n=Site.Download)

  开源性能监控工具

* [nmon_analyser](https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/Power+Systems/page/nmon_analyser) 

  个人维护的 nmon 数据文件分析工具，可以生成 excel 视图文档。受控在 IBM 社区

### 使用

#### 命令行方式

* 快速使用示例

  > nmon -s10 -c60 -f -m $HOME/logs/nmon
  >
  > ​    -s10 每 10 秒采集一次数据。
  >  
  > ​	-c60 采集 60 次，即为采集十分钟的数据。
  >  
  > ​	-f 生成的数据文件名中包含文件创建的时间。
  >  
  > ​	-m 生成的数据文件的存放目录。
  >  
  > ​		这样就会生成一个 nmon 文件，并每十秒更新一次，直到十分钟后。
  >  
  >   ​		生成的文件名如： _090824_1306.nmon ，"" 是这台主机的主机名。
  
  不建议一直采集，生成的文件太大会影响 nmon_analyser 使用。建议按时间段采集
  
* 定时采集

  ```bash
  #!/bin/bash
  
  mkdir -p $HOME/logs/nmon
  
  while true
  do
  	nmon -s10 -c360 -f -m $HOME/logs/nmon
  	sleep 3600
  done
  ```

  ## ssh

# Linux 系统使用

## 系统配置

### core

#### 配置 core 文件生成路径

* **配置文件路径**：

  /proc/sys/kernel/core_pattern

* **默认生成路径**：输入可执行文件运行命令的同一路径下

* **默认生成名字**：默认命名为core。新的core文件会覆盖旧的core文件

* **配置修改方式**
  * echo "/corefile/core-%e-%p-%t" > /proc/sys/kernel/core_pattern
  * sysctl -w kernel.core_pattern=/tmp/zcore/core.%e.%h.%p.%t

* **配置格式说明符**
  * %p - insert pid into filename 添加pid(进程id)
  * %u - insert current uid into filename 添加当前uid(用户id)
  * %g - insert current gid into filename 添加当前gid(用户组id)
  * %s - insert signal that caused the coredump into the filename 添加导致产生core的信号
  * %t - insert UNIX time that the coredump occurred into filename 添加core文件生成时的unix时间 
  * %h - insert hostname where the coredump happened into filename 添加主机名
  * %e - insert coredumping executable name into filename 添加导致产生core的命令名

#### 测试 core 文件能否生成

* kill -s SIGSEGV $$

#### 扩展：docker 容器中进行配置

* 在 docker 容器中，/proc/sys/kernel/core_pattern 是直接使用宿主机的文件。容器中不能修改此文件。修改宿主机的配置，容器中会使用宿主机的配置
