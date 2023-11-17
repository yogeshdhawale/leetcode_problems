/*
926. Nearest Exit from Entrance in Maze
Medium

You are given an m x n matrix maze (0-indexed) with empty cells (represented as '.') and walls (represented as '+'). You are also given the entrance of the maze, where entrance = [entrancerow, entrancecol] denotes the row and column of the cell you are initially standing at.

In one step, you can move one cell up, down, left, or right. You cannot step into a cell with a wall, and you cannot step outside the maze. Your goal is to find the nearest exit from the entrance. An exit is defined as an empty cell that is at the border of the maze. The entrance does not count as an exit.

Return the number of steps in the shortest path from the entrance to the nearest exit, or -1 if no such path exists.



Example 1:

Input: maze = [["+","+",".","+"],[".",".",".","+"],["+","+","+","."]], entrance = [1,2]
Output: 1
Explanation: There are 3 exits in this maze at [1,0], [0,2], and [2,3].
Initially, you are at the entrance cell [1,2].
- You can reach [1,0] by moving 2 steps left.
- You can reach [0,2] by moving 1 step up.
It is impossible to reach [2,3] from the entrance.
Thus, the nearest exit is [0,2], which is 1 step away.

Example 2:

Input: maze = [["+","+","+"],[".",".","."],["+","+","+"]], entrance = [1,0]
Output: 2
Explanation: There is 1 exit in this maze at [1,2].
[1,0] does not count as an exit since it is the entrance cell.
Initially, you are at the entrance cell [1,0].
- You can reach [1,2] by moving 2 steps right.
Thus, the nearest exit is [1,2], which is 2 steps away.

Example 3:

Input: maze = [[".","+"]], entrance = [0,0]
Output: -1
Explanation: There are no exits in this maze.



Constraints:

    maze.length == m
    maze[i].length == n
    1 <= m, n <= 100
    maze[i][j] is either '.' or '+'.
    entrance.length == 2
    0 <= entrancerow < m
    0 <= entrancecol < n
    entrance will always be an empty cell.



*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define LIMIT 100001
#define QUEUED 100002
#define WALL '+'

class Cell
{
public:
    int x;
    int y;

    Cell()
    {
        x = 0;
        y = 0;
    }
    Cell(int i, int j) : x(i), y(j)
    {
    }
};

class Grid
{
    vector<vector<char>> &data;
    int row;
    int col;
    vector<vector<int>> dist;
    queue<Cell> q;
    Cell startCell;

public:
    Grid(vector<vector<char>> &g) : data(g), row(g.size()), col(g[0].size())
    {
        dist.resize(row);
        for (int i = 0; i < row; i++)
        {
            dist[i].resize(col, LIMIT);
        }
    }

    void setStart(int i, int j)
    {
        startCell.x = i;
        startCell.y = j;
    }

    int tracePath(int dist)
    {
        int size = q.size();
        if (size == 0)
            return -1;

        for (int i = 0; i < size; i++)
        {
            Cell c = q.front();
            q.pop();

            //if (c.x == startCell.x && c.y == startCell.y)
            //    continue;
            if (c.x == 0 || c.y == 0 || c.x == row-1 || c.y == col-1)
            {
                return dist;
            }
            Enqueue(c.x + 1, c.y);
            Enqueue(c.x - 1, c.y - 1);
            Enqueue(c.x, c.y + 1);
            Enqueue(c.x, c.y - 1);
        }
        dist++;
        return tracePath(dist);
    }

    void Enqueue(int i, int j)
    {
        if (i == row || i == -1 || j == col || j == -1) // invalid position
            return;

        if (data[i][j] == WALL)
            return;

        if (dist[i][j] != LIMIT)
            return;

        q.push(Cell(i, j));
        dist[i][j] = QUEUED;
    }

    int findExit()
    {
        int x = startCell.x;
        int y = startCell.y;
        dist[x][y] = QUEUED;

        Enqueue(x + 1, y);
        Enqueue(x - 1, y);
        Enqueue(x, y + 1);
        Enqueue(x, y - 1);

        return tracePath(1);
    }
};

class Solution
{
public:
    int nearestExit(vector<vector<char>> &maze, vector<int> &entrance)
    {
        Grid g(maze);
        g.setStart(entrance[0], entrance[1]);
        return g.findExit();
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
                {'+', '+', '.', '+'},
                {'.', '.', '.', '+'},
                {'+', '+', '+','.'}};
        vector<int> entry;
        entry.push_back(1);
        entry.push_back(2);
        int result = s1.nearestExit(grid1, entry);
        cout << "Grid1 output (expected 1): " << result << endl;
    }
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<char>> grid1 =
            {
                {'+', '+', '+'},
                {'.', '.', '.'},
                {'+', '+', '+'}};
        vector<int> entry;
        entry.push_back(1);
        entry.push_back(0);
        int result = s1.nearestExit(grid1, entry);
        cout << "Grid1 output (expected 2): " << result << endl;
    }
}