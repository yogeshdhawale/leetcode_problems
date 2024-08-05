
/*
329. Longest Increasing Path in a Matrix
Hard
Topics
Companies

Given an m x n integers matrix, return the length of the longest increasing path in matrix.

From each cell, you can either move in four directions: left, right, up, or down. You may not move diagonally or move outside the boundary (i.e., wrap-around is not allowed).



Example 1:

Input: matrix = [[9,9,4},{6,6,8},{2,1,1]]
Output: 4
Explanation: The longest increasing path is [1, 2, 6, 9].

Example 2:

Input: matrix = [[3,4,5},{3,2,6},{2,2,1]]
Output: 4
Explanation: The longest increasing path is [3, 4, 5, 6]. Moving diagonally is not allowed.

Example 3:

Input: matrix = [[1]]
Output: 1



Constraints:

    m == matrix.length
    n == matrix[i].length
    1 <= m, n <= 200
    0 <= matrix[i][j] <= 231 - 1


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

typedef pair<int, int> IDX2D;

static int inputID = 1;
struct Input
{
    int id;
    vector<vector<int>> nums;

    Input() {
        id = inputID++;
    }
    Input(vector<vector<int>> nums) {
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

typedef int OUTPUT_TYPE;

struct Data {

    void clear() {
    }
    Data() {
        clear();
    }

    Data(const Data& data) = delete;

};

struct Node {
    IDX2D idx;
    int val;
    vector<IDX2D> neighbours;

    Node(IDX2D idx, int val, int level) : idx(idx), val(val), neighbours({}) {}

    void AddNeighbour(IDX2D idx) {
        neighbours.push_back(idx);
    }
};

ostream& operator<<(ostream& out, const struct Node& node) {
    out << "@[" << node.idx.first << "," << node.idx.second << "] v:" << node.val << " nc:" << node.neighbours.size();
    return out;
}
ostream& operator<<(ostream& out, const struct Node* node) {
    out << *node;
    return out;
}

struct Key {
    IDX2D idx;

    Key(IDX2D idx) : idx(idx) {}
    Key(const Key& other) : idx(other.idx) {}
    bool operator==(const Key& other) const {
        if (idx.first == other.idx.first && idx.second == other.idx.second) {
            return true;
        };
        return false;
    }
};

ostream& operator<<(ostream& out, const Key& k) {
    out << "@[" << k.idx.first << "," << k.idx.second << "]";
    return out;
}

struct KeyHasher {
    size_t operator()(const Key& k) const
    {
        size_t h1 = std::hash<int>()(k.idx.first);
        size_t h2 = std::hash<int>()(k.idx.second);
        return h1 ^ (h2 << 1);
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {

    int rows;
    int cols;
    unordered_map<Key, Node*, KeyHasher> nodes;
    unordered_map<Key, int, KeyHasher> cache;

public:

    void clear() {
        rows = 0;
        cols = 0;
        nodes.clear();
        cache.clear();
    }
    Solution() {
        clear();
    }

    inline int getVal(IDX2D idx, vector<vector<int>>& matrix) {
        return matrix[idx.first][idx.second];
    }
    inline bool isValid(IDX2D idx) {
        return idx.first >= 0 && idx.first < rows && idx.second >= 0 && idx.second < cols;
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {
        rows = matrix.size();
        cols = matrix[0].size();

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                processNeighbours({ i, j }, matrix);
            }
        }

        int result = 1;
        for (auto& node : nodes) {
            int val = dfs(node.first);
            LOG("DFS At " << node << " val=" << val << " result=" << result);
            result = max(result, val);
        }
        return result;
    }

    int dfs(Key idx) {
        if (cache.find(idx) != cache.end()) {
            return cache[idx];
        }

        if (nodes.find(idx) == nodes.end()) {
            cache[idx] = 1;
            return 1;
        }

        int res = 0;
        for (auto neighbour : nodes[idx]->neighbours) {
            int val = dfs(neighbour);
            res = max(res, val);
        }

        cache[idx] = res + 1;
        return res + 1;
    }
    void processNeighbours(IDX2D idx, vector<vector<int>>& matrix) {

        vector<IDX2D> neighbours = getNeighbours(idx, matrix);

        if (neighbours.empty()) {
            return;
        }

        if (nodes.find(idx) == nodes.end()) {
            nodes[idx] = new Node(idx, getVal(idx, matrix), 1);
        }

        auto node = nodes[idx];
        for (auto neighbour : neighbours) {
            node->AddNeighbour(neighbour);
        }
        LOG("Added:" << *node);
    }

    vector<IDX2D> getNeighbours(IDX2D idx, vector<vector<int>>& matrix) {
        vector<IDX2D> neighbours;

        vector<IDX2D> deltas = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
        for (auto delta : deltas) {
            IDX2D neighbour = { idx.first + delta.first, idx.second + delta.second };
            if (isValid(neighbour) && getVal(neighbour, matrix) > getVal(idx, matrix)) {
                neighbours.push_back(neighbour);
            }
        }
        return neighbours;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return longestIncreasingPath(nums.nums);
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
    return out;
}

/****************************************************************************/


void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ {9, 9, 4},{6, 6, 8},{2, 1, 1} }));
    expected.push_back(4);

    nums.push_back(Input({ {3,4,5},{3,2,6},{2,2,1} }));
    expected.push_back(4);

    nums.push_back(Input({ vector<int>{1} }));
    expected.push_back(1);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "*****Time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
            << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
            << endl;
        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
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
    cout << TEXT_LOG_PREFIX << "Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;

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


