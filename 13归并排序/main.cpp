#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>    
#include <chrono>    
#include <algorithm>

using namespace std;  

int total_num{0};

/*
* 23/1/8 看了施磊老师的思路,就来实践下  
* 思路:递时直到 l 和 r 相等,即说明没有数据需要分割了  
*       归时调整数顺序, l 和 r 递增时有限值,不要越界了    
*/  

// 归并的拼音命名  
void GuiBing(const int* l, const int* r)
{  
  // 要点:1. l 和 r 相等即递到底了
  if (l == r) return;
  auto sub = r - l;  
  auto mid  = sub / 2;    
  auto mid_offset = l + mid; 

  GuiBing(l, mid_offset);    
  GuiBing(mid_offset + 1, r);    

  // 要点:2. 将左右两部分的数进行排序   
  auto move_l = l;  
  auto move_r = mid_offset + 1;
  std::vector<int> vec_int_tmp;        
  // 纠错:之前写成这种造成出错,外层的逻辑循环判断不对
  // while ((move_l <= mid_offset) && (move_r <= r)) 
  while ((move_l <= mid_offset) || (move_r <= r)) 
  {
    int less;  

    // 两层嵌套 if else,要解释一下  
    // 第一层判断出两边比较小的数 
    if (*move_l > *move_r)   
    {
      // 来判断两边是否时有越界  
      // 避免越界  
      // 小的数是右边这边的索引上数  
      // 如果右边这个索引已经到头了,说明相对小的数就不是右边索引上  
      // 因为右边索引都不走了,当然最右边那个数就是最小的数  
      // 出现这种情况就要判断,所以在内层又置了一个 if else  

      // 右边索引还没到头  
      if (move_r <= r)  
      {
        less = *move_r;  
        move_r++;
      }
      else  // 右边索引到头了,从小到大的数从左边这部分来取
      {
        less = *move_l;    
        move_l++;
      }
    }
    else  
    {  
      // 左边的数是相对较小的数  
      // 判断左边的索引是否走到头,左边索引走到头,那么这个所谓的最小数就是伪最小数
      if (move_l <= mid_offset)  
      {
        less = *move_l;  
        move_l++;
      }
      else  // 左边索引已经到头了,那么数就从右边取  
      {
        less = *move_r;  
        move_r++;
      }
    }

    vec_int_tmp.emplace_back(less);      
  }

  {
    total_num += vec_int_tmp.size();
  }

  int* head = const_cast<int*>(l);
  for (const auto each : vec_int_tmp) 
  {
    *head = each;  
    head++;
  }

}

// 完全照搬施磊老师的思路,能复现也不错  
// TODO zhou    
// 发现是错的,有些数没被排进来
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

int main(int argc , char* argvs[])  
{  
  if (argc != 2) 
  {
    std::cout << "argc is: " << argc << std::endl;
    return 0;
  }
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
    GuiBing(&vec_in[0], &vec_in[vec_in.size() - 1]);  
    TimerCounter timer_counter;
  }
  
  // {      
  //   QuickSort(vec_in1);
  //   TimerCounter timer_counter;
  // }

  // for (const auto& each : vec_in)
  // {
  //   std::cout << each << std::endl;
  // }

  std::cout << " ================================  seaprate std::sort ================================== " << std::endl;

  auto it_vec = vec_in.begin();
  std::sort(vec_in1.begin(), vec_in1.end());
  for (const auto& each : vec_in1)
  {
    // std::cout << each << std::endl;  
    if (*it_vec != each) std::cout << " ************************** has broken ************************* " << std::endl;   
    it_vec++;
  }  

  return 0;
}  