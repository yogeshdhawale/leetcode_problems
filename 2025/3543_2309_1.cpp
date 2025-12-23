
/*

3543. Maximum Weighted K-Edge Path
Medium
Topics
premium lock iconCompanies
Hint

You are given an integer n and a Directed Acyclic Graph (DAG) with n nodes labeled from 0 to n - 1. This is represented by a 2D array edges, where edges[i] = [ui, vi, wi] indicates a directed edge from node ui to vi with weight wi.

You are also given two integers, k and t.

Your task is to determine the maximum possible sum of edge weights for any path in the graph such that:

    The path contains exactly k edges.
    The total sum of edge weights in the path is strictly less than t.

Return the maximum possible sum of weights for such a path. If no such path exists, return -1.



Example 1:

Input: n = 3, edges = [[0,1,1],[1,2,2]], k = 2, t = 4

Output: 3

Explanation:

    The only path with k = 2 edges is 0 -> 1 -> 2 with weight 1 + 2 = 3 < t.
    Thus, the maximum possible sum of weights less than t is 3.

Example 2:

Input: n = 3, edges = [[0,1,2],[0,2,3]], k = 1, t = 3

Output: 2

Explanation:

    There are two paths with k = 1 edge:
        0 -> 1 with weight 2 < t.
        0 -> 2 with weight 3 = t, which is not strictly less than t.
    Thus, the maximum possible sum of weights less than t is 2.

Example 3:

Input: n = 3, edges = [[0,1,6],[1,2,8]], k = 1, t = 6

Output: -1

Explanation:

    There are two paths with k = 1 edge:
        0 -> 1 with weight 6 = t, which is not strictly less than t.
        1 -> 2 with weight 8 > t, which is not strictly less than t.
    Since there is no path with sum of weights strictly less than t, the answer is -1.



Constraints:

    1 <= n <= 300
    0 <= edges.length <= 300
    edges[i] = [ui, vi, wi]
    0 <= ui, vi < n
    ui != vi
    1 <= wi <= 10
    0 <= k <= 300
    1 <= t <= 600
    The input graph is guaranteed to be a DAG.
    There are no duplicate edges.



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
typedef V2Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    int mData0;
    INPUT_TYPE mData1;
    int mData2;
    int mData3;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(int input0, INPUT_TYPE input1, int input2, int input3) {
        this->mData1 = input1;
        this->mData0 = input0;
        this->mData2 = input2;
        this->mData3 = input3;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/

class Solution {

    int n;
    vector<map<int, int>> graph;
    int maxPathSumAllowed;
    int resultMaxPathSum;

public:

    Solution() {}
    void reset(int n) {
        this->n = n;
        graph = vector<map<int, int>>(n, map<int, int>());

        this->maxPathSumAllowed = 0;
        this->resultMaxPathSum = 0;
    }

    int maxWeight(int n, vector<vector<int>>& edges, int k, int t) {

        reset(n);
        this->maxPathSumAllowed = t;

        if (k == 0) return 0;
        if (n == 1 || edges.size() == 0) return -1;

        for (auto edge : edges) {
            auto n1 = edge[0];
            auto n2 = edge[1];
            auto wt = edge[2];

            graph[n1][n2] = wt;
        }

        for (int i = 0;i < n;i++) {

            LOG("Starting visiting for " << i << " maxsum=" << resultMaxPathSum);
            vector<bool> visited(n, false);
            calculateDFS(i, k, visited, 0);

        }

        LOG("At end maxsum=" << resultMaxPathSum);

        if (resultMaxPathSum == 0) return -1;
        return resultMaxPathSum;
    }

    void calculateDFS(int nid, int pendingCntEdges, vector<bool>& visited, int curPathSum) {

        if (pendingCntEdges == 0) {
            LOG("path ended in " << nid << " with sum=" << curPathSum);
            resultMaxPathSum = max(resultMaxPathSum, curPathSum);
            return;
        }

        visited[nid] = true;
        for (auto& [nextNode, weight] : graph[nid]) {
            if (visited[nextNode]) continue;
            if (curPathSum + weight >= maxPathSumAllowed) continue;
            calculateDFS(nextNode, pendingCntEdges - 1, visited, curPathSum + weight);
        }
        visited[nid] = false;

        return;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return maxWeight(nums.mData0, nums.mData1, nums.mData2, nums.mData3);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " n:" << ipObj.mData0;
    out << " nums:" << ipObj.mData1;
    out << " k:" << ipObj.mData2;
    out << " t:" << ipObj.mData3;
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
        nums.back().setInput(3,
            { {0,1,1}, {1,2,2} },
            2, 4);
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(3,
            { {0,1,2}, {0,2,3} },
            1, 3);
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(3,
            { {0,1,6}, {1,2,8} },
            1, 6);
        nums.back().setOutput(-1);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(3,
            { {0,1,8}, {0,2,8}, {1,2,9} },
            1, 511);
        nums.back().setOutput(9);
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
        nums.back().setInput(300, load_data_v2(t1i1), 10, 11);
        nums.back().setOutput(10);
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


