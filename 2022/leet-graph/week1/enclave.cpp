#include <iostream>
#include <vector>

using namespace std;

#define LAND 1
#define WATER 0
class Solution
{

    class Grid
    {
        vector<vector<int>> &grid;
        int row;
        int col;

    public:
        Grid(vector<vector<int>> &g) : grid(g)
        {
            row = grid.size();
            col = grid[0].size();
        }

        void MarkBoundaryAreas()
        {
            for (int i = 0; i < row; i++)
            {
                MarkIsland(i, 0);
                MarkIsland(i, col - 1);
            }
            for (int j = 0; j < col; j++)
            {
                MarkIsland(0, j);
                MarkIsland(row - 1, j);
            }
        }

        void MarkIsland(int i, int j)
        {
            if (grid[i][j] == LAND)
            {
                grid[i][j] = -1;
            }
            else
                return;

            if (i > 0)
                MarkIsland(i - 1, j);
            if (i < (row - 1))
                MarkIsland(i + 1, j);
            if (j > 0)
                MarkIsland(i, j - 1);
            if (j < (col - 1))
                MarkIsland(i, j + 1);
        }
        int getLandCell()
        {
            int count = 0;
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    if (grid[i][j] == LAND)
                        ++count;
                }
            }
            return count;
        }
        int getEnclave()
        {
            MarkBoundaryAreas();
            return getLandCell();
        }
    };

public:
    int numEnclaves(vector<vector<int>> &grid)
    {
        Grid g(grid);
        return g.getEnclave();
    }
};

int main()
{

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{0, 0, 0, 0},
             {1, 0, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}};
        int result = s1.numEnclaves(grid1);
        cout << "Grid1 output (expected 3): " << result << endl;
    }

    if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{0, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 0}};

        int result = s1.numEnclaves(grid1);
        cout << "Grid1 output (expected 0): " << result << endl;
    }

    // if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{0, 0, 0, 1, 1, 1, 0, 1, 0, 0},
             {1, 1, 0, 0, 0, 1, 0, 1, 1, 1},
             {0, 0, 0, 1, 1, 1, 0, 1, 0, 0},
             {0, 1, 1, 0, 0, 0, 1, 0, 1, 0},
             {0, 1, 1, 1, 1, 1, 0, 0, 1, 0},
             {0, 0, 1, 0, 1, 1, 1, 1, 0, 1},
             {0, 1, 1, 0, 0, 0, 1, 1, 1, 1},
             {0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
             {1, 0, 1, 0, 1, 1, 0, 0, 0, 0},
             {0, 0, 0, 0, 1, 1, 0, 0, 0, 1}};

        int result = s1.numEnclaves(grid1);
        cout << "Grid1 output (expected 3): " << result << endl;
    }
}
