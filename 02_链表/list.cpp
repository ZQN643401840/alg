#pragma once
#include <iostream>
#include <cassert>  
#include <time.h>
#include <unordered_map>  
#include <vector>  
#include <algorithm>  
#include <tuple>

namespace zhou
{

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
	friend bool GetLaskKNode(List<T>& list, int k, int& val);  
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
int main_1()
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

}  

/*
* 把上面注释起来,写个链表,不带模板 2022/11/3
*/

struct Node
{	
	static Node* NewNode(const int& data = 0, Node* next = nullptr){return new Node(data, next);}    
	
	// 当前指向的节点是不能删的.如果当前节点删了,后继节点的指针挂在哪呢?
	// static void EraseNode(Node* node)
	// {
	// 	auto node_erase = node->p_next;
	// 	if (nullptr != node_erase)  
	// 	{
	// 		node->p_next = node_erase->p_next;	
	// 		delete node_erase;
	// 	}						
	// }

	// 这个是之前没想到的,就是当前节点的引用存起来,然后就可以删除当前指向的节点,后继节点的指针放在当前的引用上    
	// 给定一个指针的引用,就可以删除当前指针指向的节点
	static void EraseNode(Node*& node)
	{
		auto erase_node = node;  
		node = node->p_next;
		delete erase_node;
	}

	/*
	* @param 只逆序一个节点
	* @param Node* p_pre_node, 当前这个要逆序的节点的,它的 next 该填的值 
	* @param Node* node, 即当前要操作的这个节点    
	* @return 即下一个要迭代的节点
	*/
	static Node* ReserveOne(Node* p_pre_node, Node* node)
	{
		auto tmp = node->p_next;    
		node->p_next = p_pre_node;
		return tmp;
	}

	static void Print(Node* node)
	{
		std::cout << "node:" << node->data << std::endl;
	}

	// 输入是节点指针的引用,由此指针拿到两个节点,并交换两个节点序,输出为指向第三节点的指针引用    	
	static Node*& Swap(Node*& p_node)
	{
		assert(p_node != nullptr);
		auto tmp = p_node->p_next;  
		p_node->p_next = p_node;  
		p_node = tmp;  
		return tmp;
	}

	// 思来想去还是这么交换比较容易理解,用起来能继续容易迭代  
	// 返回的是两个节点中的后一个节点,以及指向的第三节点的指针
	static std::tuple<Node*, Node*> Swap(Node* node, Node* node1)
	{
		// 判断必须是 node 指向 node1
		assert(node1 == node->p_next);  
		auto tmp = node1->p_next;  
		node1->p_next = node;  
		node->p_next = tmp;  
		return std::make_tuple(node, node->p_next);
	}

	// 根据两个指针(分别指向两个节点),把他们拼接在一起    
	static void Cat(Node* node, Node* node1)
	{
		node->p_next = node1;
	}
	
	// 把一个新的节点插入到两个节点中间,新的节点是第二参数   
	static void Insert(Node* node, Node* node1)
	{
		auto tmp = node->p_next;  
		node->p_next = node1;  
		node1->p_next = tmp;
	}

	Node(int data_in, Node* next = nullptr):data(data_in),p_next(next){}  

	void Print(){ std::cout << "[" << data << "]";}
	
	int data{0};  
	Node* p_next{nullptr};	
};

class List
{
public:  
	List(){p_head = Node::NewNode();}

	// 节点头插法  
	Node* InsertHead(const int& data)
	{
		auto p_first = p_head->p_next;  
		auto p_node = Node::NewNode(data, p_first);				  
		p_head->p_next = p_node;		  		
			
	}  

	// 节点尾插法  
	Node* InsertTail(const int& data)
	{
		auto tail = Node::NewNode(data);    
		auto p = p_head->p_next;  
		if (nullptr == p) 
		{
			p = tail;  
			return tail;
		}

		while (nullptr != p->p_next)
		{
			p = p->p_next->p_next;
		}
		p->p_next = tail;  
		return tail;
	}  

	// 删除查找到的第一个节点  
	void Remove(const int& data)
	{
		// 注释的这段也是ok的
		// auto p = p_head;  
		// if (nullptr == p->p_next)  return;

		// while (data != p->p_next->data)
		// {
		// 	p = p->p_next->p_next;  
		// 	if (nullptr == p->p_next)	return;
		// }  
		// auto tmp = p->p_next;
		// p->p_next = p->p_next->p_next;  
		// delete tmp;  

		// 将下面 RemoveAll 稍加改动即可
		auto p = p_head;  		
		while(nullptr != p->p_next)
		{
			if (data == p->p_next->data)
			{
				Node::EraseNode(p->p_next);  
				break;
			}  
			p = p->p_next;
		}
	}  

	// 删除查找到的所有节点  
	void RemoveAll(const int& data)
	{
		auto p = p_head;  		
		while(nullptr != p->p_next)
		{
			if (data == p->p_next->data)
			{
				Node::EraseNode(p->p_next);  
				continue;
			}  
			p = p->p_next;
		}		
	}

	// 逆序  
	// 把问题压缩成的单个节点的关系  
	// 一个节点向谁逆序,即 next 填上谁?在用 next 遍历到尾的过程中,可以用 next 作为输入  
	// 一个节点操作完,要移向下一个节点,如何移动  
	// 归纳就是:1.节点的 next 域填什么? 2.下一节点如何迭代    
	// 如果有第一个节点,则第一节点单独处理.并且保存第二节点的地址,由此开始做逆序操作  
	void Reverse()  
	{
		Node* p_first{nullptr};  
		Node* p_second{nullptr};  		
		if (nullptr != p_head->p_next)  
		{
			p_first = p_head->p_next;
			p_second = p_head->p_next->p_next;  			
		}

		// 一共才只有一个节点,第二节点都不存在
		if (nullptr == p_second)	return;  
			
		auto p_it = p_second;    
		auto p_pre_node = p_first;
		while (nullptr != p_it)
		{			
			// 已经到了最后一个节点,让头节点指向这个节点
			if (nullptr == p_it->p_next)
			{
				p_head->p_next = p_it;    
			}	

			// 先缓存函数的右参数,下次迭代时会更新到左参数上  
			// 其实理这个逻辑它的规律性是很强的,它的迭代规律用纸画出来很清晰的!!
			// 关键在于,第一次写的时候可能出错,而调试这里的关系就是快速定位的关键!!
			auto tmp = p_it;
			p_it = Node::ReserveOne(p_pre_node, p_it);      			
			p_pre_node = tmp;
		}

		// 最后再来修改第一个节点的 next 域,将它置为 nullptr
		p_first->p_next = nullptr;  		
	}
	
	// 找到倒数的第几个节点
	Node* FindTail(const uint32_t& tail_index)
	{  
		auto tmp{p_head->p_next};  
		auto node_cnts = Size();

		assert(tail_index <= node_cnts);  

		// 比如只有一个节点,想找倒数第一节点, index 为 1 - 1
		auto index = node_cnts - tail_index;  
		tmp = p_head->p_next;  
		while (index)
		{
			tmp = tmp->p_next;
			index--;
		}  
		return tmp;
	}

	// 这个其实是大的数往后冒泡  
	// 可以连续调用它来实现排序
	void BubbleOnce()  
	{
		// 一个节点都没有,或者只有一个节点
		if (nullptr == p_head->p_next)
		{
			return;
		}
		if (nullptr == p_head->p_next->p_next)
		{
			return;
		}

		auto p = p_head->p_next;  
		auto p1 = p_head->p_next->p_next;
		auto p_change = p_head;
		while (p->p_next != nullptr)
		{				
			// 调换 > / < 符号实现冒泡的数左移动,或右移动		
			if (p->data > p1->data)  
			{
				auto [p_tmp, p_tmp1] = Node::Swap(p, p1);  
				// 交换之后是 p1(指向) 节点指向 p(指向) 节点
				p_change->p_next = p1;
				p_change = p1;
				p = p_tmp;  
				p1 = p_tmp1;								
			}		
			else
			{
				p = p->p_next;  
				p1 = p->p_next;
				p_change = p_change->p_next;
			}				
		}
	}

	// 节点从小到大排序
	void SortLess()
	{
		auto cnts = Size();  
		while (cnts-- > 0)
		{
			BubbleOnce();
		}
	}

	// 
	uint64_t Size()
	{
		auto tmp{p_head->p_next};  
		auto node_cnts{0};
		while (nullptr != tmp)
		{
			node_cnts++;
			tmp = tmp->p_next;
		}  
		return node_cnts;
	}

	// 合并两个有序链表  
	// 输入两个链表,合并到第一个上  
	// 从后一个链表中的节点中取一个出来,插到前一个有序链表中
	static void MergeLess(List list, List list1)
	{		
		auto p{list.p_head};
		auto p1{list1.p_head->p_next};  
		// 先遍历取第二链表上的节点
		while (nullptr != p1)
		{
			auto tmp = p1->p_next;
			// 取出来然后插入到第一链表中    
			while (true)
			{
				if ( (nullptr == p->p_next) || (p1->data < p->p_next->data) )
				{
					Node::Insert(p, p1);
					break;
				}
				else
				{
					p = p->p_next;
				}
			}
			p1 = tmp;
		}		
	}

	void Print()
	{
		auto p = p_head;
		while(nullptr != p->p_next)
		{
			p->p_next->Print();  
			p = p->p_next;
		}  
		std::cout << std::endl;
	}

private:  
	Node* p_head;
};

template<typename T>
void PrintContinar(T data)
{
	for(const auto& each : data)
	{
		std::cout << "[" << each << "]";
	}  
	std::cout << std::endl;
}

template<typename T>
void PrintContinarR(T data)
{
	auto rb = data.rbegin();
	while (rb != data.rend())
	{
		std::cout << "[" << *rb << "]";
		rb++;
	}
	std::cout << std::endl;
}

int main()
{
	srand(time(0));  
	List list;
	std::vector<int> vec_int;
	for(auto i = 0; i < 20; i++)
	{
		auto rand_num = rand() % 100;  
		list.InsertHead(rand_num);
		vec_int.insert(vec_int.begin(), rand_num);  
		list.InsertTail(rand_num);
		vec_int.insert(vec_int.end(), rand_num);
	}

	// 一 删除元素  
	auto target = vec_int.at(0);
	list.Remove(target);	  
	auto it = std::find(vec_int.begin(), vec_int.end(), target);  
	if(it != vec_int.end())	vec_int.erase(it);  
	target = vec_int.at(4);    	
 	list.Remove(target);	  
	it = std::find(vec_int.begin(), vec_int.end(), target);  
	if(it != vec_int.end())	vec_int.erase(it);  
	target = vec_int.at(10);    	
 	list.Remove(target);	  
	it = std::find(vec_int.begin(), vec_int.end(), target);  
	if(it != vec_int.end())	vec_int.erase(it); 
	target = vec_int.at(12);    	
 	list.Remove(target);	  
	it = std::find(vec_int.begin(), vec_int.end(), target);  
	if(it != vec_int.end())	vec_int.erase(it);  

	// 二 删除指定的所有元素
	// auto target = vec_int.at(10);
	// list.RemoveAll(target);	  
	// auto it = vec_int.begin();
	// while (it != vec_int.end())
	// {
	// 	if(target == *it)	it = vec_int.erase(it);
	// 	else	it++;
	// }

	// target = vec_int.at(4);
	// list.RemoveAll(target);	  
	// it = vec_int.begin();
	// while (it != vec_int.end())
	// {
	// 	if(target == *it)	it = vec_int.erase(it);
	// 	else	it++;
	// }

	// target = vec_int.at(12);
	// list.RemoveAll(target);	  
	// it = vec_int.begin();
	// while (it != vec_int.end())
	// {
	// 	if(target == *it)	it = vec_int.erase(it);
	// 	else	it++;
	// }

	// list.Print();  
	// PrintContinar(vec_int);    

	// 三 逆序链表  
	// 先打印,再逆序    	
	// list.Print();    
	list.Reverse();
	list.Print();      
	PrintContinarR(vec_int);  

	// 四 想找倒数第二节点  
	auto target_node = list.FindTail(19);    
	Node::Print(target_node);

	// 五 按从小到大排序  
	std::cout << "Sort by less" << std::endl;  
	list.SortLess();
	list.Print();        

	// 六 再造一个有序链表,把两个链表做有序合并  
	std::cout << "================== 2nd list " << std::endl;
	List list1;
	for(auto i = 0; i < 20; i++)
	{
		auto rand_num = rand() % 100;  
		list1.InsertHead(rand_num);		
		list1.InsertTail(rand_num);	
	}	
	list1.SortLess();
	list1.Print();  	
	List::MergeLess(list, list1);      
	std::cout << "================== 2nd list merge it" << std::endl;  
	list.Print();

	return 0;
}
