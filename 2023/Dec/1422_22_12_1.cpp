/*
1422. Maximum Score After Splitting a String
Easy
Topics
Companies
Hint

Given a string s of zeros and ones, return the maximum score after splitting the string into two non-empty substrings (i.e. left substring and right substring).

The score after splitting a string is the number of zeros in the left substring plus the number of ones in the right substring.



Example 1:

Input: s = "011101"
Output: 5
Explanation:
All possible ways of splitting s into two non-empty substrings are:
left = "0" and right = "11101", score = 1 + 4 = 5
left = "01" and right = "1101", score = 1 + 3 = 4
left = "011" and right = "101", score = 1 + 2 = 3
left = "0111" and right = "01", score = 1 + 1 = 2
left = "01110" and right = "1", score = 2 + 1 = 3

Example 2:

Input: s = "00111"
Output: 5
Explanation: When left = "00" and right = "111", we get the maximum score = 2 + 3 = 5

Example 3:

Input: s = "1111"
Output: 3



Constraints:

    2 <= s.length <= 500
    The string s consists of characters '0' and '1' only.

*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define __DEBUG_LOG 1

void printVector(vector<int> &nums)
{
    if (!__DEBUG_LOG)
        return;

    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }
    cout << endl;
}

class Solution
{
public:
    int maxScore(string s)
    {
        size_t len = s.length();

        if (len < 2)
        {
            return 0;
        }
        if (len == 2)
        {
            int result = 0;
            if (s[0] == '0')
                result++;
            if (s[1] == '1')
                result++;
            return result;
        }

        vector<int> count0(len, 0);
        vector<int> count1(len, 0);
        int zeros = 0;
        int ones = 0;

        size_t j = len - 1;
        for (size_t i = 0; i < len; i++, j--)
        {
            if (s[i] == '0')
                zeros++;
            if (s[j] == '1')
                ones++;

            count0[i] = zeros;
            count1[j] = ones;
        }

        if (zeros == 0 || ones == 0)
            return len - 1;

        int maxSum = 0;
        for (size_t i = 1; i < len - 1; i++)
        {
            maxSum = max(maxSum, count0[i] + count1[i]);
            cout << count0[i] << " " << count1[i] << " -> " << maxSum << endl;
        }
        return maxSum;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    vector<string> nums = {"011101", "11100", "01"};

    vector<int> expected = {5, 2, 2};

    for (size_t i = 0; i < nums.size(); i++)
    {
        auto result = s.maxScore(nums[i]);

        cout << result << " " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    // test2(2);
    // test3(3);

    return 0;
}