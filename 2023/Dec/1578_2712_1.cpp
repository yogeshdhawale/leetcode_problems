
/*
1578. Minimum Time to Make Rope Colorful
Medium
Topics
Companies
Hint

Alice has n balloons arranged on a rope. You are given a 0-indexed string colors where colors[i] is the color of the ith balloon.

Alice wants the rope to be colorful. She does not want two consecutive balloons to be of the same color, so she asks Bob for help. Bob can remove some balloons from the rope to make it colorful. You are given a 0-indexed integer array neededTime where neededTime[i] is the time (in seconds) that Bob needs to remove the ith balloon from the rope.

Return the minimum time Bob needs to make the rope colorful.



Example 1:

Input: colors = "abaac", neededTime = [1,2,3,4,5]
Output: 3
Explanation: In the above image, 'a' is blue, 'b' is red, and 'c' is green.
Bob can remove the blue balloon at index 2. This takes 3 seconds.
There are no longer two consecutive balloons of the same color. Total time = 3.

Example 2:

Input: colors = "abc", neededTime = [1,2,3]
Output: 0
Explanation: The rope is already colorful. Bob does not need to remove any balloons from the rope.

Example 3:

Input: colors = "aabaa", neededTime = [1,2,3,4,1]
Output: 2
Explanation: Bob will remove the ballons at indices 0 and 4. Each ballon takes 1 second to remove.
There are no longer two consecutive balloons of the same color. Total time = 1 + 1 = 2.



Constraints:

    n == colors.length == neededTime.length
    1 <= n <= 105
    1 <= neededTime[i] <= 104
    colors contains only lowercase English letters.


*/

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define __DEBUG_LOG 0

void Log(string msg)
{
    if (__DEBUG_LOG)
    {
        cout << msg << endl;
    }
}

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
public:
    int minCost(string colors, vector<int> &neededTime)
    {
        int len = neededTime.size();
        int result = 0;
        int maxValue = 0;

        for (int i = 1; i < len; i++)
        {
            if (colors[i] == colors[i - 1])
            {
                if (maxValue == 0)
                {
                    Log("Started dups for color " + string(1, colors[i - 1]) + " at " + to_string(i - 1));

                    result += neededTime[i - 1];
                    maxValue = neededTime[i - 1];
                }
                result += neededTime[i];
                maxValue = max(neededTime[i], maxValue);
                continue;
            }
            if (maxValue > 0)
            {
                Log("Ended dups for color " + string(1, colors[i - 1]) + " at " + to_string(i - 1) + " with " + to_string(maxValue));
                result -= maxValue;
                maxValue = 0;
            }
        }
        if (maxValue > 0)
        {
            result -= maxValue;
            maxValue = 0;
        }

        return result;
    }
};

void test1(int testID)
{

    cout << "test" << testID << endl;

    Solution s;
    vector<string> colors = {"abaac", "abc", "aabaa"};
    vector<vector<int>> neededTime = {{1, 2, 3, 4, 5}, {1, 2, 3}, {1, 2, 3, 4, 1}};
    vector<int> expected = {3, 0, 2};

    for (size_t i = 0; i < colors.size(); i++)
    // for (size_t i = colors.size() - 1; i < colors.size(); i++)
    {
        cout << colors[i] << " ";
        printVector(neededTime[i]);
        cout << " " << endl;

        auto result = s.minCost(colors[i], neededTime[i]);

        cout << result << " " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

int main()
{
    cout << "Hello world" << endl;

    test1(1);
    return 0;
}