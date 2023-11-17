#include <iostream>
#include <vector>

using namespace std;

#define LAND 1
#define WATER 0
#define ISLAND_ID 100

/*
class Island
{
    int id;
    // grid[i][j] => pos[i][j]
    // info row: col list
    // 0: [1, 2] 1: [1] 2: [1], like this
    vector < vector<int>> pos;
};
*/
class BaseMap
{
protected:
    vector<vector<int>> &grid;
    int row;
    int col;

public:
    BaseMap(vector<vector<int>> &g) : grid(g)
    {
        row = grid.size();
        col = grid[0].size();
    }
    int getVal(int i, int j)
    {
        return grid[i][j];
    }
};

class ReferenceMap : public BaseMap
{
    int totalIslands;

public:
    ReferenceMap(vector<vector<int>> &g) : BaseMap(g)
    {
        totalIslands = 0;
    }

    void MarkIslands()
    {
        int islandId = ISLAND_ID;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                if (grid[i][j] == LAND)
                {
                    MarkIsland(islandId, i, j);
                    ++islandId;
                }
            }

        totalIslands = islandId - ISLAND_ID;
        cout << "Total islands are: " << totalIslands << endl;
    }

    void MarkIsland(const int id, int i, int j)
    {
        if (grid[i][j] == LAND)
        {
            grid[i][j] = id;
        }
        else
            return;

        if (i > 0)
            MarkIsland(id, i - 1, j);
        if (i < (row - 1))
            MarkIsland(id, i + 1, j);
        if (j > 0)
            MarkIsland(id, i, j - 1);
        if (j < (col - 1))
            MarkIsland(id, i, j + 1);
    }
};

class Map : public BaseMap
{
    ReferenceMap &refMap;

public:
    Map(vector<vector<int>> &g, ReferenceMap &r) : BaseMap(g), refMap(r)
    {
    }

    int getSubislandsCount()
    {
        int subIsland = 0;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                int islandId = -1;
                if (grid[i][j] == LAND)
                {
                    bool isSubisland = true;
                    islandId = refMap.getVal(i, j);
                    if (islandId == WATER)
                        isSubisland = false;

                    CheckForIsland(islandId, i, j, isSubisland);
                    if (isSubisland)
                    {
                        subIsland++;
                        cout << "Found SUB island: @" << i << "," << j << " with id:" << islandId << endl;
                    }
                }
            }
        }
        return subIsland;
    }

    void CheckForIsland(const int id, int i, int j, bool & issub)
    {
        if (grid[i][j] == LAND)
        {
            grid[i][j] = -1;
            if (issub) {
                if (refMap.getVal(i, j) != id)
                    issub = false;
            }
        }
        else return;

        if (i > 0)
            CheckForIsland(id, i - 1, j, issub);
        if (i < (row - 1))
            CheckForIsland(id, i + 1, j, issub);
        if (j > 0)
            CheckForIsland(id, i, j - 1, issub);
        if (j < (col - 1))
            CheckForIsland(id, i, j + 1, issub);
    }
};

class Solution
{

public:
    int countSubIslands(vector<vector<int>> &grid1, vector<vector<int>> &grid2)
    {
        ReferenceMap refMap(grid1);
        refMap.MarkIslands();

        Map dataMap(grid2, refMap);

        return dataMap.getSubislandsCount();
    }
};

int main()
{
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{1, 1, 1, 0, 0},
             {0, 1, 1, 1, 1},
             {0, 0, 0, 0, 0},
             {1, 0, 0, 0, 0},
             {1, 1, 0, 1, 1}};

        vector<vector<int>> grid2 =
            {{1, 1, 1, 0, 0},
             {0, 0, 1, 1, 1},
             {0, 1, 0, 0, 0},
             {1, 0, 1, 1, 0},
             {0, 1, 0, 1, 0}};

        int result = s1.countSubIslands(grid1, grid2);
        cout << "Grid1 output (expected 3): " << result << endl;
    }
}