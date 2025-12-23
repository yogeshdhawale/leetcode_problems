/*
3367. Maximize Sum of Weights after Edge Removals
Hard
Topics
Companies
Hint

There exists an undirected tree with n nodes numbered 0 to n - 1. You are given a 2D integer array edges of length n - 1, where edges[i] = [ui, vi, wi] indicates that there is an edge between nodes ui and vi with weight wi in the tree.

Your task is to remove zero or more edges such that:

    Each node has an edge with at most k other nodes, where k is given.
    The sum of the weights of the remaining edges is maximized.

Return the maximum possible sum of weights for the remaining edges after making the necessary removals.



Example 1:

Input: edges = [[0,1,4],[0,2,2],[2,3,12],[2,4,6]], k = 2

Output: 22

Explanation:

    Node 2 has edges with 3 other nodes. We remove the edge [0, 2, 2], ensuring that no node has edges with more than k = 2 nodes.
    The sum of weights is 22, and we can't achieve a greater sum. Thus, the answer is 22.

Example 2:

Input: edges = [[0,1,5],[1,2,10],[0,3,15],[3,4,20],[3,5,5],[0,6,10]], k = 3

Output: 65

Explanation:

    Since no node has edges connecting it to more than k = 3 nodes, we don't remove any edges.
    The sum of weights is 65. Thus, the answer is 65.



Constraints:

    2 <= n <= 105
    1 <= k <= n - 1
    edges.length == n - 1
    edges[i].length == 3
    0 <= edges[i][0] <= n - 1
    0 <= edges[i][1] <= n - 1
    1 <= edges[i][2] <= 106
    The input is generated such that edges form a valid tree.


---
format:
    n1    -   n2(weight)
e.g.1
    0   -   1(4)
        -   2(2)
            -   3(12)
            -   4(6)

---
e.g.2
    0   -   1(5)
            -   2(10)
        -   3(15)
            -   4(20)
            -   5(5)
        -   6(10)
e.g.3
    0   -   1(5)
            -   2(10)
        -   3(7)
            -   4(20)
            -   5(5)
        -   6(10)
*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <chrono>
#include <string_view>
#include <numeric>

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
#define MY_MAX_VAL INT32_MAX
#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, WEIGHT, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V2Int INPUT_TYPE;
typedef long long OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    int mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1, int input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};
/*---------------------------------------------------------------------------*/

struct Node {
    IDX id;
    map<WEIGHT, set<IDX>> nbrByWt;
    map<IDX, WEIGHT> nbrList;
    Node(IDX id = -1) : id(id) {
        nbrByWt.clear();
        nbrList.clear();
    }
    void setId(IDX id) {
        this->id = id;
    }
    int getNbrCnt() {
        return nbrList.size();
    }
    void addNbr(IDX other, WEIGHT wt) {
        nbrByWt[wt].insert(other);
        nbrList[other] = wt;
    }
};

struct Graph {

    vector<Node> mNodeList;
    int mThreshold;
    OUTPUT_TYPE mInitialWtSum;
    OUTPUT_TYPE mResultWtSum;

    Graph(int n, V2Int& edges, int k) :mThreshold(k), mInitialWtSum(0), mResultWtSum(0) {
        mNodeList.resize(n);

        for (auto edge : edges) {
            auto v1 = edge[0];
            auto v2 = edge[1];
            auto wt = edge[2];
            addEdge(v1, v2, wt);
            mInitialWtSum += wt;
        }
        LOG("At start, max weight:" << mInitialWtSum);
    }
    void addEdge(IDX v1, IDX v2, WEIGHT wt) {
        mNodeList[v1].addNbr(v2, wt);
        mNodeList[v2].addNbr(v1, wt);
    }

    OUTPUT_TYPE reduceExtraEdgesInGraph() {
        int size = mNodeList.size();
        map<IDX, int> targets;

        for (int i = 0; i < size; i++) {
            LOG("Node:" << i << " nbrCnt:" << mNodeList[i].getNbrCnt());
            if (mNodeList[i].getNbrCnt() > mThreshold) targets[i] = mNodeList[i].getNbrCnt();
        }
        LOG("At start, pending targets:" << targets);

        if (targets.empty()) return mInitialWtSum;

        map<IDX, map< IDX, bool>> selectedEdges;
        while (!targets.empty()) {
            targets = specialDFS(targets, mInitialWtSum, selectedEdges);
        }

        return mResultWtSum;
    }
    map<IDX, int> specialDFS(map<IDX, int>& pendingTargets, OUTPUT_TYPE curWtSum, map<IDX, map< IDX, bool>>& selectedEdges) {
        //LOG("pendingTargets:" << pendingTargets << " curWtSum:" << curWtSum << " mResultWtSum:" << mResultWtSum);
        if (pendingTargets.size() == 0) {
            if (curWtSum > mResultWtSum) {
                mResultWtSum = curWtSum;
                LOG("updating mResultWtSum:" << mResultWtSum << " selectedEdges :" << selectedEdges);
            }
            LOG("At 1: curWtSum:" << curWtSum << " vs " << mResultWtSum << " pendingTargets:" << pendingTargets);
            return pendingTargets;
        }
        if (curWtSum < mResultWtSum) {
            LOG("\tAborting path as curWtSum:" << curWtSum << " vs " << mResultWtSum << " pendingTargets:" << pendingTargets << "selectedEdges :" << selectedEdges);
            return map<IDX, int>();
        }

        map<IDX, int> cur_pts = pendingTargets;
        for (auto [n1, n1cnt] : pendingTargets) {
            for (auto [wt, list] : mNodeList[n1].nbrByWt) {
                for (auto n2 : list) {
                    int n2cnt = -1;
                    if (selectedEdges.find(n1) != selectedEdges.end() && selectedEdges[n1].find(n2) != selectedEdges[n1].end()) continue;
                    if (selectedEdges.find(n2) != selectedEdges.end() && selectedEdges[n2].find(n1) != selectedEdges[n2].end()) continue;

                    selectedEdges[n1][n2] = true;
                    cur_pts[n1]--;
                    if (cur_pts[n1] == mThreshold) cur_pts.erase(n1);
                    if (cur_pts.find(n2) != cur_pts.end()) {
                        n2cnt = cur_pts[n2];
                        cur_pts[n2]--;
                        if (cur_pts[n2] == mThreshold) cur_pts.erase(n2);
                    }

                    LOG("\tselected edge:( " << n1 << " - " << n2 << " ): " << wt << " curWtSum:" << curWtSum << " vs " << mResultWtSum << " pendingTargets:" << cur_pts);

                    map<IDX, int> ret_pts = specialDFS(cur_pts, curWtSum - wt, selectedEdges);
                    while (!ret_pts.empty()) {
                        ret_pts = specialDFS(cur_pts, curWtSum - wt, selectedEdges);
                    }

                    cur_pts[n1] = n1cnt;
                    if (n2cnt != -1) cur_pts[n2] = n2cnt;
                    selectedEdges[n1].erase(n2);
                    if (selectedEdges[n1].empty()) selectedEdges.erase(n1);

                }
            }
        }
        return map<IDX, int>();
    }
};

class Solution {
public:

    Solution() {}

    long long maximizeSumOfWeights(vector<vector<int>>& edges, int k) {

        Graph graph(edges.size() + 1, edges, k);
        return graph.reduceExtraEdgesInGraph();
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return maximizeSumOfWeights(nums.mData1, nums.mData2);
    }
};

/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " k:" << ipObj.mData2;
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
        nums.back().setInput(
            { {0, 1, 4},{0, 2, 2},{2, 3, 12},{2, 4, 6} },
            2);
        nums.back().setOutput(22);
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { {0, 1, 5},{1, 2, 10},{0, 3, 15},{3, 4, 20},{3, 5, 5},{0, 6, 10} },
            3);
        nums.back().setOutput(65);
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { {0, 1, 5},{1, 2, 10},{0, 3, 15},{3, 4, 20},{3, 5, 5},{0, 6, 10} },
            2);
        nums.back().setOutput(55);
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { {0, 1, 5},{1, 2, 10},{0, 3, 5},{3, 4, 20},{3, 5, 5},{0, 6, 10} },
            2);
        nums.back().setOutput(50);
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { {0, 1, 5},{1, 2, 10},{0, 3, 7},{3, 4, 20},{3, 5, 5},{0, 6, 10} },
            2);
        nums.back().setOutput(50);
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { {0, 1, 15},{1, 2, 30},{1, 3, 3} },
            1);
        nums.back().setOutput(30);
    } {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { {0, 3, 49},{3, 4, 21},{3, 1, 47},{1, 2, 34} },
            1);
        nums.back().setOutput(83);
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
        nums.back().setInput(load_data_v2(t1i1), 1);
        nums.back().setOutput(1);
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


