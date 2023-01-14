// 08_环形队列.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>
using namespace std;

// 环形队列   queue  push  pop  front  back  empty size
class Queue
{
public:
    Queue(int size = 10)
        : cap_(size)
        , front_(0)
        , rear_(0)
        , size_(0)
    {
		pQue_ = new int[cap_];
    }
    ~Queue()
    {
		delete []pQue_;  
		pQue_ = nullptr;
    }

public:
    // 入队 O(1)
    void push(int val)
    {
		if (check_wether_residue_one())	expand(2 * cap_);  
		pQue_[rear_] = val;  
		rear_++;  
		size_++;
    }
    // 出队 O(1)
    void pop()
    {
		if (empty())	throw "Queue is empty";  
		front_++;  
		size_--;
    }
    // 队头元素
    int front() const
    {
		if (empty())	throw "Queue is empty";    
		return pQue_[front_];
    }
    // 队尾元素
    int back() const
    {
		if (empty())	throw "Queue is empty";      
		return pQue_[rear_ - 1];
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
    // 扩容接口
    void expand(int size)
    {
		auto new_buf = new int[size];  
		memcpy(new_buf, pQue_, sizeof(int) * size_);  
		delete []pQue_;  
		pQue_ = new_buf;  
		cap_ = size;  		
    }

	// 检查环形队列是否再插入一个元素就满了  
	// 而不能判断现在环形队列是否就满了(逻辑上实现不了)
	bool check_wether_residue_one()
	{
		auto next = (rear_ + 1) % cap_;
		if (next == front_)	return true;  
		else return false;
	}  

	// 很重要的一点不用再计算从队头索引循环到队尾(注意:是循环到队尾),这之间的元素个数  
	// 用什么来记录队列中的元素个数呢? 用 size_  
	// 同样用队列的容量减去已存储元素个数可拿到队列中剩余的空间,避免计算队头到队尾的元素个数
private:
    int* pQue_;
    int cap_;   // 空间容量
    int front_; // 队头
    int rear_;  // 队尾,也就是即将可以插入元素的位置的索引
    int size_;  // 队列元素个数
};

int main()
{
    int arr[] = { 12,4,56,7,89,31,53,75 };
    Queue que;
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
	for (auto i = 0; i < 20000000; i++)
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

