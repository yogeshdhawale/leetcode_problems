/*
264. Largest 3-Same-Digit Number in String
Easy
Topics
Companies
Hint

You are given a string num representing a large integer. An integer is good if it meets the following conditions:

    It is a substring of num with length 3.
    It consists of only one unique digit.

Return the maximum good integer as a string or an empty string "" if no such integer exists.

Note:

    A substring is a contiguous sequence of characters within a string.
    There may be leading zeroes in num or a good integer.



Example 1:

Input: num = "6777133339"
Output: "777"
Explanation: There are two distinct good integers: "777" and "333".
"777" is the largest, so we return "777".

Example 2:

Input: num = "2300019"
Output: "000"
Explanation: "000" is the only good integer.

Example 3:

Input: num = "42352338"
Output: ""
Explanation: No substring of length 3 consists of only one unique digit. Therefore, there are no good integers.



Constraints:

    3 <= num.length <= 1000
    num only consists of digits.


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 1

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
public:
    string largestGoodInteger(string num)
    {
        auto len = num.length();
        auto prev = num[0];
        auto count = 1;
        char result = '0';
        bool resultFound = false;

        for (size_t i = 1; i < len; i++)
        {
            prev = num[i - 1];
            if (num[i] != prev || num[i] < result)
            {
                count = 1;
            }
            else
            {
                count++;
                if (count == 3)
                {
                    result = num[i];
                    resultFound = true;
                    i++;
                    count = 1;
                }
            }
        }

        if (!resultFound)
        {
            return "";
        }
        return string(3, result);
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = string("6777133339");
    auto expected = "777";

    auto result = s.largestGoodInteger(nums);

    cout << result << " vs " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    // test2(2);
    // test3(3);

    return 0;
}