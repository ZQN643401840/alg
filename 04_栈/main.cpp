// 06_栈.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>  
#include <cstring>
using namespace std;

// 顺序栈  C++容器适配器 stack    push   pop   top   empty  size
class SeqStack
{
public:
	SeqStack(int size = 10)
		: mtop(0)
		, mcap(size)
	{
		mpStack = new int[mcap];
	}
	~SeqStack()
	{
		delete []mpStack;  
        mpStack = nullptr;
	}

public:
	// 入栈
	void push(int val)
	{
        if (mtop >= val)    expand(2 * mcap);  
        mpStack[mtop++] = val;
	}
	// 出栈
	void pop()
	{
        if (empty())    throw std::string("stack is empty");  
        mtop--;
	}

	// 获取栈顶元素
	int top() const
	{
        if (empty())    throw std::string("stack is empty");  
        return mpStack[mtop-1];
	}

	// 栈空
	bool empty() const
	{
        return mtop == 0;
	}

	int size() const { return mtop;}

private:
	void expand(int size)
	{
        auto new_buf = new int[size];  
        memcpy(new_buf, mpStack, sizeof(int) * mtop);  
        delete []mpStack;  
        mpStack = new_buf;  
        mcap = size;
	}

private:
	int* mpStack;
	int mtop;       // 栈顶位置,是指即将可以放入元素的位置
	int mcap;       // 栈空间大小
};

int main()
{
	int arr[] = { 12,4,56,7,89,31,53,75 };
	SeqStack s;
	
	for (int v : arr)
	{
		s.push(v);
	}

	while (!s.empty())
	{
		cout << s.top() << " ";
		s.pop();
	}
	cout << endl;
}