
/*
2421. Number of Good Paths
Hard
Topics
Companies
Hint

There is a tree (i.e. a connected, undirected graph with no cycles) consisting of n nodes numbered from 0 to n - 1 and exactly n - 1 edges.

You are given a 0-indexed integer array vals of length n where vals[i] denotes the value of the ith node. You are also given a 2D integer array edges where edges[i] = [ai, bi] denotes that there exists an undirected edge connecting nodes ai and bi.

A good path is a simple path that satisfies the following conditions:

    The starting node and the ending node have the same value.
    All nodes between the starting node and the ending node have values less than or equal to the starting node (i.e. the starting node's value should be the maximum value along the path).

Return the number of distinct good paths.

Note that a path and its reverse are counted as the same path. For example, 0 -> 1 is considered to be the same as 1 -> 0. A single node is also considered as a valid path.



Example 1:

Input: vals = [1,3,2,1,3], edges = [[0,1],[0,2],[2,3],[2,4]]
Output: 6
Explanation: There are 5 good paths consisting of a single node.
There is 1 additional good path: 1 -> 0 -> 2 -> 4.
(The reverse path 4 -> 2 -> 0 -> 1 is treated as the same as 1 -> 0 -> 2 -> 4.)
Note that 0 -> 2 -> 3 is not a good path because vals[2] > vals[0].

Example 2:

Input: vals = [1,1,2,2,3], edges = [[0,1],[1,2],[2,3],[2,4]]
Output: 7
Explanation: There are 5 good paths consisting of a single node.
There are 2 additional good paths: 0 -> 1 and 2 -> 3.

Example 3:

Input: vals = [1], edges = []
Output: 1
Explanation: The tree consists of only one node, so there is one good path.



Constraints:

    n == vals.length
    1 <= n <= 3 * 104
    0 <= vals[i] <= 105
    edges.length == n - 1
    edges[i].length == 2
    0 <= ai, bi < n
    ai != bi
    edges represents a valid tree.


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
    V2Int edges;

    Input() {
        id = inputID++;
    }
    Input(V1Int nums, V2Int edges) {
        this->nums = nums;
        this->edges = edges;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

typedef int NODE_ID;
typedef int WEIGHT;

struct Node {
    NODE_ID id;
    WEIGHT weight;
    vector<Node*> neighbors;

    Node(NODE_ID id = INVALID, WEIGHT weight = 0) {
        this->id = id;
        this->weight = weight;
        neighbors.clear();
    }
    void AddNeighbor(Node& other) {

        if (neighbors.size() == 0) {
            neighbors.push_back(&other);
            return;
        }

        for (size_t i = 0; i < neighbors.size(); ++i) {
            auto& n = *neighbors[i];
            if (n.weight < other.weight) continue;
            neighbors.insert(neighbors.begin() + i, &other);
            return;
        }
        neighbors.push_back(&other);
    }

    void traverse(WEIGHT startw, vector<bool>& visited) {
        for (auto& ptr : neighbors) {
            auto& nextNode = *ptr;

            if (visited[nextNode.id]) continue;
            if (nextNode.weight > startw) { return; }

            visited[nextNode.id] = true;
            nextNode.traverse(startw, visited);
        }
        return;
    }
};

ostream& operator<<(ostream& out, const Node& d);

struct Data {

    int totalNodes;
    vector <Node> nodes;
    map<WEIGHT, vector<NODE_ID>> wmap;
    void clear() {
        nodes.clear();
        wmap.clear();
    }


    Data(vector<int>& weights, vector<vector<int>>& edges) : totalNodes(weights.size()) {

        clear();
        nodes.resize(totalNodes);
        for (int i = 0; i < totalNodes; ++i) {
            nodes[i].id = i;
            nodes[i].weight = weights[i];
            wmap[weights[i]].push_back(i);
        }

        for (auto& edge : edges) {
            auto& n1 = nodes[edge[0]];
            auto& n2 = nodes[edge[1]];

            n1.AddNeighbor(n2);
            n2.AddNeighbor(n1);
        }
    }


    int getValidPaths() {
        int result = 0;
        map<NODE_ID, bool> pathMap;

        for (auto& w : wmap) {

            if (w.second.size() <= 1) continue;
            for (auto& n : w.second) {

                auto& node = nodes[n];
                if(pathMap[n]) continue;

                LOG("@:" << node << "result:" << result);

                vector<bool> visited(totalNodes, false);
                visited[node.id] = true;
                
                int curSize = 0;
                node.traverse(node.weight, visited);

                for (auto& n : wmap[node.weight]) {
                    if (!visited[n]) continue;
                    curSize++;
                    pathMap[n] = true;
                }

                auto res = (curSize * ( curSize - 1)) / 2;
                result += res;
                LOG("node:" << node << " curSize:" << curSize << " res:" << res << " result:" << result);
            }
        }
        return result;
    }
};

ostream& operator<<(ostream& out, const Data& d);

ostream& operator<<(ostream& out, const Node& d) {

    out << "{id:" << d.id << " weight:" << d.weight << " neighbors(" << d.neighbors.size() << ") : ";

    for (auto& n : d.neighbors) out << n->id << ", ";
    out << "} ";

    return out;
}

class Solution {
public:
    int numberOfGoodPaths(vector<int>& vals, vector<vector<int>>& edges) {

        if (vals.size() == 1) return 1;
        if (edges.size() == 0) return vals.size();

        Data data(vals, edges);

        LOG("data:" << data);
        int result = vals.size();
        result += data.getValidPaths();

        return result;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return numberOfGoodPaths(nums.nums, nums.edges);
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
    out << "wmap:" << d.wmap;
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

    nums.push_back(Input({ 1,3,2,1,3 }, { {0,1},{0,2},{2,3},{2,4} }));
    expected.push_back(6);

    nums.push_back(Input({ 1,1,2,2,3 }, { {0,1},{1,2},{2,3},{2,4} }));
    expected.push_back(7);


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

    nums.push_back(Input(d1, d2));
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


