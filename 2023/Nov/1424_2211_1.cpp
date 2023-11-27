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
        map<int, vector<int>> m;
        for (size_t i = 0; i < nums.size(); i++)
        {
            for (size_t j = 0; j < nums[i].size(); j++)
            {
                m[i + j].push_back(nums[i][j]);
            }
        }
        vector<int> result;
        auto itr = m.begin();
        for (; itr != m.end(); ++itr)
        {
            copy(itr->second.rbegin(), itr->second.rend(), back_inserter(result));
        }

        return result;
    };
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