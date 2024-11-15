
/*
913. Cat and Mouse
Hard
Topics
Companies

A game on an undirected graph is played by two players, Mouse and Cat, who alternate turns.

The graph is given as follows: graph[a] is a list of all nodes b such that ab is an edge of the graph.

The mouse starts at node 1 and goes first, the cat starts at node 2 and goes second, and there is a hole at node 0.

During each player's turn, they must travel along one edge of the graph that meets where they are.  For example, if the Mouse is at node 1, it must travel to any node in graph[1].

Additionally, it is not allowed for the Cat to travel to the Hole (node 0).

Then, the game can end in three ways:

    If ever the Cat occupies the same node as the Mouse, the Cat wins.
    If ever the Mouse reaches the Hole, the Mouse wins.
    If ever a position is repeated (i.e., the players are in the same position as a previous turn, and it is the same player's turn to move), the game is a draw.

Given a graph, and assuming both players play optimally, return

    1 if the mouse wins the game,
    2 if the cat wins the game, or
    0 if the game is a draw.



Example 1:

Input: graph = [[2,5],[3],[0,4,5],[1,4,5],[2,3],[0,2,3]]
Output: 0

Example 2:

Input: graph = [[1,3],[0],[3],[0,2]]
Output: 1



Constraints:

    3 <= graph.length <= 50
    1 <= graph[i].length < graph.length
    0 <= graph[i][j] < graph.length
    graph[i][j] != i
    graph[i] is unique.
    The mouse and the cat can always move.


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

/****************************************************************************/
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
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 8);

using namespace std;

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

typedef V2Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    INPUT_TYPE mData1;

    Input() {
        mID = inputID++;
    }
    Input(INPUT_TYPE d1) {
        this->mData1 = d1;
        mID = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};


enum PlayerType {
    P_MOUSE = 1,
    P_CAT = 2,
};

enum ResultType {
    R_DRAW = 0,
    R_MOUSE = 1,
    R_CAT = 2,
    R_NO = 3
};
enum Home {
    HOLE = 0,
    HOME_MOUSE = 1,
    HOME_CAT = 2,
};


struct Data;
ostream& operator<<(ostream& out, const Data& d);

struct Data {

    int id;
    int h_distance;
    int c_distance;
    void clear() {
        h_distance = INVALID;
        c_distance = INVALID;
    }
    Data() {
        clear();
    }
};


class Solution {
public:

    int mousePosition;
    int catPosition;
    vector<Data> nodes;
    map<int, set<int>> levelMap;

    void clear() {
        mousePosition = HOME_CAT;
        catPosition = HOME_CAT;
        nodes.clear();
    }
    int catMouseGame(vector<vector<int>>& graph) {

        int n = graph.size();
        if (n <= 3)  return R_DRAW;

        clear();

        nodes.resize(n);
        for (int i = 0;i < n;++i) {
            nodes[i].id = i;
            nodes[i].clear();
        }

        nodes[HOLE].h_distance = 0;
        levelMap[0].insert(HOLE);
        queue<int> bfsQ;
        bfsQ.push(HOLE);
        while (!bfsQ.empty()) {
            auto curNode = bfsQ.front();
            bfsQ.pop();

            int curDistance = nodes[curNode].h_distance;
            for (auto e : graph[curNode]) {
                if (nodes[e].h_distance == INVALID) {
                    nodes[e].h_distance = curDistance + 1;
                    levelMap[curDistance + 1].insert(e);
                    bfsQ.push(e);
                }
            }
        }

        bfsQ.push(HOME_CAT);
        nodes[HOME_CAT].c_distance = 0;
        while (!bfsQ.empty()) {
            auto curNode = bfsQ.front();
            bfsQ.pop();

            int curDistance = nodes[curNode].c_distance;
            for (auto e : graph[curNode]) {
                if (e == HOLE) continue;
                if (nodes[e].c_distance == INVALID) {
                    nodes[e].c_distance = curDistance + 1;
                    bfsQ.push(e);
                }
            }
        }

        set<int> currOptions;
        set<int> nextOptions;
        currOptions.insert(HOME_MOUSE);
        int moveCount = 0;
        while (currOptions.size() > 0) {
            nextOptions.clear();
            moveCount++;
            LOG("At move: " << moveCount << " options: " << currOptions);

            for (auto e : currOptions) {
                if (nodes[e].c_distance != INVALID && nodes[e].c_distance < moveCount)
                    continue;
                for (auto ne : graph[e]) {
                    if (ne == HOLE) return R_MOUSE;
                    nextOptions.insert(ne);
                }
            }
            currOptions = nextOptions;
        }
        // mouse can't win, can cat win or draw
        return R_DRAW;
    }



    string getResultStr(ResultType result) {
        if (result == R_MOUSE) return "Mouse";
        if (result == R_CAT) return "Cat";
        if (result == R_DRAW) return "Draw";
        return "No";
    }

    bool isRepetation(deque<pair<int, int>>& path) {
        if (path.size() < 4) return false;
        if (path.size() > 4) {
            path.pop_front();
        }

        if (path[0].first == path[2].first && path[1].first == path[3].first &&
            path[0].second == path[2].second && path[1].second == path[3].second)
            return true;
        return false;
    }


    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return catMouseGame(nums.mData1);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1 << " }";
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    return out;
}


/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int GenerateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ {1, 3}, { 0 }, { 3 }, { 0, 2} }));
    expected.push_back(1);
    nums.push_back(Input({ {2, 5}, { 3 }, { 0, 4, 5 }, { 1, 4, 5 }, { 2, 3 }, { 0, 2, 3} }));
    expected.push_back(0);
    nums.push_back(Input({ {2, 5}, { 6 }, { 0, 4, 5 }, { 4, 5,6 }, { 2, 3 }, { 0, 2, 3},{1,3} }));
    expected.push_back(2);
    nums.push_back(Input({ {3,4,5}, { 7 }, { 3,4}, { 0,2,4}, { 0, 2, 3,6 }, { 0,7}, {4}, {1,5} }));
    expected.push_back(1);
    nums.push_back(Input({ {3,4,5}, { 7 }, { 3,4}, { 0,2,4}, { 0, 2, 3,5,6 }, { 0,4,7}, {4}, {1,5} }));
    expected.push_back(2);
    nums.push_back(Input({ {3,4,5}, { 7 }, { 3,4}, { 0,2,4}, { 0, 2, 3,5,6 }, { 0,4,7}, {4,7}, {1,5,6} }));
    expected.push_back(2);

    nums.push_back(Input({ {3,4,5}, { 6,7 }, { 3,4}, { 0,2,4}, { 0, 2, 3,5,6 }, { 0,4,7}, {1, 4,7}, {1,5,6} }));
    expected.push_back(0);
    nums.push_back(Input({ {3,4,5}, { 6,7 }, { 3,5}, { 0,2,4}, { 0, 3,5,6 }, { 0,2,4,7}, {1, 4,7}, {1,5,6} }));
    expected.push_back(2);

    nums.push_back(Input({
        {3,4,5}, {10,14}, {5,8}, {0,6},{0,5,7},
        {0,2,4}, {3,9}, {4,10}, {2},{6,11},
        {1,7,11}, {9,10, 12}, {11,13}, {12,14}, {1,13},
        }));
    expected.push_back(1);

    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.ProxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] =>\t"
            << result << " vs e:" << expected[i] << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#ifdef __DEBUG_BIG_TEST


V1Int load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    nums.push_back(
        Input(load_data_v2(t1i1), load_data_int(t1i2))
    );
    expected.push_back(load_data_int(t1e1));

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
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


