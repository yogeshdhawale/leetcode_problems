/*

// TODO timeout

37. Sudoku Solver
Hard

Write a program to solve a Sudoku puzzle by filling the empty cells.

A sudoku solution must satisfy all of the following rules:

    Each of the digits 1-9 must occur exactly once in each row.
    Each of the digits 1-9 must occur exactly once in each column.
    Each of the digits 1-9 must occur exactly once in each of the 9 3x3 sub-boxes of the grid.

The '.' character indicates empty cells.



Example 1:

Input: board = [["5","3",".",".","7",".",".",".","."],["6",".",".","1","9","5",".",".","."],[".","9","8",".",".",".",".","6","."],["8",".",".",".","6",".",".",".","3"],["4",".",".","8",".","3",".",".","1"],["7",".",".",".","2",".",".",".","6"],[".","6",".",".",".",".","2","8","."],[".",".",".","4","1","9",".",".","5"],[".",".",".",".","8",".",".","7","9"]]
Output: [["5","3","4","6","7","8","9","1","2"],["6","7","2","1","9","5","3","4","8"],["1","9","8","3","4","2","5","6","7"],["8","5","9","7","6","1","4","2","3"],["4","2","6","8","5","3","7","9","1"],["7","1","3","9","2","4","8","5","6"],["9","6","1","5","3","7","2","8","4"],["2","8","7","4","1","9","6","3","5"],["3","4","5","2","8","6","1","7","9"]]
Explanation: The input board is shown above and the only valid solution is shown below:




Constraints:

    board.length == 9
    board[i].length == 9
    board[i][j] is a digit or '.'.
    It is guaranteed that the input board has only one solution.


*/

#include <iostream>
#include <vector>
#include <map>
#include <functional>

using namespace std;

class CellProperty
{
public:
    int rid;
    int cid;
    int gid;

    bool isFound;
    map<int, int> values;
    CellProperty(int i, int j, int v)
    {
        rid = i;
        cid = j;
        gid = getGridID();
        isFound = false;

        if (v == -1)
        {
            values[1] = 1;
            values[2] = 1;
            values[3] = 1;
            values[4] = 1;
            values[5] = 1;
            values[6] = 1;
            values[7] = 1;
            values[8] = 1;
            values[9] = 1;
        }
        else
        {
            values[v] = 1;
        }
    }
    int getGridID()
    {
        // returns 0 to 8
        return (rid / 3) * 3 + (cid / 3);
    }

    void eraseValue(int v)
    {
        values.erase(v);
    }
};

class Sudoku
{
private:
    vector<vector<char>> &b;
    int r;
    int c;
    int gridCnt;
    int foundValues;

    int totalValues;

    vector<vector<std::reference_wrapper<CellProperty>>> prop;
    vector<vector<std::reference_wrapper<CellProperty>>> grids;

public:
    Sudoku(vector<vector<char>> &board) : b(board)
    {
        r = c = gridCnt = 9;
        // r = b.size();
        // c = b[0].size();
        // gridCnt = (r/3) * (c/3);

        foundValues = 0;
        totalValues = r * c;

        init();
    }
    void init()
    {
        prop.resize(r);
        grids.resize(r);
        // prop.resize(r, vector<CellProperty & >(c));
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                int value = b[i][j] == '.' ? -1 : b[i][j] - '0';
                CellProperty *p = new CellProperty(i, j, value);
                CellProperty &ref = *p;
                prop[i].push_back(ref);
                grids[p->gid].push_back(ref);
            }
        }
    }
    void process()
    {
        while (foundValues < totalValues)
        {
            // cout << "Looping ... foundValues:" << foundValues << endl;
            PrintState();
            for (int i = 0; i < r; i++)
            {
                for (int j = 0; j < c; j++)
                {
                    CellProperty &p = prop[i][j];
                    if (p.isFound)
                        continue;

                    if (p.values.size() == 1)
                    {
                        foundValues++;
                        int v = p.values.begin()->first;

                        // process row
                        for (int j = 0; j < c; j++)
                            prop[p.rid][j].get().eraseValue(v);
                        // process col
                        for (int i = 0; i < r; i++)
                            prop[i][p.cid].get().eraseValue(v);
                        // process grid
                        for (int k = 0; k < 9; k++)
                            grids[p.gid][k].get().eraseValue(v);

                        p.values[v] = 1;
                        p.isFound = true;
                    }
                }
            }
        }

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                b[i][j] = prop[i][j].get().values.begin()->first + '0';
            }
        }
        // PrintBoard();
    }

    void PrintBoard()
    {
        cout << "Printing board:" << endl;
        for (int i = 0; i < r; i++)
        {
            cout << i << "=> ";
            for (int j = 0; j < c; j++)
                cout << b[i][j] << ' ';
            cout << endl;
        }
    }
    void PrintState()
    {
        cout << "Printing data:" << endl;
        for (int i = 0; i < r; i++)
        {
            cout << i << "=> ";
            for (int j = 0; j < c; j++)
            {
                CellProperty &p = prop[i][j];
                // 1 print found values
                (p.values.size() == 1) ? cout << p.values.begin()->first : cout << '.';
                cout << " ";
                // 2
                // cout << p.values.size();
                //(p.values.size() == 1 && !p.isFound) ?  cout << '^' : cout << ' ';

                // 3 print possiblities
                if (0)
                {
                    cout << "(" << i << " " << j << "):";
                    for (auto const &pair : p.values)
                        cout << pair.first << ", ";
                    cout << endl;
                }
            }
            cout << endl;
        }
        cout << endl;
    }
};

class Solution
{
public:
    void solveSudoku(vector<vector<char>> &board)
    {
        Sudoku s(board);
        s.process();
    }
};

int main()
{
    // if (false)
    {
        Solution s;
        vector<vector<char>> b = {{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
                                  {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
                                  {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
                                  {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
                                  {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
                                  {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
                                  {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
                                  {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
                                  {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};
        s.solveSudoku(b);
        cout << "1-Result: Expected 4, actual " << b[0][2];
    }

    return 0;
}
