

/*1091. Shortest Path in Binary Matrix
Medium

Given an n x n binary matrix grid, return the length of the shortest clear path in the matrix. If there is no clear path, return -1.

A clear path in a binary matrix is a path from the top-left cell (i.e., (0, 0)) to the bottom-right cell (i.e., (n - 1, n - 1)) such that:

    All the visited cells of the path are 0.
    All the adjacent cells of the path are 8-directionally connected (i.e., they are different and they share an edge or a corner).

The length of a clear path is the number of visited cells of this path.



Example 1:

Input: grid = [[0,1],[1,0]]
Output: 2

Example 2:

Input: grid = [[0,0,0],[1,1,0],[1,1,0]]
Output: 4

Example 3:

Input: grid = [[1,0,0],[1,1,0],[1,1,0]]
Output: -1



Constraints:

    n == grid.length
    n == grid[i].length
    1 <= n <= 100
    grid[i][j] is 0 or 1

*/

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;
#define LIMIT 20000

class Grid
{
    vector<vector<int>> &grid;
    int row;
    int col;
    bool pathFound;
    int pathLength;

    int counter;
    vector<vector<int>> dist;

public:
    Grid(vector<vector<int>> &g) : grid(g), row(g.size()), col(g[0].size()) //, dist(row, vector<int>(col))
    {
        pathFound = false;
        pathLength = LIMIT;

        dist.resize(row);
        for (int i = 0; i < row; i++)
            dist[i].resize(col, LIMIT);
        counter = 0;
    }

    bool TraceAhead(int i, int j, int length)
    {
        counter++;
        if (i == row || i == -1 || j == col || j == -1)
            return false;

        if (grid[i][j] == 1)
            return false;

        if (dist[i][j] <= length)
            return false; // already processed and on short path

        dist[i][j] = length;

        if (length >= pathLength)
            return false;

        int distRemain = (row - i) > (col - j) ? row - i : col - i;
        distRemain--;

        if (length >= (pathLength - distRemain))
            return false;

        if (i == row - 1 && j == col - 1)
        {
            cout << "Path found: calls: " << counter << " \t OldpathL:" << pathLength << endl;

            pathFound = true;
            if (length < pathLength)
                pathLength = length;

            return true;
        }

        length++;
        TraceAhead(i + 1, j + 1, length); // diag1

        TraceAhead(i + 1, j, length); // non diag1
        TraceAhead(i, j + 1, length); // non diag2

        TraceAhead(i + 1, j - 1, length); // diag2
        TraceAhead(i - 1, j + 1, length); // diag3
        TraceAhead(i - 1, j - 1, length); // diag4

        TraceAhead(i - 1, j, length); // non diag3
        TraceAhead(i, j - 1, length); // non diag4

        // if (ret1 || ret2 || ret3 || ret4)
        //     return true;
        return false;
    }

    int findPath()
    {
        if (grid[0][0] != 0 || grid[row - 1][col - 1] != 0)
            return -1;

        int length = 1;

        TraceAhead(0, 0, length);
        cout << "Total calls:" << counter << endl;

        if (pathLength == LIMIT)
            return -1;
        return pathLength;
    }
};
class Solution
{
public:
    int shortestPathBinaryMatrix(vector<vector<int>> &grid)
    {
        Grid g(grid);
        return g.findPath();
    }
};

int main()
{
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 1},
                {1, 0}};
        int result = s1.shortestPathBinaryMatrix(grid1);
        cout << "Grid1 output (expected 2): " << result << endl;
    }
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 0, 0},
                {0, 1, 0},
                {1, 1, 0}};
        int result = s1.shortestPathBinaryMatrix(grid1);
        cout << "Grid1 output (expected 4): " << result << endl;
    }

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{0, 1, 0, 1, 0},
             {1, 0, 0, 0, 1},
             {0, 0, 1, 1, 1},
             {0, 0, 0, 0, 0},
             {1, 0, 1, 0, 0}};

        int result = s1.shortestPathBinaryMatrix(grid1);
        cout << "Grid1 output (expected 6): " << result << endl;
    }
}
