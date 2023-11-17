/*

// TODO: not completed

378. Kth Smallest Element in a Sorted Matrix
Medium

7389

270

Add to List

Share
Given an n x n matrix where each of the rows and columns is sorted in ascending order, return the kth smallest element in the matrix.

Note that it is the kth smallest element in the sorted order, not the kth distinct element.

You must find a solution with a memory complexity better than O(n^2).



Example 1:

Input: matrix = [[1,5,9],[10,11,13],[12,13,15]], k = 8
Output: 13
Explanation: The elements in the matrix are [1,5,9,10,11,12,13,13,15], and the 8th smallest number is 13
Example 2:

Input: matrix = [[-5]], k = 1
Output: -5


Constraints:

n == matrix.length == matrix[i].length
1 <= n <= 300
-109 <= matrix[i][j] <= 109
All the rows and columns of matrix are guaranteed to be sorted in non-decreasing order.
1 <= k <= n2


Follow up:

Could you solve the problem with a constant memory (i.e., O(1) memory complexity)?
Could you solve the problem in O(n) time complexity? The solution may be too advanced for an interview but you may find reading this paper fun.
http://www.cse.yorku.ca/~andy/pubs/X+Y.pdf

*/

#include <iostream>
#include <vector>

using namespace std;

#define MAX 1000

class Cell
{
public:
    int i;
    int j;

    Cell() : i(-1), j(-1) {}
    Cell(int x, int y) : i(x), j(y) {}
};

class Matrix
{
private:
    vector<vector<int>> &data;

    int row;
    int col;

public:
    Matrix(vector<vector<int>> &m) : data(m), row(m.size()), col(m.size())
    {
    }

    int getVal(Cell &c)
    {
        if(c.i >=row || c.j >= col)
            return MAX;

        return data[c.i][c.j];
    }

    int goDown(Cell & c, Cell pr)
    {
        if (c.i < (row-1))
            c.i++;
        else
        {
            // move to next col
            c.j++;
            c.i = pr.i+1;
        } 
        return 0;
    }

    int goRight(Cell & c, Cell pr)
    {
        if (c.j < (col-1))
            c.j++;
        else
        {
            // move to next row
            c.j = pr.j+1;
            c.i++;
        } 
        return 0;
    }

    int findElement(int sr)
    {
        if (sr > row * col)
            return -1;


        if (sr == 1)
            return data[0][0];
        sr = sr -2;
        Cell down(1, 0);
        Cell right(0, 1);

        while (sr > 0)
        {
            //cout << sr << "::Comparing: (" << getVal(down) << ", " << getVal(right) << ")" << endl;
            if (getVal(down) < getVal(right))
            {
                goDown(down, right);
            }
            else
            {
                goRight(right, down);
            }
            sr--;
        }

        if(getVal(down) < getVal(right))
            return getVal(down);
        else 
        return getVal(right);
    }
};
class Solution
{
public:
    int kthSmallest(vector<vector<int>> &matrix, int k)
    {
        Matrix obj(matrix);
        return obj.findElement(k);
    }
};

int main()
{
    if (false)
    {
        cout << "Test 1:" << endl;
        Solution s;
        vector<vector<int>> data = {{1, 5, 9},
                                    {10, 11, 13},
                                    {12, 13, 15}};

        int result = s.kthSmallest(data, 8);
        cout << "Result is (expected=13): " << result << endl;
    }


    /*
    [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]]
    5
    */
    //if (false)
    {
        cout << "Test 1:" << endl;
        Solution s;
        vector<vector<int>> data = {{1,4,7,11,15},
                                    {2,5,8,12,19},
                                    {3,6,9,16,22},
                                    {10,13,14,17,24},
                                    {18,21,23,26,30}};

        int result = s.kthSmallest(data, 5);
        cout << "Result is (expected=5): " << result << endl;
    }

}
