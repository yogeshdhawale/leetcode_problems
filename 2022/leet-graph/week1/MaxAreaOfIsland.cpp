#include <iostream>
#include <vector>

using namespace std;

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
            int size;
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
                    if (grid[i][j] == 1)
                        processIslandInfo(i, j);
                }
            }
        }

        void processIslandInfo(int i, int j)
        {
            int count = 0;
            MarkIsland(i, j, count);

            property p;
            p.r = i;
            p.c = j;
            p.size = count;
            cout << "Found start of island @" << i << "," << j << " size:" << count << endl;

            info.push_back(p);
        }

        void MarkIsland(int i, int j, int &count)
        {
            // if (i < 0 || i > row) return;
            // if (j < 0 || j > col) return;

            if (grid[i][j] == 1)
            {
                grid[i][j] = -1;
                count++;
            }
            else
                return;

            if (i > 0)
                MarkIsland(i - 1, j, count);
            if (i < (row - 1))
                MarkIsland(i + 1, j, count);
            if (j > 0)
                MarkIsland(i, j - 1, count);
            if (j < (col - 1))
                MarkIsland(i, j + 1, count);
        }

        int getMaxIslandSize()
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
        }
    };

public:
    int maxAreaOfIsland(vector<vector<int>> &grid)
    {
        Grid g(grid);
        g.MapIslandInfo();
        return g.getMaxIslandSize();
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
        int result = s1.maxAreaOfIsland(grid1);
        cout << "Grid1 output (expected 7): " << result << endl;
    }

    // if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
             {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0},
             {0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0}};
        int result = s1.maxAreaOfIsland(grid1);
        cout << "Grid1 output (expected 6): " << result << endl;
    }
}
