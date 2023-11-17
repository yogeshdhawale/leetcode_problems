/*

1461. Check If a String Contains All Binary Codes of Size K
Medium

Given a binary string s and an integer k, return true if every binary code of length k is a substring of s. Otherwise, return false.



Example 1:

Input: s = "00110110", k = 2
Output: true
Explanation: The binary codes of length 2 are "00", "01", "10" and "11". They can be all found as substrings at indices 0, 1, 3 and 2 respectively.

Example 2:

Input: s = "0110", k = 1
Output: true
Explanation: The binary codes of length 1 are "0" and "1", it is clear that both exist as a substring.

Example 3:

Input: s = "0110", k = 2
Output: false
Explanation: The binary code "00" is of length 2 and does not exist in the array.



Constraints:

    1 <= s.length <= 5 * 105
    s[i] is either '0' or '1'.
    1 <= k <= 20


*/
#include <iostream>
#include <vector>

using namespace std;

class CheckHashCodes
{
    string &data;
    int n;

    int range;
    vector<bool> flags;
    int cnt;

public:
    CheckHashCodes(string &s, int k) : data(s), n(k)
    {
        cnt = 0;
        range = 1 << n;
        flags.resize(range, false);
    }
    bool process()
    {
        int size = data.length();

        if (size < n)
            return false;

        int i = 0;
        int val = 0;

        for (i = 0; i < n - 1; i++)
        {
            if (data[i])
                val += 1;
            val << 1;
        }
        val >> 1;

        for (; i < size; i++)
        {
            if (cnt == range)
                return true;

            val << 1;

            if (val >= range)
                val -= range;

            if (data[i])
                val += 1;

            if (flags[val])
                continue;

            flags[val] = true;
            cnt++;
        }
        return false;
    }
};

class Solution
{
public:
    bool hasAllCodes(string s, int k)
    {
        CheckHashCodes obj(s, k);
        return obj.process();
    }
};

int main()
{
    if (false)
    {
        Solution s;
        bool result = s.hasAllCodes("01110100101", 2);
        cout << "1- Result (expected:1) actual: " << result << endl;
    }

    //if (false)
    {
        Solution s;
        bool result = s.hasAllCodes("00110110", 2);
        cout << "2- Result (expected:1) actual: " << result << endl;
    }
}