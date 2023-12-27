/*

1155. Number of Dice Rolls With Target Sum
Medium
Topics
Companies
Hint

You have n dice, and each die has k faces numbered from 1 to k.

Given three integers n, k, and target, return the number of possible ways (out of the kn total ways) to roll the dice, so the sum of the face-up numbers equals target. Since the answer may be too large, return it modulo 109 + 7.



Example 1:

Input: n = 1, k = 6, target = 3
Output: 1
Explanation: You throw one die with 6 faces.
There is only one way to get a sum of 3.

Example 2:

Input: n = 2, k = 6, target = 7
Output: 6
Explanation: You throw two dice, each with 6 faces.
There are 6 ways to get a sum of 7: 1+6, 2+5, 3+4, 4+3, 5+2, 6+1.

Example 3:

Input: n = 30, k = 30, target = 500
Output: 222616187
Explanation: The answer must be returned modulo 109 + 7.



Constraints:

    1 <= n, k <= 30
    1 <= target <= 1000
*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <chrono>
using namespace std;

#define __DEBUG_LOG 0

void printVector(vector<int> &nums, string msg = "")
{
    if (!__DEBUG_LOG)
        return;

    if (!msg.empty())
    {
        cout << msg << ": ";
    }
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }
    cout << endl;
}

class Solution
{

public:
    Solution()
    {
    }
    int numRollsToTarget(int n, int k, int target)
    {

        return roll(n, k, target);
    }

    int roll(int n, int k, int target)
    {
        if (target <= 0 || n <= 0)
            return 0;
        if (target > n * k || target < n)
            return 0;

        vector<int> prev(target + 1, 0);
        vector<int> curr;
        prev[0] = 1;

        for (int i = 1; i <= n; i++)
        {
            curr.clear();
            curr.resize(target + 1, 0);

            for (int j = i; j <= target && j <= (i * k); j++)
            {
                for (int x = 1; x <= k && x <= j; x++)
                {
                    curr[j] = (curr[j] + prev[j - x]) % 1000000007;
                }
                // cout << "dp[" << i << "][" << j << "] = " << dp[i][j] << endl;
            }

            if (__DEBUG_LOG)
            {
                cout << endl;
                printVector(prev, "prev:" + to_string(i - 1));
                printVector(curr, "curr:" + to_string(i));
            }
            prev.clear();
            prev = curr;
        }

        return curr[target];
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({1, 6, 3});

    auto expected = 1;

    auto result = s.numRollsToTarget(nums[0], nums[1], nums[2]);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({2, 6, 7});

    auto expected = 6;

    /*
        d1  d2  sum
        1   6   1
        2   5   1
        3   4   1
        4   3   1
        5   2   1
        6   1   1

    */

    auto result = s.numRollsToTarget(nums[0], nums[1], nums[2]);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test3(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({3, 6, 10});
    auto expected = 27;

    auto result = s.numRollsToTarget(nums[0], nums[1], nums[2]);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test4(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({30, 30, 500});

    auto expected = 222616187;

    auto start = std::chrono::steady_clock::now();
    auto result = s.numRollsToTarget(nums[0], nums[1], nums[2]);
    auto end = std::chrono::steady_clock::now();
    cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " miliseconds" << endl;
    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    test2(2);
    test3(3);
    test4(4);

    return 0;
}