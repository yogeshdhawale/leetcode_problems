
/*


2966. Divide Array Into Arrays With Max Difference
Medium
Topics
Companies
Hint

You are given an integer array nums of size n and a positive integer k.

Divide the array into one or more arrays of size 3 satisfying the following conditions:

    Each element of nums should be in exactly one array.
    The difference between any two elements in one array is less than or equal to k.

Return a 2D array containing all the arrays. If it is impossible to satisfy the conditions, return an empty array. And if there are multiple answers, return any of them.



Example 1:

Input: nums = [1,3,4,8,7,9,3,5,1], k = 2
Output: [[1,1,3],[3,4,5],[7,8,9]]
Explanation: We can divide the array into the following arrays: [1,1,3], [3,4,5] and [7,8,9].
The difference between any two elements in each array is less than or equal to 2.
Note that the order of elements is not important.

Example 2:

Input: nums = [1,3,3,2,7,3], k = 3
Output: []
Explanation: It is not possible to divide the array satisfying all the conditions.



Constraints:

    n == nums.length
    1 <= n <= 105
    n is a multiple of 3.
    1 <= nums[i] <= 10^5
    1 <= k <= 10^5

*/

#include <iostream>
#include <cassert>
#include <chrono>
#include <string_view>
#include <algorithm>
#include <vector>
#include <map>
#include <numeric>

using namespace std;

#define LOGDATE __TIMESTAMP__

#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2

#define __DEBUG_LOG 0
#define LOG_MSG(msg) \
    clog << "[" << LOGDATE << "]\t" << msg << endl

#ifdef __DEBUG_LOG
#define LOG(x)                         \
    if (__DEBUG_LOG != __DEBUG_LOG_NO) \
    LOG_MSG(x)
#define LOGV(x)                        \
    if (__DEBUG_LOG >= __DEBUG_LOG_VB) \
    LOG_MSG(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

template <typename T>
ostream &operator<<(ostream &out, const vector<T> &nums)
{
    size_t size = nums.size();
    if (nums.size() > 20)
        size = 20;
    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << ", ";
    }
    if (nums.size() > 20)
    {
        out << "...";
    }
    return out;
}

class Solution
{
    map<int, int> freq;

public:
    vector<vector<int>> divideArray(vector<int> &nums, int k, u_int32_t rsize = 3)
    {
        freq.clear();
        vector<vector<int>> result;

        for (auto num : nums)
            freq[num]++;

        int idx = 0;
        int remain = 0;
        for (auto it = freq.begin(); it != freq.end(); it++)
        {
            int val = it->first;
            int cnt = it->second;
            LOG("val: " << val << ", cnt: " << cnt << ", idx: " << idx);

            if (remain)
            {
                if (val - result[idx][0] > k)
                {
                    return vector<vector<int>>();
                }
                if (cnt >= remain)
                {
                    fill_n(back_inserter(result[idx]), remain, val);
                    idx++;
                    cnt -= remain;
                    remain = 0;
                }
                else
                {
                    fill_n(back_inserter(result[idx]), cnt, val);
                    remain -= cnt;
                    continue;
                }
            }

            while (cnt / rsize > 0)
            {
                result.push_back(vector<int>(rsize, val));
                idx++;
                cnt -= rsize;
            }
            if (cnt == 0)
                continue;

            result.push_back(vector<int>(cnt, val));
            remain = rsize - cnt;
        }

        return result;
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<pair<vector<int>, int>>(
        {
            make_pair<vector<int>, int>({1, 3, 4, 8, 7, 9, 3, 5, 1}, 2),
            make_pair<vector<int>, int>({1, 3, 3, 2, 7, 3}, 3),
        });

    auto expected = vector<vector<vector<int>>>({
        {{1, 1, 3}, {3, 4, 5}, {7, 8, 9}},
        {},
    });

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i].first << " k: " << nums[i].second << endl;
        Solution s;
        auto result = s.divideArray(nums[i].first, nums[i].second);
        for (size_t j = 0; j < expected[i].size(); j++)
        {
            assert(result[j].size() == expected[i][j].size());
            if (result[j].size() != 0)
            {
                cout << "result: " << result[j] << " vs: " << expected[i][j] << endl;
                assert(result[j] == expected[i][j]);
            }
        }
    }
}

void test2(int testID)
{
    cout << "test" << testID << endl;
    auto nums = pair<vector<int>, int>();
    auto expected = vector<array<int, 3>>();
    cout << "test for: " << nums.first << " k: " << nums.second << endl;

    Solution s;
    auto result = s.divideArray(nums.first, nums.second);
    // cout << result << " vs " << expected << endl;
    // assert(result == expected);
}

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG > __DEBUG_LOG_VB
    test2(2);
#endif
    // test3(3);

    return 0;
}
