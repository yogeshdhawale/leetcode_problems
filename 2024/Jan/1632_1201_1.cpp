/*
1632. Rank Transform of a Matrix
Hard
Topics
Companies
Hint

Given an m x n matrix, return a new matrix answer where answer[row][col] is the rank of matrix[row][col].

The rank is an integer that represents how large an element is compared to other elements. It is calculated using the following rules:

    The rank is an integer starting from 1.
    If two elements p and q are in the same row or column, then:
        If p < q then rank(p) < rank(q)
        If p == q then rank(p) == rank(q)
        If p > q then rank(p) > rank(q)
    The rank should be as small as possible.

The test cases are generated so that answer is unique under the given rules.



Example 1:

Input: matrix = [[1,2],[3,4]]
Output: [[1,2],[2,3]]
Explanation:
The rank of matrix[0][0] is 1 because it is the smallest integer in its row and column.
The rank of matrix[0][1] is 2 because matrix[0][1] > matrix[0][0] and matrix[0][0] is rank 1.
The rank of matrix[1][0] is 2 because matrix[1][0] > matrix[0][0] and matrix[0][0] is rank 1.
The rank of matrix[1][1] is 3 because matrix[1][1] > matrix[0][1], matrix[1][1] > matrix[1][0], and both matrix[0][1] and matrix[1][0] are rank 2.

Example 2:

Input: matrix = [[7,7],[7,7]]
Output: [[1,1],[1,1]]

Example 3:

Input: matrix = [[20,-21,14],[-19,4,19],[22,-47,24],[-19,4,19]]
Output: [[4,2,3],[1,3,4],[5,1,6],[1,3,4]]



Constraints:

    m == matrix.length
    n == matrix[i].length
    1 <= m, n <= 500
    -109 <= matrix[row][col] <= 109


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <chrono>

using namespace std;

#define __DEBUG_LOG 1

template <typename T>
void printVector(vector<T> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << ", ";
    }
    cout << endl;
}
void printMatrix(vector<vector<int>> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        printVector(nums[i]);
    }
    cout << endl;
}

struct Cell
{
    int i;
    int j;
    int value;
    Cell() : i(-1), j(-1), value(0) {}
    Cell(int _i, int _j, int _value) : i(_i), j(_j), value(_value) {}
    Cell(const Cell &other) : i(other.i), j(other.j), value(other.value) {}
    Cell &operator=(const Cell &other)
    {
        i = other.i;
        j = other.j;
        value = other.value;
        return *this;
    }
};

class Solution
{
public:
    vector<vector<int>> matrixRankTransform(vector<vector<int>> &matrix)
    {
        vector<vector<int>> result(matrix.size(), vector<int>(matrix[0].size(), 0));
        vector<int> rowRank(matrix.size(), 0);
        vector<int> colRank(matrix[0].size(), 0);

        map<int, vector<Cell>> cellGroup;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            for (size_t j = 0; j < matrix[0].size(); j++)
            {
                cellGroup[matrix[i][j]].push_back(Cell(i, j, matrix[i][j]));
            }
        }

        for (auto it = cellGroup.begin(); it != cellGroup.end(); it++)
        {
            map<int, bool> affectedRows;
            map<int, bool> affectedCols;

            for (auto cell : it->second)
            {
                int i = cell.i;
                int j = cell.j;
                if (affectedRows.find(i) == affectedRows.end())
                {
                    rowRank[i] = rowRank[i] + 1;
                }
                if (affectedCols.find(j) == affectedCols.end())
                {
                    colRank[j] = colRank[j] + 1;
                }
                affectedRows[i] = true;
                affectedCols[j] = true;
            }

            map<int, vector<Cell>> rankedCells;
            for (auto cell : it->second)
            {
                int i = cell.i;
                int j = cell.j;
                int newRank = max(rowRank[i], colRank[j]);

                rowRank[i] = newRank;
                colRank[j] = newRank;
                rankedCells[newRank].push_back(cell);
                if (__DEBUG_LOG)
                {
                    cout << "cell: [" << i << ", " << j << "]\t" << matrix[i][j] << "\t r:c:" << rowRank[i] << ":" << colRank[j] << "\tnewrank is:" << newRank << endl;
                }
            }

            for (auto itr = rankedCells.rbegin(); itr != rankedCells.rend(); itr++)
            {
                for (auto citr = itr->second.rbegin(); citr != itr->second.rend(); citr++)
                {
                    int i = citr->i;
                    int j = citr->j;
                    int newRank = max(rowRank[i], colRank[j]);
                    if (__DEBUG_LOG)
                    {
                        cout << "updating cell: [" << i << ", " << j << "]\t" << matrix[i][j] << "\t r:c:" << rowRank[i] << ":" << colRank[j] << "\tnewrank is:" << itr->first << endl;
                    }
                    result[i][j] = newRank;
                    rowRank[i] = newRank;
                    colRank[j] = newRank;
                }
            }
        }
        return result;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{-37, -26, -47, -40, -13}, {22, -11, -44, 47, -6}, {-35, 8, -45, 34, -31}, {-16, 23, -6, -43, -20}, {47, 38, -27, -8, 43}});

    auto expected = vector<vector<int>>({{3, 4, 1, 2, 7}, {9, 5, 3, 10, 8}, {4, 6, 2, 7, 5}, {7, 9, 8, 1, 6}, {12, 10, 4, 5, 11}});

    cout << "test for: " << endl;
    printMatrix(nums);
    auto result = s.matrixRankTransform(nums);

    cout << "expected: " << endl;
    printMatrix(expected);
    cout << "result: " << endl;
    printMatrix(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        assert(result[i] == expected[i]);
    }
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{7, 7},
                                     {7, 7}});

    auto expected = vector<vector<int>>({{1, 1}, {1, 1}});

    cout << "test for: " << endl;
    printMatrix(nums);
    auto result = s.matrixRankTransform(nums);
    cout << "expected: " << endl;
    printMatrix(expected);
    cout << "result: " << endl;
    printMatrix(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        assert(result[i] == expected[i]);
    }
}

void test3(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{20, -21, -14},
                                     {-19, 4, 19},
                                     {22, -47, 24},
                                     {-19, 4, 19}});

    auto expected = vector<vector<int>>({{4, 2, 3},
                                         {1, 3, 4},
                                         {5, 1, 6},
                                         {1, 3, 4}});

    cout << "test for: " << endl;
    printMatrix(nums);
    auto result = s.matrixRankTransform(nums);
    cout << "expected: " << endl;
    printMatrix(expected);
    cout << "result: " << endl;
    printMatrix(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        assert(result[i] == expected[i]);
    }
}

void test4(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{-37, -50, -3, 44}, {-37, 46, 13, -32}, {47, -42, -3, -40}, {-17, -22, -39, 24}});

    auto expected = vector<vector<int>>({{2, 1, 4, 6}, {2, 6, 5, 4}, {5, 2, 4, 3}, {4, 3, 1, 5}});

    cout << "test for: " << endl;
    printMatrix(nums);
    auto result = s.matrixRankTransform(nums);
    cout << "expected: " << endl;
    printMatrix(expected);
    cout << "result: " << endl;
    printMatrix(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        assert(result[i] == expected[i]);
    }
}

void test5(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{-23, 20, -49, -30, -39, -28, -5, -14}, {-19, 4, -33, 2, -47, 28, 43, -6}, {-47, 36, -49, 6, 17, -8, -21, -30}, {-27, 44, 27, 10, 21, -8, 3, 14}, {-19, 12, -25, 34, -27, -48, -37, 14}, {-47, 40, 23, 46, -39, 48, -41, 18}, {-27, -4, 7, -10, 9, 36, 43, 2}, {37, 44, 43, -38, 29, -44, 19, 38}});

    auto expected = vector<vector<int>>({{7, 13, 1, 5, 4, 6, 9, 8}, {8, 11, 2, 10, 1, 12, 14, 9}, {2, 14, 1, 11, 13, 7, 5, 3}, {3, 19, 16, 12, 14, 7, 10, 13}, {8, 12, 6, 14, 5, 1, 4, 13}, {2, 16, 15, 17, 4, 18, 3, 14}, {3, 7, 11, 6, 12, 13, 14, 10}, {16, 19, 18, 3, 15, 2, 11, 17}});

    cout << "test for: " << endl;
    printMatrix(nums);
    auto result = s.matrixRankTransform(nums);
    cout << "expected: " << endl;
    printMatrix(expected);
    cout << "result: " << endl;
    printMatrix(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        assert(result[i] == expected[i]);
    }
}

void test6(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({
        {25, 8, 31, 42, -39, 8, 31, -10, 33, -44, 7, -30, 9, 44, 15, 26},
        {-3, -48, -17, -18, 9, -12, -21, 10, 1, 44, -17, 14, -27, 48, -21, -6},
        {49, 28, 27, -18, -31, 4, -13, 34, 49, 48, 47, -18, 33, 40, 15, 38},
        {5, -28, -49, -38, 1, 32, -25, -50, 29, -32, 35, -46, -43, 48, -49, -6},
        {-27, -24, 23, -14, -47, -12, 7, 6, 25, -16, 47, -26, 13, -12, -33, -18},
        {45, -48, 3, -26, -23, -36, -17, 38, 17, 12, 15, 46, 37, 40, 47, 26},
        {-19, -24, -21, -2, -7, -48, 47, 30, 5, -8, 23, -46, 21, -32, -33, -26},
        {-27, 32, 27, -26, 21, -32, -49, -10, 5, 20, -29, 46, -43, -44, 39, 22},
        {-43, 48, 27, 26, -27, 12, -1, -10, -27, 12, -29, -34, 41, -28, -25, -30},
        {25, -36, 35, -26, 37, -20, 31, 14, -19, -40, -29, -2, -39, -28, 11, 46},
        {49, -32, -29, -6, -47, 32, -17, -18, -23, 24, 23, 22, -47, -44, 27, 14},
        {37, -44, -33, -18, -47, 24, -17, -46, -43, -32, 15, -46, -27, -8, -25, 46},
        {41, -40, 31, -30, 13, -24, -29, 22, -15, -16, 47, 2, -39, 4, -25, -42},
        {-3, 12, 7, 14, -7, 8, -37, -34, -7, -12, 39, -38, 1, 44, 27, -34},
        {-47, 4, 7, -2, -43, -32, 27, 2, -43, -8, -33, 14, 49, -48, -5, 30},
        {-15, 8, -33, -26, -23, -32, -25, 22, 13, -20, -9, 26, 29, 4, -1, 2},
    });

    auto expected = vector<vector<int>>({{25, 22, 28, 30, 6, 22, 28, 13, 29, 1, 16, 7, 23, 33, 24, 27},
                                         {18, 1, 14, 13, 20, 16, 12, 21, 19, 28, 14, 22, 8, 34, 12, 17},
                                         {35, 28, 27, 13, 7, 17, 15, 30, 35, 34, 33, 13, 29, 32, 24, 31},
                                         {19, 9, 2, 5, 18, 29, 11, 1, 27, 7, 30, 3, 4, 34, 2, 17},
                                         {9, 11, 25, 15, 1, 16, 21, 20, 26, 14, 33, 10, 24, 16, 4, 12},
                                         {33, 1, 15, 10, 12, 2, 14, 31, 25, 23, 24, 34, 30, 32, 35, 27},
                                         {13, 11, 12, 18, 17, 1, 29, 27, 20, 16, 26, 3, 25, 5, 4, 8},
                                         {9, 29, 27, 10, 25, 7, 1, 13, 20, 24, 8, 34, 4, 2, 30, 26},
                                         {3, 32, 27, 25, 10, 23, 16, 13, 10, 23, 8, 5, 31, 9, 11, 7},
                                         {25, 7, 29, 10, 30, 13, 28, 22, 14, 2, 8, 15, 6, 9, 19, 32},
                                         {35, 8, 9, 16, 1, 29, 14, 12, 11, 27, 26, 23, 1, 2, 28, 20},
                                         {26, 4, 6, 13, 1, 25, 14, 3, 5, 7, 24, 3, 8, 17, 11, 32},
                                         {29, 5, 28, 7, 21, 12, 8, 24, 15, 14, 33, 16, 6, 20, 11, 1},
                                         {18, 23, 21, 24, 17, 22, 5, 6, 17, 15, 31, 4, 19, 33, 28, 6},
                                         {2, 20, 21, 18, 5, 7, 23, 19, 5, 16, 6, 22, 32, 1, 17, 28},
                                         {14, 22, 6, 10, 12, 7, 11, 24, 23, 13, 15, 25, 26, 20, 18, 19}});

    cout << "test for: " << endl;
    printMatrix(nums);

    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.matrixRankTransform(nums);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << "time taken " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << endl;

    cout << "expected: " << endl;
    printMatrix(expected);
    cout << "result: " << endl;
    printMatrix(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        for (size_t j = 0; j < result[i].size(); j++)
        {
            if (result[i][j] != expected[i][j])
            {
                cout << "failed for [" << i << "," << j << "]= " << nums[i][j] << "\tresult: " << result[i][j] << " vs: " << expected[i][j] << endl;
            }
        }
    }

    for (size_t i = 0; i < result.size(); i++)
    {
        assert(result[i] == expected[i]);
    }
}

int main()
{

    cout << "--- " << endl;
    /*
    test1(1);
    test2(2);
    test3(3);
    test4(4);
    test5(5);
    */
    test6(6);
    return 0;
}