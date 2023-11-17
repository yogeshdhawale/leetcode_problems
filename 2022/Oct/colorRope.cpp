
/*

1578. Minimum Time to Make Rope Colorful
Medium

1505

54

Add to List

Share
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
#include <vector>

using namespace std;

class Solution
{
public:
    int minCost(string colors, vector<int> &neededTime)
    {
        int size = neededTime.size();
        vector<bool> removeB(size, false);

        for (int i = 0; i < size; i++)
        {
            int j = i;
            int maxId = j;
            for (; j < size - 1 && (colors[j] == colors[j + 1]); j++)
            {
                if (neededTime[maxId] < neededTime[j + 1])
                    maxId = j + 1;
            }

            for (int k = i; k <= j; k++)
            {
                if (k != maxId)
                    removeB[k] = true;
            }
            i = j;
        }

        int result = 0;
        for (int i = 0; i < size; i++)
            if (removeB[i])
            {
                //cout << i << ", ";
                result += neededTime[i];
            }
        //cout << endl;

        return result;
    }
};

int main()
{
    if (false)
    {
        cout << "Test 1:" << endl;
        Solution s;

        vector<int> data = {1, 2, 3, 4, 5};
        int result = s.minCost("abaac", data);
        cout << "Result is (expected=3):" << result << endl;
    }
    if (false)
    {
        cout << "Test 2:" << endl;
        Solution s;

        vector<int> data = {1, 2, 3};
        int result = s.minCost("abc", data);
        cout << "Result is (expected=0):" << result << endl;
    }
    if (false)
    {
        cout << "Test 3:" << endl;
        Solution s;

        vector<int> data = {1, 2, 3, 4, 1};
        int result = s.minCost("aabaa", data);
        cout << "Result is (expected=2):" << result << endl;
    }
    // if (false)
    {
        cout << "Test 3:" << endl;
        Solution s;

        vector<int> data = {4, 9, 3, 8, 8, 9};
        int result = s.minCost("bbbaaa", data);
        cout << "Result is (expected=23):" << result << endl;
    }
}