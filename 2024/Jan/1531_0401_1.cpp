/*

1531. String Compression II
Hard
Topics
Companies
Hint

Run-length encoding is a string compression method that works by replacing consecutive identical characters (repeated 2 or more times) with the concatenation of the character and the number marking the count of the characters (length of the run). For example, to compress the string "aabccc" we replace "aa" by "a2" and replace "ccc" by "c3". Thus the compressed string becomes "a2bc3".

Notice that in this problem, we are not adding '1' after single characters.

Given a string s and an integer k. You need to delete at most k characters from s such that the run-length encoded version of s has minimum length.

Find the minimum length of the run-length encoded version of s after deleting at most k characters.



Example 1:

Input: s = "aaabcccd", k = 2
Output: 4
Explanation: Compressing s without deleting anything will give us "a3bc3d" of length 6. Deleting any of the characters 'a' or 'c' would at most decrease the length of the compressed string to 5, for instance delete 2 'a' then we will have s = "abcccd" which compressed is abc3d. Therefore, the optimal way is to delete 'b' and 'd', then the compressed version of s will be "a3c3" of length 4.

Example 2:

Input: s = "aabbaa", k = 2
Output: 2
Explanation: If we delete both 'b' characters, the resulting compressed string would be "a4" of length 2.

Example 3:

Input: s = "aaaaaaaaaaa", k = 0
Output: 3
Explanation: Since k is zero, we cannot delete anything. The compressed string is "a11" of length 3.



Constraints:

    1 <= s.length <= 100
    0 <= k <= s.length
    s contains only lowercase English letters.



*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>

using namespace std;

#define __DEBUG_LOG 0

template <typename T>
void printVector(vector<T> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << ", ";
    }
    cout << endl;
}

void LOG(string msg)
{
    if (__DEBUG_LOG)
        cout << msg << endl;
}

int getLen(char c, int count = 1)
{
    int len = 1;
    if (count > 1)
        len += to_string(count).length();
    return len;
}

struct keyHash;

class Data
{
public:
    int k;
    int i;
    char prevChar;
    int count;
    Data(int k, int i, char pc, int count) : k(k), i(i), prevChar(pc), count(count)
    {
    }
    bool operator==(const Data &other) const
    {
        return k == other.k && i == other.i && count == other.count && prevChar == other.prevChar;
    }
};
ostream &operator<<(ostream &os, const Data &d)
{
    os << d.k << "," << d.i << "," << d.count << "," << d.prevChar;
    return os;
}

struct keyHash
{
    std::size_t operator()(const Data &d) const
    {
        return std::hash<int>{}(d.k) ^ (std::hash<char>{}(d.i)) ^ (std::hash<int>{}(d.count)) ^ (std::hash<char>{}(d.prevChar));
    }
};

class Solution
{
private:
    unordered_map<Data, int, keyHash> dp;

public:
    Solution()
    {
        dp.clear();
    }
    int getCount(string &s, int start, int end, int k, int prevCharCnt, char prevChar)
    {

        if (k < 0)
            return INT32_MAX;
        if (start > end)
            return 0;

        Data obj(k, start, prevChar, prevCharCnt);
        int retVal = 0;
        if (dp.find(obj) != dp.end())
        {
            return dp[obj];
        }
        auto v1 = INT32_MAX;
        auto v2 = INT32_MAX;
        if (prevChar != s[start])
        {
            // delete char
            auto v1 = getCount(s, start + 1, end, k - 1, prevCharCnt, prevChar);
            // keep char
            auto v2 = getCount(s, start + 1, end, k, 1, s[start]);
            if (v2 != INT32_MAX)
                v2 += 1;
            retVal = min(v1, v2);
        }
        else
        {
            v2 = getCount(s, start + 1, end, k, prevCharCnt + 1, prevChar);
            if (v2 != INT32_MAX && (prevCharCnt == 1 || prevCharCnt == 9 || prevCharCnt == 99))
            {
                v2 += 1;
            }

            retVal = v2;
        }

        if (__DEBUG_LOG)
        {
            cout << "start:" << start << " end:" << end << " k:" << k << " prevCharCnt:" << prevCharCnt << " prevChar:" << prevChar << " v1:" << v1 << " v2:" << v2 << " retVal:" << retVal << endl;
        }
        dp[obj] = retVal;
        return retVal;
    }
    int getLengthOfOptimalCompression(string s, int k)
    {
        auto size = s.length();

        if ((size_t)k == size)
            return 0;

        dp.clear();

        return getCount(s, 0, size - 1, k, 0, '*');
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<pair<string, int>>(
        {{"a", 1},
         {"abc", 1},
         {"aaabcccd", 2},
         {"aabbaa", 2},
         {"aaaaaaaaaaa", 0},
         {"aabaabbcbbbaccc", 6},
         {"bbabbbabbbbcbb", 4}});

    auto expected = vector<int>({0, 2, 4, 2, 3, 4, 3});

    // for (size_t i = 0; i < nums.size(); i++)
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
    {
        Solution s;
        cout << "test " << i + 1 << "-" << nums[i].first << " " << nums[i].second << endl;
        auto result = s.getLengthOfOptimalCompression(nums[i].first, nums[i].second);
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