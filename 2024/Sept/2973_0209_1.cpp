
/*
2973. Find Number of Coins to Place in Tree Nodes
Hard
Topics
Companies
Hint

You are given an undirected tree with n nodes labeled from 0 to n - 1, and rooted at node 0. You are given a 2D integer array edges of length n - 1, where edges[i] = [ai, bi] indicates that there is an edge between nodes ai and bi in the tree.

You are also given a 0-indexed integer array cost of length n, where cost[i] is the cost assigned to the ith node.

You need to place some coins on every node of the tree. The number of coins to be placed at node i can be calculated as:

    If size of the subtree of node i is less than 3, place 1 coin.
    Otherwise, place an amount of coins equal to the maximum product of cost values assigned to 3 distinct nodes in the subtree of node i. If this product is negative, place 0 coins.

Return an array coin of size n such that coin[i] is the number of coins placed at node i.



Example 1:

Input: edges = [[0,1],[0,2],[0,3],[0,4],[0,5]], cost = [1,2,3,4,5,6]
Output: [120,1,1,1,1,1]
Explanation: For node 0 place 6 * 5 * 4 = 120 coins. All other nodes are leaves with subtree of size 1, place 1 coin on each of them.

Example 2:

Input: edges = [[0,1],[0,2],[1,3],[1,4],[1,5],[2,6],[2,7],[2,8]], cost = [1,4,2,3,5,7,8,-4,2]
Output: [280,140,32,1,1,1,1,1,1]
Explanation: The coins placed on each node are:
- Place 8 * 7 * 5 = 280 coins on node 0.
- Place 7 * 5 * 4 = 140 coins on node 1.
- Place 8 * 2 * 2 = 32 coins on node 2.
- All other nodes are leaves with subtree of size 1, place 1 coin on each of them.

Example 3:

Input: edges = [[0,1],[0,2]], cost = [1,2,-2]
Output: [0,1,1]
Explanation: Node 1 and 2 are leaves with subtree of size 1, place 1 coin on each of them. For node 0 the only possible product of cost is 2 * 1 * -2 = -4. Hence place 0 coins on node 0.



Constraints:

    2 <= n <= 2 * 104
    edges.length == n - 1
    edges[i].length == 2
    0 <= ai, bi < n
    cost.length == n
    1 <= |cost[i]| <= 104
    The input is generated such that edges represents a valid tree.


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

typedef V1Int INPUT_TYPE;
typedef vector<long long> OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    V2Int mData1;
    V1Int mData2;

    Input() {
        mID = inputID++;
    }
    Input(V2Int d1, V1Int d2) {
        this->mData1 = d1;
        this->mData2 = d2;
        mID = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};


typedef int NODE_ID;
typedef int COST;
typedef int DEPTH;

struct node {
    NODE_ID id;
    COST cost;
    int depth;
    NODE_ID parent;
    vector<NODE_ID> children;
    deque<COST> positiveMaxNu;
    deque<COST> negativeMaxNu;

    node() {
        id = INVALID;
        depth = INVALID;
        parent = INVALID;
        cost = 0;
    }
    void Init(NODE_ID id, int cost) {
        this->id = id;
        this->cost = cost;
        if (cost > 0) this->positiveMaxNu.push_back(cost);
        else this->negativeMaxNu.push_back(cost);
    }
};

struct Data;
ostream& operator<<(ostream& out, const node& n);
ostream& operator<<(ostream& out, const Data& d);

struct Data {

    void clear() {
    }
    Data() {
        clear();
    }
};

class Solution {
public:

    vector<node> tree;
    vector<long long> placedCoins(vector<vector<int>>& edges, vector<int>& cost) {

        if (cost.size() == 1) return { 1 };
        if (cost.size() == 2) return { 1, 1 };

        tree.clear();
        tree.resize(cost.size());
        for (int i = 0; i < cost.size(); i++) {
            tree[i].Init(i, cost[i]);
        }

        for (int i = 0; i < edges.size(); i++) {
            tree[edges[i][0]].children.push_back(edges[i][1]);
            tree[edges[i][1]].children.push_back(edges[i][0]);
        }

        map<int, vector<int>> levels;

        queue<NODE_ID> q;
        q.push(0);
        int level = 0;

        while (!q.empty()) {
            int size = q.size();
            level++;
            while (size--) {
                NODE_ID idx = q.front();
                q.pop();

                if (tree[idx].depth != INVALID) continue;
                tree[idx].depth = level;
                levels[level].push_back(idx);

                for (auto n : tree[idx].children) {
                    if (tree[n].parent == INVALID) {
                        tree[n].parent = idx;
                        q.push(n);
                    }
                }
            }
        }
        tree[0].parent = INVALID;
        LOG("Info:" << tree);
        vector<long long> result(cost.size(), 1);
        for (auto itr = levels.rbegin(); itr != levels.rend(); ++itr) {
            LOG("Level: " << itr->first << " nodes:" << itr->second);
            for (auto node : itr->second) {
                result[node] = updateResult(node);
                auto parent = tree[node].parent;
                if (parent != INVALID) AddCost(parent, node);
            }
        }

        LOG("At End:" << tree);
        LOG(result);
        return result;
    }
    long long updateResult(NODE_ID idx) {

        if ((tree[idx].positiveMaxNu.size() + tree[idx].negativeMaxNu.size()) < 3) return 1;

        // only -ve numbers
        if (tree[idx].positiveMaxNu.size() == 0) return 0;

        long long pResult = 1;
        for (auto c : tree[idx].positiveMaxNu) pResult *= c;

        // only +ve numbers
        if (tree[idx].negativeMaxNu.size() == 0) return pResult;

        if (tree[idx].negativeMaxNu.size() >= 2) {
            long long nResult = tree[idx].negativeMaxNu[0] * tree[idx].negativeMaxNu[1];
            nResult = nResult * tree[idx].positiveMaxNu.back();
            if (tree[idx].positiveMaxNu.size() < 3) return nResult;
            return pResult > nResult ? pResult : nResult;
        }
        if (tree[idx].positiveMaxNu.size() >= 3)return pResult;
        return 0;
    }

    int getCost(NODE_ID idx) {
        return tree[idx].cost;
    }
    void AddCost(NODE_ID parent, NODE_ID child) {
        for (auto c : tree[child].positiveMaxNu) tree[parent].positiveMaxNu.push_back(c);
        for (auto c : tree[child].negativeMaxNu) tree[parent].negativeMaxNu.push_back(c);

        sort(tree[parent].positiveMaxNu.begin(), tree[parent].positiveMaxNu.end());
        sort(tree[parent].negativeMaxNu.begin(), tree[parent].negativeMaxNu.end());

        while (tree[parent].positiveMaxNu.size() > 3) tree[parent].positiveMaxNu.pop_front();
        while (tree[parent].negativeMaxNu.size() > 3) tree[parent].negativeMaxNu.pop_back();
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return placedCoins(nums.mData1, nums.mData2);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const node& n) {
    out << "id:" << n.id << " cost:" << n.cost << " parent:" << n.parent << " depth:" << n.depth << " children:" << n.children << " maxPCost:" << n.positiveMaxNu << " maxNCost:" << n.negativeMaxNu;
    out << endl;
    return out;
}

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "id:" << ipObj.mID;
    out << " edges:" << ipObj.mData1;
    out << " cost:" << ipObj.mData2;
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

    nums.push_back(Input({ {0,1},{0,2},{0,3},{0,4},{0,5} }, { 1,2,3,4,5,6 }));
    expected.push_back({ 120,1,1,1,1,1 });

    nums.push_back(Input({ { 0, 1 }, { 0,2 }, { 1,3 }, { 1,4 }, { 1,5 }, { 2,6 }, { 2,7 }, { 2,8 } },
        { 1,4,2,3,5,7,8,-4,2 }));
    expected.push_back({ 280,140,32,1,1,1,1,1,1 });

    nums.push_back(Input({ {0,1},{0,2} }, { 1,2,-2 }));
    expected.push_back({ 0,1,1 });

    nums.push_back(Input({ { 0, 1 }, { 0,2 }, { 0,3 }, { 0,4 }, { 0,5 }, { 0,6 }, { 0,7 }, { 0,8 }, { 0,9 }, { 0,10 }, { 0,11 }, { 0,12 }, { 0,13 }, { 0,14 }, { 0,15 }, { 0,16 }, { 0,17 }, { 0,18 }, { 0,19 }, { 0,20 }, { 0,21 }, { 0,22 }, { 0,23 }, { 0,24 }, { 0,25 }, { 0,26 }, { 0,27 }, { 0,28 }, { 0,29 }, { 0,30 }, { 0,31 }, { 0,32 }, { 0,33 }, { 0,34 }, { 0,35 }, { 0,36 }, { 0,37 }, { 0,38 }, { 0,39 }, { 0,40 }, { 0,41 }, { 0,42 }, { 0,43 }, { 0,44 }, { 0,45 }, { 0,46 }, { 0,47 }, { 0,48 }, { 0,49 }, { 0,50 }, { 0,51 }, { 0,52 }, { 0,53 }, { 0,54 }, { 0,55 }, { 0,56 }, { 0,57 }, { 0,58 }, { 0,59 }, { 0,60 }, { 0,61 }, { 0,62 }, { 0,63 }, { 0,64 }, { 0,65 }, { 0,66 }, { 0,67 }, { 0,68 }, { 0,69 }, { 0,70 }, { 0,71 }, { 0,72 }, { 0,73 }, { 0,74 }, { 0,75 }, { 0,76 }, { 0,77 }, { 0,78 }, { 0,79 }, { 0,80 }, { 0,81 }, { 0,82 }, { 0,83 }, { 0,84 }, { 0,85 }, { 0,86 }, { 0,87 }, { 0,88 }, { 0,89 }, { 0,90 }, { 0,91 }, { 0,92 }, { 0,93 }, { 0,94 }, { 0,95 }, { 0,96 }, { 0,97 }, { 0,98 }, { 0,99 } },
        { -5959,602,-6457,7055,-1462,6347,7226,-8422,-6088,2997,-7909,6433,5217,3294,-3792,7463,8538,-3811,5009,151,5659,4458,-1702,-1877,2799,9861,-9668,-1765,2181,-8128,7046,9529,6202,-8026,6464,1345,121,1922,7274,-1227,-9914,3025,1046,-9368,-7368,6205,-6342,8091,-6732,-7620,3276,5136,6871,4823,-1885,-4005,-3974,-2725,-3845,-8508,7201,-9566,-7236,-3386,4021,6793,-8759,5066,5879,-5171,1011,1242,8536,-8405,-9646,-214,2251,-9934,-8820,6206,1006,1318,-9712,7230,5608,-4601,9185,346,3056,8913,-2454,-3445,-4295,4802,-8852,-6121,-4538,-5580,-9246,-6462 }));
    expected.push_back({ 971167251036,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });

    nums.push_back(Input({ {14,1},{11,4},{12,6},{8,3},{3,5},{11,9},{11,10},{2,10},{13,12},{0,13},{2,0},{7,2},{7,5},{5,14} },
        { -5,3,-9,8,7,-8,-8,1,-8,-1,3,-2,3,3,5 }));
    expected.push_back({ 576,1,576,1,1,512,1,512,1,1,14,14,1,0,1 });

    nums.push_back(Input({ {0,8},{8,1},{9,2},{4,6},{7,4},{3,7},{3,8},{5,8},{5,9} },
        { -4, 83, -97, 40, 86, -85, -6, -84, -16, -53 }));
    expected.push_back({ 709070,1,1,43344,1,0,1,43344,709070,1 });

    cout << endl << TEXT_LOG_PREFIX << "test:" << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_TEST
    for (size_t i = __D_CASE_ID; i <= __D_CASE_ID; i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "test loop_id: " << i << " ==>\t" << result << " vs e:" << expected[i] << endl;
        logtime("Time: ", starttime, endtime);
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
    cout << "***Start***" << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VERBOSE
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG
    test2(2);
#endif

    cout << "***END***" << endl;
    return 0;
}


