#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>  

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

int main()  
{  
    srand(time(NULL));
    std::vector<int> vec_in;  
    for (auto i = 0 ; i < 10000; i++)
    {
      vec_in.emplace_back(rand() % 1000);
    }
    
    SortArray(vec_in);    

    for (const auto& each : vec_in)
    {
      std::cout << each << std::endl;
    }
}  