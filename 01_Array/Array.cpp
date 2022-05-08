#include <stdio.h>  
#include <stdint.h>  
#include <iostream>  
#include <time.h>

// 模板只支持内置数据类型,因为我没写空间适配器    
// 如何索引到内存的? 借助 m_arr 和 m_cur 
template<typename T>
class Array
{
public:
	// 宜用参数初始化列表
	Array(size_t cap = 20) : m_cap(cap), m_cur(0)
	{		
		m_arr = new T[m_cap];
	}

	~Array()
	{ 
		// 易错点,删除动态数组.以及指针赋 nullptr   
		delete[] m_arr;  
		m_arr = nullptr;
	}

	// 末尾增加元素
	void push_back(T val)
	{			
		if (m_cur == m_cap)	expand();  

		m_arr[m_cur++] = val;
	}  

	// 末尾删除元素  
	// 易错,检查参数正确性
	void pop_back()
	{  
		if (m_cur == 0)	return;
		--m_cur;
	}

	// 按位置增加元素
	void insert(const int& pos, T val)
	{  
		if (pos > m_cur || pos < 0)	return;

		if (pos == m_cap) expand();
		
		for (auto i = m_cur; i > pos; --i)
		{
			m_arr[i] = m_arr[i - 1];  
		}  

		m_arr[pos] = val;  
		// 易错!!++m_cur  
		++m_cur;
	}  

	// 按位置删除
	void erase(const int& pos)
	{  
		if (pos >= m_cur || pos < 0)	return;    

		/*
		* 这个算法明显错了
		for (auto i = m_cur - 1; i > pos; --i)
		{
			m_arr[i -1] = m_arr[i];
		} 
		*/  
		for (auto i = pos; i < m_cur-1; i++)
		{  
			m_arr[i] = m_arr[i + 1];
		}
		--m_cur;
	}

	// 元素查询
	int find(const int& val)
	{
		for (auto i = 0; i < m_cur; ++i)
		{
			if (val == m_arr[i])	return i;
		}  
		return -1;
	}

	// 打印数据
	void show()const
	{		
		for (auto i = 0; i < m_cur; ++i)
		{
			std::cout << m_arr[i] << std::endl;
		}
	}
private:  
	void expand()
	{
		auto mult = m_cap * m_factor;   

		auto tmp = new T[mult];  
		memcpy(tmp, m_arr, sizeof(T) * m_cap);  		
		delete[] m_arr;  
		m_arr = tmp;  
		// 易错!!更新 m_cap  
		m_cap = mult;
	}

private:    
	// Array 本质上就是借用这几个指针或叫 index 管理一段线性内存  
	T* m_arr;
	uint32_t m_cap;		// 容量
	uint32_t m_cur;		// 当前可用空间
	const uint16_t m_factor = 2;		// 扩容因子
};

// 1  
#if 0
int main()
{
	Array<double> arr;

	srand(time(0));
	for (int i = 0; i < 10; i++)
	{
		arr.push_back(rand() % 100);
	}

	arr.show();
	arr.pop_back();
	arr.show();

	arr.insert(0, 100);
	arr.show();

	arr.insert(10, 200);
	arr.show();

	int pos = arr.find(100);
	if (pos != -1)
	{
		arr.erase(pos);
		arr.show();
	}

	getchar();
	return 0;
}  
#endif // 0


// 逆序字符串  
// 推荐这个
void Reverse(char arr[], int size)
{
	char tmp;  
	auto i = 0, j = size -1;  

	while (i < j)
	{
		tmp = arr[i];
		arr[i] = arr[j];  
		arr[j] = tmp;

		++i;
		--j;
	}
}

void Reverse1(char arr[], int size)
{
	/*
	* 这种做法就是错的,不用判断是奇数个元素或偶数个元素
	* 元素个数直接除以二,索引值小于该结果,就能实现全部的数交换
	// odd check  
	auto flag_odd = false;	
	if (size % 2 != 0) flag_odd = true;  

	if (flag_odd == false)
	{
		for (auto i = 0; i < size / 2; ++i)
		{  
			auto tmp = arr[i];
			arr[i] = arr[size - 1 - i];
			arr[size - 1 - i] = tmp;
		}
	}  
	else
	{
		size += 1;
		for (auto i = 0; i < size / 2; ++i)
		{
			auto tmp = arr[i];
			arr[i] = arr[size - 2 - i];
			arr[size - 2 - i] = tmp;
		}
	}  
	*/  

	auto index_mid = size / 2;    
	auto index = 0;
	while (index < index_mid)
	{
		auto tmp = arr[index];  
		arr[index] = arr[size - 1 - index];  
		arr[size - 1 - index] = tmp;  
		index++;
	}
}

// 2
#if 0
int main()
{
	char arr[] = "hello world";
	Reverse1(arr, strlen(arr));
	std::cout << arr << std::endl;  
	getchar();
	return 0;

}
#endif // 1

// 整形数组，把偶数调整到数组的左边，把奇数调整到数组的右边  
// 2022-5-7 今天看,逻辑不清晰,又有遍历又有双指针,不晓得什么时候写的  
// 竟然通过了力扣测试,说明逻辑是没问题的
void AdjustArray1(int arr[], int size)
{
	auto right = size - 1;    
	auto left = 0;  
	auto tmp = arr[0];
	// 注意!! 加上循环控制变量 && left < right 复杂度缩小一半多 
	for (auto i = 0; i < size -1 && left < right; ++i)  
	{
		// even
		if (arr[i] % 2 == 0)
		{
			while (arr[left] % 2 == 0 && left < right)
			{
				++left;
			}
			printf("e: %d, %d \n", arr[i], arr[left]); 
			if (left < right && left < i)
			{
				tmp = arr[left];
				arr[left] = arr[i];
				arr[i] = tmp;
				++left;
			}			
		}    
		// odd
		else
		{  
			while (arr[right] % 2 != 0 && left < right)
			{
				--right;
			}  
			printf("o: %d, %d \n", arr[i], arr[right]);  

			if (left < right && right > i)
			{
				tmp = arr[right];
				arr[right] = arr[i];
				arr[i] = tmp;
				--right;
			}			
		}
	}  
}  

// 推荐,双指针逻辑更清晰    
// 判断循环中奇数,偶数的条件对调下,又能实现奇数/偶数方向的对调
void AdjustArray(int arr[], int size)
{
	auto left = 0, right = size - 1;
	auto tmp = arr[0];
	while (left < right)
	{
		// find first odd index
		// for (; arr[left] % 2 == 0 && left < right; ++left){}  
		// 注意!! 严谨的写法,能限制 ++left,避免越界 
		while (arr[left] % 2 == 0 && left < right)
		{
			++left;
		}

		// find first even index
		// for (; arr[right] % 2 != 0 && left < right; --right){}  
		while (arr[right] % 2 != 0 && left < right)
		{
			--right;
		}  

		// 注意!!易忘这个判断    
		if (left >= right)	break;

		tmp = arr[left];  
		arr[left] = arr[right];  
		arr[right] = tmp;  
	}	
}

const auto arr_num = 100;

// 3
#if 0  
int main()
{
	int arr[arr_num] = { 0 };
	srand(time(0));
	for (int i = 0; i < arr_num; i++)
	{
		arr[i] = rand() % 100;
	}

	for (int v : arr)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	AdjustArray(arr, arr_num);
	for (int v : arr)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;  
	getchar();  
	return 0;
}
#endif // 1  

// 移除指定元素  
int removeElement1(int* nums, int numsSize, int val) 
{

	if (numsSize <= 0) return 0;

	if (numsSize == 1)
	{
		if (nums[0] == val) return 0;
		else return 1;
	}

	auto left = 0;
	auto right = numsSize - 1;

	// decltype(nums[0]) tmp;
	auto tmp = nums[0];

	while (left < right)
	{
		while (nums[left] != val && left < right)
		{
			++left;
		}

		while (nums[right] == val && left < right)
		{
			--right;
		}

		printf("left:%d , right: %d, nums[left]: %d, nums[right]: %d \n ", left, right, nums[left], nums[right]);

		if (left < right)
		{
			tmp = nums[left];
			nums[left] = nums[right];
			nums[right] = tmp;
		}
	}

	if (right == numsSize - 1 && nums[right] != val) return right + 1;
	if (right == numsSize - 1 && nums[right] == val) return right;

	return right;

	// 风格不同而已
	// if (right == numsSize - 1)
	// {
	// 	 if (nums[right] != val) return right + 1;
	// 	 if (nums[right] == val) return right;
	// }
	// else 
	// {
	//     return right;  	
	// }
}

// 移除指定元素  
// 逻辑稍微清晰点  
int removeElement2(int* nums, int numsSize, int val)
{
	// 参数合法性检查
	if (numsSize <= 0) return 0;

	// 数组只有一个元素  
	if (numsSize == 1)
	{
		if (nums[0] == val) return 0;
		else return 1;
	}

	auto left = 0;
	auto right = numsSize - 1;
	auto tmp = nums[0];

	while (left < right)
	{
		auto flag_change = false;

		do
		{
			if (nums[left] != val)  break;
			if (nums[right] == val) break;
			flag_change = true;
		} while (0);

		if (flag_change && left < right)
		{
			tmp = nums[left];
			nums[left] = nums[right];
			nums[right] = tmp;
			++left;
			--right;
			continue;
		}

		if (nums[left] == val && nums[right] == val)
		{
			--right;
			continue;
		}

		if (nums[left] != val && nums[right] == val)
		{
			++left;
			--right;
			continue;
		}

		if (nums[left] != val && nums[right] != val)
		{
			++left;
			continue;
		}
	}

	printf("left: %d, right: %d \n", left, right);

	for (int i = 0; i < numsSize; ++i)
	{
		printf("nums[i]: %d \n", nums[i]);
	}

	// 数组被重新排序了, 遍历下找到第一个 val      
	// 可以确定的是数组被重新排序这件事是正确了的  
	auto i = 0;
	auto flag_val = false;
	for (; i < numsSize; i++)
	{
		if (nums[i] == val)
		{
			printf("i: %d \n", i);
			flag_val = true;
			break;
		}
	}

	if (flag_val)  return i;
	else return numsSize;
}

// 移除指定元素   
// 前面两种搞了一下午,前面的逻辑还不够清晰  
int removeElement3(int* nums, int numsSize, int val)
{  
	auto left = 0;
	auto right = numsSize - 1;

	while (left < right)
	{
		// 找到要交换的目标
		if (nums[left] != val)
		{
			++left;
			continue;
		}
		// 找到右边可以用来被交换的数
		while (left < right)
		{
			if (nums[right] != val)	break;
			--right;
		}
		if (left < right)
		{
			auto tmp = nums[left];
			nums[left] = nums[right];
			nums[right] = tmp;
			++left;
			--right;
		}
	}

	if (left == right && nums[left] != val)  return left + 1;
	else return left;
}

// 移除指定元素   
// 仿照施磊老师思路    
// 思路和我的完全不一样:将不需要移除的元素放到 "不移区", 至于要移除的元素不关心,它自然会被覆盖或者是抛弃  
int removeElement4(int* nums, int numsSize, int val)
{
	auto left = 0;
	auto right = numsSize - 1;

	while (left <= right)
	{
		// 从右边找到第一个不需要移除的数
		if (nums[right] == val)
		{
			--right;
			continue;
		}

		// 找左边第一个需要被覆盖的数  
		while (left < right)
		{
			if (nums[left] == val)   break;
			++left;
		}

		if (left <= right)
		{
			nums[left] = nums[right];
			++left;
			--right;
		}
	}
	return left;
}

// 注意!!这才是仿照施磊老师的最简单思路   
// 想象成从左右跳跃式地向中间迈进
int removeElement(int* nums, int numsSize, int val)
{
	auto left = 0;
	auto right = numsSize - 1;

	while (left <= right)
	{
		// 从右边找到第一个不需要移除的数  
		// 也即从右边找第一个可以被保留的数  
		if (nums[right] == val)
		{
			right--;
			continue;
		}

		// 看下左边能不能被覆盖
		if (nums[left] == val)
		{
			nums[left] = nums[right];
			right--;
		}
		left++;
	}

	return left;
}
const auto arr_size = 3;  

using namespace std;  

/*  
* 2022-5-1
* 双指针的思路试了一下午,试了很久都不对,看 leetcode 题解,换另一种思路
* 双层循环,外层从左到右遍历,定位到第一个需要被覆盖的数,然后内层循环是在刚才定位到的可以被覆盖的数的右边区间从有到左遍历找到第一个可以拿走的数(即不是目标数,可以被保留的数)    
* 要记录被覆盖了几次,即是数组中找到的目标数
* 
* 这个时间复杂度也是经过考虑了的,外层循环并不是每个元素都会遍历    
* 循环控制条件 right 会向左边缩小    
* 这个算是自己能想到的逻辑最清晰,真正用代码实现的方案    
* 这个算法很有必要,因为 stl 中也有这个接口  
*/  
int removeElement_2022_5_1_1(int* nums, int numsSize, int val) {
	auto right = numsSize - 1;
	auto i = 0;
	for (; i <= right; i++)
	{
		// 从左到右定位到目标数,就是可以被覆盖的数
		if (val == nums[i])
		{
			bool flag_exist = false;
			// 在剩下的区间从右到左查找第一个可以拿走去覆盖的数(也就是不需要被移除的数)
			for (auto j = right; j > i; j--)
			{
				if (val != nums[j])
				{
					nums[i] = nums[j];
					right = j - 1;		 // 右边区间也要缩小 
					flag_exist = true;
					break;
				}
			}
			if (!flag_exist) break;      // 说明上一步,所查找的区间内没找到不需要移动的数,说明剩余的区间都是要移除的数,也就没必要遍历后面的数                
		}
	}
	return i;
}

/*
* 2022-5-1 看了 leetcode 双指针法受到启发,之前自己搞双指针都是 先左指针 while() 一次, 再然后右指针 while() 
* 换种思路,for() 循环(实际我是在外面搞了个while() ),每次一步,决定左右指针是否向中间移动    
* 用双指针,分类的思路也实现了,注意,这种 4 个分支的策略勉强接受,如果分的太多不推荐  
* 总结:有些妙的点,逻辑不复杂,简单明了的思路相比较, leetcode 上应该还有思路要学习 
*/  
int removeElement_2022_5_1_2(int* nums, int numsSize, int val) {
	auto left = 0;
	auto right = numsSize - 1;  
	while (left <= right)
	{
		bool flag_need_remove = false;
		// 找到了需要被移除的数,需要被覆盖
		if (val == nums[left])
		{
			flag_need_remove = true;
		}  
		
		// 右指针找到了可以拿来用的数
		if (val != nums[right])
		{  
			if (true == flag_need_remove)
			{
				nums[left] = nums[right];  
				left++;  
				right--;
				continue;
			}
			else
			{
				left++;  
				continue;
			}
		}
		else
		{
			if (true == flag_need_remove)
			{	
				right--;
				continue;
			}
			else
			{
				left++;
				right--;  
				continue;
			}
		}
	}
	return left;	
}

/*
* 总结了下施磊老师的思路,为什么自己自己模仿他的就不对呢?   
* 问题出在我是用两个 while 来筛选出左右指针指向的 可以被覆盖的数,和可以拿来去覆盖别人的数  
* 施磊老师的思路是 while + if() 而不是两个 while
*/
// 4    
#if 0
int main()
{
	// int arr[arr_size] = { 3, 2, 2, 3 };
	// int arr[arr_size]{3,3,3};  
	// int arr[arr_size]{3};
	int arr[arr_size]{ 2, 3, 3};  

	for (const auto& it: arr)
	{
		cout << "it:" << it << endl;
	}  


	auto ret = removeElement(arr, arr_size, 3);
	
	std::cout << "ret: " << ret << std::endl;
	
	for (auto i = 0; i < ret; ++i)
	{
		std::cout << "arr[i]: " << arr[i] << std::endl;
	}

	return 0;
}
#endif // 1

// 这个在 leetcode 上崩溃,但是调试时又没有报错或崩溃,能正常运行
int removeElement_2022_5_5_1(int* nums, int numsSize, int val) {
	auto left = 0;
	auto right = numsSize - 1;

	while (left <= right)
	{
		// 先找左边第一个可以被覆盖的数
		if (val != nums[left])
		{
			left++;
			continue;
		}

		// 从右边找第一个不需要被抛弃的数          
		while (nums[right] == val && left <= right)   right--;

		if (left >= right)    break;

		nums[left] = nums[right];
		left++;
		right--;
	}
	return left;  
}  

// 看力扣的题解,自己复现的,所以多学习别人的代码  
// 其实这种思路最简单,时间复杂度和空间复杂度并没消耗得太多     
// 真的在项目实战上推崇这种写法,因为双指针也好,都需要遍历每个元素
int removeElement_2022_5_6(int* nums, int numsSize, int val) 
{
    auto left = 0;
    for(auto i = 0; i < numsSize; i++)
    {
        // 从首到尾遍历每个数,如果他不是我要移除的数,那我就保留他  
        if(nums[i] != val)  nums[left++] = nums[i];
    }
	return left;  
}

// 双指针 
// 测试通过,逻辑不明显,不推荐这种
int removeElement_2022_5_7(int* nums, int numsSize, int val) {
    auto left = 0;  
    auto right = numsSize - 1;  

    while(left <= right)
    {
        if(val == nums[right])  
        {
            right--;
            continue;
        }

        if(left > right)    break;

        while(left < right && nums[left] != val)  
        {
            left++;
        }
        if(left <= right)
        {
            nums[left] = nums[right];  
            left++;  
            right--;
        }
    }
	return left;  
}

// 双指针,受题解里思路启发
// 逻辑简洁明了,推荐做法  
int removeElement_2022_5_7_1(int* nums, int numsSize, int val) {
    auto left = 0;  
    auto right = numsSize - 1;  

    while(left <= right)
    {
        if(val == nums[left] && val != nums[right])  
        {
            nums[left] = nums[right];
            left++;
            right--;  
            continue;
        }

        if(val != nums[left]) 
        {
            left++;
        }

        if(val == nums[right])
        {
            right--;
        }        
        
    }
	return left;  
}  

int main()
{
	int array[1]; // = {1};// = {1};  
	auto cnt_input = sizeof array / sizeof(int);
	auto cnt = removeElement_2022_5_5_1(array, cnt_input, 1);    
	std::cout << "cnt: " << cnt << std::endl;

	return 0;
}