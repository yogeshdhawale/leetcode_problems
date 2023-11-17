/*


576. Out of Boundary Paths
Medium


There is an m x n grid with a ball. The ball is initially at the position [startRow, startColumn]. You are allowed to move the ball to one of the four adjacent cells in the grid (possibly out of the grid crossing the grid boundary). You can apply at most maxMove moves to the ball.

Given the five integers m, n, maxMove, startRow, startColumn, return the number of paths to move the ball out of the grid boundary. Since the answer can be very large, return it modulo 109 + 7.



Example 1:


Input: m = 2, n = 2, maxMove = 2, startRow = 0, startColumn = 0
Output: 6
Example 2:


Input: m = 1, n = 3, maxMove = 3, startRow = 0, startColumn = 1
Output: 12


Constraints:

1 <= m, n <= 50
0 <= maxMove <= 50
0 <= startRow < m
0 <= startColumn < n


breakdown:

we should reach to outer row/col with one move left.
so if we are starting at 5,5 in 10x10 matrix with max moves of 2, we will have zero paths.

going to corner cell costs 2 moves.



*/

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdarg>

using namespace std;



//missing string printf
//this is safe and convenient but not exactly efficient
inline std::string format(const char* fmt, ...){
    int size = 512;
    char* buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if(size<=nsize){ //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize+1]; //+1 for /0
        nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
    return ret;
}


const int MODVAL = 1e9 + 7;

class Cell
{
public:
    int i;
    int j;

    Cell(int x, int y) : i(x), j(y)
    {
    }
};

class Grid
{

    int row;
    int col;
    int maxMove;

public:
    Grid(int r, int c, int m) : row(r), col(c), maxMove(m)
    {
    }

    bool isValidCell(int i, int j)
    {
        if (i < 0 || j < 0)
            return false;

        if (i >= row || j >= col)
            return false;

        return true;
    }

    bool getDistance(int i, int j, int moves)
    {
        if ((i - moves) < 0 || (i + moves) >= row)
            return true;

        if ((j - moves) < 0 || (j + moves) >= col)
            return true;
        return false;
    }

    int getMaxMovesHelper(int level, int startRow, int startColumn, int *memory)
    {
        if (level < 0)
            return 0;


        if (startRow < 0 || startColumn < 0 || startRow >= row || startColumn >= col)
            return 1;

        int memId = level * (row * col) + startRow * (row) + startColumn;

        if (memory[memId] != 0)
        {
            //cout << "Using memory" << endl;
            return memory[memId];
        }

        int v1 = getMaxMovesHelper(level - 1, startRow - 1, startColumn, memory);
        int v2 = getMaxMovesHelper(level - 1, startRow + 1, startColumn, memory);
        int v3 = getMaxMovesHelper(level - 1, startRow, startColumn - 1, memory);
        int v4 = getMaxMovesHelper(level - 1, startRow, startColumn + 1, memory);

        //cout << format(" lvl: %d : (%d, %d) => %d, %d, %d, %d", level, startRow, startColumn, v1, v2, v3,v4) <<  endl;

        int finalVal = (v1 + v2 + v3 + v4) % MODVAL;
        memory[memId] = finalVal;
        return finalVal;
    }

    int getMaxMoves(int startRow, int startColumn)
    {
        int memsize = (maxMove+1) * row * col;

        int *memory = new int[memsize];
        for (int i = 0; i < memsize; i++)
            memory[i] = 0;

        int result = getMaxMovesHelper(maxMove, startRow, startColumn, memory);
        
        delete[]  memory;
        return result;
    }
};

class Solution
{
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn)
    {
        if (m == 0 || n == 0 || startRow >= m || startColumn >= n)
            return 0;

        Grid g(m, n, maxMove);
        return g.getMaxMoves(startRow, startColumn);
    }
};

int main()
{
    if (false)
    {
        Solution s;

        cout << "Test 1:" << endl;
        int result = s.findPaths(2, 2, 2, 0, 0);
        cout << "Result is (expected=6): " << result << endl;
    }

    if (false)
    {
        Solution s;

        cout << "Test 2:" << endl;
        int result = s.findPaths(1, 3, 3, 0, 1);
        cout << "Result is (expected=12): " << result << endl;
    }

    if (false)
    {
        Solution s;

        cout << "Test 3:" << endl;
        int result = s.findPaths(2, 1, 2, 0, 0);
        cout << "Result is (expected=6): " << result << endl;
    }

    //if (false)
    {
        Solution s;

        cout << "Test 4:" << endl;
        int result = s.findPaths(8, 7, 16, 1, 5);
        cout << "Result is (expected=?): " << result << endl;
    }
}
