
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

struct Node {
    IDX id;
    map<WEIGHT, map<IDX, bool>> nbrByWt;
    int nbrCnt;

    Node(IDX id = -1) : id(id) {
        nbrCnt = 0;
        nbrByWt.clear();
    }

    void setId(IDX id) {
        this->id = id;
    }

    int getNbrCnt() {
        return nbrCnt;
    }

    void addNbr(IDX other, WEIGHT wt) {
        nbrByWt[wt][other] = true;
        nbrCnt++;
    }
    void removeNbr(IDX other, WEIGHT wt) {
        nbrByWt[wt].erase(other);
        nbrCnt--;
        if (nbrByWt[wt].size() == 0) nbrByWt.erase(wt);
    }
};

struct extraEdgesMap {

    // key: extra edges count (nbrCnt - threshold)
    // value: List<nodeId> whoes extra edges count is key
    map<int, map<IDX, int>> mEEInfo;
    extraEdgesMap() {
        mEEInfo.clear();
    }

    bool empty() {
        return mEEInfo.empty();
    }
    void addEntry(IDX n1, int  cnt) {
        if (cnt <= 0) return;
        mEEInfo[cnt][n1] = true;
    }
    void removeEntry(IDX n1, int cnt) {
        mEEInfo[cnt].erase(n1);
        if (mEEInfo[cnt].size() == 0) mEEInfo.erase(cnt);
    }
};

struct Graph {

    vector<Node> mNodeList;
    int mTotalNodeCnt;
    int mThreshold;
    OUTPUT_TYPE mInitialWtSum;
    extraEdgesMap mEEMap;

    Graph(int n, V2Int& edges, int k) {
        mTotalNodeCnt = n;
        mInitialWtSum = 0;
        mThreshold = k;

        mNodeList.resize(mTotalNodeCnt);

        for (int i = 0; i < mTotalNodeCnt; i++) mNodeList[i].setId(i);

        for (auto edge : edges) {
            auto v1 = edge[0];
            auto v2 = edge[1];
            auto wt = edge[2];
            addEdge(v1, v2, wt);
            mInitialWtSum += wt;
        }

        for (int i = 0; i < mTotalNodeCnt; i++)
            mEEMap.addEntry(i, mNodeList[i].getNbrCnt() - mThreshold);


        //LOG("At start, tree:" << tree);
        LOG("At start, max weight:" << mInitialWtSum);

    }
    OUTPUT_TYPE getWeightSum() {
        return mInitialWtSum;
    }

    void addEdge(IDX n1, IDX n2, WEIGHT wt) {
        mNodeList[n1].addNbr(n2, wt);
        mNodeList[n2].addNbr(n1, wt);
    }
    void removeEdge(IDX n1, IDX n2, WEIGHT wt) {

        mEEMap.removeEntry(n1, mNodeList[n1].getNbrCnt() - mThreshold);
        mEEMap.removeEntry(n2, mNodeList[n2].getNbrCnt() - mThreshold);
        mNodeList[n1].removeNbr(n2, wt);
        mNodeList[n2].removeNbr(n1, wt);
        mEEMap.addEntry(n1, mNodeList[n1].getNbrCnt() - mThreshold);
        mEEMap.addEntry(n2, mNodeList[n2].getNbrCnt() - mThreshold);
    }

    pair<IDX, WEIGHT> selectEdgeForRemoval(IDX n1) {
        pair<IDX, WEIGHT> ret = { -1,-1 };
        ret.second = mNodeList[n1].nbrByWt.begin()->first;
        auto& options = mNodeList[n1].nbrByWt.begin()->second;
        auto n2 = options.begin()->first;

        for (auto itr = options.begin(); itr != options.end(); itr++) {
            auto onode = itr->first;
            if (mNodeList[onode].getNbrCnt() > mThreshold) {
                n2 = onode;
                break;
            }
        }
        ret.first = n2;
        return ret;
    }

    void removeReverseEdgeOnly(IDX n1, IDX n2, WEIGHT wt) {
        mNodeList[n2].removeNbr(n1, wt);
    }

    void removeAllEdges(IDX nid) {
        auto& node = mNodeList[nid];
        while (node.nbrByWt.size() > 0) {
            auto wt = node.nbrByWt.begin()->first;
            auto nlist = node.nbrByWt.begin()->second;
            for (auto [n2, _] : nlist)
                removeReverseEdgeOnly(nid, n2, wt);
            node.nbrByWt.erase(node.nbrByWt.begin());
        }
    }
    void reduceExtraEdgesInGraph() {

        for (auto& node : mNodeList) {

            if (mEEMap.empty()) return;

            bool isCurNodeWithIssue = node.getNbrCnt() > mThreshold;
            bool isNbrWithIssue = isNodeWithIssue(node);

            if (!isCurNodeWithIssue && !isNbrWithIssue) {
                LOG("\tRemoving all edges for node:" << node.id);
                removeAllEdges(node.id);
                continue;
            }
            else if (isCurNodeWithIssue && !isNbrWithIssue) {
                // neighbors with no extra edges, just remove N edges
                int edgesTobeRemoved = node.getNbrCnt() - mThreshold;
                mEEMap.removeEntry(node.id, node.getNbrCnt() - mThreshold);
                for (;edgesTobeRemoved > 0; edgesTobeRemoved--) {
                    auto wt = node.nbrByWt.begin()->first;
                    auto n2 = node.nbrByWt.begin()->second.begin()->first;
                    removeEdge(node.id, n2, wt);
                    mInitialWtSum -= wt;
                }
                LOG("\tRemoving all edges for node after reducing wt:" << node.id << " new weight:" << mInitialWtSum);
                removeAllEdges(node.id);
                continue;
            }
        }

        LOG("At 2nd: max weight:" << mInitialWtSum);

        while (true) {
            if (mEEMap.empty()) break;
            auto nid = mEEMap.mEEInfo.begin()->second.begin()->first;
            auto ret = selectEdgeForRemoval(nid);
            LOG("\tRemoved edge between " << nid << " to " << ret.first << " with weight " << ret.second);
            removeEdge(nid, ret.first, ret.second);
            mInitialWtSum -= ret.second;
        }
    }
    bool isNodeWithIssue(Node& node) {
        for (auto itr = node.nbrByWt.begin(); itr != node.nbrByWt.end(); itr++) {
            for (auto nbr = itr->second.begin(); nbr != itr->second.end(); nbr++) {
                if (mNodeList[nbr->first].getNbrCnt() <= mThreshold) continue;
                return true;
            }
        }
        return false;
    }
};
/*---------------------------------------------------------------------------*/

class Solution {
public:

    Solution() {}

    long long maximizeSumOfWeights(vector<vector<int>>& edges, int k) {

        Graph graph(edges.size() + 1, edges, k);
        graph.reduceExtraEdgesInGraph();
        return graph.getWeightSum();
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


