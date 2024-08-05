
/*
3161. Block Placement Queries
Hard
Topics
Companies
Hint

There exists an infinite number line, with its origin at 0 and extending towards the positive x-axis.

You are given a 2D array queries, which contains two types of queries:

    For a query of type 1, queries[i] = [1, x]. Build an obstacle at distance x from the origin. It is guaranteed that there is no obstacle at distance x when the query is asked.
    For a query of type 2, queries[i] = [2, x, sz]. Check if it is possible to place a block of size sz anywhere in the range [0, x] on the line, such that the block entirely lies in the range [0, x]. A block cannot be placed if it intersects with any obstacle, but it may touch it. Note that you do not actually place the block. Queries are separate.

Return a boolean array results, where results[i] is true if you can place the block specified in the ith query of type 2, and false otherwise.



Example 1:

Input: queries = [[1,2],[2,3,3],[2,3,1],[2,2,2]]

Output: [false,true,true]

Explanation:

For query 0, place an obstacle at x = 2. A block of size at most 2 can be placed before x = 3.

Example 2:

Input: queries = [[1,7],[2,7,6],[1,2],[2,7,5],[2,7,6]]

Output: [true,true,false]

Explanation:

    Place an obstacle at x = 7 for query 0. A block of size at most 7 can be placed before x = 7.
    Place an obstacle at x = 2 for query 2. Now, a block of size at most 5 can be placed before x = 7, and a block of size at most 2 before x = 2.



Constraints:

    1 <= queries.length <= 15 * 104
    2 <= queries[i].length <= 3
    1 <= queries[i][0] <= 2
    1 <= x, sz <= min(5 * 104, 3 * queries.length)
    The input is generated such that for queries of type 1, no obstacle exists at distance x when the query is asked.
    The input is generated such that there is at least one query of type 2.


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
#define VLOG(x)
#endif

#define INVALID -1
#define RESULT_MOD 1000000007
#define MY_MAX_VAL INT32_MAX

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef vector<bool> OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    V2Int nums;

    Input() {
        id = inputID++;
    }
    Input(V2Int nums) {
        this->nums = nums;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};


enum NodeType {
    LEAF = 0,
    INTERNAL = 1
};



struct node {

    int startIdx;
    long long distance;
    long long currentMaxDistance;
    NodeType type;

    node* left;
    node* right;
    node* parent;
    node(int startIdx, int distance, NodeType type = INTERNAL) :
        startIdx(startIdx), distance(distance), currentMaxDistance(distance), type(type),
        left(nullptr), right(nullptr), parent(nullptr) {
    }
    void setParent(node* parent) {
        this->parent = parent;
    }
};

ostream& operator<<(ostream& out, const node* node);


struct Data {

    node* mSTRoot;

    void clear() {
    }
    Data() {
        mSTRoot = new node(0, MY_MAX_VAL, LEAF);
        clear();
    }

    node* AddObstacle(int x, node* cur) {
        if (cur == nullptr)
            return nullptr;

        if (x <= cur->startIdx || x >= cur->startIdx + cur->distance) {
            return nullptr;
        }

        if (cur->type == LEAF) {
            cur->type = INTERNAL;
            cur->left = new node(cur->startIdx, x - cur->startIdx, LEAF);
            cur->right = new node(x, cur->startIdx + cur->distance - x, LEAF);
            cur->left->setParent(cur);
            cur->right->setParent(cur);

            LOG("AddObstacle @" << x << " " << cur->left << " " << cur->right);
            return cur;
        }

        auto parent = AddObstacle(x, cur->left);
        if (parent != nullptr) {
            return parent;
        }
        return AddObstacle(x, cur->right);
    }
    void AddObstacle(int x) {
        auto parent = AddObstacle(x, mSTRoot);
        if (parent != nullptr) {
            UpdateBlockSize(parent);
        }
    }

    void UpdateBlockSize(node* cur) {
        if (cur == nullptr) {
            return;
        }

        int leftDistance = cur->left == nullptr ? 0 : cur->left->currentMaxDistance;
        int rightDistance = cur->right == nullptr ? 0 : cur->right->currentMaxDistance;
        cur->currentMaxDistance = max(leftDistance, rightDistance);
        LOG("UpdateBlockSize @" << cur->startIdx << " d:" << cur->currentMaxDistance << " -> " << leftDistance << " vs " << rightDistance);
        UpdateBlockSize(cur->parent);
    }


    bool GetMaxBlockSize(int x, int blockSize, node* cur) {

        if (cur == nullptr) return false;
        LOG("GetMaxBlockSize @" << cur);

        if (x <= cur->startIdx) return GetMaxBlockSize(x, blockSize, cur->left);
        if (x >= cur->startIdx + cur->distance) return cur->currentMaxDistance >= blockSize ? true : false;

        auto leftSideFlag = GetMaxBlockSize(x, blockSize, cur->left);
        LOG("@" << cur << " l=" << leftSideFlag);
        if (leftSideFlag == true) return true;

        if (cur->type == LEAF) {
            auto curFlag = (x - cur->startIdx) >= blockSize ? true : false;
            LOG("@" << cur << " returning: " << curFlag);
            return curFlag;
        }

        auto rightSideFlag = GetMaxBlockSize(x, blockSize, cur->right);
        LOG("@" << cur << " r=" << rightSideFlag);
        return rightSideFlag;
    }
    bool CheckBlockSize(int x, int sz) {
        if (x < sz) return false;
        LOG("Searching for - before " << x << " size: " << sz);
        return GetMaxBlockSize(x, sz, mSTRoot);
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:
    vector<bool> getResults(vector<vector<int>>& queries) {


        Data data;

        vector<bool> results;

        for (auto& query : queries) {
            if (query[0] == 1) {
                int x = query[1];
                data.AddObstacle(x);
                continue;
            }
            if (query[0] == 2) {

                int x = query[1];
                int sz = query[2];

                if (sz > x) {
                    results.push_back(false);
                    continue;
                }

                results.push_back(data.CheckBlockSize(x, sz));
                /*
                                if (obstacles.size() == 0) {
                                    results.push_back(true);
                                    continue;
                                }

                                auto it = obstacles.lower_bound(x);
                                if (it == obstacles.end())  it = prev(obstacles.end());

                                int maxFreeBlockSize = it->second;

                                if (it->first > x) {
                                    if (it == obstacles.begin())
                                        maxFreeBlockSize = x;
                                    else {
                                        it--;
                                        maxFreeBlockSize = it->second;
                                        maxFreeBlockSize = max(maxFreeBlockSize, x - it->first);
                                    }
                                }
                                else {
                                    maxFreeBlockSize = max(maxFreeBlockSize, x - it->first);
                                }

                                LOG("query: x:" << x << " sz:" << sz);
                                LOG("\tobstacles:" << obstacles << " (maxFreeBlockSize:" << maxFreeBlockSize << ")");

                                if (sz <= maxFreeBlockSize) {
                                    results.push_back(true);
                                    continue;
                                }
                                if (sz <= x - obstacles.rbegin()->first) {
                                    results.push_back(true);
                                    continue;
                                }
                                results.push_back(false);*/
            }
        }
        return results;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return getResults(nums.nums);
    }
};


/****************************************************************************/
string to_string(NodeType type) {
    switch (type) {
    case LEAF:
        return "L";
    case INTERNAL:
        return "I";
    default:
        return "U";
    }
}

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    return out;
}

ostream& operator<<(ostream& out, const node* n) {

    out << "(" << n->startIdx << "," << n->startIdx + n->distance << ")-" << to_string(n->type) << ",";
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

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ {1, 440000}, {2, 3, 1} }));
    expected.push_back({ true });

    nums.push_back(Input({ {1,2},{2,3,3},{2,3,1},{2,2,2} }));
    expected.push_back({ false, true, true });

    nums.push_back(Input({ {1,7}, {2,7,6}, {1,2}, {2,7,5}, {2,7,6} }));
    expected.push_back({ true, true, false });

    nums.push_back(Input({ { 2,1,1 } }));
    expected.push_back({ true });

    nums.push_back(Input({ { 2,1,2} }));
    expected.push_back({ false });

    nums.push_back(Input({ { 1, 4}, {2, 1, 2} }));
    expected.push_back({ false });

    nums.push_back(Input({ {1,1},{1,5},{1,13},{1,14},{2,12,8} }));
    expected.push_back({ false });

    nums.push_back(Input({ {1, 1},{1, 11},{1, 4},{1, 8},{2, 13, 7} }));
    expected.push_back({ false });

    nums.push_back(Input({ {1, 4}, {1, 11}, {2, 15, 8}, {1, 5}, {2, 3, 1} }));
    expected.push_back({ false, true });

    nums.push_back(Input({ {1, 4}, {1, 2}, {1, 8}, {2, 7, 4},{2, 7, 3} }));
    expected.push_back({ false, true });

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

    nums.push_back(Input(d2));
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


