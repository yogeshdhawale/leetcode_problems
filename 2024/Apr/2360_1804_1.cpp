
/*
2360. Longest Cycle in a Graph
Hard
Topics
Companies
Hint

You are given a directed graph of n nodes numbered from 0 to n - 1, where each node has at most one outgoing edge.

The graph is represented with a given 0-indexed array edges of size n, indicating that there is a directed edge from node i to node edges[i]. If there is no outgoing edge from node i, then edges[i] == -1.

Return the length of the longest cycle in the graph. If no cycle exists, return -1.

A cycle is a path that starts and ends at the same node.



Example 1:

Input: edges = [3,3,4,2,3]
Output: 3
Explanation: The longest cycle in the graph is the cycle: 2 -> 4 -> 3 -> 2.
The length of this cycle is 3, so 3 is returned.

Example 2:

Input: edges = [2,-1,3,1]
Output: -1
Explanation: There are no cycles in this graph.



Constraints:

    n == edges.length
    2 <= n <= 105
    -1 <= edges[i] < n
    edges[i] != i


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
typedef pair<int, int> IDX2D;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    V1Int nums;
    Input() { id = inputID++; }
    Input(V1Int nums) {
        this->nums = nums;
        id = inputID++;
    }
};

typedef int NODE_ID;
typedef int CYCLE_ID;


struct Data {

    int cycleCnt;
    vector<int>& edges;
    vector<CYCLE_ID> node2cycle;

    void clear() {
        cycleCnt = 0;
        node2cycle.clear();
    }
    Data(vector<int>& edges) : edges(edges) {
        clear();
        node2cycle = vector<CYCLE_ID>(edges.size(), INVALID);
    }

    bool isPartOfLoop(NODE_ID start) {
        return node2cycle[start] != INVALID;
    }

    int getCount(NODE_ID start) {
        int cnt = 1;
        NODE_ID next = edges[start];
        CYCLE_ID cycleID = node2cycle[start];

        for (; next != start; cnt++, next = edges[next]) {
            if (!isPartOfLoop(next)) return -1;
            if (cycleID != node2cycle[next]) return -1;
        }
        return cnt;
    }

    int checkLoop(NODE_ID start) {

        if (isPartOfLoop(start)) return -1;

        LOG("n2c:" << node2cycle);

        cycleCnt++;
        for (;edges[start] != INVALID; start = edges[start]) {
            if (isPartOfLoop(start)) {
                if (node2cycle[start] != cycleCnt) return -1;
                return getCount(start);
            }
            node2cycle[start] = cycleCnt;
        }

        return -1;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:
    int longestCycle(vector<int>& edges) {
        Data data(edges);
        int maxCycleSize = INVALID;
        for (size_t i = 0; i < edges.size(); i++) {

            if (edges[i] == INVALID) continue;

            int curLoopSize = data.checkLoop(i);
            LOG("@" << i << " curLoopSize=" << curLoopSize << " maxCycleSize=" << maxCycleSize);
            if (curLoopSize > 1)
                maxCycleSize = max(curLoopSize, maxCycleSize);
        }
        return maxCycleSize;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return longestCycle(nums.nums);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    out << "{cycleCnt: " << d.cycleCnt << ", node2cycle: " << d.node2cycle << "}";
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

    nums.push_back(Input({ 3,3,4,2,3 }));
    expected.push_back(3);

    nums.push_back(Input({ 2,-1,3,1 }));
    expected.push_back(-1);

    nums.push_back(Input({ -1, 4, -1, 2, 0, 4 }));
    expected.push_back(-1);

    nums.push_back(Input({ 4, 3, 3, 4, 7, 2, 3, 3 }));
    expected.push_back(3);

    nums.push_back(Input({ 5, 8, -1, 5, -1, 6, 1, 6, 6, 5 }));
    expected.push_back(3);

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


