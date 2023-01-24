#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>    
#include <chrono>    
#include <algorithm>
#include <functional>    

// 计算一个数有几个位,个位,十位,千位
// 拼音命名函数,桶排序
void BucketSort(int* data, uint64_t size)
{
  // 声明,加定义桶,相同的桶可能有多个数,所以用内层的 std::vector<int> 来存储 
  std::vector<std::vector<int>> vec2_int;    

  // risize() 这个操作要注意，因为这个操作会改变内存空间
  // 刚开始就踩了很多坑
  vec2_int.resize(10);

  // 有数它仍然有位  
  const auto mod{10};  
  auto dev{1};
  auto flag_should_be_dev{false};
  do
  {
    // 把 vec2_int.resize(10); 放到这里会 core dump,根本原因没查出来
    for (auto& each : vec2_int)  each.clear();
    
    flag_should_be_dev = false;
    // 遍历这些数,作桶排序
    for (auto i = 0; i < size; i++)
    {
      auto bucket_num = (data[i] / dev) % mod;  
      // 桶的序号不为0,说明仍然可以来作为被除数来除  
      if (false == flag_should_be_dev)	flag_should_be_dev = (0 != bucket_num);
      // 根据这个数所属的桶的序号将它放到对应的桶内      
      // 桶认为是小数在前 0 - 9,这样的话就是从小到大排序  
      // vec2_int[bucket_num].emplace_back(data[i]);  
      // 从大到小排序
      vec2_int[9 - bucket_num].emplace_back(data[i]);  
    }

    // 二维数组,但是我知道它们其实是线性分布的,前提是将内层的 vector 要 shrink_to_fit();这样内层的 vector 才是连续的
    for (auto& each_vec : vec2_int) each_vec.shrink_to_fit();  

    auto offset{0};
    for(const auto& each_vec : vec2_int)
    {
	    const auto vec_size = each_vec.size();  
	    memcpy(data + offset, &each_vec[0], sizeof(int) * vec_size);  
	    offset += vec_size;
    }
    dev *= 10;
  } while (flag_should_be_dev);  
}

int main()
{

  // 大根堆  
  std::priority_queue<int, std::vector<int>, std::less<int>> std_queue;
  // 小根堆
  // std::priority_queue<int, std::vector<int>, std::greater<int>> std_queue;  

  std::vector<int> vec_int;
  srand(time(0));
  // std::array<int, 15> array_int{88, 0, 12, 2, 23, 2, 0, 56, 78, 45, 199, 100, 288, 17, 18};  
  // for (const auto& each : array_int)
  for (uint64_t i = 0; i < 1000; i++)
  {
    auto rand_num = rand() % 1000;
    std_queue.push(rand_num);   
    vec_int.emplace_back(rand_num);
    // vec_int.emplace_back(each);
  }

  BucketSort(&vec_int[0], vec_int.size());  

  auto cnts{0};
  while (!std_queue.empty())
  {
    auto flag{false};  
    flag = (vec_int[cnts] == std_queue.top());
    if (!flag) std::cout << "cnts: " << cnts << std::endl;  
    assert(flag);
    std_queue.pop();    
    cnts++;
  }
  return 0;
}  

