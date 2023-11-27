/*
1424. Diagonal Traverse II
Medium
Topics
Companies
Hint

Given a 2D integer array nums, return all elements of nums in diagonal order as shown in the below images.



Example 1:

Input: nums = [[1,2,3],[4,5,6],[7,8,9]]
Output: [1,4,2,7,5,3,8,6,9]

Example 2:

Input: nums = [[1,2,3,4,5],[6,7],[8],[9,10,11],[12,13,14,15,16]]
Output: [1,6,2,8,7,3,9,4,12,10,5,13,11,14,15,16]



Constraints:

    1 <= nums.length <= 105
    1 <= nums[i].length <= 105
    1 <= sum(nums[i].length) <= 105
    1 <= nums[i][j] <= 105


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 0

class Solution
{
public:
    vector<int> findDiagonalOrder(vector<vector<int>> &nums)
    {
        vector<int> result;
        size_t total_arrays = nums.size();
        size_t max_subarray_size = 0;

        if (total_arrays == 0)
            return result;

        if (total_arrays == 1)
        {
            for (size_t i = 0; i < nums[0].size(); i++)
            {
                result.push_back(nums[0][i]);
            }
            return result;
        }

        for (size_t x = 0; x < total_arrays; x++)
        {
            if (nums[x].size() > max_subarray_size)
                max_subarray_size = nums[x].size();

            for (size_t j = 0; j <= x; j++)
            {
                int i = x - j;
                if (nums[i].size() <= j)
                    continue;

                if (__DEBUG_LOG)
                    cout << "i = " << i << ",j = " << j << "\t\t nums = " << nums[i][j] << endl;
                result.push_back(nums[i][j]);
            }
        }

        for (size_t x = 1; x < max_subarray_size; x++)
        {
            size_t j = x;
            for (size_t i = total_arrays - 1;; i--, j++)
            {
                if (nums[i].size() <= j)
                {
                    if (i == 0)
                        break;
                    continue;
                }

                if (__DEBUG_LOG)
                    cout << "i = " << i << ",j = " << j << "\t\t nums = " << nums[i][j] << endl;
                result.push_back(nums[i][j]);

                if (i == 0)
                    break;
            }
        }

        return result;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    auto expected = vector<int>({1, 4, 2, 7, 5, 3, 8, 6, 9});

    auto result = s.findDiagonalOrder(nums);

    for (size_t i = 0; i < expected.size(); i++)
    {
        cout << result[i] << " " << expected[i] << endl;
        assert(result[i] == expected[i]);
    }
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{1, 2, 3, 4, 5}, {6, 7}, {8}, {9, 10, 11}, {12, 13, 14, 15, 16}});
    auto expected = vector<int>({1, 6, 2, 8, 7, 3, 9, 4, 12, 10, 5, 13, 11, 14, 15, 16});

    auto result = s.findDiagonalOrder(nums);

    for (size_t i = 0; i < expected.size(); i++)
    {
        cout << result[i] << " " << expected[i] << endl;
        assert(result[i] == expected[i]);
    }
}

void test3(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{1, 2, 3, 4, 5}, {8}});
    auto expected = vector<int>({1, 8, 2, 3, 4, 5});

    auto result = s.findDiagonalOrder(nums);

    for (size_t i = 0; i < expected.size(); i++)
    {
        cout << result[i] << " " << expected[i] << endl;
        assert(result[i] == expected[i]);
    }
}

int main()
{

    cout << "1424. Diagonal Traverse II" << endl;
    test1(1);
    test2(2);
    test3(3);

    return 0;
}