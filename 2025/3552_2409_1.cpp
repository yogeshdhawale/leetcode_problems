
/*
3552. Grid Teleportation Traversal
Medium
Topics
premium lock iconCompanies
Hint

You are given a 2D character grid matrix of size m x n, represented as an array of strings, where matrix[i][j] represents the cell at the intersection of the ith row and jth column. Each cell is one of the following:

    '.' representing an empty cell.
    '#' representing an obstacle.
    An uppercase letter ('A'-'Z') representing a teleportation portal.

You start at the top-left cell (0, 0), and your goal is to reach the bottom-right cell (m - 1, n - 1). You can move from the current cell to any adjacent cell (up, down, left, right) as long as the destination cell is within the grid bounds and is not an obstacle.

If you step on a cell containing a portal letter and you haven't used that portal letter before, you may instantly teleport to any other cell in the grid with the same letter. This teleportation does not count as a move, but each portal letter can be used at most once during your journey.

Return the minimum number of moves required to reach the bottom-right cell. If it is not possible to reach the destination, return -1.



Example 1:

Input: matrix = ["A..",".A.","..."]

Output: 2

Explanation:

    Before the first move, teleport from (0, 0) to (1, 1).
    In the first move, move from (1, 1) to (1, 2).
    In the second move, move from (1, 2) to (2, 2).

Example 2:

Input: matrix = [".#...",".#.#.",".#.#.","...#."]

Output: 13

Explanation:



Constraints:

    1 <= m == matrix.length <= 103
    1 <= n == matrix[i].length <= 103
    matrix[i][j] is either '#', '.', or an uppercase English letter.
    matrix[0][0] is not an obstacle.


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include<list>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

using namespace std;

//#define __DEBUG_LOG __DEBUG_LOG_ON
//#define __DEBUG_BIG_TEST
enum TestOptions {
    TEST_ALL = 10,
    TEST_SINGLE = 11,
};

struct TestConfig {
    TestOptions options;
    int testCaseID;
};
#include "log.h"
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 1);
int gCurTestId = -1;

/****************************************************************************/
/****************************************************************************/

#ifndef __DEBUG_LOG
#define LOG(x)
#define LOGV(x)
#endif

#define INVALID -1
#define MY_MAX_VAL INT32_MAX
#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef vector<string> INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1) {
        this->mData1 = input1;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/


struct TracePath {
    int row;
    int col;
    int steps;
    vector<bool> usedPortals;
    vector<vector<int>> visited;

    TracePath(int r, int c, int s, vector<bool> p, vector<vector<int>> v) :
        row(r), col(c), steps(s), usedPortals(p), visited(v) {}

    bool operator>(const TracePath& other) const {
        return steps > other.steps;
    }
};
class Solution {
public:

    int rows;
    int cols;
    Solution() {}

    int minMoves(vector<string>& matrix) {

        rows = matrix.size();
        cols = matrix[0].size();

        LOG("Rows:" << rows << " Cols:" << cols << " total cells:" << rows * cols);

        if (matrix[0][0] == '#' || matrix[rows - 1][cols - 1] == '#') return -1;
        if (rows == 1 && cols == 1) return 0;


        if (isPortal(0, 0, matrix) || isPortal(rows - 1, cols - 1, matrix)) {
            if (matrix[0][0] == matrix[rows - 1][cols - 1]) {
                return 0;
            }
        }


        vector<vector<IDX2D> > portalMap(26);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char cell = matrix[i][j];
                if (cell >= 'A' && cell <= 'Z') {
                    auto idx = cell - 'A';
                    portalMap[idx].push_back({ i, j });
                }
            }
        }

        vector<IDX2D> directions = { {0,1},{1,0},{0,-1},{-1,0} };
        int maxSteps = rows * cols + 1; // impossible high value


        priority_queue<TracePath, vector<TracePath>, greater<TracePath> > traces;

        {
            // mark obstacles as visited so that we don't use them
            // visited => 0+, -1 not visited, -2 obstacle
            vector<vector<int>> visitedInit(rows, vector<int>(cols, -1));
            visitedInit[0][0] = 0;
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    if (matrix[i][j] == '#') visitedInit[i][j] = -2;

            // in the beginning, no portals are used
            vector<bool> usedPortals(26, false);
            // add starting point (0,0)
            traces.push(TracePath(0, 0, 0, usedPortals, visitedInit));
        }
        int maxQueueSize = 0;
        int processedPaths = 0;

        while (!traces.empty()) {

            auto cur = traces.top();
            maxQueueSize = max(maxQueueSize, (int)traces.size());
            traces.pop();

            //LOG("Processing path row:" << cur.row << " col:" << cur.col << " steps:" << cur.steps);

            if (cur.steps >= maxSteps) {
                continue;
            }

            if (cur.row == rows - 1 && cur.col == cols - 1) {
                LOG("Found a path with steps:" << cur.steps << " visited:" << cur.visited);

                LOG("maxQueueSize:" << maxQueueSize << " processedPaths:" << processedPaths << " elements left:" << traces.size());
                return cur.steps;
            }

            processedPaths++;

            if (isPortal(cur.row, cur.col, matrix)) {

                int portalIdx = matrix[cur.row][cur.col] - 'A';

                if (!cur.usedPortals[portalIdx]) {
                    cur.usedPortals[portalIdx] = true;

                    //LOG("\tProcessing portal " << matrix[cur.row][cur.col]);

                    for (auto& exitPoint : portalMap[portalIdx]) {
                        if (cur.visited[exitPoint.first][exitPoint.second] != -1) continue;

                        auto newvisited = cur.visited;
                        newvisited[exitPoint.first][exitPoint.second] = cur.steps;

                        traces.push(TracePath(exitPoint.first, exitPoint.second, cur.steps, cur.usedPortals, newvisited));
                    }
                }
            }

            //LOG("\tprocessing neighbors");
            for (auto& d : directions) {
                int newRow = cur.row + d.first;
                int newCol = cur.col + d.second;
                if (isOutOfBound(newRow, newCol)) continue;
                if (cur.visited[newRow][newCol] != -1) continue;

                auto newvisited = cur.visited;
                newvisited[newRow][newCol] = cur.steps + 1;

                traces.push(TracePath(newRow, newCol, cur.steps + 1, cur.usedPortals, newvisited));
            }
        }

        LOG("maxQueueSize:" << maxQueueSize << " processedPaths:" << processedPaths);

        return -1;
    }

    bool isPortal(int r, int c, vector<string>& matrix) {
        char cell = matrix[r][c];
        return (cell >= 'A' && cell <= 'Z');
    }

    bool isObstacle(int r, int c, vector<string>& matrix) {
        return (matrix[r][c] == '#');
    }

    bool isOutOfBound(int r, int c) {
        return (r < 0 || r >= rows || c < 0 || c >= cols);
    }
    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return minMoves(nums.mData1);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " }";
    return out;
}

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int generateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}


void test1(int testID)
{
    vector<TestCaseStruct> nums;
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "A..",".A.","..." });
        nums.back().setOutput(2);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ ".#...", ".#.#.", ".#.#.", "...#." });
        nums.back().setOutput(13);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ ".A.B.", "AC.B.", ".D.A.", ".D.C." });
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "C#", "#B" });
        nums.back().setOutput(-1);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "...", "..#", ".#." });
        nums.back().setOutput(-1);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "..C.","C.A." });
        nums.back().setOutput(3);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "BA..GEDEF..BA.#AH#BB", "...C#FB.E#.H.CFC.E#.", ".DAGG.H..A.DBE.HCEBH", "..CA.FCAH..H.H.EDB.E", "DBFAG.HD...B#..E.CF.", "CC.#EGF#.....DCD.G.C", "F#..DACH.F#.ECHDE.HF", "DGC.B...D...#G..G.FF", "HBDDAHE.A.C.GEC.D.GA", "DA..EF.D#........FG.", "#F..GD.ED...HFAF..FA", "G..FGEE.CAGACG.FB.ED", "..GEDH.G.EB.CH..F...", "CH....EDFB...GBF..G." });
        nums.back().setOutput(2);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ ".D", "EH" });
        nums.back().setOutput(2);
    }

    cout << endl << __LOGPREFIX << "test:" << testID << " sub-tests count (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        string strTestId = "test:" + to_string(testID) + "." + to_string(nums[i].mID) + " => ";
        cout << __LOGPREFIX << strTestId << nums[i] << endl;

        gCurTestId = nums[i].mID;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << strTestId << "returns:" << result << " vs e:" << nums[i].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected1);
    }
}

#ifdef __DEBUG_BIG_TEST
template <typename T> vector<T> load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<TestCaseStruct> nums;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(load_data_v1<int>(t1i1), load_data_v2(t1i2));
        nums.back().setOutput(load_data_v1<int>(t1e1));
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.proxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    //assert(result == nums[0].mExpected1);
}
#endif

int main()
{
    cout << "^^^Start^^^" << endl;
    test1(1);
#ifdef __DEBUG_BIG_TEST
    test2(2);
#endif
    cout << "^^^END^^^" << endl;
    return 0;
}


