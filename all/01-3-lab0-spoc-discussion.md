# lab0 SPOC思考题

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。
>基本可以读懂，但是也有一些不懂之处，如outb %al, $0x64。  

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
>个人现在对ucore一无所知，实在谈不上对硬件设计或功能细节不够了解。

请给出你觉得的中断的作用是什么？使用中断有何利弊？
>中断的作用：它能使CPU在运行过程中对外部事件发出的中断请求及时地进行处理，处理完成后又立即返回断点，继续进行CPU原来的工作。它是计算机可以更好更快利用有限的系统资源解决系统响应速度和运行效率的一种控制技术。它的作用就是可以使你的电脑系统运行更快，更平稳。一方面，有了中断功能，PC系统就可以使CPU和外设同时工作，使系统可以及时地响应外部事件。而且有了中断功能，CPU可允许多个外设同时工作。这样就大大提高了CPU的利用率，也提高了数据输入、输出的速度。另一方面，有了中断功能，就可以使CPU及时处理各种软硬件故障。计算机在运行过程中，往往会出现事先预料不到的情况或出现一些故障，如电源掉电、存储出错，运算溢出等等。计算机可以利用中断系统自行处理，而不必停机或报告工作人员。  
中断：利：提高了cpu效率;
      弊：硬件实现较为复杂。

哪些困难（请分优先级）会阻碍你自主完成lab实验？
>
* 对ucore中涉及的硬件设计或功能细节不了解；
* 实验源代码理解难度大，对汇编语言了解不够深刻。

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？
>反汇编代码。

了解函数调用栈对lab实验有何帮助？
>可以了解操作系统对于函数的调用分配的具体操作顺序，从而对操作系统有更深刻的认识。

你希望从lab中学到什么知识？
>我希望知道操作系统的运行方式以及软硬件如何结合起来的。

---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。
>我的电脑无法进行virtual box和虚拟机的安装，后来我自己只好自己在ubuntu系统下进行了环境配置。

熟悉基本的git命令行操作命令，从github上
的 http://www.github.com/chyyuu/ucore_lab 下载
ucore lab实验
>使用git clone http://www.github.com/chyyuu/ucore_lab 即可。

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1
>已经尝试。

对于如下的代码段，请说明”：“后面的数字是什么含义
```
/* Gate descriptors for interrupts and traps */
struct gatedesc {
    unsigned gd_off_15_0 : 16;        // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;            // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;            // reserved(should be zero I guess)
    unsigned gd_type : 4;            // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;                // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;                // Present
    unsigned gd_off_31_16 : 16;        // high bits of offset in segment
};
```

>后面的数字表示位数的数目。

对于如下的代码段，
```
#define SETGATE(gate, istrap, sel, off, dpl) {            \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;        \
    (gate).gd_ss = (sel);                                \
    (gate).gd_args = 0;                                    \
    (gate).gd_rsv1 = 0;                                    \
    (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = (dpl);                                \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}
```

如果在其他代码段中有如下语句，
```
unsigned intr;
intr=8;
SETGATE(intr, 0,1,2,3);
```
请问执行上述指令后， intr的值是多少？

>2345

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
>大致含义就是对与自己定义的一个list_entry进行的一系列操作，包括初始化(init)，添加(add)，在特定元素前面添加(add_before)，在特定元素后面添加(add_after)，删除(del)，删除并初始化(del_init)，清空(empty)，链表下一个(next)，前缀(prev)等。

---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。
- [x]  

>  

---
