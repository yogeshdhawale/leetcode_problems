
/*
3108. Minimum Cost Walk in Weighted Graph
Hard
Topics
Companies
Hint

There is an undirected weighted graph with n vertices labeled from 0 to n - 1.

You are given the integer n and an array edges, where edges[i] = [ui, vi, wi] indicates that there is an edge between vertices ui and vi with a weight of wi.

A walk on a graph is a sequence of vertices and edges. The walk starts and ends with a vertex, and each edge connects the vertex that comes before it and the vertex that comes after it. It's important to note that a walk may visit the same edge or vertex more than once.

The cost of a walk starting at node u and ending at node v is defined as the bitwise AND of the weights of the edges traversed during the walk. In other words, if the sequence of edge weights encountered during the walk is w0, w1, w2, ..., wk, then the cost is calculated as w0 & w1 & w2 & ... & wk, where & denotes the bitwise AND operator.

You are also given a 2D array query, where query[i] = [si, ti]. For each query, you need to find the minimum cost of the walk starting at vertex si and ending at vertex ti. If there exists no such walk, the answer is -1.

Return the array answer, where answer[i] denotes the minimum cost of a walk for query i.



Example 1:

Input: n = 5, edges = [[0,1,7],[1,3,7],[1,2,1]], query = [[0,3],[3,4]]

Output: [1,-1]

Explanation:

To achieve the cost of 1 in the first query, we need to move on the following edges: 0->1 (weight 7), 1->2 (weight 1), 2->1 (weight 1), 1->3 (weight 7).

In the second query, there is no walk between nodes 3 and 4, so the answer is -1.

Example 2:

Input: n = 3, edges = [[0,2,7],[0,1,15],[1,2,6],[1,2,1]], query = [[1,2]]

Output: [0]

Explanation:

To achieve the cost of 0 in the first query, we need to move on the following edges: 1->2 (weight 1), 2->1 (weight 6), 1->2 (weight 1).



Constraints:

    2 <= n <= 105
    0 <= edges.length <= 105
    edges[i].length == 3
    0 <= ui, vi <= n - 1
    ui != vi
    0 <= wi <= 105
    1 <= query.length <= 105
    query[i].length == 2
    0 <= si, ti <= n - 1
    si != ti


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
#define __DEBUG_BIG_TEST
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
#define MY_MAX_VAL 100001
#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, WEIGHT, IDXR, IDXC;
typedef pair<IDX, WEIGHT> WALK;
/*---------------------------------------------------------------------------*/
typedef V2Int INPUT_TYPE;
typedef V1Int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;

    int mData1;
    INPUT_TYPE mData2;
    INPUT_TYPE mData3;

    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(int input1, INPUT_TYPE input2, INPUT_TYPE input3) {
        this->mData1 = input1;
        this->mData2 = input2;
        this->mData3 = input3;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/


struct Graph {

    vector<int> node2gid;
    vector < unordered_map<IDX, WEIGHT>> gMap;
    vector<WEIGHT> groups;

    void clear() {
        gMap.clear();
        groups.clear();
        node2gid.clear();
    }
    Graph(int n, V2Int& edges) {
        clear();
        gMap.resize(n, unordered_map<IDX, WEIGHT>());
        node2gid.resize(n, -1);
        for (auto edge : edges) {
            this->addEdge(edge[0], edge[1], edge[2]);
        }

        // add group for singles
        groups.push_back(-1);

    }
    void addEdge(int s, int e, int wt) {

        if (gMap[s].find(e) == gMap[s].end()) gMap[s][e] = wt;
        else gMap[s][e] = gMap[s][e] & wt;

        if (gMap[e].find(s) == gMap[e].end()) gMap[e][s] = wt;
        else gMap[e][s] = gMap[e][s] & wt;
    }

    inline bool noNeighbours(int node) {
        if (gMap[node].size() == 0) return true;
        return false;
    }


    void addNeighborsToQ(int node, queue<WALK>& q) {
        if (noNeighbours(node)) return;

        for (auto entry : gMap[node])
            q.push({ entry.first, entry.second });
    }
    void processAllConnectedGroups() {

        while (true) {

            auto node = -1;
            for (int i = 0; i < node2gid.size(); i++) {
                if (node2gid[i] == -1) {
                    node = i;
                    break;
                }
            }
            if (node == -1) break;

            if (noNeighbours(node)) {
                node2gid[node] = 0;
                LOG("Node " << node << " has no neighbours");
                continue;
            }

            unordered_map<IDX, bool> neighboursAdded;
            unordered_map<WEIGHT, bool> edgeWeights;
            int visitedCnt = 0;
            queue<WALK> q;

            addNeighborsToQ(node, q);

            while (!q.empty()) {
                auto cur = q.front();
                q.pop();
                auto curNode = cur.first;
                auto curCost = cur.second;

                edgeWeights[curCost] = true;
                if (neighboursAdded[curNode]) continue;

                visitedCnt++;
                neighboursAdded[curNode] = true;
                addNeighborsToQ(curNode, q);
            }


            auto group_id = groups.size();
            int group_wt = edgeWeights.begin()->first;
            for (auto [edgeWeight, _] : edgeWeights) {
                group_wt &= edgeWeight;
            }
            for (auto [node, _] : neighboursAdded) {
                node2gid[node] = group_id;
            }
            groups.push_back(group_wt);

            LOG("cc_id: " << group_id << "=" << group_wt
                << "\twith size:" << visitedCnt << " and nodes:" << neighboursAdded);
        }
    }
    int getCost(int start, int end) {
        if (node2gid[start] != node2gid[end]) return -1;
        auto groupid = node2gid[start];
        return groups[groupid];
    }
};

class Solution {
public:

    Solution() {}

    vector<int> minimumCost(int n, vector<vector<int>>& edges, vector<vector<int>>& query) {

        Graph mGraph(n, edges);
        mGraph.processAllConnectedGroups();

        V1Int result;
        for (auto q : query) {
            auto ret = mGraph.getCost(q[0], q[1]);
            LOG("Processing query " << q << " returned:" << ret);
            result.push_back(ret);
        }
        return result;
    }


    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return minimumCost(nums.mData1, nums.mData2, nums.mData3);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1 << " }";
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
        nums.back().setInput(5,
            { {0, 1, 7},{1, 3, 7},{1, 2, 1} },
            { {0, 3},{3, 4} });
        nums.back().setOutput({ 1,-1 });
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(3,
            { {0, 2, 7},{0, 1, 15},{1, 2, 6},{1, 2, 1} },
            { { 1, 2 } });
        nums.back().setOutput({ 0 });
    }

    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test:" << testID << " => " << nums[i] << endl;
        gCurTestId = nums[i].mID;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << " => " << nums[i].mID << " returns: \t"
            << result << " vs e:" << nums[i].mExpected1 << endl;
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
        nums.back().setInput(100000, load_data_v2(t1i1), load_data_v2(t1i2));
        //nums.back().setOutput(load_data_v1<int>(t1e1));
        nums.back().setOutput({});
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


