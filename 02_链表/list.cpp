#pragma once
#include <iostream>
#include <cassert>  
#include <time.h>
#include <unordered_map>  

using namespace std;

// 记录内存使用情况的 map 
// key --> void*; val --> cnts
std::unordered_map<void*, int16_t> mem_record;

void print_mem_use()
{
	for (const auto& it : mem_record)
	{
		cout << it.first << " - " << it.second << endl;
	}
}


// 单向链表 
template<typename T>
struct Node
{
	Node(T data_in, Node* p_next_in) :data(data_in), p_next(p_next_in) {}
	T data;
	Node* p_next;  

	// 试下重载 operator new() 和 operator delete() 来检查内存是否泄露  
	void* operator new(std::size_t size)
	{		
		auto p = ::operator new(size);
		mem_record[p]++;
		// cout << "+ addr: " << p << endl;
		return p;
	}

	void operator delete(void* p)
	{
		mem_record[p]--;
		// cout << "- addr: " << p << endl;
		return ::operator delete(p);
	}
};

// 头节点不带数据  
template<typename T>
class List
{
public:
	List() :p_head(new Node<T>(T(), nullptr)) {}
	~List()
	{
		// 首节点虽然不带数据,但是它的内存是要释放的 
		auto cur = p_head;
		decltype(cur) tmp;
		while (cur)
		{
			tmp = cur;  
			cur = cur->p_next;    
			delete tmp;
		}
	}

	void InsertHead(T val)
	{
		Insert(p_head, val);
		return;
	}

	void RemoveHead()
	{
		// [1] 节点先保存下来
		auto tmp = p_head->p_next;  
		// 直接改变链接的指向关系  
		p_head->p_next = p_head->p_next->p_next;  
		// 释放 [1] 节点的内存    
		delete tmp;   
		tmp = nullptr;
	}

	void InsertTail(T val)
	{
		auto p_trail = FindTail();
		Insert(p_trail, val);
	}

	// 返回新的尾节点地址
	Node<T>* RemoveTail()
	{  
		auto tmp = p_head;   
		// 除了 [0] 首节点外没有其他节点了  
		if (tmp->p_next == nullptr)
		{
			return tmp;
		}
		while (tmp->p_next->p_next != nullptr)
		{
			tmp = tmp->p_next;
		}  
		delete tmp->p_next;  
		tmp->p_next = nullptr;  
		return tmp;
	}  

	// 只删除链表中首次出现的目标  
	void Remove(T val)
	{
		auto tmp = p_head;

		while (tmp->p_next->data != val)
		{
			tmp = tmp->p_next;
		}  

		auto tmp1 = tmp->p_next;  
		tmp->p_next = tmp->p_next->p_next;  
		delete tmp1;  
		tmp1 = nullptr;
	}

	// 删除节点中所有匹配的目标  
	void RemoveAll(const T& val)
	{
		auto tmp = p_head;
		while (tmp->p_next)
		{
			// 这是找一个元素的那部分代码  
			while (tmp->p_next->data != val)
			{
				tmp = tmp->p_next;
			}

			auto tmp1 = tmp->p_next;
			tmp->p_next = tmp->p_next->p_next;
			delete tmp1;
			tmp1 = nullptr;
		}		
	}

	bool Find(const T& val)
	{
		// 在遍历的算法(show)上稍加改造,每次遍历做个判断即可
		auto flag_find = false;
		auto tmp = p_head->p_next;  
		while (tmp)
		{
			if(val == tmp->data)
			{
				flag_find =true;  
				break;
			}			
			tmp = tmp->p_next;
		}
		return flag_find;		
	}

	void Show()
	{
		// 注意!! 这是错误的示范
		// 错在 tmp->p_next 原始指针会被更改		
		/*
		auto tmp = p_head;

		while (tmp->p_next != nullptr)
		{
			cout << "val: " << tmp->p_next->data << endl;  			
			tmp->p_next = tmp->p_next->p_next;
		}
		*/  

		// 这才是正确的
		auto tmp = p_head->p_next;  
		while (tmp)
		{
			cout << "val: " << tmp->data << endl;
			tmp = tmp->p_next;
		}
	}

	// 返回新产生的这个节点的地址   
	// 新节点是在 p_node 后面插入的
	static Node<T>* Insert(Node<T>* p_node, T val)
	{
		assert(p_node);
		p_node->p_next = new Node<T>(val, p_node->p_next);  
		return p_node->p_next;
	}

private:


	Node<T>* FindTail()
	{
		auto p_tmp = p_head;
		while (p_tmp->p_next != nullptr)
		{
			p_tmp = p_tmp->p_next;
		}
		return p_tmp;
	}

private:

	Node<T>* p_head;	// 头节点上不带数据  

	// 单链表逆序  
	template<typename K>
	friend void ReverseList(List<K>& list);  

	// 求倒数第k个节点的值
	// friend bool GetLaskKNode(List<T>& list, int k, int& val);  
	// 合并两个有序的单链表
	// friend void MergeList(List<T>& list, List<T>& list1);
};

// 单链表逆序  
template<typename T>
void ReverseList(List<T>& list)
{
	// 先找到尾节点,并记录尾节点地址
	// 将尾节点安插到 [1] 节点位置,并删除尾节点  
	auto head = list.p_head->p_next;  
	auto tail = list.FindTail();
	auto tmp = list.p_head;
	while (tail != head)
	{
		tmp = List<T>::Insert(tmp, tail->data);
		tail = list.RemoveTail();    
	}

}

// 判断单链表是否存在环，存在返回环的入口节点
// bool IsLinkHasCircle(Node *head, int& val)
// {

// }

// // 判断两个单链表是否相交，如果相交，返回相交节点的值
// bool IsLinkHasMerge(Node* head1, Node* head2, int& val)
// {

// }

#if 1
int main()
{
	{
		List<int> link;
		srand(time(0));
		for (int i = 0; i < 10; i++)
		{
			int val = rand() % 100;
			// link.InsertHead(val);
			link.InsertTail(val);
			// cout << val << " ";
		}
		cout << endl;

		 link.InsertTail(23);
		 link.InsertHead(23);
		 link.InsertHead(23);

		link.Show();
		cout << " ----- " << endl;
		link.RemoveAll(23);
		link.Show();  

		// 逆序链表    
		cout << " ReverseLink " << endl;
		ReverseList<int>(link);  
		link.Show();  

	}
	print_mem_use();
	getchar();
}
#endif  


#if 0    
// 头节点上也有数据的做法太恶心了,遍历时很不方便
template<typename T>
class List
{
public:
	List() :p_head(nullptr) {}
	~List()
	{
		// TODO 析构链表  
		// 从头节点依次先后析构,但是要提前拿到待析构的节点的指针
	}

	void InsertHead(T val)
	{
		if (p_head == nullptr)
		{
			p_head = new Node(val, nullptr);
			return;
		}

		Insert(p_head, val);
		return;
	}

	void InsertTail(T val)
	{
		if (p_head == nullptr)
		{
			p_head = new Node(val, nullptr);
			return;
		}

		auto p_trail = FindTail();
		Insert(p_trail, val);
	}

	// 只删除链表中首次出现的目标  
	// 看起来逻辑并不简洁  
	void Remove(T val)
	{
		do
		{
			// 链表中至少有两个节点
			if (p_head->p_next != nullptr)	break;
			// 链表中虽然只有一个节点,但是节点中的数据不是要删除的数据,那就直接 return  
			if (p_head->data != val)	return;

			// 那么剩下的这种情况就是只有首节点,并且首节点就是要删除的目标  
			// 删掉节点并 return 
			{
				delete p_head;
				p_head = nullptr;
				return;
			}
		} while (0);

		// 节点数 >= 2, 但首节点就是要删除的目标,这种如何处理  
		if (p_head->data == val)
		{
			auto tmp = p_head;
			p_head = tmp->p_next;
			delete tmp;
			return;
		}

		// 目标节点在第二个及以后的节点上  
		auto tmp = p_head;
		while (tmp->p_next->data != val)
		{
			tmp = tmp->p_next;
		}

		auto tmp1 = tmp->p_next;
		tmp->p_next = tmp->p_next->p_next;
		delete tmp1;
		tmp1 = nullptr;
		return;
	}

	// 删除节点中所有匹配的目标  
	void RemoveAll(T val)
	{
		auto tmp = p_head;

		while (tmp->data != val)
		{
			tmp = tmp->p_next;
		}
		// 

	}

	bool Find(T val)
	{

	}

	void Show()
	{

	}

	static void Insert(Node<T>* p_node, T val)
	{
		assert(p_node);
		p_node->p_next = new Node(val, p_node->p_next);
	}

private:


	Node<T>* FindTail()
	{
		if (p_head == nullptr)	return p_head;

		auto p_tmp = p_head;
		while (p_tmp->p_next != nullptr)
		{
			p_tmp = p_tmp->p_next;
		}
		return p_tmp;
	}

private:
	Node<T>* p_head;	// 头节点上也是要带数据的
};
#endif // 0