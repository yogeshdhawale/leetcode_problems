/*
1143. Longest Common Subsequence
Medium
Topics
Companies
Hint

Given two strings text1 and text2, return the length of their longest common subsequence. If there is no common subsequence, return 0.

A subsequence of a string is a new string generated from the original string with some characters (can be none) deleted without changing the relative order of the remaining characters.

    For example, "ace" is a subsequence of "abcde".

A common subsequence of two strings is a subsequence that is common to both strings.



Example 1:

Input: text1 = "abcde", text2 = "ace"
Output: 3
Explanation: The longest common subsequence is "ace" and its length is 3.

Example 2:

Input: text1 = "abc", text2 = "abc"
Output: 3
Explanation: The longest common subsequence is "abc" and its length is 3.

Example 3:

Input: text1 = "abc", text2 = "def"
Output: 0
Explanation: There is no such common subsequence, so the result is 0.



Constraints:

    1 <= text1.length, text2.length <= 1000
    text1 and text2 consist of only lowercase English characters.


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <array>

using namespace std;

#define __DEBUG_LOG 0

class Solution
{
public:
    int longestCommonSubsequence(string text1, string text2)
    {
        if (text1.length() == 0 || text2.length() == 0)
        {
            return 0;
        }

        string big = text1;
        string small = text2;
        if (big.length() < small.length())
        {
            big = text2;
            small = text1;
        }
        size_t biglen = big.length();
        size_t smalllen = small.length();

        vector<int> *prev = new vector<int>(smalllen + 1, 0);
        vector<int> *cur = new vector<int>(smalllen + 1, 0);

        for (size_t i = 0; i < biglen; i++)
        {
            swap(prev, cur);
            cur->erase(cur->begin(), cur->end());
            cur->resize(smalllen + 1, 0);
            for (size_t j = 0; j < smalllen; j++)
            {
                if (big[i] == small[j])
                {
                    (*cur)[j + 1] = (*prev)[j] + 1;
                }
                else
                {
                    (*cur)[j + 1] = max((*prev)[j + 1], (*cur)[j]);
                }
            }
        }
        return cur->back();
    }
};

void test1(int id)
{

    auto input = vector<array<string, 3>>({
        {"abcde", "ace", "3"},
        {"abc", "abc", "3"},
        {"abc", "def", "0"},
    });

    for (size_t i = 0; i < input.size(); i++)
    {
        cout << "test id: " << id << " input: loop: " << i << "=" << input[i][0] << " " << input[i][1] << endl;
        auto s = Solution();
        auto result = s.longestCommonSubsequence(input[i][0], input[i][1]);
        cout << "result: " << result << " vs " << input[i][2] << endl;
        assert(result == stoi(input[i][2]));
    }
}
int main()
{
    test1(1);
    return 0;
}