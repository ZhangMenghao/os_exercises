#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。  
> BIOS是一种所谓的“固件”，负责在开机时做硬件启动和检测等工作，并且担任操作系统控制硬件时的中介角色。UEFI是一种详细描述类型接口的标准。这种接口用于操作系统自动从预启动的操作环境，加载到一种操作系统上。与BIOS显著不同的是，UEFI是用模块化、C语言风格的参数堆栈传递方式、动态链接的形式构建系统，它比BIOS更易于实现，容错和纠错特性也更强，从而缩短了系统研发的时间。更加重要的是，它运行于32位或64位模式，突破了传统16位代码的寻址能力，达到处理器的最大寻址，此举克服了BIOS代码运行缓慢的弊端。

 1. 描述PXE的大致启动流程。
>* 客户端个人电脑开机后， 在 TCP/IP Bootrom 获得控制权之前先做自我测试。
>* Bootprom 送出 BOOTP/DHCP 要求以取得 IP。
>* 如果服务器收到个人电脑所送出的要求， 就会送回 BOOTP/DHCP 回应，内容包括客户端的 IP 地址， 预设网关， 及开机映像文件。否则，服务器会忽略这个要求。
>* Bootprom 由 TFTP 通讯协议从服务器下载开机映像文件。
>* 个人电脑通过这个开机映像文件开机， 这个开机文件可以只是单纯的开机程式也可
以是操作系统。
>* 开机映像文件将包含 kernel loader 及压缩过的 kernel，此 kernel 将支持NTFS root
系统。
>* 远程客户端根据下载的文件启动机器。


## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。
>（1）电源自检程序开始运行  
（2）主引导记录被装入内存，并且程序开始执行  
（3）活动分区的引导扇区被装入内存  
（4）NTLDR从引导扇区被装入并初始化  
（5）将处理器的实模式改为32位平滑内存模式  
（6）NTLDR开始运行适当的小文件系统驱动程序。小文件系统驱动程序是建立在NTLDR内部的，它能读FAT或NTFS。  
（7）NTLDR读boot.ini文件  
（8）NTLDR装载所选操作系统  

 1. 了解GRUB的启动流程。
>* 开机；
>* BIOS加电自检（POST——Power On Self Test），内存地址为0fff：0000；
>* 将硬盘第一个扇区（0头0道1扇区，也就是Boot Sector）读入内存地址0000：7c00处；
>* 检查（WORD）0000：7dfe是否等于0xaa55.若不等于则转去尝试其他介质；如果没有其他启动介质，则显示 “No ROM BASIC” ，然后死机；
>* 跳转到0000：7c00处执行MBR中的程序；
>* MBR先将自己复制到0000：0600处，然后继续执行；假如先装XP后装LINUX，并且LINUX没有装在MBR，那这个MBR中的数据还是WIN 写的数据，它的作用都是下步中所说的作用，就是搜索主分区表中标志为活动的分区，那么这个时候就必须把GRUB所在的主分区设置为活动的分区，这个时候才能正常的启动GRUB，然后GRUB的STAGE1在调STAGE1.5和其他的，从而来引导整个系统。假如说先装XP后装LINUX，但是GRUB装在了MBR，那样STAGE1直接调入内存，STAGE1在调STAGE1.5和STAGE2等，从而来引导系统。那这个时候是不需要将GRUB其他文件所在的主分区设为活动分区的，它直接调STAGE1.5等，然后再调STAGE2等，来识别文件系统，从而实现可多启动。
>* 在主分区表中搜索标志为活动的分区。如果发现没有活动分区或者不止一个活动分区，则停止；
>* 将活动分区的第一个扇区读入内存地址0000：7c00处；
>* 检查（WORD）0000：7dfe是否等于0xaa55，若不等于则显示 “Missing Operating System”，然后停止，或尝试软盘启动；
>* 跳转到0000：7c00处继续执行特定系统的启动程序；
>* 启动系统。

 1. 比较NTLDR和GRUB的功能有差异。
>ntldr功能很少，只能引导win，只能装在硬盘；grub是第三方操作系统引导器，可以引导硬盘，光盘，网络，U盘，winxp，winpe，win7，linux，dos等

 1. 了解u-boot的功能。
>* 系统引导支持NFS挂载、RAMDISK(压缩或非压缩)形式的根文件系统；支持NFS挂载、从FLASH中引导压缩或非压缩系统内核；
>* 基本辅助功能强大的操作系统接口功能；可灵活设置、传递多个关键参数给操作系统，适合系统在不同开发阶段的调试要求与产品发布，尤以Linux支持最为强劲；支持目标板环境参数多种存储方式，如FLASH、NVRAM、EEPROM；
>* CRC32校验可校验FLASH中内核、RAMDISK镜像文件是否完好；
>* 设备驱动串口、SDRAM、FLASH、以太网、LCD、NVRAM、EEPROM、键盘、USB、PCMCIA、PCI、RTC等驱动支持；
>* 上电自检功能SDRAM、FLASH大小自动检测；SDRAM故障检测；CPU型号；
>* 特殊功能XIP内核引导；

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？
>中断：  
>* 可屏蔽中断
>* 非屏蔽中断 
>
>异常: 
>* 故障（Fault),如缺页异常处理程序
>* 陷阱（Trap),当执行没有必要重新执行已终止的指令时，出发陷阱。
>* 异常终止（Abort),用于报告严重的错误。
>* 编程异常（Programmed exception),在编程者发出请求是发生,是由int 或(int3)指令时触。


 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
>主要有进程控制，文件读写操作，文件系统操作，系统控制，内存管理，网络，Socket 套接字，用户管理，进程间通信，sigaction 设置对指定信号的处理方法，消息，管道，信号量，共享内存等。  
* 进程控制主要用于控制计算机的进程，主要有：  
fork 创建一个新进程  
clone 按指定条件创建子进程  
execve 运行可执行文件  
exit 中止进程  
_exit 立即中止当前进程  
getdtablesize 进程所能打开的最大文件数  
getpgid 获取指定进程组标识号   
setpgid 设置指定进程组标志号  
* 文件读写操作，主要用于文件读写，主要有：  
fcntl 文件控制  
open 打开文件  
creat 创建新文件  
close 关闭文件描述字  
read 读文件  
* 文件系统操作，主要用于文件的相关操作，主要有：  
access 确定文件的可存取性  
chdir 改变当前工作目录  
fchdir 参见chdir  
chmod 改变文件方式  
fchmod 参见chmod  
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 
 ```
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
>在ucore_lab-master/labcodes_answer/lab8_result/kern/syscall里面，打开syscall.c文件，可以看到二十二个系统调用的函数，主要分为：  
>进程调度：       
>* [SYS_exit]                sys_exit,
>* [SYS_fork]              sys_fork,
>* [SYS_wait]              sys_wait,
>* [SYS_exec]              sys_exec,
>* [SYS_yield]             sys_yield,
>* [SYS_kill]              sys_kill,
>* [SYS_gettime]           sys_gettime,
>* [SYS_lab6_set_priority] sys_lab6_set_priority,
>* [SYS_sleep]             sys_sleep,
>
>文件操作：
>* [SYS_getpid]            sys_getpid,
>* [SYS_putc]              sys_putc,
>* [SYS_pgdir]             sys_pgdir,
>* [SYS_open]              sys_open,
>* [SYS_close]             sys_close,
>* [SYS_read]              sys_read,
>* [SYS_write]             sys_write,
>* [SYS_seek]              sys_seek,
>* [SYS_fstat]             sys_fstat,
>* [SYS_fsync]             sys_fsync,
>* [SYS_getcwd]            sys_getcwd,
>* [SYS_getdirentry]       sys_getdirentry,
>* [SYS_dup]               sys_dup,
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)
 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
>objdump命令是Linux下的反汇编命令工具。常用命令如下：
>* objdump -f lab1-ex0.exe:显示lab1-ex0.exe的文件头信息
>* objdump -d lab1-ex0.exe:显示lab1-ex0.exe的需要执行指令的那些section
>* objdump -D lab1-ex0.exe:显示lab1-ex0.exe的所有section，汇编代码很清楚，这个指令应该是最常用的
>* objdump -h lab1-ex0.exe:显示lab1-ex0.exe的SectionHeader信息
>* objdump -x lab1-ex0.exe:显示lab1-ex0.exe的全部Header信息
>* objdump -s lab1-ex0.exe:显示lab1-ex0.exe的全部Header信息以及对应十六进制文件代码的对照。 
>
>nm命令显示关于指定 File 中符号的信息，文件可以是对象文件、可执行文件或对象文件库。如果文件没有包含符号信息，nm 命令报告该情况，但不把它解释为出错条件。nm 命令缺省情况下报告十进制符号表示法下的数字值。命令格式如右所示：nm -A a.exe。而中间的符号不同的话，表示的含义也不同：
>* A Global absolute 符号。
>* a Local absolute 符号。
>* B Global bss 符号。
>* b Local bss 符号。
>* D Global data 符号。
>* d Local data 符号。
>* f 源文件名称符号。
>* T Global text 符号。
>* t Local text 符号。
>* U 未定义符号。
>
>file命令用户查看文件类型信息，格式： file[OPTIONS...] [FILE...] 主要参数：
>* --help 显示帮助信息
>* -v,--version 输出版本信息并退出
>* -b,--brief 不显示文件名字
>* -f,--files-fromFILE 读取待测试的名称文件
>* -F,--seperatorSTRING 使用字符串作为分隔符，不再使用“：”
>* -i,--mime 显示文件的mime类型
>* -L,--dereference 显示符号链接所指向文件信息
>* -d,--debug 输出调试信息
>
>系统调用指运行在使用者空间的程序向操作系统内核请求需要更高权限运行的服务。系统调用提供了用户程序与操作系统之间的接口。大多数系统交互式操作需求在内核态执行。如设备IO操作或者进程间通信。在这里，movl $SYS_write,%eax movl $STDOUT,%ebx movl $hello,%ecx movl $12,%edx int $0x80，其中涉及的系统调用时write写操作，首先程序将eax，ebx，ecx，edx四个寄存器存储上对应的参数，随后代码int $0x80启动了系统调用，程序运行时命令行输出hello world。
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 ```
  + 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
>strace常用来跟踪进程执行时的系统调用和所接收的信号,strace可以跟踪到一个进程产生的系统调用,包括参数，返回值，执行消耗的时间。
 
## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。
 1. ucore的系统调用中返回结果的传递代码分析。
 1. 以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
 1. 以ucore lab8的answer为例，尝试修改并运行代码，分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能
 
