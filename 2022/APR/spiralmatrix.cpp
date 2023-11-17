/*
59. Spiral Matrix II
Medium

Given a positive integer n, generate an n x n matrix filled with elements from 1 to n2 in spiral order.
*/

#include <iostream>
#include <vector>

using namespace std;

using TWO_D = vector<vector<int>>;

class Grid
{
    TWO_D mData;
    int mRow;
    int mCol;

public:
    Grid(int row, int col) : mRow(row), mCol(col)
    {
        mData.resize(mRow);
        for (int i = 0; i < row; i++)
            mData[i].resize(mCol, 0);
    }
    void PrintGrid(string msg)
    {
        cout << "Grid is (" << msg << ")::" << endl;
        for (int i = 0; i < mRow; i++)
        {
            for (int j = 0; j < mCol; j++)
                cout << mData[i][j] << ", ";
            cout << endl;
        }
    }

    TWO_D generateMatrix()
    {
        int row = mRow;
        int col = mCol;
        int startI = 0;
        int startJ = 0;
        int cnt = 1;

        while (startI <= row)
        {
            // process one lap
            //forward
            for (int j=startJ;j<col;j++)
                mData[startI][j] = cnt++;
            // down
            for (int i=startI+1;i<row;i++)
                mData[i][col-1] = cnt++;
            // back
            for (int j=col-2;j>=startJ;j--)
                mData[row-1][j] = cnt++;
            // up
            for (int i=row-2;i>startI;i--)
                mData[i][startJ] = cnt++;

            startI++;
            startJ++;
            row--;
            col--;
        }

        PrintGrid("result");
        return mData;
    }
};

class Solution
{
public:
    vector<vector<int>> generateMatrix(int n)
    {
        Grid g(n, n);
        return g.generateMatrix();
    }
};

int main()
{
    // if (false)
    {
        cout << "*** Starting test 1" << endl;
        Solution s1;
        TWO_D result = s1.generateMatrix(3);
        cout << " output (expected 9): " << result[1][1] << endl;
    }
        // if (false)
    {
        cout << "*** Starting test 1" << endl;
        Solution s1;
        TWO_D result = s1.generateMatrix(4);
        cout << " output (expected 13): " << result[1][1] << endl;
    }
}