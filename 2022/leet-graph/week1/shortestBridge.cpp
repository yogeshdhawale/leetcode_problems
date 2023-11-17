/*You are given an n x n binary matrix grid where 1 represents land and 0 represents water.

An island is a 4-directionally connected group of 1's not connected to any other 1's. There are exactly two islands in grid.

You may change 0's to 1's to connect the two islands to form one island.

Return the smallest number of 0's you must flip to connect the two islands.



Example 1:

Input: grid = [[0,1],[1,0]]
Output: 1

Example 2:

Input: grid = [[0,1,0],[0,0,0],[0,0,1]]
Output: 2

Example 3:

Input: grid = [[1,1,1,1,1],[1,0,0,0,1],[1,0,1,0,1],[1,0,0,0,1],[1,1,1,1,1]]
Output: 1



Constraints:

    n == grid.length == grid[i].length
    2 <= n <= 100
    grid[i][j] is either 0 or 1.
    There are exactly two islands in grid.


solution:
    we have two islands of 1's. we have to find sequese of 0's that we can flip to connect two islands, length of the sequence should be shortest.
    use previous problem, matrix.cpp. after the distance is marked find the s

*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define WATER 0
#define LAND1 1
#define LAND2 2

#define LIMIT 100001
#define QUEUED 100002

class Cell
{

public:
    int x;
    int y;

    Cell(int i, int j) : x(i), y(j)
    {
    }
};

class Grid
{
    vector<vector<int>> &data;
    int row;
    int col;
    vector<vector<int>> dist1;
    vector<vector<int>> dist2;
    queue<Cell> q;

public:
    Grid(vector<vector<int>> &g) : data(g), row(g.size()), col(g[0].size())
    {

        dist1.resize(row);
        dist2.resize(row);
        for (int i = 0; i < row; i++)
        {
            dist1[i].resize(col, LIMIT);
            dist2[i].resize(col, LIMIT);
        }
    }
    void PrintGrid(string msg)
    {
        cout << "Output data is (" << msg << ")::" << endl;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << data[i][j] << ", ";
            }
            cout << endl;
        }

        cout << "Output dist1 is (" << msg << ")::" << endl;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << dist1[i][j] << ", ";
            }
            cout << endl;
        }

        cout << "Output dist2 is (" << msg << ")::" << endl;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << dist2[i][j] << ", ";
            }
            cout << endl;
        }
    }

    void MarkIsland(int i, int j, int land)
    {
        if (i == row || i == -1 || j == col || j == -1) // invalid position
            return;

        if (data[i][j] == WATER)
            return;

        vector<vector<int>> &dist = (land == LAND1) ? dist1 : dist2;
        if (dist[i][j] == QUEUED)
            return;

        q.push(Cell(i, j));
        data[i][j] = land;
        dist[i][j] = QUEUED;

        MarkIsland(i + 1, j, land);
        MarkIsland(i - 1, j, land);
        MarkIsland(i, j - 1, land);
        MarkIsland(i, j + 1, land);
    }
    void LableIslands()
    {
        // as there are 2 islands, 1st one we found would be labled as 2.
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (data[i][j] == LAND1)
                {
                    MarkIsland(i, j, LAND2);
                    return;
                }
            }
        }
    }

    void Enqueue(int i, int j, int land, vector<vector<int>> &dist)
    {
        if (i == row || i == -1 || j == col || j == -1) // invalid position
            return;

        if (dist[i][j] != LIMIT)
            return;
        if (data[i][j] == land)
            return;
        // if (dist[i][j] == QUEUED)
        //     return;

        q.push(Cell(i, j));
        dist[i][j] = QUEUED;
    }
    void CalculateDistanceTo(int land, vector<vector<int>> &dist, int length)
    {
        int size = q.size();
        if (size == 0)
            return;

        for (int i = 0; i < size; i++)
        {
            Cell c = q.front();
            q.pop();

            if (data[c.x][c.y] == land)
                dist[c.x][c.y] = -1;

            dist[c.x][c.y] = length;

            Enqueue(c.x, c.y - 1, land, dist);
            Enqueue(c.x, c.y + 1, land, dist);
            Enqueue(c.x - 1, c.y, land, dist);
            Enqueue(c.x + 1, c.y, land, dist);
        }
        length++;
        CalculateDistanceTo(land, dist, length);
    }

    void calculateDistanceFromIslands()
    {
        while (!q.empty())
            q.pop();

        LableIslands();

        CalculateDistanceTo(LAND2, dist2, 0);

        while (!q.empty())
            q.pop();

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (data[i][j] == LAND1)
                {
                    MarkIsland(i, j, LAND1);
                    break;
                }
            }
        }
        CalculateDistanceTo(LAND1, dist1, 0);
    }
    int getShortestDistance()
    {
        int result = LIMIT;

        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                if (data[i][j] == WATER)
                {
                    int nval = dist1[i][j] + dist2[i][j] - 1;
                    if (result > nval)
                        result = nval;
                }
            }

        return result;
    }
};

class Solution
{
public:
    int shortestBridge(vector<vector<int>> &grid)
    {
        Grid g(grid);
        g.calculateDistanceFromIslands();
        return g.getShortestDistance();
    }
};

int main()
{
    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 0, 1},
                {1, 0, 0},
                {1, 0, 0}};
        int result = s1.shortestBridge(grid1);
        cout << "Grid1 output (expected 2): " << result << endl;
    }
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 1, 1},
                {1, 0, 0},
                {1, 1, 0}};
        int result = s1.shortestBridge(grid1);
        cout << "Grid1 output (expected 1): " << result << endl;
    }
}