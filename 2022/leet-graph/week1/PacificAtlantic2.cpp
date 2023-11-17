#include <iostream>
#include <vector>
#include <memory>

using namespace std;

/*There is an m x n rectangular island that borders both the Pacific Ocean and Atlantic Ocean. The Pacific Ocean touches the island's left and top edges, and the Atlantic Ocean touches the island's right and bottom edges.

The island is partitioned into a grid of square cells. You are given an m x n integer matrix heights where heights[r][c] represents the height above sea level of the cell at coordinate (r, c).

The island receives a lot of rain, and the rain water can flow to neighboring cells directly north, south, east, and west if the neighboring cell's height is less than or equal to the current cell's height. Water can flow from any cell adjacent to an ocean into the ocean.

Return a 2D list of grid coordinates result where result[i] = [ri, ci] denotes that rain water can flow from cell (ri, ci) to both the Pacific and Atlantic oceans.


---
top left triangle is pacific
bottom right triagle is atalanta
find cell such that it traces to both pacific and atalanta.

basically find a path with decresing sequence of numbers going in both the directions.

we have to do this activity for each cell.
we need to keep track if each cell can connect to pacific or atlanta.
 */

enum OCEANS
{
    PACIFIC = 1001, // top left
    ATLANTA = 1002  // bottom right
};

class CellPropery
{
public:
    bool isConn2P;
    bool isConn2A;
    bool isProcessedForP;
    bool isProcessedForA;
    bool left;
    bool right;
    bool top;
    bool bottom;

public:
    CellPropery()
    {
        isConn2A = isConn2P = false;
        isProcessedForA = isProcessedForP = false;
        left = right = top = bottom = false;
    }

    bool isProcessed(OCEANS o)
    {
        if (o == PACIFIC)
            return isProcessedForP;
        if (o == ATLANTA)
            return isProcessedForA;
        return false;
    }
    bool isConnected(OCEANS o)
    {
        if (o == PACIFIC)
            return isConn2P;
        if (o == ATLANTA)
            return isConn2A;
        return false;
    }
    void setProcessed(OCEANS o)
    {
        if (o == PACIFIC)
            isProcessedForP = true;
        if (o == ATLANTA)
            isProcessedForA = true;
    }

    void connectTo(OCEANS o)
    {
        if (o == PACIFIC)
            isConn2P = true;
        if (o == ATLANTA)
            isConn2A = true;
    }
};

// typedef vector<vector<int>> GRID;
// typedef vector<vector<bool>> GINFO;
using GRID = vector<vector<int>>;
using GRIDFLAG = vector<vector<CellPropery>>;

class Grid
{
    GRID &grid;
    int row;
    int col;

    GRIDFLAG pr;

public:
    Grid(vector<vector<int>> &g) : grid(g), row(grid.size()), col(grid[0].size()), pr(row, vector<CellPropery>(col))
    {
    }

    ~Grid()
    {
    }

    int get(int i, int j)
    {
        return grid[i][j];
    }
    void PrintGrid(string msg)
    {
        cout << "Grid is (" << msg << ")::" << endl;
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                cout << get(i, j) << ", ";
            }
            cout << endl;
        }
    }

    void MarkBoundaryAreas()
    {
        for (int i = 0; i < row; i++)
        {
            // left
            pr[i][0].connectTo(PACIFIC);

            // right
            pr[i][col - 1].connectTo(ATLANTA);
        }
        for (int j = 0; j < col; j++)
        {
            // top
            pr[0][j].connectTo(PACIFIC);

            // bottom
            pr[row - 1][j].connectTo(ATLANTA);
        }
    }

    vector<vector<int>> getCellsForBothOcean()
    {
        vector<vector<int>> result;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                if (pr[i][j].isConn2A && pr[i][j].isConn2P)
                {
                    vector<int> data;
                    data.push_back(i);
                    data.push_back(j);
                    result.push_back(data);
                    cout << "Found cell: (" << i << "," << j << ")" << endl;
                }
            }

        return result;
    }

    void CheckNeighborsAndMarkPath(int i, int j)
    {
        if (i > 0)
        {
            if (grid[i][j] <= grid[i - 1][j])
                pr[i][j].top = true;
        }
        if (i < (row - 1))
        {
            if (grid[i][j] <= grid[i + 1][j])
                pr[i][j].bottom = true;
        }
        if (j > 0)
        {
            if (grid[i][j] <= grid[i][j - 1])
                pr[i][j].left = true;
        }
        if (j < (col - 1))
        {
            if (grid[i][j] <= grid[i][j + 1])
                pr[i][j].right = true;
        }
    }

    void MarkWaterPath()
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                CheckNeighborsAndMarkPath(i, j);
    }

    void ProcessCell(int i, int j, OCEANS o)
    {
        CellPropery &cp = pr[i][j];
        if (cp.isProcessed(o))
            return;

        if (!cp.isConnected(o))
            return;

        cp.setProcessed(o);
        if (cp.top)
        {
            pr[i - 1][j].connectTo(o);
            ProcessCell(i - 1, j, o);
        }
        if (cp.bottom)
        {
            pr[i + 1][j].connectTo(o);
            ProcessCell(i + 1, j, o);
        }
        if (cp.left)
        {
            pr[i][j - 1].connectTo(o);
            ProcessCell(i, j - 1, o);
        }
        if (cp.right)
        {
            pr[i][j + 1].connectTo(o);
            ProcessCell(i, j + 1, o);
        }
    }

    void ProcessAllAreas()
    {
        // for pacific: TL
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                ProcessCell(i, j, PACIFIC);
            }
        }

        // for Atalanta: BR
        for (int i = row - 1; i >= 0; i--)
        {
            for (int j = col - 1; j >= 0; j--)
            {
                ProcessCell(i, j, ATLANTA);
            }
        }
    }

    vector<vector<int>> getCellsForPacificAtlanta()
    {
        // tag cells if water can flow into current cell to neighbouring cell.
        MarkWaterPath();
        MarkBoundaryAreas();
        ProcessAllAreas();
        return getCellsForBothOcean();
    }
};

class Solution
{
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>> &heights)
    {
        Grid g(heights);
        return g.getCellsForPacificAtlanta();
    }
};

int main()
{

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {9, 9, 11},
                {9, 9, 9},
                {11, 9, 9}};

        vector<vector<int>> result = s1.pacificAtlantic(grid1);

        cout << "Grid1 output (expected 2): " << result.size() << endl;
        // Output: [[0,2],[2,0]
    }
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{1, 2, 2, 3, 5},
             {3, 2, 3, 4, 4},
             {2, 4, 5, 3, 1},
             {6, 7, 1, 4, 5},
             {5, 1, 1, 2, 4}};

        vector<vector<int>> result = s1.pacificAtlantic(grid1);

        cout << "Grid1 output (expected 7): " << result.size() << endl;
        // Output: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]
    }

    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{3, 3, 3, 3, 3, 3},
             {3, 0, 3, 3, 0, 3},
             {3, 3, 3, 3, 3, 3}};

        vector<vector<int>> result = s1.pacificAtlantic(grid1);

        cout << "Grid1 output (expected 16): " << result.size() << endl;
        // Output: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]
    }
}
