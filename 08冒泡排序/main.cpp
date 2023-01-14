// 08_环形队列.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstring>  
#include <time.h>  
#include <queue>
#include <cassert>  
#include <vector>  

using namespace std;

// 听完施磊老师讲冒泡排序,完了自己来上手练习 
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) 
    {
        auto size = nums.size();  
        for (auto i = size - 1; i > 0; i--)
        {
            bool flag{false};  

            for (auto j = 0; j < i; j++) 
            {
                if (nums[j] > nums[ j + 1 ])  
                {
                    auto tmp = nums[j + 1];  
                    nums[j + 1] = nums[j];  
                    nums[j] = tmp;  
                    flag = true;
                }                
            }  

            if (!flag)  return nums;

        }
        return nums;
    }      
};  
  

int main()  
{  
  srand(time(NULL));
  std::vector<int> vec_in;  
  for (auto i = 0 ; i < 1000000; i++)
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

