//

#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>  

using namespace std;

// 听完施磊老师讲选择排序,完了自己来上手练习 
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) 
    {
        auto size = nums.size();    
        for (auto i = 0; i < (size - 1); i++)
        {
          auto min{nums[i]};
          for (auto j = i + 1; j < size; j++)  
          {
            if (nums[j] < min)  
            {
              auto tmp = nums[j];
              nums[j] = min;               
              nums[i] = tmp;
            }
          }
        }
        return nums;
    }      
};  
  

int main()  
{  
  srand(time(NULL));
  std::vector<int> vec_in;  
  for (auto i = 0 ; i < 1000; i++)
  {
    vec_in.emplace_back(rand() % 1000);
  }
  Solution solution;  
  solution.sortArray(vec_in);    

  for (const auto& each : vec_in)
  {
    std::cout << each << std::endl;
  }
}  

