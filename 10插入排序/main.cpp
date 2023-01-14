#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>  

using namespace std;  

vector<int> SortArray(vector<int>& nums)   
{
  // 
  auto size = nums.size();
  for (auto i = 0; i < size - 1; i++)
  {
    for (auto j = i + 1; j > 1; j--) 
    {
      if (nums[j] < nums[j-1])
      {
        auto tmp = nums[j];  
        nums[j] = nums[j-1];  
        nums[j-1] = tmp;
      }  
      else break;
    }    
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