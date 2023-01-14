#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>    
#include <chrono>  

using namespace std;  

// 完全照搬施磊老师的思路,能复现也不错
vector<int> SortArray(vector<int>& nums)   
{
  auto size = nums.size();
  for (auto i = 1; i < size; i++)
  {
    int val = nums[i];  
    int j = i - 1;  
    for ( ; j > 1; j--) 
    {
      if (nums[j] > val)
      {
        nums[j + 1] = nums[j];  
      }  
      else break;
    }      
    nums[j] = val;
  }  
  return nums;
}

void SortOnce(int* const l, int* const r)
{
  // 只有一个数了
  if (l == r) return; 

  // 只有两个数可以快速判断
  if ((r - l) == 1)
  {
    if (*l > *r)
    {
      auto tmp = *r;  
      *r = *l;  
      *l = tmp;
    }
    return;
  }  

  auto move_l = l;  
  auto move_r = r;  
  auto target = *move_l;
  while (move_l != move_r)  
  {
    if (target < *move_r)    
    {
      move_r--;
      continue;
    }
    *move_l = *move_r;    
    move_l++;  

    while (move_l != move_r)  
    {
      if (target > *move_l)  
      {
        move_l++;  
        continue;
      }    
      else  break;
    }
    
    if (move_l == move_r) break;  
    else  
    {
      *move_r = *move_l;    
      move_r--;
    }
  }  

  // 到此的条件应该是 move_l 和 move_r 相等了  
  *move_l = target;    
  
  if (move_l != l)
  {
    SortOnce(l, move_l - 1);  
  }  
  if (move_l != r)  
  {
    SortOnce(move_l + 1, r);  
  }
}

// 听了施磊老师的演示后,自己总结思路  
// 递归操作,每次根据中间目标应该放的位置来切割开,把两边递归调用函数   
// 先验条件:1.达到从小到大序目的; 2.选取的基准数默认是l位置的数  
// 从左到右找第一个大于基准数的数,找到该位置后标记下,表示该数要挪到右边,该位置可以被侵占  
// 从上一条就知道:l 和 r 指代可以被侵占的位置;而选取的基准数,认为是 l 上的数,所以 l 的位置就是首次被侵占的位置   
// 在 SortOnce() 里要注意判断:  
//  1.是否是 l 和 r 都相等了,只有一个数,不需要排序了  
//  2.l 和 r 相差一就可以快速判断(只有两个数的嘛)  
// l 和 r 向中间走的过程中终止条件是 l 和 r 相等,或者是找到第一个可以被侵占的位置,可以被挪走的数;该相等位置也就是基准数可以放到的位置  
vector<int> QuickSort(vector<int>& nums)   
{
  int* first = &nums[0];  
  int* last = first + nums.size() - 1;
  SortOnce(first, last);  
  return nums;
}

class TimerCounter
{
private:
  // decltype(std::chrono::system_clock::now()) start;  
  // decltype(std::chrono::system_clock::now()) end;    
  decltype(std::chrono::high_resolution_clock::now()) start;  
  decltype(std::chrono::high_resolution_clock::now()) end;    
  
public:
  TimerCounter()
  {
    // start = std::chrono::system_clock::now();  
    start = std::chrono::high_resolution_clock::now();
  }
  ~TimerCounter()
  {
    end   = std::chrono::system_clock::now();  
    auto duration =  std::chrono::duration_cast<std::chrono::milliseconds>(end - start);  
    std::cout <<  "cost: " 
     << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den 
     << " s" << std::endl;  

    // end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::milli> duration = end - start;
    // std::cout <<  "cost: " 
    // << duration.count()
    // << " s" << std::endl;

  }
};

int main(int argc , char* argvs[])  
{  
  std::string str_cnts = argvs[1];  
  auto cnts = atoi(str_cnts.c_str());  

  srand(time(NULL));
  std::vector<int> vec_in;  
  std::vector<int> vec_in1;  
  
  for (auto i = 0 ; i < cnts; i++)
  {
    auto rand_num = rand() % 1000;
    vec_in.emplace_back(rand_num);  
    vec_in1.emplace_back(rand_num);
  }
  
  {
    QuickSort(vec_in);  
    TimerCounter timer_counter;
  }
  

  std::cout << "insert sort  ================================================== " << std::endl;

  {      
    SortArray(vec_in1);
    TimerCounter timer_counter;
  }

  // for (const auto& each : vec_in)
  // {
  //   std::cout << each << std::endl;
  // }

  // for (const auto& each : vec_in1)
  // {
  //   std::cout << each << std::endl;
  // }
}  