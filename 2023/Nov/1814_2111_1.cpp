/*
1814. Count Nice Pairs in an Array
Medium
Topics
Companies
Hint

You are given an array nums that consists of non-negative integers. Let us define rev(x) as the reverse of the non-negative integer x. For example, rev(123) = 321, and rev(120) = 21. A pair of indices (i, j) is nice if it satisfies all of the following conditions:

    0 <= i < j < nums.length
    nums[i] + rev(nums[j]) == nums[j] + rev(nums[i])

Return the number of nice pairs of indices. Since that number can be too large, return it modulo 109 + 7.



Example 1:

Input: nums = [42,11,1,97]
Output: 2
Explanation: The two pairs are:
 - (0,3) : 42 + rev(97) = 42 + 79 = 121, 97 + rev(42) = 97 + 24 = 121.
 - (1,2) : 11 + rev(1) = 11 + 1 = 12, 1 + rev(11) = 1 + 11 = 12.

Example 2:

Input: nums = [13,10,35,24,76]
Output: 4



Constraints:

    1 <= nums.length <= 105
    0 <= nums[i] <= 109



*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 0

#define MODULE_NUM 1000000007

class Solution
{
public:
    int countNicePairs(vector<int> &nums)
    {
        map<int, int> m;
        for (long unsigned int i = 0; i < nums.size(); i++)
        {
            long unsigned int data = nums[i];
            nums[i] = data - rev(data);
            if (__DEBUG_LOG)
                cout << "nums[" << i << "] = " << data << "\tnewval:" << nums[i] << endl;
            m[nums[i]] += 1;
        }

        long unsigned int result = 0, v1 = 0, v2 = 0;
        for (auto it = m.begin(); it != m.end(); it++)
        {
            int cur_cnt = it->second;
            if (__DEBUG_LOG)
                cout << "result = " << result << "\tvalue:" << it->first << "->" << cur_cnt << endl;

            if (cur_cnt % 2)
            {
                v1 = (cur_cnt) % MODULE_NUM;
                v2 = ((cur_cnt - 1) / 2) % MODULE_NUM;
            }
            else
            {
                v1 = (cur_cnt / 2) % MODULE_NUM;
                v2 = (cur_cnt - 1) % MODULE_NUM;
            }

            result = (result + (v1 * v2)) % MODULE_NUM;
        }
        return result;
    }
    int rev(int n)
    {
        int r = 0;
        while (n)
        {
            r = r * 10;
            r += n % 10;
            n /= 10;
        }
        return r;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({42, 11, 1, 97});
    auto expected = 2;

    auto result = s.countNicePairs(nums);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({13, 10, 35, 24, 76});
    auto expected = 4;

    auto result = s.countNicePairs(nums);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "1814. Count Nice Pairs in an Array" << endl;
    test1(1);
    test2(2);
    // test3(3);

    return 0;
}