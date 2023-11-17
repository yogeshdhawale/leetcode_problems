
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

// *** optimal inplace
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
        for (int i = 1; i < size; i++)
        {
            for (int j = 0; j < data[i].size(); j++)
            {
                int upprev = INT_MAX;
                int upnext = INT_MAX;

                if (j != 0)
                    upprev = data[i - 1][j - 1];
                if (j < data[i - 1].size())
                    upnext = data[i - 1][j];

                data[i][j] += min(upprev, upnext);
            }
        }

        int min = data[size-1][0];
        for (int i = 1; i < size; i++)
            if (min > data[size-1][i])
                min = data[size-1][i];

        return min;
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