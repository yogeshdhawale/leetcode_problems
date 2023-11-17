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

// typedef vector<vector<int>> GRID;
// typedef vector<vector<bool>> GINFO;
using GRID = vector<vector<int>>;
using GRIDFLAG = vector<vector<bool>>;

enum OCEANS
{
    PACIFIC = 1001,
    ATLANTA = 1002
};

class OceanInfo
{
    GRID grid;
    int row;
    int col;

public:
    OceanInfo(int r, int c) : grid(r, vector<int>(c))
    {
        row = r;
        col = c;
        int val = -1;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
                grid[i][j] = val;
        }
    }

    int get(int i, int j)
    {
        return grid[i][j];
    }

    void MarkCell2Ocean(int i, int j)
    {
        grid[i][j] = 1;
    }
    void UnMarkCell2Ocean(int i, int j)
    {
        grid[i][j] = 0;
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
};

class Grid
{
    GRID &grid;
    int row;
    int col;
    OceanInfo *pacificInfo;
    OceanInfo *atlantaInfo;

    GRIDFLAG processed;

public:
    Grid(vector<vector<int>> &g) : grid(g), row(grid.size()), col(grid[0].size()), processed(row, vector<bool>(col))
    {
        pacificInfo = new OceanInfo(row, col);
        atlantaInfo = new OceanInfo(row, col);

        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                processed[i][j] = false;
            }
        }
    }

    ~Grid()
    {
        delete pacificInfo;
        delete atlantaInfo;
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

    void ProcessBoundaryAreas()
    {
        for (int i = 0; i < row; i++)
        {
            pacificInfo->MarkCell2Ocean(i, 0);
            processed[i][0] = true;
            atlantaInfo->MarkCell2Ocean(i, col - 1);
            processed[i][col-1] = true;
        }
        for (int j = 0; j < col; j++)
        {
            pacificInfo->MarkCell2Ocean(0, j);
            processed[0][j] = true;
            atlantaInfo->MarkCell2Ocean(row - 1, j);
            processed[row-1][j] = true;
        }
    }

    vector<vector<int>> getCellsForBothOcean()
    {
        vector<vector<int>> result;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                if ((pacificInfo->get(i, j) == 1) && (atlantaInfo->get(i, j) == 1))
                {
                    vector<int> data;
                    data.push_back(i);
                    data.push_back(j);
                    result.push_back(data);
                    //cout << "Found cell: (" << i << "," << j << ")" << endl;
                }
            }

        return result;
    }
    vector<vector<int>> getCellsForPacificAtlanta()
    {
        ProcessBoundaryAreas();
        ProcessAllAreas();
        return getCellsForBothOcean();
    }

    void ProcessAllAreas()
    {
        // FindFlow(2, 1);

        for (int i = 1; i < row - 1; i++)
        {
            for (int j = 1; j < col - 1; j++)
            {
                processed.resize(row);
                for(int k=0;k<col;k++)
                    processed[i].resize(col,false);

                FindFlow(i, j);
            }
        }

        for (int i = 0; i < row; i++)
        {
            FindFlowEdge(i, 0);
            FindFlowEdge(i, col - 1);
        }
        for (int j = 0; j < col; j++)
        {
            FindFlowEdge(0, j);
            FindFlowEdge(row - 1, j);
        }
    }

    void FindFlowEdge(int i, int j)
    {
        if (i < 0 || j < 0 || i >= (row) || j >= (col))
            return;

        bool toP1, toP2, toP3, toP4;
        toP1 = toP2 = toP3 = toP4 = false;
        bool toA1, toA2, toA3, toA4;
        toA1 = toA2 = toA3 = toA4 = false;

        if (i > 0)
        {
            if (grid[i][j] >= grid[i - 1][j])
            {
                toP1 = (pacificInfo->get(i - 1, j) == 1);
                toA1 = (atlantaInfo->get(i - 1, j) == 1);
            }
        }
        if (i < (row - 1))
        {
            if (grid[i][j] >= grid[i + 1][j])
            {
                toP2 = pacificInfo->get(i + 1, j) == 1;
                toA2 = atlantaInfo->get(i + 1, j) == 1;
            }
        }
        if (j > 0)
        {
            if (grid[i][j] >= grid[i][j - 1])
            {

                toP3 = pacificInfo->get(i, j - 1) == 1;
                toA3 = atlantaInfo->get(i, j - 1) == 1;
            }
        }
        if (j < (col - 1))
        {
            if (grid[i][j] >= grid[i][j + 1])
            {
                toP4 = pacificInfo->get(i, j + 1) == 1;
                toA4 = atlantaInfo->get(i, j + 1) == 1;
            }
        }

        if (toP1 || toP2 || toP3 || toP4)
        {
            //cout << "Marking cell to pacific: (" << i << "," << j << ")" << endl;
            pacificInfo->MarkCell2Ocean(i, j);
        }
        if (toA1 || toA2 || toA3 || toA4)
        {
            //cout << "Marking cell to atlanta: (" << i << "," << j << ")" << endl;
            atlantaInfo->MarkCell2Ocean(i, j);
        }
    }
    void FindFlow(int i, int j)
    {
        //static int depth = 0;
        int furtherProcessed = 0;
        if (i < 0 || j < 0 || i >= (row) || j >= (col))
            return;

        if (processed[i][j] == true)
            return;

        processed[i][j] = true;

        //depth++;
        //cout << "depth: " << depth << " called:(" << i << "," << j << ")" << endl;

        bool toP1, toP2, toP3, toP4;
        toP1 = toP2 = toP3 = toP4 = false;
        bool toA1, toA2, toA3, toA4;
        toA1 = toA2 = toA3 = toA4 = false;

        if (i > 0)
        {
            if (grid[i][j] >= grid[i - 1][j])
            {
                furtherProcessed++;
                FindFlow(i - 1, j);
                toP1 = (pacificInfo->get(i - 1, j) == 1);
                toA1 = (atlantaInfo->get(i - 1, j) == 1);
            }
        }
        if (i < (row - 1))
        {
            if (grid[i][j] >= grid[i + 1][j])
            {
                furtherProcessed++;
                FindFlow(i + 1, j);
                toP2 = pacificInfo->get(i + 1, j) == 1;
                toA2 = atlantaInfo->get(i + 1, j) == 1;
            }
        }
        if (j > 0)
        {
            if (grid[i][j] >= grid[i][j - 1])
            {
                furtherProcessed++;
                FindFlow(i, j - 1);
                toP3 = pacificInfo->get(i, j - 1) == 1;
                toA3 = atlantaInfo->get(i, j - 1) == 1;
            }
        }
        if (j < (col - 1))
        {
            if (grid[i][j] >= grid[i][j + 1])
            {
                furtherProcessed++;
                FindFlow(i, j + 1);
                toP4 = pacificInfo->get(i, j + 1) == 1;
                toA4 = atlantaInfo->get(i, j + 1) == 1;
            }
        }

        if (toP1 || toP2 || toP3 || toP4)
        {
            //cout << "Marking cell to pacific: (" << i << "," << j << ")" << endl;
            pacificInfo->MarkCell2Ocean(i, j);
        }

        if (toA1 || toA2 || toA3 || toA4)
        {
            //cout << "Marking cell to atlanta: (" << i << "," << j << ")" << endl;
            atlantaInfo->MarkCell2Ocean(i, j);
        }

        if (furtherProcessed == 0)
        {
            if (pacificInfo->get(i, j) == -1)
                pacificInfo->UnMarkCell2Ocean(i, j);
            if (atlantaInfo->get(i, j) == -1)
                atlantaInfo->UnMarkCell2Ocean(i, j);
        }

        if (pacificInfo->get(i,j) != -1 && atlantaInfo->get(i,j) != -1)
            processed[i][j] = true;
            
        //depth--;
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
