#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define LAND 1
#define WATER 0

class Cell
{

public:
    int i;
    int j;

public:
    Cell(int x, int y) : i(x), j(y) {}
};

class Map
{
protected:
    vector<vector<int>> &grid;
    int row;
    int col;

public:
    Map(vector<vector<int>> &g) : grid(g)
    {
        row = grid.size();
        col = grid[0].size();
    }
    int getVal(int i, int j)
    {
        return grid[i][j];
    }

    void PrintGrid(string msg)
    {
        cout << "Grid is (" << msg << ")::" << endl;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << getVal(i, j) << ", ";
            }
            cout << endl;
        }
    }

    int processWaterDistance()
    {
        MarkIslands(-1);
        MarkWater(999);

        std::queue<Cell> q;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                if (grid[i][j] == -1)
                    q.push(Cell(i, j));

        int prevlevel = 1;
        int bigValue = -1;

        while (!q.empty())
        {
            //cout << "Processing level:" << prevlevel << endl;
            int size = q.size();
            for (int i = 0; i < size; i++)
            {
                Cell c = q.front();
                q.pop();
                FindBestVauleForCell(c.i, c.j);
                QueueNeighbors(c.i, c.j, q);

                if (getVal(c.i,c.j) > bigValue)
                    bigValue = getVal(c.i,c.j);
            }

            prevlevel++;
        }
        return bigValue;
    }

    void QueueNeighbors(int i, int j, std::queue<Cell> &q)
    {
        if (i > 0 && getVal(i - 1, j) == 999)
            q.push(Cell(i - 1, j));
        if (i < (row - 1)&& getVal(i + 1, j) == 999)
            q.push(Cell(i + 1, j));
        if (j > 0 && getVal(i, j -1) == 999)
            q.push(Cell(i, j - 1));
        if (j < (col - 1) && getVal(i, j + 1) == 999)
            q.push(Cell(i, j + 1));
    }
    void MarkNeighbors(int i, int j)
    {
        if (i > 0)
            FindBestVauleForCell(i - 1, j);
        if (i < (row - 1))
            FindBestVauleForCell(i + 1, j);
        if (j > 0)
            FindBestVauleForCell(i, j - 1);
        if (j < (col - 1))
            FindBestVauleForCell(i, j + 1);
    }

    void FindBestVauleForCell(int i, int j)
    {
        if (grid[i][j] == -1)
            return;

        int arry[4] = {999, 999, 999, 999};

        if (i > 0)
            arry[0] = grid[i - 1][j];
        if (i < (row - 1))
            arry[1] = grid[i + 1][j];
        if (j > 0)
            arry[2] = grid[i][j - 1];
        if (j < (col - 1))
            arry[3] = grid[i][j + 1];

        int smallIndex = 0;
        for (int i = 1; i < 4; i++)
            if (arry[smallIndex] > arry[i])
            {
                smallIndex = i;
            }
        if (arry[smallIndex] == -1)
            grid[i][j] = 1;
        else
            grid[i][j] = arry[smallIndex] + 1;
    }
    void MarkWater(int mark)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                if (grid[i][j] == WATER)
                {
                    grid[i][j] = mark;
                }
    }

    void MarkIslands(int mark)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                if (grid[i][j] == LAND)
                {
                    grid[i][j] = mark;
                }
    }
};

class Solution
{
public:
    int maxDistance(vector<vector<int>> &grid)
    {
        Map m(grid);
        return m.processWaterDistance();
    }
};

int main()
{
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{1, 0, 1},
             {0, 0, 0},
             {1, 0, 1}};
        int result = s1.maxDistance(grid1);
        cout << "Grid1 output (expected 2): " << result << endl;
    }
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{1, 0, 0},
             {0, 0, 0},
             {0, 0, 0}};
        int result = s1.maxDistance(grid1);
        cout << "Grid1 output (expected 4): " << result << endl;
    }

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{0, 0, 0},
             {0, 0, 0},
             {0, 0, 0}};
        int result = s1.maxDistance(grid1);
        cout << "Grid1 output (expected -1): " << result << endl;
    }
}