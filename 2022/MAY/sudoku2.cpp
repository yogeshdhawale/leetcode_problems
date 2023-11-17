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

#define __LOG 1

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
    ~CellProperty()
    {
        values.clear();
    }
};

static int LEVEL = 0;

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
        LEVEL++;
        if (__LOG)
            cout << "LEVEL increased: " << LEVEL << endl;
        r = c = gridCnt = 9;
        // r = b.size();
        // c = b[0].size();
        // gridCnt = (r/3) * (c/3);

        foundValues = 0;
        totalValues = r * c;

        init();
    }

    ~Sudoku()
    {
        LEVEL--;
        if (__LOG)
            cout << "LEVEL decresed: " << LEVEL << endl;

        for (int i = 0; i < r; i++)
        {
            grids[i].clear();
        }
        for (int i = 0; i < r; i++)
        {
            prop[i].clear();
        }
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
    bool process()
    {
        bool startGuess = false;
        int prevFV = -1;
        while (foundValues < totalValues)
        {
            if (foundValues == prevFV)
            {
                startGuess = true;
                break;
            }
            prevFV = foundValues;

            if (__LOG)
                cout << "Looping ... foundValues:" << foundValues << endl;
            PrintState();

            for (int i = 0; i < r; i++)
            {
                for (int j = 0; j < c; j++)
                {
                    CellProperty &pr = prop[i][j];
                    if (pr.isFound)
                        continue;

                    if (pr.values.size() == 1)
                    {
                        foundValues++;
                        int v = pr.values.begin()->first;
                        if (__LOG) cout << "Setting up: (" << i << ","<< j << "):" << v << endl;

                        // process row
                        for (int q = 0; q < c; q++)
                        {
                            prop[pr.rid][q].get().eraseValue(v);
                            if (prop[pr.rid][q].get().values.size() == 0 && q != j)
                            {
                                if (__LOG)
                                    cout << "Failing here: (" << pr.rid << ", " << q << ") is empty" << endl;
                                return false;
                            }
                        }
                        // process col
                        for (int p = 0; p < r; p++)
                        {

                            prop[p][pr.cid].get().eraseValue(v);
                            if (prop[p][pr.cid].get().values.size() == 0 && p != i)
                            {
                                if (__LOG)
                                    cout << "Failing here: (" << p << ", " << pr.cid << ") is empty" << endl;
                                return false;
                            }
                        }
                        // process grid

                        for (int k = 0; k < 9; k++)
                        {
                            grids[pr.gid][k].get().eraseValue(v);
                            if (prop[pr.gid][k].get().values.size() == 0 && pr.rid != i && pr.cid != j)
                            {
                                if (__LOG)
                                    cout << "Failing here: (" << pr.rid << ", " << pr.cid << ") is empty" << endl;
                                return false;
                            }
                        }

                        pr.values[v] = 1;
                        pr.isFound = true;
                    }
                }
            }
        }
        copyValuesToResult();
        // PrintBoard();

        if (startGuess == true)
        {
            return doGuessWork();
        }

        return validateResult();
    }

    bool doGuessWork()
    {
        if (__LOG)
            cout << "We have to guess to move forward !" << endl;
        vector<vector<char>> cp;
        cp.resize(r);
        for (int i = 0; i < r; i++)
            cp[i].resize(c, '.');
        MakeCopy(cp);

        while (true)
        {
            int gi, gj, gv;
            gi = gj = gv = -1;

            findGuess(gi, gj, gv);

            if (gi == -1 || gj == -1)
            {
                // guess failed
                return false;
            }

            if (__LOG)
                cout << "Guessing, (" << gi << "," << gj << ") with value:" << gv << endl;
            cp[gi][gj] = gv + '0';

            Sudoku s2(cp);
            if (s2.process())
            {
                MakeCopyR(cp);
                return true;
            }
            else
            {
                // wrong guess
                if (__LOG)
                    cout << "Guess failed: (" << gi << "," << gj << ") with value:" << gv << endl;
                prop[gi][gj].get().eraseValue(gv);
                cp[gi][gj] = '.';
            }
        }
    }

    bool validateResult()
    {
        vector<int> sumRows(r, 0);
        vector<int> sumCols(r, 0);
        vector<int> sumGrids(r, 0);
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                int val = b[i][j] - '0';
                sumRows[i] += val;
                sumCols[j] += val;
                sumGrids[prop[i][j].get().gid] += val;
            }
        }

        for (int i = 0; i < r; i++)
        {
            if (sumRows[i] != 45 || sumCols[i] != 45 || sumGrids[i] != 45)
                return false;
        }
        return true;
    }
    void findGuess(int &gi, int &gj, int &gv)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if (!prop[i][j].get().isFound)
                {
                    if (prop[i][j].get().values.size() == 0)
                        return;

                    gi = i;
                    gj = j;
                    gv = prop[i][j].get().values.begin()->first;
                    return;
                }
            }
        }
    }
    void MakeCopy(vector<vector<char>> &cp)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                cp[i][j] = b[i][j];
            }
        }
    }
    void MakeCopyR(vector<vector<char>> &cp)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                b[i][j] = cp[i][j];
            }
        }
    }
    void copyValuesToResult()
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if (prop[i][j].get().values.size() == 1)
                    b[i][j] = prop[i][j].get().values.begin()->first + '0';
            }
        }
    }
    void PrintBoard()
    {
        if (!__LOG)
            return;
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
        if (!__LOG)
            return;
        cout << "Printing data:" << endl;

        for (int i = 0; i < r; i++)
        {
            cout << i << "=> ";
            for (int j = 0; j < c; j++)
            {
                CellProperty &p = prop[i][j];
                (p.isFound) ? cout << p.values.begin()->first : cout << '.';
                cout << " ";
            }
            cout << endl;
        }

        if (1)
        {
            for (int i = 0; i < r; i++)
            {
                for (int j = 0; j < c; j++)
                {
                    CellProperty &p = prop[i][j];
                    // 2
                    // cout << p.values.size();
                    //(p.values.size() == 1 && !p.isFound) ?  cout << '^' : cout << ' ';

                    // 3 print possiblities
                    if (1 && p.values.size() != 1)
                    {
                        cout << "(" << i << " " << j << "):";
                        for (auto const &pair : p.values)
                            cout << pair.first << ", ";
                        cout << endl;
                    }
                }
                cout << endl;
            }
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
    if (false)
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

    if (false)
    {
        Solution s;
        vector<vector<char>> b = {{'.', '.', '9', '7', '4', '8', '.', '.', '.'},
                                  {'7', '.', '.', '.', '.', '.', '.', '.', '.'},
                                  {'.', '2', '.', '1', '.', '9', '.', '.', '.'},
                                  {'.', '.', '7', '.', '.', '.', '2', '4', '.'},
                                  {'.', '6', '4', '.', '1', '.', '5', '9', '.'},
                                  {'.', '9', '8', '.', '.', '.', '3', '.', '.'},
                                  {'.', '.', '.', '8', '.', '3', '.', '2', '.'},
                                  {'.', '.', '.', '.', '.', '.', '.', '.', '6'},
                                  {'.', '.', '.', '2', '7', '5', '9', '.', '.'}};
        s.solveSudoku(b);
        cout << "1-Result: Expected 5, actual " << b[0][0];
    }
    // if (false)
    {
        Solution s;
        vector<vector<char>> b = {{'.', '.', '.', '2', '.', '.', '.', '6', '3'},
                                  {'3', '.', '.', '.', '.', '5', '4', '.', '1'},
                                  {'.', '.', '1', '.', '.', '3', '9', '8', '.'},
                                  {'.', '.', '.', '.', '.', '.', '.', '9', '.'},
                                  {'.', '.', '.', '5', '3', '8', '.', '.', '.'},
                                  {'.', '3', '.', '.', '.', '.', '.', '.', '.'},
                                  {'.', '2', '6', '3', '.', '.', '5', '.', '.'},
                                  {'5', '.', '3', '7', '.', '.', '.', '.', '8'},
                                  {'4', '7', '.', '.', '.', '1', '.', '.', '.'}};
        s.solveSudoku(b);
        cout << "1-Result: Expected 5, actual " << b[0][0];
    }

    return 0;
}
