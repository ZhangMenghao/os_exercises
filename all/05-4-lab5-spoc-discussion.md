# lab5 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。


## 个人思考题

### 总体介绍

 - 第一个用户进程创建有什么特殊的？
 - 系统调用的参数传递过程？
 - getpid的返回值放在什么地方了？

### 进程的内存布局

 - 尝试在进程运行过程中获取内核堆栈和用户堆栈的调用栈？
 - 尝试在进程运行过程中获取内核空间中各进程相同的页表项（代码段）和不同的页表项（内核堆栈）？

### 执行ELF格式的二进制代码-do_execve的实现

 - 在do_execve中进程清空父进程时，当前进程是哪一个？在什么时候开始使用新加载进程的地址空间？
 - 新加载进程的第一级页表的建立代码在哪？

### 执行ELF格式的二进制代码-load_icode的实现

 - 第一个内核线程和第一个用户进程的创建有什么不同？
 - 尝试跟踪分析新创建的用户进程的开始执行过程？

### 进程复制

 - 为什么新进程的内核堆栈可以先于进程地址空间复制进行创建？
 - 进程复制的代码在哪？复制了哪些内容？
 - 进程复制过程中有哪些修改？为什么要修改？

### 内存管理的copy-on-write机制
 - 什么是写时复制？
 - 写时复制的页表在什么时候进行复制？共享地址空间和写时复制有什么不同？

## 小组练习与思考题

### (1)(spoc) 在真实机器的u盘上启动并运行ucore lab,

请准备一个空闲u盘，然后请参考如下网址完成练习

https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-boot-with-grub2-in-udisk.md

(报告可课后完成)请理解grub multiboot spec的含义，并分析ucore_lab是如何实现符合grub multiboot spec的，并形成spoc练习报告。

### (2)(spoc) 理解用户程的生命周期。

> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 练习用的[lab5 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab5/lab5-spoc-discuss)

#### 掌握知识点
1. 用户进程的启动、运行、就绪、等待、退出
2. 用户进程的管理与简单调度
3. 用户进程的上下文切换过程
4. 用户进程的特权级切换过程
5. 用户进程的创建过程并完成资源占用
6. 用户进程的退出过程并完成资源回收

> 注意，请关注：内核如何创建用户进程的？用户进程是如何在用户态开始执行的？用户态的堆栈是保存在哪里的？

阅读代码，在现有基础上再增加一个用户进程A，并通过增加cprintf函数到ucore代码中，
能够把个人思考题和上述知识点中的内容展示出来：即在ucore运行过程中通过`cprintf`函数来完整地展现出来进程A相关的动态执行和内部数据/状态变化的细节。(约全面细致约好)

请完成如下练习，完成代码填写，并形成spoc练习报告

> 设计思路：  
通过对已有代码的分析，发现内核线程，线程编号0为idle线程，创建编号为1的内核线程，然后该线程创建编号为2的用户进程，并为其分配内存等资源，然后进行exit.c中的用户程序。在这个用户程序中，调用fork的系统调用，复制得到了一个用户进程。在这里修改程序，增加了一个用户进程，并使得这两个进程轮流运行。最终编号为2的父进程等待子进程运行结束。最后退出进程完成资源回收。
输出结果如下： 
```
++ setup timer interrupts
     from 0 to 1 schedule//分析：初始时从idle内核线程，调度到新创建的编号为1的线程，以下进行堆栈、寄存器、上下文的切换。
     switch kstack
     switch cr3
     switch context
     do_fork, fork ring is 0//分析:在内核态中，调用do_fork，并输出此时的要创建的进程的特权级。
     wake up 2//分析：将创建的进程的state从UNINIt->RUNNABLE
     from 1 to 2 schedule
     switch kstack
     switch cr3
     switch context
kernel_execve: pid = 2, name = "exit".//分析：调用kernel_execve开始铭为exit的用户进程的创建工作。
sys_exec , i will create a user program!
++++++create user process!
+++++alloc user  resource ++++++//分析：以下是分配资源的过程，主要对用户进程进行了内存空间、页表项、内存管理块等相关资源的分配工作，并设置了对应用户环境的中断。
     create a mm  resource 
     create a PDT  resource 
     copy TEXT/DATA section, build BSS parts in binary to memory space of process 
     get the file header of the bianry program 
     get the entry of the program section headers of the bianry program 
     get new vma 
     alloc memory 
     copy TEXT/DATA section of bianry program
     build BSS section of binary program
     get new vma 
     alloc memory 
     copy TEXT/DATA section of bianry program
     build BSS section of binary program
     get new vma 
     alloc memory 
     copy TEXT/DATA section of bianry program
     build BSS section of binary program
     build user stack memory
     set current process's mm, sr3, and set CR3 reg 
     setup trapframe for user environment 
+++++alloc resource end++++
++++++switch from kernel ring0 to user ring3!++++//分析：在进行完用户进程资源分配之后，还需要进行特权级的转换，为用户进程设置对应寄存器的值。
      switch cs
      switch ds es ss
      switch esp
      switch eip
      switch eflags
++++++switch from kernel ring0 to user ring3! end ++++
//分析：以下就开始运行用户进程了
I am the parent. Forking the child...//分析：这是用户进程中的一句输出，表示的是用户进程2是父进程，将进行fork操作
++++syscall fork ,from user to kernel//分析：通过系统调用，从用户态进入内核态
     do_fork, fork ring is 3//分析:在内核态中，调用do_fork，并输出此时的要创建的进程的特权级。
     wake up 3//分析：将创建的进程的state从UNINIt->RUNNABLE
++++syscall fork end,from kernel to user
//分析：fork完一个新进程，回到父进程，输出创建的子进程的编号
I am parent, fork a child pid 3
//分析：以下是第二个进程的复制创建，过程与上一个类似。
++++syscall fork ,from user to kernel
     do_fork, fork ring is 3
     wake up 4
++++syscall fork end,from kernel to user
I am parent, fork a child pid 4
I am the parent, waiting now..
//分析：父进程创建完子进程后系统调用wait，等待子进程运行结束
++++syscall wait ,from user to kernel
     from 2 to 4 schedule
     switch kstack
     switch cr3
     switch context
++++syscall fork end,from kernel to user
//分析：以下开始两个子进程的交替运行，每次运行一步后，通过系统调用yield，转换到内核态，运行schdule，进行两个进程之间的互相调度。
I am the child 4. step0
++++++syscall yield,switch user to kernel //分析：这是第一次调用的过程
     from 4 to 3 schedule
     switch kstack//分析：需要转换堆栈
     switch cr3//分析：转换cr3
     switch context//分析：转换上下文
++++syscall fork end,from kernel to user
//分析：以下调度过程重复，子进程3，4交替运行
I am the child 3. step0
++++++syscall yield,switch user to kernel 
     from 3 to 4 schedule
     switch kstack
     switch cr3
     switch context
++++++syscall yield end ,switch kernel to user 
I am the child 4. step1
++++++syscall yield,switch user to kernel 
     from 4 to 3 schedule
     switch kstack
     switch cr3
     switch context
++++++syscall yield end ,switch kernel to user 
I am the child 3. step1
++++++syscall yield,switch user to kernel 
     from 3 to 4 schedule
     switch kstack
     switch cr3
     switch context
++++++syscall yield end ,switch kernel to user 
I am the child 4. step2
++++++syscall yield,switch user to kernel 
     from 4 to 3 schedule
     switch kstack
     switch cr3
     switch context
++++++syscall yield end ,switch kernel to user 
I am the child 3. step2
++++++syscall yield,switch user to kernel 
     from 3 to 4 schedule
     switch kstack
     switch cr3
     switch context
++++++syscall yield end ,switch kernel to user 
++++++4 exit ,recource relase!++++++//分析：进程4退出，回收资源
     vma relaese and release page dir entry exit_mmap//回收vma，页目录项
     relase page table put_pgdir//回收页表项
     release mm//回收mm
++++++4 exit ,recource release end!++++++
//分析：进程4回收后，调度到进程3
     wake up 2
     from 4 to 3 schedule
     switch kstack
     switch cr3
     switch context
++++++syscall yield end ,switch kernel to user 
++++++3 exit ,recource relase!++++++//分析：进程3的资源回收与4类似。
     vma relaese and release page dir entry exit_mmap
     relase page table put_pgdir
     release mm
++++++3 exit ,recource release end!++++++
     from 3 to 2 schedule
     switch kstack
     switch cr3
     switch context
++++syscall wait end,from kernel to user
//分析：子进程运行结束后，父进程从wait状态中结束，开始继续运行。
wait pid 3 and 4 ok.
exit pass.
++++++2 exit ,recource relase!++++++//分析：父进程运行结束，开始回收资源
     vma relaese and release page dir entry exit_mmap
     relase page table put_pgdir
     release mm
++++++2 exit ,recource release end!++++++
     wake up 1
     from 2 to 1 schedule
     switch kstack
     switch cr3
     switch context
all user-mode processes have quit.//分析：所有用户进程运行结束
init check memory pass.
++++++1 exit ,recource relase!++++++//分析：内核线程也回收资源
```
