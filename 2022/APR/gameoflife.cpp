/*

289. Game of Life
Medium

According to Wikipedia's article: "The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970."

The board is made up of an m x n grid of cells, where each cell has an initial state: live (represented by a 1) or dead (represented by a 0). Each cell interacts with its eight neighbors (horizontal, vertical, diagonal) using the following four rules (taken from the above Wikipedia article):

    Any live cell with fewer than two live neighbors dies as if caused by under-population.
    Any live cell with two or three live neighbors lives on to the next generation.
    Any live cell with more than three live neighbors dies, as if by over-population.
    Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

The next state is created by applying the above rules simultaneously to every cell in the current state, where births and deaths occur simultaneously. Given the current state of the m x n grid board, return the next state.



Example 1:

Input: board = [[0,1,0],[0,0,1],[1,1,1],[0,0,0]]
Output: [[0,0,0],[1,0,1],[0,1,1],[0,1,0]]

Example 2:

Input: board = [[1,1],[1,0]]
Output: [[1,1],[1,1]]



Constraints:

    m == board.length
    n == board[i].length
    1 <= m, n <= 25
    board[i][j] is 0 or 1.



Follow up:

    Could you solve it in-place? Remember that the board needs to be updated simultaneously: You cannot update some cells first and then use their updated values to update other cells.
    In this question, we represent the board using a 2D array. In principle, the board is infinite, which would cause problems when the active area encroaches upon the border of the array (i.e., live cells reach the border). How would you address these problems?


*/

#include <iostream>
#include <vector>

using namespace std;

using TWO_D = vector<vector<int>>;

enum CELL_FLAG
{
    DEAD = 0,
    LIVING = 1
};

class Grid
{
    TWO_D &data;
    int row;
    int col;

public:
    Grid(TWO_D &board) : data(board), row(board.size()), col(board[0].size())
    {
    }

    int getNeighborCnt(int i, int j)
    {
        int cnt = 0;
        // check for living in previous row neighbors
        if (i > 0)
        {
            if (j > 0 && data[i - 1][j - 1])
                cnt++;
            if (data[i - 1][j])
                cnt++;
            if (j < (col - 1) && data[i - 1][j + 1])
                cnt++;
        }

        // check for living in current row neighbors
        if (j > 0 && data[i][j - 1])
            cnt++;
        //if (data[i][j])
        //    cnt++;
        if (j < (col - 1) && data[i][j + 1])
            cnt++;

        if (cnt > 3)
            return cnt;

        if (i < (row - 1))
        {
            if (j > 0 && data[i + 1][j - 1])
                cnt++;
            if (data[i + 1][j])
                cnt++;
            if (j < (col - 1) && data[i + 1][j + 1])
                cnt++;
        }
        // check for living in next row neighbors
        return cnt;
    }

    void processRow(int rid, vector<int> &result)
    {
        for (int j = 0; j < col; j++)
        {
            int cell = data[rid][j];
            int neighborCnt = getNeighborCnt(rid, j);
            //cout << "Neighbour-CNT for (" << rid << "," << j << ") is: " << neighborCnt << endl;
            if (cell == LIVING)
            {
                if (neighborCnt == 2 || neighborCnt == 3)
                    result[j] = LIVING;
                else
                    result[j] = DEAD;
            }
            else if (cell == DEAD && neighborCnt == 3)
            {
                result[j] = LIVING;
            }
        }
    }
    void ProcessGrid()
    {
        vector<int> prev(col, 0);

        // process 1st row
        {
            vector<int> cur(col, 0);
            processRow(0, cur);
            for (int j = 0; j < col; j++)
                prev[j] = cur[j];
        }

        // process remaining rows
        for (int i = 1; i < row; i++)
        {
            vector<int> cur(col, 0);
            processRow(i, cur);

            for (int j = 0; j < col; j++)
            {
                data[i - 1][j] = prev[j];
                prev[j] = cur[j];
            }
        }

        // update last row
        for (int j = 0; j < col; j++)
            data[row - 1][j] = prev[j];
    }

    void PrintGrid(string msg)
    {
        cout << "Grid is (" << msg << ")::" << endl;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << data[i][j] << ", ";
            }
            cout << endl;
        }
    }
};

class Solution
{
public:
    void gameOfLife(vector<vector<int>> &board)
    {
        Grid g(board);
        g.ProcessGrid();
        //g.PrintGrid("result");
    }
};

int main()
{
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid = {{0, 1, 0},
                                    {0, 0, 1},
                                    {1, 1, 1},
                                    {0, 0, 0}};
        s1.gameOfLife(grid);
        cout << " output (expected 1): " << grid[3][1] << endl;
    }
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid = {{1, 1},
                                    {1, 0}};
        s1.gameOfLife(grid);
        cout << " output (expected 1): " << grid[1][1] << endl;
    }
}