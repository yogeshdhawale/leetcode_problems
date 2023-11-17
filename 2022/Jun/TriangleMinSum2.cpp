
/*

120. Triangle
Medium

Given a triangle array, return the minimum path sum from top to bottom.

For each step, you may move to an adjacent number of the row below. More formally, if you are on index i on the current row, you may move to either index i or index i + 1 on the next row.



Example 1:

Input: triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
Output: 11
Explanation: The triangle looks like:
   2
  3 4
 6 5 7
4 1 8 3
The minimum path sum from top to bottom is 2 + 3 + 5 + 1 = 11 (underlined above).

Example 2:

Input: triangle = [[-10]]
Output: -10

Constraints:

    1 <= triangle.length <= 200
    triangle[0].length == 1
    triangle[i].length == triangle[i - 1].length + 1
    -104 <= triangle[i][j] <= 104


Follow up: Could you do this using only O(n) extra space, where n is the total number of rows in the triangle?
Accepted
451,015


 */

// *** Dynamic programming
#include <iostream>
#include <vector>

using namespace std;

class Tree
{
private:
    vector<vector<int>> &data;
    int size;

public:
    Tree(vector<vector<int>> &triangle) : data(triangle), size(triangle.size())
    {
    }
    int getMinSum()
    {
        return helper(0, 0);
    }

    int helper(int i, int j)
    {
        if (i >= size)
            return 0;

        int left = helper(i+1, j);
        int right = helper(i+1, j+1);
        return data[i][j] + min(left, right);
    }
};

class Solution
{
public:
    int minimumTotal(vector<vector<int>> &triangle)
    {
        Tree t(triangle);
        return t.getMinSum();
    }
};

int main()
{
    if (true)
    {
        Solution s;

        vector<vector<int>> data =
            {{2},
             {3, 4},
             {6, 5, 7},
             {4, 1, 8, 3}};

        cout << "Test for:(10, 3)" << endl;
        int result = s.minimumTotal(data);
        cout << "Result is (expected=11): " << result << endl;
    }
}