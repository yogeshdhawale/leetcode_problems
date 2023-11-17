
/*

Given an m x n binary matrix mat, return the distance of the nearest 0 for each cell.

The distance between two adjacent cells is 1.



Example 1:

Input: mat = [[0,0,0],[0,1,0],[0,0,0]]
Output: [[0,0,0],[0,1,0],[0,0,0]]

Example 2:

Input: mat = [[0,0,0],[0,1,0],[1,1,1]]
Output: [[0,0,0],[0,1,0],[1,2,1]]



Constraints:

    m == mat.length
    n == mat[i].length
    1 <= m, n <= 10^4
    1 <= m * n <= 10^4
    mat[i][j] is either 0 or 1.
    There is at least one 0 in mat.


*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
#define ORGVAL 100000000
#define QUEUED 100000001

class Cell
{

public:
    int x;
    int y;

    Cell(int i, int j) : x(i), y(j)
    {
    }
};

class Matrix
{
    vector<vector<int>> &data;
    int row;
    int col;
    vector<vector<int>> dist;
    queue<Cell> q;

public:
    Matrix(vector<vector<int>> &g) : data(g), row(g.size()), col(g[0].size())
    {

        dist.resize(row);
        for (int i = 0; i < row; i++)
            dist[i].resize(col, ORGVAL);
    }
    vector<vector<int>> getDist()
    {
        return dist;
    }

    void QueueAll0()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (data[i][j] == 0)
                {
                    dist[i][j] = QUEUED;
                    q.push(Cell(i, j));
                }
            }
        }
    }

    void queueCell(int i, int j, int run)
    {
        if (i == row || i == -1 || j == col || j == -1) // invalid position
            return;

        if (data[i][j] == 0)
            return;

        if (dist[i][j] == QUEUED)
            return;

        if (dist[i][j] <= run)
            return;
        q.push(Cell(i, j));
    }

    void explore(int run)
    {
        int size = q.size();
        if (size == 0)
            return;

        for (int i = 0; i < size; i++)
        {
            Cell c = q.front();
            q.pop();

            if (dist[c.x][c.y] < run)
                continue;
            dist[c.x][c.y] = run;

            queueCell(c.x - 1, c.y, run);
            queueCell(c.x, c.y - 1, run);
            queueCell(c.x, c.y + 1, run);
            queueCell(c.x + 1, c.y, run);
        }
        run++;
        explore(run);
    }
    void process()
    {
        QueueAll0();
        explore(0);
        PrintGrid("--");
    }

    void PrintGrid(string msg)
    {
        cout << "Output is (" << msg << ")::" << endl;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << dist[i][j] << ", ";
            }
            cout << endl;
        }
    }
};

class Solution
{
public:
    vector<vector<int>> updateMatrix(vector<vector<int>> &mat)
    {
        Matrix m(mat);
        m.process();
        return m.getDist();
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
                {0, 0, 0},
                {0, 1, 0},
                {1, 1, 1}};
        s1.updateMatrix(grid1);
        // cout << "Grid1 output (expected 2): " << result << endl;
    }
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 0, 0},
                {0, 1, 1},
                {0, 1, 1}};
        s1.updateMatrix(grid1);
        // cout << "Grid1 output (expected 2): " << result << endl;
    }
}