// 08_环形队列.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>
using namespace std;

// 环形队列   queue  push  pop  front  back  empty size
class ListQueue
{
public:
    ListQueue():  
      size_(0)
    {
      head_ = new Node(0);     
      head_->next = nullptr;  
      head_->pre = nullptr;
    }

    ~ListQueue()
    {
      while (!empty())
      {
        pop();
      }
    }

public:
    // 入队 O(1)
    void push(int val)
    {
      auto node = new Node(val);     
      if (nullptr == head_->next)
      {
        node->next = head_;  
        node->pre = head_;        
        head_->next = node;  
        head_->pre = node;  
      }
      else 
      {
        auto tail = head_->pre;    
        node->next = head_;  
        node->pre = tail;     
        tail->next = node;
        head_->pre = node;
      }              
      size_++;
    }
    // 出队 O(1)
    void pop()
    {
		  if (empty())	throw "ListQueue is empty";  
      auto node = head_->next;    
      head_->next = head_->next->next;  
      head_->next->pre = head_;
      delete node;  
      size_--;
    }
    // 队头元素
    int front() const
    {
		  if (empty())	throw "ListQueue is empty";      
      return head_->next->data;
    }
    // 队尾元素
    int back() const
    {
		  if (empty())	throw "ListQueue is empty";        
      return head_->pre->data;
    }
    // 队空
    bool empty() const
    {
		  return size_ == 0;
    }
    // 队列元素的个数
    int size() const
    {
		  return size_;
    }

private:
  // 容器用 size_ 来记录元素个数太实用,方便了
	// 很重要的一点不用再计算从队头索引循环到队尾(注意:是循环到队尾),这之间的元素个数  
	// 用什么来记录队列中的元素个数呢? 用 size_  
	// 同样用队列的容量减去已存储元素个数可拿到队列中剩余的空间,避免计算队头到队尾的元素个数  

	struct Node
	{
    Node(int data_in):  
    data(data_in),  
    pre(nullptr),
    next(nullptr)
    {}
    Node* pre;  
    Node* next;  
    int data;
	}; 

private:
    Node* head_;        
    int size_;  // 队列元素个数
};

int main()
{
    int arr[] = { 12,4,56,7,89,31,53,75 };
    ListQueue que;
  	std::queue<int> que_int;

    for (int v : arr)
    {
      que.push(v);
		  que_int.push(v);
    }
    
    // cout << que.front() << endl;
    // cout << que.back() << endl;

    que.push(100);
    que.push(200);
    que.push(300);

    que_int.push(100);
    que_int.push(200);
    que_int.push(300);

    // cout << que.front() << endl;
    // cout << que.back() << endl;

	srand(time(0));  
	for (auto i = 0; i < 200000000; i++)
	{
		auto rand_num = rand() % 100;   
		que.push(rand_num);  
		que_int.push(rand_num);  
	}

    while (!que.empty())
    {
      // cout << que.front() << " " << que.back() << endl;  		  
      assert(que.front() == que_int.front());	  
      assert(que.back() == que_int.back());
      que.pop();  
      que_int.pop();  
    }  	
}