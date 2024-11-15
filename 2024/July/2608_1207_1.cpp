
/*

2608. Shortest Cycle in a Graph
Hard
Topics
Companies
Hint

There is a bi-directional graph with n vertices, where each vertex is labeled from 0 to n - 1. The edges in the graph are represented by a given 2D integer array edges, where edges[i] = [ui, vi] denotes an edge between vertex ui and vertex vi. Every vertex pair is connected by at most one edge, and no vertex has an edge to itself.

Return the length of the shortest cycle in the graph. If no cycle exists, return -1.

A cycle is a path that starts and ends at the same node, and each edge in the path is used only once.



Example 1:

Input: n = 7, edges = [[0,1],[1,2],[2,0],[3,4],[4,5],[5,6],[6,3]]
Output: 3
Explanation: The cycle with the smallest length is : 0 -> 1 -> 2 -> 0

Example 2:

Input: n = 4, edges = [[0,1],[0,2]]
Output: -1
Explanation: There are no cycles in this graph.



Constraints:

    2 <= n <= 1000
    1 <= edges.length <= 1000
    edges[i].length == 2
    0 <= ui, vi < n
    ui != vi
    There are no repeated edges.



*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

#include "log.h"

using namespace std;

/****************************************************************************/

#include <ranges>

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
typedef pair<int, int> IDX2D;

typedef V2Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    int n;
    INPUT_TYPE nums;

    Input() {
        id = inputID++;
    }
    Input(int n, INPUT_TYPE nums) : n(n), nums(nums) {
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data;
struct Node;

ostream& operator<<(ostream& out, const Data& d);
ostream& operator<<(ostream& out, shared_ptr<Node> node);

struct Node {

    int id;
    map<int, bool> neighbors;
    void clear() {
        neighbors.clear();
    }

    Node(int id = INVALID) : id(id) {
        clear();
    }

    void AddNeighbor(int other) {
        neighbors[other] = true;
    }
};
struct Data {
    int totalNodes;
    int visitedCount;
    int minLoopSize;

    map<int, shared_ptr<Node>> graph;
    vector<int> visitedDepth;
    void clear() {
        totalNodes = 0;
        visitedCount = 0;
        minLoopSize = 0;

        graph.clear();
        visitedDepth.clear();
    }
    Data(int n, vector<vector<int>>& edges) {
        clear();

        totalNodes = n;
        visitedCount = 0;
        minLoopSize = n + 1;

        visitedDepth.resize(totalNodes, 0);
        for (int i = 0; i < totalNodes; ++i) {
            graph[i] = make_shared<Node>(i);
        }

        for (auto& edge : edges) {
            graph[edge[0]]->AddNeighbor(edge[1]);
            graph[edge[1]]->AddNeighbor(edge[0]);
        }
        LOG("Graph: " << endl << graph << endl);
    }

    bool allVisited() {
        return visitedCount == totalNodes;
    }
    int getAvailableNode() {
        for (int i = 0; i < totalNodes; ++i) {
            if (visitedDepth[i] == 0) {
                return i;
            }
        }
        return INVALID;
    }
    int getLoopSize() {
        int nodeId = getAvailableNode();
        if (nodeId == INVALID) return minLoopSize;

        LOG("Searching @: " << nodeId);
        dfs(nodeId, INVALID);

        LOG("VisitedStatus: " << visitedCount << " -> " << visitedDepth);
        LOG("Ending @: " << nodeId << "\tfound minLoopSize: " << minLoopSize);

        return minLoopSize;
    }

    bool isVisited(int nodeId) {
        return visitedDepth[nodeId] != 0;
    }
    void dfs(int curNodeId, int prevNodeId) {

        visitedDepth[curNodeId] = (prevNodeId == INVALID) ? 1 : visitedDepth[prevNodeId] + 1;
        visitedCount += 1;

        for (auto& [nextNodeId, _] : graph[curNodeId]->neighbors) {

            if (minLoopSize == 3) break;
            if (nextNodeId == prevNodeId) continue;

            if (isVisited(nextNodeId)) {
                auto currentLoopSize = abs(visitedDepth[curNodeId] - visitedDepth[nextNodeId]) + 1;
                LOG("\tLoop detected: " << curNodeId << " -> " << nextNodeId << " with depth: " << currentLoopSize);
                minLoopSize = min(minLoopSize, currentLoopSize);
                continue;
            }

            dfs(nextNodeId, curNodeId);
        }
    }
};

class Solution {
public:

    int findShortestCycle(int n, vector<vector<int>>& edges) {

        if (n <= 2) return -1;

        Data data(n, edges);

        int retVal = n + 1;
        while (data.allVisited() == false) {
            int curLoopSize = data.getLoopSize();
            if (curLoopSize != INVALID) retVal = curLoopSize;
            if (retVal == 3) return retVal;
        }

        return retVal == n + 1 ? -1 : retVal;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return findShortestCycle(nums.n, nums.nums);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " n:" << nums.n;
    out << " nums:" << nums.nums;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    return out;
}

ostream& operator<<(ostream& out, shared_ptr<Node> node) {
    out << " -> {";
    for (auto& [key, _] : node->neighbors) out << key << ",";
    out << "}";

    return out;
}

/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << TEXT_LOG_PREFIX << msg
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

    nums.push_back(Input(4, { { 0, 1 }, { 0, 2 } }));
    expected.push_back(-1);

    nums.push_back(Input(7, { { 0, 1 }, { 1, 2 }, { 2, 0 }, { 3, 4 }, { 4, 5 }, { 5, 6 }, { 6, 3 } }));
    expected.push_back(3);

    nums.push_back(Input(10, { {0,4}, {4,8}, {8,3}, {3,5}, {5,7}, {7,2}, {2,9}, {9,6} , {5,9}, {0,9} }));
    expected.push_back(4);

    nums.push_back(Input(10, { {0,4}, {4,8}, {8,3}, {3,5}, {5,7}, {7,2}, {2,9}, {1,6} , {5,9}, {0,9} }));
    expected.push_back(4);

    nums.push_back(Input(6, { {4, 1}, {5, 1}, {3, 2}, {5, 0}, {4, 0}, {3, 0}, {2, 1} }));
    expected.push_back(4);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = __D_CASE_ID; i <= __D_CASE_ID; i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
        logtime("*****Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG

#include "data.h"

void test2(int testID)
{

    cout << TEXT_LOG_PREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input(d1));
    expected.push_back(o1);

    cout << TEXT_LOG_PREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << TEXT_LOG_PREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("*****Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG
    test2(2);
#endif

    return 0;
}


