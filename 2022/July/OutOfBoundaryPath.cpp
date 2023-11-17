/*


576. Out of Boundary Paths
Medium

1824

192

Add to List

Share
There is an m x n grid with a ball. The ball is initially at the position [startRow, startColumn]. You are allowed to move the ball to one of the four adjacent cells in the grid (possibly out of the grid crossing the grid boundary). You can apply at most maxMove moves to the ball.

Given the five integers m, n, maxMove, startRow, startColumn, return the number of paths to move the ball out of the grid boundary. Since the answer can be very large, return it modulo 109 + 7.



Example 1:


Input: m = 2, n = 2, maxMove = 2, startRow = 0, startColumn = 0
Output: 6
Example 2:


Input: m = 1, n = 3, maxMove = 3, startRow = 0, startColumn = 1
Output: 12


Constraints:

1 <= m, n <= 50
0 <= maxMove <= 50
0 <= startRow < m
0 <= startColumn < n


breakdown:

we should reach to outer row/col with one move left.
so if we are starting at 5,5 in 10x10 matrix with max moves of 2, we will have zero paths.

going to corner cell costs 2 moves.



*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Cell
{
public:
    int i;
    int j;

    Cell(int x, int y) : i(x), j(y)
    {
    }
};

class Grid
{

    vector<vector<int>> grid;
    int row;
    int col;

    queue<Cell> q;

public:
    Grid(int r, int c) : row(r), col(c)
    {
        grid.resize(row);
        for (int i = 0; i < row; i++)
        {
            grid[i].resize(col, 0);
            grid[i][0] += 1;
            grid[i][col - 1] += 1;
        }

        for (int j = 0; j < col; j++)
        {
            grid[0][j] += 1;
            grid[row - 1][j] += 1;
        }
    }

    bool isValidCell(int i, int j)
    {
        if (i < 0 || j < 0)
            return false;

        if (i >= row || j >= col)
            return false;

        return true;
    }

    bool getDistance(int i, int j, int moves)
    {
        if ((i - moves) < 0 || (i + moves) >= row)
            return true;

        if ((j - moves) < 0 || (j + moves) >= col)
            return true;
        return false;
    }
    int getPathCount(int i, int j)
    {
        return grid[i][j];
    }

    int getMaxMoves(int maxMove, int startRow, int startColumn)
    {
        long long paths = 0;

        q.push(Cell(startRow, startColumn));
        while (maxMove)
        {
            int qsize = q.size();
            cout << "processing level " << maxMove << " with qsize: " << qsize << endl;

            while (qsize--)
            {
                Cell c = q.front();
                q.pop();

                // cout << "(" << c.i << "," << c.j << ") ";
                paths += getPathCount(c.i, c.j);
                paths = paths % 1000000007;
                // TODO modulo

                if (!getDistance(c.i, c.j, maxMove))
                    continue;

                if (isValidCell(c.i - 1, c.j))
                {
                    q.push(Cell(c.i - 1, c.j));
                }
                if (isValidCell(c.i + 1, c.j))
                {
                    q.push(Cell(c.i + 1, c.j));
                }
                if (isValidCell(c.i, c.j - 1))
                {
                    q.push(Cell(c.i, c.j - 1));
                }
                if (isValidCell(c.i, c.j + 1))
                {
                    q.push(Cell(c.i, c.j + 1));
                }
            }
            cout << endl;
            --maxMove;
        }
        return paths;
    }
};

class Solution
{
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn)
    {
        if (m == 0 || n == 0 || startRow >= m || startColumn >= n)
            return 0;

        Grid g(m, n);
        return g.getMaxMoves(maxMove, startRow, startColumn);
    }
};

int main()
{
    if (false)
    {
        Solution s;

        cout << "Test 1:" << endl;
        int result = s.findPaths(2, 2, 2, 0, 0);
        cout << "Result is (expected=6): " << result << endl;
    }

    if (false)
    {
        Solution s;

        cout << "Test 2:" << endl;
        int result = s.findPaths(1, 3, 3, 0, 1);
        cout << "Result is (expected=12): " << result << endl;
    }

    if (false)
    {
        Solution s;

        cout << "Test 3:" << endl;
        int result = s.findPaths(2, 1, 2, 0, 0);
        cout << "Result is (expected=6): " << result << endl;
    }

    // if (false)
    {
        Solution s;

        cout << "Test 3:" << endl;
        int result = s.findPaths(8, 7, 16, 1, 5);
        cout << "Result is (expected=6): " << result << endl;
    }
}
