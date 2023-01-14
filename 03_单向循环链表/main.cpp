// 03_单向循环链表.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

struct Node
{
    Node(int data = 0) : data_(data), next_(nullptr) {}
    int data_;
    Node* next_;
};

#if 1// 约瑟夫环问题 - 不带头结点的单项循环链表应用
void Joseph(Node* head, int k, int m)
{
    Node* p = head;
    Node* q = head;

    // q指向最后一个
    while (q->next_ != head)
    {
        q = q->next_;
    }

    // 从第k个人开始报数的
    for (int i = 1; i < k; i++)
    {
        q = p;
        p = p->next_;
    }

    // p -> 第k个人
    for (;;)
    {
        for (int i = 1; i < m; i++)
        {
            q = p;
            p = p->next_;
        }

        // 删除p指向的结点
        // q p node
        cout << p->data_ << " ";

        if (p == q)
        {
            delete p;
            break;
        }

        q->next_ = p->next_;
        delete p;
        p = q->next_;          
    }  
    std::cout << std::endl;
}
#endif

// 自己写的约瑟夫环 2022/11/28
// 约瑟夫环问题 - 不带头结点的单项循环链表应用  
// 编号为 k 的人开始报数,数到 m 的那个人踢出去,留下的人数到 m 继续踢出去  
// 然后把出列顺序打印出来
void JosephZhou(Node* head, int k, int m)
{
    Node* another_list{nullptr};
    auto pre{head};      
    while (pre->next_ != head)
    {
        pre = pre->next_;
    }

    auto p{head};  
    
    // 从第k个人开始报数的
    for (int i = 1; i < k; i++)
    {
        pre = pre->next_;
        p = p->next_;
    }

    while (p != pre)
    {
        for (auto i = 1; i < m; i++)
        {   
            pre = pre->next_;
            p = p->next_;
        }             
        auto tmp_next = p->next_;
        std::cout << "[" << p->data_  << "]";   
        if (nullptr == another_list)    
        {
            p->next_ = nullptr;
            another_list = p;    
        }
        else 
        {            
            p->next_ = another_list;
            another_list = p;              
        }
        pre->next_ = tmp_next;  
        p = tmp_next;
    }
}

# if 1
int main()
{
    Node* head = new Node(1);
    Node* n2 = new Node(2);
    Node* n3 = new Node(3);
    Node* n4 = new Node(4);
    Node* n5 = new Node(5);
    Node* n6 = new Node(6);
    Node* n7 = new Node(7);
    Node* n8 = new Node(8);

    head->next_ = n2;
    n2->next_ = n3;
    n3->next_ = n4;
    n4->next_ = n5;
    n5->next_ = n6;
    n6->next_ = n7;
    n7->next_ = n8;
    n8->next_ = head;

    // Joseph(head, 1, 5);  
    JosephZhou(head, 1, 5);  
}

#endif  

#if 0  
/*
* 笔记: 2022/11/28  
* 自己写了一遍感觉还是比较简单,主要注意哪种情况需要更新 tail_ 指针
*/
// 单向循环链表
class CircleLink
{
public:
    CircleLink()
    {
        // 首先创建头节点,空的节点,不装载数据  
        auto p = new Node();   
        p->next_ = p;
        head_ = p;  
        tail_ = p;
    }
    ~CircleLink()
    {
        // 只有头节点
        if (head_->next_ == head_)
        {
            return;
        }
        // 遍历到尾部,逐个释放节点 
        auto p = head_->next_;            
        while (p->next_ != head_)
        {                  
            auto tmp = p->next_;      
            delete p;                
            p = tmp;
        }
        delete p;  

        // 尾节点更新  
        tail_ = head_;        
    }

public:
    // 尾插法   O(1)
    void InsertTail(int val)
    {
        auto p = new Node(val);      
        p->next_ = head_;
        tail_->next_ = p;  
        tail_ = p;
    }

    // 头插法
    void InsertHead(int val)
    {
        auto first = head_->next_;
        auto p = new Node(val);      
        p->next_ = first;     
        head_->next_ = p;
    }

    // 删除节点  
    // 只删除了一个节点
    void Remove(int val)
    {
        auto p = head_->next_;      
        auto pre = head_;  
        auto flag{false};
        while (p != head_)
        {
            if (p->data_ == val)
            {
                flag = true;  
                break;
            }  
            p = p->next_;
            pre = pre->next_;  
        }
        if (flag)
        {            
            if (tail_ == p) tail_ = pre;
            pre->next_ = pre->next_->next_;  
            delete p;
        }
    }

    // 查询
    bool Find(int val) const
    {
        auto p = head_->next_;              
        auto flag{false};
        while (p != head_)
        {
            if (p->data_ == val)
            {
                flag = true;  
                break;
            }  
            p = p->next_;            
        }  
        return flag;
    }

    // 打印链表
    void Show() const
    {
        auto p = head_->next_;              
        while (p != head_)
        {
            std::cout << "[" << p->data_ << "]";
            p = p->next_;            
        }    
        std::cout << std::endl;
    }

private:
    struct Node
    {
        Node(int data = 0) : data_(data), next_(nullptr) {}
        int data_;
        Node* next_;
    };

    Node* head_; // 指向头节点
    Node* tail_; // 指向末尾节点
};


int main()
{
    CircleLink clink;
    srand(time(NULL));

    clink.InsertHead(100);

    for (int i = 0; i < 10; i++)
    {
        clink.InsertTail(rand() % 100);
    }

    clink.InsertTail(200);
    clink.Show();

    clink.Remove(200);
    clink.Show();

    clink.InsertTail(300);
    clink.Show();
}
#endif