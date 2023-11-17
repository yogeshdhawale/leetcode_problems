#include <iostream>
#include <vector>

using namespace std;

class Solution
{

    class GridMap
    {
    private:
        int row;
        int col;
        vector<vector<char>> &data;

        int count;

    public:
        GridMap(int r, int c, vector<vector<char>> &d) : data(d)
        {
            row = r;
            col = c;
            count = 0;
        }

        void findIsland()
        {
            int i = 0;
            int j = 0;
            for (; i < row; i++)
            {
                int j = 0;
                for (; j < col; j++)
                {
                    // if (data[i][j] == 'x')
                    //     continue; // already processed
                    // if(data[i][j] == '0')
                    //     continue; // its water

                    if (isNewIsland(i, j))
                    {
                        cout << "found new @ " << i << "," << j << endl;
                        count++;
                    }
                    if (data[i][j] == '1')
                    {
                        MarkNeighbours(i, j);
                    }
                }
            }
        }

        inline bool isNewIsland(int i, int j)
        {
            if (data[i][j] != '1')
                return false;
            return true;
        }

        void MarkNeighbours(int i, int j)
        {
            if (Mark(i, j) == false)
                return;
            MarkNeighbours(i - 1, j);
            MarkNeighbours(i + 1, j);
            MarkNeighbours(i, j - 1);
            MarkNeighbours(i, j + 1);
        }
        bool Mark(int i, int j)
        {
            if (i < 0 || i >= row)
                return false;
            if (j < 0 || j >= col)
                return false;

            if (data[i][j] != '1')
                return false;

            cout << "Marking: " << i << "," << j << endl;
            data[i][j] = 'x';
            return true;
        }

        int getIslandCount()
        {
            return count;
        }
    };

public:
    int numIslands(vector<vector<char>> &grid)
    {
        int row = grid.size();
        int col = grid[0].size();

        GridMap info(row, col, grid);
        info.findIsland();
        return info.getIslandCount();
    }
};

int main()
{

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<char>> grid1 =
            {
                {'1', '1', '1'},
                {'0', '1', '0'},
                {'1', '1', '1'}};
        int result = s1.numIslands(grid1);
        cout << "Grid1 output (expected 1): " << result << endl;
    }
    if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<char>> grid1 =
            {
                {'1', '1'},
                {'0', '0'}};
        int result = s1.numIslands(grid1);
        cout << "Grid1 output (expected 1): " << result << endl;
    }

    if (false)
    {
        cout << "*** Starting test 3" << endl;

        Solution s1;
        vector<vector<char>> grid1 =
            {
                {'1', '1', '1', '1', '0'},
                {'1', '1', '0', '1', '0'},
                {'1', '1', '0', '0', '0'},
                {'0', '0', '0', '0', '0'}};

        int result = s1.numIslands(grid1);
        cout << "Grid1 output (expected 1): " << result << endl;
    }
    // if (false)
    {
        cout << "*** Starting test 4" << endl;

        Solution s1;
        vector<vector<char>> grid1 =
            {{'1', '0', '1', '1', '1'},
             {'1', '0', '1', '0', '1'},
             {'1', '1', '1', '0', '1'}};

        int result = s1.numIslands(grid1);
        cout << "Grid1 output (expected 1): " << result << endl;
    }
}