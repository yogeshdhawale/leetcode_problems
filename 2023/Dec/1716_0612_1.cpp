/*
1716. Calculate Money in Leetcode Bank
Easy
Topics
Companies
Hint

Hercy wants to save money for his first car. He puts money in the Leetcode bank every day.

He starts by putting in $1 on Monday, the first day. Every day from Tuesday to Sunday, he will put in $1 more than the day before. On every subsequent Monday, he will put in $1 more than the previous Monday.

Given n, return the total amount of money he will have in the Leetcode bank at the end of the nth day.



Example 1:

Input: n = 4
Output: 10
Explanation: After the 4th day, the total is 1 + 2 + 3 + 4 = 10.

Example 2:

Input: n = 10
Output: 37
Explanation: After the 10th day, the total is (1 + 2 + 3 + 4 + 5 + 6 + 7) + (2 + 3 + 4) = 37. Notice that on the 2nd Monday, Hercy only puts in $2.

Example 3:

Input: n = 20
Output: 96
Explanation: After the 20th day, the total is (1 + 2 + 3 + 4 + 5 + 6 + 7) + (2 + 3 + 4 + 5 + 6 + 7 + 8) + (3 + 4 + 5 + 6 + 7 + 8) = 96.



Constraints:

    1 <= n <= 1000



*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define __DEBUG_LOG 0

class Solution
{
public:
    int totalMoney(int n)
    {
        // is there better way to declare firstWeek?
        

        static int firstWeek = 7 * (7 + 1) / 2;

        int totalWeeks = n / 7;
        int days = n % 7;

        if (totalWeeks == 0)
        {
            return days * (days + 1) / 2;
        }

        int result = firstWeek * totalWeeks;
        result += (totalWeeks * (totalWeeks - 1) / 2) * 7;
        result += totalWeeks * days;
        result += (days * (days + 1) / 2);
        return result;
    }
};

void test1(int testID)
{

    Solution s;
    cout << "test" << testID << endl;
    auto n = 4;
    auto expected = 10;
    auto result = s.totalMoney(n);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{

    Solution s;
    cout << "test" << testID << endl;
    auto n = 10;
    auto expected = 37;
    auto result = s.totalMoney(n);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test3(int testID)
{

    Solution s;
    cout << "test" << testID << endl;
    auto n = 20;
    auto expected = 96;
    auto result = s.totalMoney(n);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test4(int testID)
{

    Solution s;
    cout << "test" << testID << endl;
    auto n = 26;
    auto expected = 135;
    auto result = s.totalMoney(n);
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
}
