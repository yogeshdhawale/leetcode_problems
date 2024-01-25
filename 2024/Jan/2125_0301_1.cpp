/*
2125. Number of Laser Beams in a Bank
Medium
Topics
Companies
Hint

Anti-theft security devices are activated inside a bank. You are given a 0-indexed binary string array bank representing the floor plan of the bank, which is an m x n 2D matrix. bank[i] represents the ith row, consisting of '0's and '1's. '0' means the cell is empty, while'1' means the cell has a security device.

There is one laser beam between any two security devices if both conditions are met:

    The two devices are located on two different rows: r1 and r2, where r1 < r2.
    For each row i where r1 < i < r2, there are no security devices in the ith row.

Laser beams are independent, i.e., one beam does not interfere nor join with another.

Return the total number of laser beams in the bank.



Example 1:

Input: bank = ["011001","000000","010100","001000"]
Output: 8
Explanation: Between each of the following device pairs, there is one beam. In total, there are 8 beams:
 * bank[0][1] -- bank[2][1]
 * bank[0][1] -- bank[2][3]
 * bank[0][2] -- bank[2][1]
 * bank[0][2] -- bank[2][3]
 * bank[0][5] -- bank[2][1]
 * bank[0][5] -- bank[2][3]
 * bank[2][1] -- bank[3][2]
 * bank[2][3] -- bank[3][2]
Note that there is no beam between any device on the 0th row with any on the 3rd row.
This is because the 2nd row contains security devices, which breaks the second condition.

Example 2:

Input: bank = ["000","111","000"]
Output: 0
Explanation: There does not exist two devices located on two different rows.



Constraints:

    m == bank.length
    n == bank[i].length
    1 <= m, n <= 500
    bank[i][j] is either '0' or '1'.


*/

#include <iostream>
#include <vector>
#include <cassert>

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

class Solution
{
public:
    int numberOfBeams(vector<string> &bank)
    {
        int prevCount = 0;
        int curCount = 0;
        int result = 0;
        int size = bank[0].size();
        for (size_t i = 0; i < bank.size(); i++)
        {
            curCount = getCount(bank[i], size);
            LOG("count = " + to_string(i) + ":" + to_string(curCount));
            if (curCount == 0)
                continue;

            if (prevCount > 0)
            {
                result += prevCount * curCount;
                LOG("result = " + to_string(result));
            }
            prevCount = curCount;
        }
        return result;
    }

    inline int getCount(const string &bank, const size_t &size)
    {
        int count = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (bank[i] == '1')
            {
                count++;
            }
        }
        return count;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<string>>({
        {"011001", "000000", "010100", "001000"},
        {"000", "111", "000"},
    });

    auto expected = vector<int>({8, 0});

    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << "test for: " << endl;
        printVector(nums[i]);
        auto result = s.numberOfBeams(nums[i]);
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