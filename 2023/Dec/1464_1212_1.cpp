/*
1464. Maximum Product of Two Elements in an Array
Easy
Topics
Companies
Hint
Given the array of integers nums, you will choose two different indices i and j of that array. Return the maximum value of (nums[i]-1)*(nums[j]-1).



Example 1:

Input: nums = [3,4,5,2]
Output: 12
Explanation: If you choose the indices i=1 and j=2 (indexed from 0), you will get the maximum value, that is, (nums[1]-1)*(nums[2]-1) = (4-1)*(5-1) = 3*4 = 12.

Example 2:

Input: nums = [1,5,4,5]
Output: 16
Explanation: Choosing the indices i=1 and j=3 (indexed from 0), you will get the maximum value of (5-1)*(5-1) = 16.

Example 3:

Input: nums = [3,7]
Output: 12



Constraints:

    2 <= nums.length <= 500
    1 <= nums[i] <= 10^3


*/

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class Solution
{
public:
    int maxProduct(vector<int> &nums)
    {
        int first = nums[0];
        int second = nums[1];
        if (first < second)
        {
            swap(first, second);
        }

        if (nums.size() == 2)
        {
            return (first - 1) * (second - 1);
        }

        for (size_t i = 2; i < nums.size(); i++)
        {
            if (nums[i] > first)
            {
                second = first;
                first = nums[i];
            }
            else if (nums[i] > second)
            {
                second = nums[i];
            }
        }

        return (first - 1) * (second - 1);
    }
};

void test1(int testID)
{

    Solution s;
    cout << "test" << testID << endl;

    auto nums = vector<int>({3, 4, 5, 2});
    auto expected = 12;

    auto result = s.maxProduct(nums);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    // test2(2);
    //  test3(3);

    return 0;
}