/*
2147. Number of Ways to Divide a Long Corridor
Hard
Topics
Companies
Hint

Along a long library corridor, there is a line of seats and decorative plants. You are given a 0-indexed string corridor of length n consisting of letters 'S' and 'P' where each 'S' represents a seat and each 'P' represents a plant.

One room divider has already been installed to the left of index 0, and another to the right of index n - 1. Additional room dividers can be installed. For each position between indices i - 1 and i (1 <= i <= n - 1), at most one divider can be installed.

Divide the corridor into non-overlapping sections, where each section has exactly two seats with any number of plants. There may be multiple ways to perform the division. Two ways are different if there is a position with a room divider installed in the first way but not in the second way.

Return the number of ways to divide the corridor. Since the answer may be very large, return it modulo 109 + 7. If there is no way, return 0.



Example 1:

Input: corridor = "SSPPSPS"
Output: 3
Explanation: There are 3 different ways to divide the corridor.
The black bars in the above image indicate the two room dividers already installed.
Note that in each of the ways, each section has exactly two seats.

Example 2:

Input: corridor = "PPSPSP"
Output: 1
Explanation: There is only 1 way to divide the corridor, by not installing any additional dividers.
Installing any would create some section that does not have exactly two seats.

Example 3:

Input: corridor = "S"
Output: 0
Explanation: There is no way to divide the corridor because there will always be a section that does not have exactly two seats.



Constraints:

    n == corridor.length
    1 <= n <= 105
    corridor[i] is either 'S' or 'P'.




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
    int numberOfWays(string corridor)
    {
        size_t len = corridor.length();
        size_t i = 0;

        unsigned long result = 1;
        int seatCount = 0;
        bool validSeats = false;

        while (i < len)
        {
            if (corridor[i] == 'P')
            {
                i++;
                continue;
            }

            seatCount++;
            i++;
            validSeats = false;

            if (seatCount != 2)
                continue;

            seatCount = 0;
            validSeats = true;

            int val = 1;
            for (; i < len && corridor[i] == 'P'; i++)
            {
                val++;
            }
            if (i != len)
                result = (result * val) % 1000000007;
        }

        return validSeats ? result : 0;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;

    vector<string> corridor = {"S", "P", "SP", "PS", "PP", "SPP", "PPP", "PPSP"};
    auto expected = 0;

    for (auto x : corridor)
    {
        auto result = s.numberOfWays(x);
        cout << result << " " << expected << endl;
        assert(result == expected);
    }
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;

    auto corridor = "SSPPSPS";
    auto expected = 3;

    auto result = s.numberOfWays(corridor);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    test2(2);
    // test3(3);

    return 0;
}