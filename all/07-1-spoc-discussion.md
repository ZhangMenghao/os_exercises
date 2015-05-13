# 同步互斥(lec 17) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 背景
 - 请给出程序正确性的定义或解释。
 - 在一个新运行环境中程序行为与原来的预期不一致，是错误吗？

> 并发程序问题，由于多个进程之间存在资源共享，不确定性，不可重现性，可能出现错误。

 - 程序并发执行有什么好处和障碍？

> 好处：共享资源;加速;模块化。障碍：可能引起错误。

 - 什么是原子操作？

> 一次不存在任何中断或失的操作。要么操作成功完成，要么操作没有执行，不会出现部分执行的状态。

### 现实生活中的同步问题

 - 家庭采购中的同步问题与操作系统中进程同步有什么区别？

> 家庭采购中的有些操作在操作系统中可能被打断。

 - 如何通过枚举和分类方法检查同步算法的正确性？

> 

 - 尝试描述方案四的正确性。

> 枚举所有可能后，可以确认它确实是有效的。

 - 互斥、死锁和饥饿的定义是什么？

> 互斥：一个进程占有资源，其他进程不能使用。   
  死锁：多个进程占有部分资源，形成循环等待。   
  饥饿：其他进程可能轮流占用资源，一个进程一直得不到资源。

### 临界区和禁用硬件中断同步方法

 - 什么是临界区？
 - 临界区的访问规则是什么？
 - 禁用中断是如何实现对临界区的访问控制的？有什么优缺点？

### 基于软件的同步方法

 - 尝试通过枚举和分类方法检查Peterson算法的正确性。
 - 尝试准确描述Eisenberg同步算法，并通过枚举和分类方法检查其正确性。

### 高级抽象的同步方法

 - 如何证明TS指令和交换指令的等价性？
 - 为什么硬件原子操作指令能简化同步算法的实现？
 
## 小组思考题

1. （spoc）阅读[简化x86计算机模拟器的使用说明](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/lab7-spoc-exercise.md)，理解基于简化x86计算机的汇编代码。

2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？  
>dx=-1
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？  
>两个线程的dx=-1,注意要为每个线程设置dx
 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？  
>两个线程的dx=-1
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？  
>x=1，寄存器初值为0
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？  
>x=6，bx寄存器属于每个线程，在切换前都会被保存，故循环3次。
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？  
>x=1或者2。
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？  
>x=1或者2
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？   
>x=1或者2
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？   
>x=1
3. （spoc） 了解software-based lock, hardware-based lock, [software-hardware-lock代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/software-hardware-locks)

  - 理解flag.s,peterson.s,test-and-set.s,ticket.s,test-and-test-and-set.s 请通过x86.py分析这些代码是否实现了锁机制？请给出你的实验过程和结论说明。能否设计新的硬件原子操作指令Compare-And-Swap,Fetch-And-Add？
```
flag.s没有实现锁机制，多个线程可以同时进入临界区
------ Interrupt ------  ------ Interrupt ------  ------ Interrupt ------  
1006 mov  %ax, count
------ Interrupt ------  ------ Interrupt ------  ------ Interrupt ------  
                         1003 mov  $1, flag
                         1004 mov  count, %ax
------ Interrupt ------  ------ Interrupt ------  ------ Interrupt ------  
                                                  1004 mov  count, %ax
                                                  1005 add  $1, %ax

peterson.s也没有实现锁机制，多个线程也可以同时进入临界区
------ Interrupt ------  ------ Interrupt ------  ------ Interrupt ------  
                         1008 jne .fini
                         1012 mov count, %ax
                         1013 add $1, %ax
------ Interrupt ------  ------ Interrupt ------  ------ Interrupt ------  
                                                  1013 add $1, %ax
                                                  1014 mov %ax, count
                                                  1015 mov $0, 0(%fx,%bx,4)
------ Interrupt ------  ------ Interrupt ------  ------ Interrupt ------  
1012 mov count, %ax
1013 add $1, %ax
1014 mov %ax, count

test-and-set.s实现了锁机制，因为不管在任何位置切换线程都能保证有且只有一个线程进入临界区。
ticket.s也实现了锁机制，因为不管在任何位置切换线程都能保证有且只有一个线程进入临界区。
test-and-test-and-set.s也实现了锁机制，因为不管在任何位置切换线程都能保证有且只有一个线程进入临界区。
上述都是实验过程证明的结果。
```
```
Compare-And-Swap

int CompareAndSwap(int *ptr, int expected, int new) {
  int actual = *ptr;
  if (actual == expected)
    *ptr = new;
  return actual;
}
```

```
Fetch-And-Add

int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}
```
