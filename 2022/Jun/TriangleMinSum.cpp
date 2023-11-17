
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

#include <iostream>
#include <vector>

using namespace std;

class Info
{

public:
    int sum;
    int id;

    Info(int data, int index)
    {
        sum = data;
        id = index;
    }
    void add(int vaule, int index)
    {
        sum += vaule;
        id = index;
    }
};

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
        if (size == 1)
            return data[0][0];

        int totalSize = size * 2;
        vector<int> result(totalSize, 0);

        result[0] = data[0][0];
        int row = 0;
        while (row < (size - 1))
        {
            midCopy(result, row);
            row++;

            int i = 0, k = 0;
            result[k++] += data[row][i];
            for (i = 1; i < data[row].size() - 1; i++)
            {
                result[k++] += data[row][i];
                result[k++] += data[row][i];
            }
            result[k++] += data[row][i];
        }

        int min = result[0];
        for (int i = 1; i < size; i++)
            if (min > result[i])
                min = result[i];

        return min;
    }

    void midCopy(vector<int> &result, int cnt)
    {
        int totalSize = result.size();

        if (cnt == 0)
        {
            result[1] = result[0];
            return;
        }

        int lcnt = (totalSize < cnt*2) ? totalSize : cnt*2;
        vector<int> temp(totalSize, 0);
        int i = 0, k = 0;
        //temp[k++] = result[i];
        for (i = 0; i < lcnt; i++)
        {
            temp[k++] = result[i];
            temp[k++] = result[i];
        }
        //temp[k++] = result[i];

        for (int i = 0; i < totalSize; i++)
            result[i] = temp[i];
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