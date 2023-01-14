// 07_链式栈.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

// 比较符号优先级的
bool Priority(char ch, char topch)
{
	if ((ch == '*' || ch == '/') && (topch == '+' || topch == '-'))
		return true;
	if (topch == '(' && ch != ')')
		return true;
	return false;
}

// 链式栈
class LinkStack
{
public:
	LinkStack(): size_(0)
	{
		auto head = new Node;  
		head->next_ = nullptr;
		head_ = head;  
	}
	~LinkStack()
	{	
		while (nullptr != head_->next_)
		{
			auto next = head_->next_->next_;
			delete head_->next_;
			head_->next_ = next;     			
		}	  
	}

public:
	// 入栈 O(1)   把链表头节点后面，第一个有效节点的位置，当作栈顶位置
	void push(int val)
	{		
		auto next = head_->next_;  
		auto node = new Node(val);    
		node->next_ = next;
		head_->next_ = node;
		size_++;
	}
	// 出栈 O(1)
	void pop()
	{		
		if(empty()) throw "stack_list is empty";
		auto next = head_->next_->next_;		
		auto node = head_->next_;     
		delete node;  
		head_->next_ = next;  
		size_--;
	}
	// 获取栈顶元素
	int top() const
	{
		if(empty()) throw "stack_list is empty";  
		return head_->next_->data_;
	}
	// 判空
	bool empty() const
	{
		return head_->next_ == nullptr;
	}
	// 返回栈元素个数   遍历一遍链表，记录节点个数O(n)    想达到O(1)
	int size() const
	{
		auto cnts{0};   
		auto p = head_->next_;
		while (nullptr != p)
		{
			p = p->next_;  
			cnts++;
		}
		return cnts;
	}

private:
	struct Node
	{
		// Node(int data = 0) : data_(data), next_(nullptr) {}
		// int data_;  
		Node(char data = ' ') : data_(data), next_(nullptr) {}
		char data_;
		Node* next_;
	};

	Node* head_;
	int size_;
};



#if 1
// 中缀表达式 => 后缀表达式
string MiddleToEndExpr(string expr)
{
	string result;
	stack<char> s;

	for (char ch : expr)
	{
		if (ch >= '0' && ch <= '9')
		{
			result.push_back(ch);
		}
		else
		{
			for (;;)
			{
				// 处理符号了
				if (s.empty() || ch == '(')
				{
					s.push(ch);
					break;
				}

				// 比较当前符号ch和栈顶符号top的优先级
				char topch = s.top();
				// Priority:true ch > topch   false ch <= topch
				if (Priority(ch, topch))
				{
					s.push(ch);
					break;
				}
				else
				{
					s.pop();
					if (topch == '(') // 如果遇见)，一直出栈，直到(
						break;
					result.push_back(topch);
				}
			}
		}
	}

	// 如果符号栈还存留符号，直接输出到后缀表达式里面     + /
	while (!s.empty())
	{
		result.push_back(s.top());
		s.pop();
	}

	return result;
}
#endif  


#if 1  
// 中缀表达式 => 后缀表达式  
/*
*	22/12/4 我自己实现的,分成两部分来对待:
* 	1.操作数,它的出现(排布顺序和中缀表达式是一致不变的)  
* 	2.就是调整运算符的顺序,把小括号干掉
* 	然后讲下具体逻辑:遇到操作数就留下放到后缀表达式的后面(无脑跟在末尾) 
*	表达式的话就让它入栈,什么时候出栈呢?下面几条  
* 	即将入栈的运算符比栈顶运算符优先级更低,就将栈顶出栈,然后将优先级更低那个跟在后面(高优先级后面)  
*	遇到了右小括号,将运算符出栈(默认小括号内只能是两个操作数和一个运算符那种)  
*	解析到式子末尾了,栈内还右元素就栈内依次出栈
*/
string MiddleToEndExprZhou(string expr)
{
	std::string result{""};      
	LinkStack stack_operator;
	auto expr_size = expr.size();    	
	auto i = 0;
	while (i < expr_size)
	{
		const char single_element = expr[i];
		if ('0' <= single_element && single_element <= '9')	
		{
			result.append(1, single_element);    
			i++;
			continue;
		}
		if ('(' == single_element)	
		{		
			stack_operator.push('(');
			i++;	  
			continue;
		}  
		if (')' == single_element)
		{
			char top_element = stack_operator.top();   			
			result.append(1, top_element);		    
			stack_operator.pop();   			
			stack_operator.pop();   			
			i++;
			continue;
		}
		auto flag_stack_empty{false};
		flag_stack_empty = stack_operator.empty();  
		char top_element; 
		bool pority{true};
		if (!flag_stack_empty)
		{
			top_element = stack_operator.top();   
			pority = Priority(single_element, top_element);
		}
		
		if (pority || flag_stack_empty)
		{
			stack_operator.push(single_element);  
			i++;
			continue;
		}
		else 
		{
			result.append(1, top_element);
			stack_operator.pop();  		
			result.append(1, single_element);	
			i++;  
			continue;			
		}		
	} 
	while (!stack_operator.empty())
	{
		char top_element = stack_operator.top();  
		result.append(1, top_element);		
		stack_operator.pop();
	}
	return result;
}
#endif


int main()
{
	cout << MiddleToEndExpr("(1+2)*(3+4)") << endl;
	cout << MiddleToEndExpr("2+(4+6)/2+6/3") << endl;
	cout << MiddleToEndExpr("2+6/(4-2)+(4+6)/2") << endl;  
	cout << "===========================================" << endl;  
	cout << MiddleToEndExprZhou("(1+2)*(3+4)") << endl;
	cout << MiddleToEndExprZhou("2+(4+6)/2+6/3") << endl;
	cout << MiddleToEndExprZhou("2+6/(4-2)+(4+6)/2") << endl;
}

#if 0

int main()
{
	int arr[] = { 12,4,56,7,89,31,53,75 };
	LinkStack s;

	for (int v : arr)
	{
		s.push(v);
	}

	cout << s.size() << endl;

	while (!s.empty())
	{
		cout << s.top() << " ";
		s.pop();
	}
	cout << endl;

	cout << s.size() << endl;
}
#endif