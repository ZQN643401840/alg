#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>    
#include <chrono>    
#include <algorithm>
#include <functional>    

// 施磊老师的二叉堆学完了思路就来写代码练习    

using Cmp = std::function<bool(int, int)>;  

class PriorityQueue
{
public:  
  PriorityQueue(Cmp cmp, uint64_t cap = 1000):cmp_(cmp)
  {
    cap_ = cap;
    data_ = new int[cap_];  
  } 

  ~PriorityQueue()
  {
    delete[] data_; 
  }  

  void Push(const int& in_put)
  {
    if (last_ == cap_)  Resize();

    // 先把数放到数组末尾,也就是最后一个叶子节点(靠最左边的叶子节点上)    
    int64_t child = last_;
    data_[child] = in_put;   
    
    // 然后该叶子节点操作上浮(有上浮必要才上浮)    
    int64_t parent = (child - 1) / 2;

    while (parent >= 0 && child > 0) 
    {
      auto parent_val = data_[parent];  
      if (cmp_(parent_val, in_put))  
      {
        data_[child] = parent_val;  
        child = parent;
        parent = (child - 1) / 2;
      }  
      else  break;
    }
    data_[child] = in_put;
    
    last_++;
  }  

  void Pop()  
  {
    if (0 == last_) return;  

    // 将最末尾的数放置在堆顶  
    data_[0] = data_[last_ - 1];
    // 最后能放置数的索引减一  
    last_--;  
    // 然后调整该堆,成为大根堆/小根堆    
    Adjust(0);
  }

  int Top()
  {
    return data_[0];
  }

  bool Empty()
  {
    return 0 == last_;
  }
private: 
  // 左叶子节点和右叶子节点都有可能交换到父节点的可能  
  // 大根堆的前置条件下,比如当前拟放置的节点(父节点),比左子节点数小,那么父节点的数就应向下走,来达到稳定 
  // 但是父节点的数不一定向左子节点交换就能达到稳定;之所以不稳定是因为,父节点数不应该待在父节点.但是谁(左节点,还是右节点)交换到父节点这个就必须判断    
  void Adjust(const uint64_t& parent)
  {
    auto parent_val{data_[parent]};   
    auto child_left = -1;
    auto child_right = -1;
    auto child_left_val{data_[parent]};    
    auto child_right_val{data_[parent]};

    // 有左子节点才取节点上的数
    if ((2 * parent + 1) <= (last_ - 1))  
    {
      child_left = 2 * parent + 1;
      child_left_val = data_[child_left];
    }
    // 有右子节点才取节点上的数
    if ((2 * parent + 2) <= (last_ - 1)) 
    {
      child_right = 2 * parent + 2;
      child_right_val = data_[child_right];  
    } 

    // 父节点上这数先和左子节点比较  
    if (cmp_(parent_val, child_left_val))
    {
      auto tmp = data_[parent];  
      data_[parent] = child_left_val;    
      data_[child_left] = tmp;    
      parent_val = child_left_val;
      // 进一步将父节点数和右子节点比较,有可能父节点的数仍然不稳定  
      if (cmp_(parent_val, child_right_val))
      {
        auto tmp = data_[parent];  
        data_[parent] = child_right_val;    
        data_[child_right] = tmp;  
      }
    }  
    else if (cmp_(parent_val, child_right_val))  // 父节点和左节点比较显得稳定,然后父节点直接和右子节点比较
    {
      auto tmp = data_[parent];  
      data_[parent] = child_right_val;    
      data_[child_right] = tmp;  
    }
    
    if (last_ <= 2) return;
    const auto last_parent = (last_ - 2) / 2;  
    if (parent >= last_parent) // 如果这个父节点是最后一个父节点
    {
      return;
    }
    else  // 将左右子节点分别作为父节点,递归调用 Adjust()    
    {
      if (-1 != child_left)    Adjust(child_left); 
      if (-1 != child_right)    Adjust(child_right); 
    }
  }

  void Resize()
  {
    auto tmp = new int[cap_ * 2]; 
    memcpy(tmp, data_, sizeof(int) * cap_);  
    delete[] data_;  
    data_ = tmp;  
    cap_ *= 2;
  }

private:  
  int* data_{nullptr};  
  uint64_t last_{0};  // 指可以放置数的位置  
  uint64_t cap_;    
  const Cmp cmp_;

};  

// 计算一个数有几个位,个位,十位,千位


// 拼音命名函数,桶排序
void BucketSort(int* data, uint64_t size)
{
  // 声明,加定义桶,相同的桶可能有多个数,所以用内层的 std::vector<int> 来存储 
  std::vector<std::vector<int>> vec2_int;  

  // 有数它仍然有位  
  auto mod{10};  
  auto dev{1};
  auto flag_should_be_dev{false};
  do
  {
    vec2_int.resize(10);
    flag_should_be_dev = false;
    // 遍历这些数,作桶排序
    for (auto i = 0; i < size; i++)
    {
      auto bucket_num = (data[i] / dev) % mod;  
      // 桶的序号不为0,说明仍然可以来作为被除数来除  
      flag_should_be_dev = (0 != bucket_num);
      // 根据这个数所属的桶的序号将它放到对应的桶内      
      // 桶认为是小数在前 0 - 9,这样的话就是从小到大排序  
      vec2_int[bucket_num].emplace_back(data[i]);  
      // 从大到小排序
      // vec2_int[9 - bucket_num].emplace_back(data[i]);  
    }

    // 二维数组,但是我知道它们其实是线性分布的,前提是将内层的 vector 要 shrink_to_fit();这样内层的 vector 才是连续的
    for (auto& each_vec : vec2_int) each_vec.shrink_to_fit();  

    memcpy(data, &(vec2_int[0][0]), size * sizeof(int));
    // mod*=10;
    dev*=10;
  } while (flag_should_be_dev);  
}

int main()
{

  // 大根堆  
  // std::priority_queue<int, std::vector<int>, std::less<int>> std_queue;
  // 小根堆
  std::priority_queue<int, std::vector<int>, std::greater<int>> std_queue;  

  std::vector<int> vec_int;
  srand(time(0));
  for (uint64_t i = 0; i < 1000; i++)
  {
    auto rand_num = rand() % 1000;
    std_queue.push(rand_num);   
    vec_int.emplace_back(rand_num);
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

// 回过头看施磊老师的实现  
// 上浮操作思路很清晰,就是将最后位置的节点当作子节点,和父节点比较,如此迭代,直到找到合适的位置   
// 同样下沉操作也清晰,通过代码就能理解到