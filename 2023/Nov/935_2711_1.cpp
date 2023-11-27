/*
935. Knight Dialer
Medium
Topics
Companies

The chess knight has a unique movement, it may move two squares vertically and one square horizontally, or two squares horizontally and one square vertically (with both forming the shape of an L). The possible movements of chess knight are shown in this diagaram:

A chess knight can move as indicated in the chess diagram below:

We have a chess knight and a phone pad as shown below, the knight can only stand on a numeric cell (i.e. blue cell).

Given an integer n, return how many distinct phone numbers of length n we can dial.

You are allowed to place the knight on any numeric cell initially and then you should perform n - 1 jumps to dial a number of length n. All jumps should be valid knight jumps.

As the answer may be very large, return the answer modulo 109 + 7.



Example 1:

Input: n = 1
Output: 10
Explanation: We need to dial a number of length 1, so placing the knight over any numeric cell of the 10 cells is sufficient.

Example 2:

Input: n = 2
Output: 20
Explanation: All the valid number we can dial are [04, 06, 16, 18, 27, 29, 34, 38, 40, 43, 49, 60, 61, 67, 72, 76, 81, 83, 92, 94]

Example 3:

Input: n = 3131
Output: 136006598
Explanation: Please take care of the mod.



Constraints:

    1 <= n <= 5000


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 1

#define MOD 1000000007

void printVector(vector<int> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }
    cout << endl;
}

class Solution
{
private:
    vector<vector<int>> nextDigits;
    vector<int> nextCount;

public:
    Solution()
    {
        nextDigits.resize(10, vector<int>());
        nextCount.resize(10, 0);

        nextDigits[0] = {4, 6};
        nextDigits[1] = {6, 8};
        nextDigits[2] = {7, 9};
        nextDigits[3] = {4, 8};
        nextDigits[4] = {0, 3, 9};
        nextDigits[5] = {};
        nextDigits[6] = {0, 1, 7};
        nextDigits[7] = {2, 6};
        nextDigits[8] = {1, 3};
        nextDigits[9] = {2, 4};

        for (int i = 0; i < 10; i++)
        {
            nextCount[i] = nextDigits[i].size();
        }
    }

    int knightDialer(int n)
    {
        if (n < 1)
            return 0;
        if (n == 1)
            return 10;

        vector<int> current_counts = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1};

        for (int i = 1; i < n; i++)
        {
            vector<int> newDigits(10, 0);
            for (int j = 0; j < 10; j++)
            {
                for (int move : nextDigits[j])
                {
                    newDigits[move] = (newDigits[move] + current_counts[j]) % MOD;
                }
            }
            current_counts = newDigits;
        }

        int result = 0;
        for (int i = 0; i < 10; i++)
        {
            result = (result + current_counts[i]) % MOD;
        }
        return result;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto input = 1;
    auto expected = 10;

    auto result = s.knightDialer(input);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto input = 2;
    auto expected = 20;

    auto result = s.knightDialer(input);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test3(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto input = 3;
    auto expected = 46;
    // 0 -> 2 -> 3 = 6 -> 040, 043, 049, 060, 061, 067
    // 1 -> 2 -> 1*2 + 1*3 = 5 -> 160, 161, 167, 181, 183
    // 6 + 5 + 4 + 5 + 6 + 0 + 6 + 5 + 4 + 5 = 46
    auto result = s.knightDialer(input);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test4(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto input = 3131;
    auto expected = 136006598;

    auto result = s.knightDialer(input);

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