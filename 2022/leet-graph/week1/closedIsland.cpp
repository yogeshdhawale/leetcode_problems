#include <iostream>
#include <vector>

using namespace std;

#define LAND 0
#define WATER 1
class Solution
{

    class Grid
    {
        vector<vector<int>> &grid;
        int row;
        int col;
        struct property
        {
            int r;
            int c;
            bool closed;
        };

        vector<property> info;

    public:
        Grid(vector<vector<int>> &g) : grid(g)
        {
            row = grid.size();
            col = grid[0].size();
        }

        void MapIslandInfo()
        {
            int i = 0;
            int j = 0;
            for (; i < row; i++)
            {
                j = 0;
                for (; j < col; j++)
                {
                    if (grid[i][j] == LAND)
                        processIslandInfo(i, j);
                }
            }
        }

        void processIslandInfo(int i, int j)
        {
            bool closed = true;
            MarkIsland(i, j, closed);

            property p;
            p.r = i;
            p.c = j;
            // p.size = count;
            p.closed = closed;
            cout << "Found start of island @" << i << "," << j << " closed?:" << closed << endl;

            info.push_back(p);
        }

        void MarkIsland(int i, int j, bool &closed)
        {
            if (grid[i][j] == LAND)
            {
                grid[i][j] = -1;
            }
            else
                return;

            if (i > 0)
                MarkIsland(i - 1, j, closed);
            else
                closed = false;
            if (i < (row - 1))
                MarkIsland(i + 1, j, closed);
            else
                closed = false;
            if (j > 0)
                MarkIsland(i, j - 1, closed);
            else
                closed = false;
            if (j < (col - 1))
                MarkIsland(i, j + 1, closed);
            else
                closed = false;
        }

        /*int getMaxIslandSize()
        {
            int islandSize = 0;
            for (auto &p : info)
            {
                if (p.size > islandSize)
                {
                    islandSize = p.size;
                }
            }
            return islandSize;
        }*/

        int getClosedIslands()
        {
            int result = 0;
            for (auto &p : info)
            {
                if (p.closed)
                    result++;
            }
            return result;
        }
    };

public:
    int closedIsland(vector<vector<int>> &grid)
    {
        Grid g(grid);
        g.MapIslandInfo();
        return g.getClosedIslands();
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
                {1, 1, 1},
                {0, 1, 0},
                {1, 1, 1}};
        int result = s1.closedIsland(grid1);
        cout << "Grid1 output (expected 0): " << result << endl;
    }

    //if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {1, 1, 1, 1, 1, 1, 1, 0},
                {1, 0, 0, 0, 0, 1, 1, 0},
                {1, 0, 1, 0, 1, 1, 1, 0},
                {1, 0, 0, 0, 0, 1, 0, 1},
                {1, 1, 1, 1, 1, 1, 1, 0}};

        int result = s1.closedIsland(grid1);
        cout << "Grid1 output (expected 2): " << result << endl;
    }
}
