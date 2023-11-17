/*
*** NOT working
// TODO

29. Divide Two Integers
Medium

Given two integers dividend and divisor, divide two integers without using multiplication, division, and mod operator.

The integer division should truncate toward zero, which means losing its fractional part. For example, 8.345 would be truncated to 8, and -2.7335 would be truncated to -2.

Return the quotient after dividing dividend by divisor.

Note: Assume we are dealing with an environment that could only store integers within the 32-bit signed integer range: [−231, 231 − 1]. For this problem, if the quotient is strictly greater than 231 - 1, then return 231 - 1, and if the quotient is strictly less than -231, then return -231.

Example 1:

Input: dividend = 10, divisor = 3
Output: 3
Explanation: 10/3 = 3.33333.. which is truncated to 3.

Example 2:

Input: dividend = 7, divisor = -3
Output: -2
Explanation: 7/-3 = -2.33333.. which is truncated to -2.



Constraints:

    -231 <= dividend, divisor <= 231 - 1
    divisor != 0


*/

#include <iostream>
#include <cmath>

using namespace std;

const long max_int = 2147483647;
const long min_int = -2147483647 -1 ;

class Solution
{
public:
    int divide(int above, int below)
    {
        long long dividend = above;
        long long divisor = below;
        bool isNeg = false;
        if (dividend < 0)
            isNeg = !isNeg;
        if (divisor < 0)
            isNeg = !isNeg;

        if (dividend > max_int || dividend <= min_int)
            return isNeg ? min_int : max_int;

        if (divisor > max_int || divisor <= min_int)
            return isNeg ? min_int : max_int;

        if (divisor == 1)
            return dividend;
        if (divisor == -1)
            return -dividend;

        int result = 0;

        if (dividend < 0)
            dividend = -dividend;
        if (divisor < 0)
            divisor = -divisor;

        while (dividend >= divisor)
        {
            result++;
            dividend -= divisor;
        }

        if (isNeg)
            result = -result;
        return result;
    }
};

int main()
{
    if (false)
    {
        Solution s;

        cout << "Test for:(10, 3)" << endl;
        int result = s.divide(10, 3);
        cout << "Result is (expected=3): " << result << endl;
    }
    //if (false)
    {
        Solution s;

        cout << "Test for:(-1, 1)" << endl;
        int result = s.divide(-1, 1);
        cout << "Result is (expected=-1): " << result << endl;
    }
    if (false)
    {
        Solution s;

        cout << "Test for:(7, -3)" << endl;
        int result = s.divide(7, -3);
        cout << "Result is (expected=-2): " << result << endl;
    }
    return 0;
}
