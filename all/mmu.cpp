#include "iostream"
#include <malloc.h>
using namespace std;

#define Free 0                             //空闲状态
#define Busy 1                             //已用状态
#define OK 1                               //完成
#define ERROR 0                            //出错
#define MAX_length 500                     //最大内存空间为500KB
int flag;

typedef struct freeSpace                   //定义一个空闲区说明表结构
{
    long size;                             //分区大小
    long address;                          //分区地址
    int state;                             //状态
}ElemType;

// 线性表的双向链表存储结构
typedef struct DuLNode
{
    ElemType data; 
    struct DuLNode *prior;                 //前趋指针
    struct DuLNode *next;                  //后继指针
}
DuLNode,*DuLinkList; 
DuLinkList head_Node;                      //头结点
DuLinkList end_Node;                       //尾结点
int alloc(int);                            //内存分配
int free(int);                             //内存回收
int Worst_fit(int);                        //最差适应算法
void show();                               //查看分配
int Initblock();                           //开创空间表 

int Initblock()                            //开创带头结点的内存空间链表
{
    head_Node=(DuLinkList)malloc(sizeof(DuLNode));
    end_Node=(DuLinkList)malloc(sizeof(DuLNode));
    head_Node->prior=NULL;                //头结点的前驱指针指向空
    head_Node->next=end_Node;             //头结点的后继指针指向尾结点
    end_Node->prior=head_Node;            //尾结点的前驱指针指向头结点
    end_Node->next=NULL;                  //尾结点的后继指针指向空
    end_Node->data.address=0;             //尾结点的地址是0 
    end_Node->data.size=MAX_length;       //分区大小是最大分区
    end_Node->data.state=Free;            //状态是空
    return OK;
}

int main()
{
    int ch;                               //算法选择标记
    cout<<"****最差适应算法模拟****\n";

    Initblock();                          //开创空间表
    int choice;                           //操作选择标记
    while(1)
    {
	show();
	cout<<"请输入您的操作：";
        cout<<"\n1: 分配内存\n2: 回收内存\n0: 退出\n";
        
        cin>>choice;
        if(choice==1) 
        {
	    int need = 0;
	    cout<<"请输入需要分配的主存大小(单位:KB):"; 
	    cin>>need;
	    while (need<0 ||need==0) 
	    {
		cout<<"请重新输入分配大小!"<<endl;
		cin>>need;
	    }
	    if(Worst_fit(need)==OK) 
		cout<<"分配成功!"<<endl;
	    else 
		cout<<"内存不足，分配失败!"<<endl;
        }
			                  // 分配内存
        else if(choice==2)              // 内存回收
        {
            int flag;
            cout<<"请输入您要释放的分区号：";
            cin>>flag;
            free(flag);
        }
        else if(choice==0) 
			break;                     //退出
        else                           //输入操作有误
        {
            cout<<"输入有误，请重试!"<<endl;
            continue;
        }
    }
}
 
//最差适应算法
int Worst_fit(int need)
{
    int ch;                                            //记录最大剩余空间
    DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode)); 
    temp->data.size=need;
    temp->data.state=Busy;
    DuLNode *p=head_Node->next;
    DuLNode *q=NULL;                                   //记录最佳插入位置

    while(p)                                           //初始化最大空间和最佳位置
    {
        if(p->data.state==Free && (p->data.size>=need) )
        {
			if(q==NULL)
			{
				q=p;
				ch=p->data.size-need;
			}
			else if(q->data.size < p->data.size)
			{
				q=p;
				ch=p->data.size-need;
			}
        }
        p=p->next;
    }

    if(q==NULL) return ERROR;                         //没有找到空闲块
    else if(q->data.size==need)
    {
        q->data.state=Busy;
        return OK;
    }
	else
	{
        temp->prior=q->prior;
        temp->next=q;
        temp->data.address=q->data.address;
        q->prior->next=temp;
        q->prior=temp;
        q->data.address+=need;
        q->data.size=ch;
        return OK;
    }
	return OK;
}


//主存回收
int free(int flag)
{
    DuLNode *p=head_Node;
	for(int i= 0; i <= flag; i++)
		if(p!=NULL)
			p=p->next;
		else
			return ERROR;

	p->data.state=Free;
    if(p->prior!=head_Node && p->prior->data.state==Free)//与前面的空闲块相连
    {
        p->prior->data.size+=p->data.size;
        p->prior->next=p->next;
        p->next->prior=p->prior;
		p=p->prior;
    }
    if(p->next!=end_Node && p->next->data.state==Free)//与后面的空闲块相连
    {
        p->data.size+=p->next->data.size;
        p->next->next->prior=p;
        p->next=p->next->next; 
    }
    if(p->next==end_Node && p->next->data.state==Free)//与最后的空闲块相连
    {
        p->data.size+=p->next->data.size;
        p->next=NULL; 
    }

    return OK;
}
 
//显示主存分配情况
void show()
{
	int flag = 0;
    cout<<"\n主存分配情况:\n";
    cout<<"++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    DuLNode *p=head_Node->next;
	cout<<"分区号\t起始地址\t分区大小\t状态\n\n";
    while(p)
    {
        cout<<"  "<<flag++<<"\t";
        cout<<"  "<<p->data.address<<"\t\t";
        cout<<" "<<p->data.size<<"KB\t\t";
        if(p->data.state==Free) 
			cout<<"空闲\n\n";
        else 
			cout<<"已分配\n\n";
        p=p->next;
    }
    cout<<"++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}
