
/*
1123. Lowest Common Ancestor of Deepest Leaves
Medium
Topics
Companies
Hint

Given the root of a binary tree, return the lowest common ancestor of its deepest leaves.

Recall that:

    The node of a binary tree is a leaf if and only if it has no children
    The depth of the root of the tree is 0. if the depth of a node is d, the depth of each of its children is d + 1.
    The lowest common ancestor of a set S of nodes, is the node A with the largest depth such that every node in S is in the subtree with root A.



Example 1:

Input: root = [3,5,1,6,2,0,8,null,null,7,4]
Output: [2,7,4]
Explanation: We return the node with value 2, colored in yellow in the diagram.
The nodes coloured in blue are the deepest leaf-nodes of the tree.
Note that nodes 6, 0, and 8 are also leaf nodes, but the depth of them is 2, but the depth of nodes 7 and 4 is 3.

Example 2:

Input: root = [1]
Output: [1]
Explanation: The root is the deepest node in the tree, and it's the lca of itself.

Example 3:

Input: root = [0,1,3,null,2]
Output: [2]
Explanation: The deepest leaf node in the tree is 2, the lca of one node is itself.



Constraints:

    The number of nodes in the tree will be in the range [1, 1000].
    0 <= Node.val <= 1000
    The values of the nodes in the tree are unique.



Note: This question is the same as 865: https://leetcode.com/problems/smallest-subtree-with-all-the-deepest-nodes/


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <stack>
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
/*
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};


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


TreeNode* CreateTree(V1Int data) {

    TreeNode* root = new TreeNode(data[0]);
    queue<TreeNode*> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < data.size()) {
        TreeNode* node = q.front();
        q.pop();
        if (data[i] != -1) {
            node->left = new TreeNode(data[i]);
            q.push(node->left);
        }
        i++;
        if (i >= data.size()) break;
        if (data[i] != -1) {
            node->right = new TreeNode(data[i]);
            q.push(node->right);
        }
        i++;
    }
    return root;
}

ostream& operator << (ostream& out, const TreeNode* rootNode) {
    if (rootNode == nullptr) {
        out << "null";
        return out;
    }

    queue<const TreeNode*> q;
    q.push(rootNode);
    out << "[";
    while (!q.empty()) {
        const TreeNode* node = q.front();
        q.pop();
        if (node == nullptr) {
            out << "null,";
            continue;
        }
        out << node->val << ",";
        q.push(node->left);
        q.push(node->right);
    }
    out << "]";
    return out;
}

typedef TreeNode* INPUT_TYPE;
typedef TreeNode* OUTPUT_TYPE;



static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1) {
        this->mData1 = input1;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/

class Solution {

    unordered_map<TreeNode*, TreeNode*> parentMap;

public:

    Solution() {}

    TreeNode* lcaDeepestLeaves(TreeNode* root) {
        if (root == nullptr) return nullptr;
        if (isLeafNode(root)) return root;

        parentMap.clear();
        parentMap[root] = nullptr;

        set<TreeNode*> curNodes;
        set<TreeNode*> nextLevelNodes;

        curNodes.insert(root);
        while (true) {

            for (auto& item : curNodes) {
                if (isLeafNode(item)) continue;

                if (item->left != nullptr) {
                    parentMap[item->left] = item;
                    nextLevelNodes.insert(item->left);
                }
                if (item->right != nullptr) {
                    parentMap[item->right] = item;
                    nextLevelNodes.insert(item->right);
                }
            }

            if (nextLevelNodes.size() == 0) {
                break;
            }
            curNodes = nextLevelNodes;
            nextLevelNodes.clear();
        }

        return getLCA(curNodes, root);
    }

    TreeNode* getLCA(set<TreeNode*>& lastLevelList, TreeNode* root) {

        set<TreeNode*> curNodes = lastLevelList;
        while (curNodes.size() > 0) {
            LOG("curNodes:" << curNodes.size());
            if (curNodes.size() == 1) {
                LOG("found lca node:" << *curNodes.begin());
                return *curNodes.begin();
            }

            set<TreeNode*> parentList;
            for (auto& item : curNodes) {
                parentList.insert(parentMap[item]);
            }
            curNodes = parentList;
        }
        return root;
    }

    bool isLeafNode(TreeNode* node) {
        if (node == nullptr) return true;
        if (node->left == nullptr && node->right == nullptr) return true;
        return false;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return lcaDeepestLeaves(nums.mData1);
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
        nums.back().setInput(CreateTree({ 3, 5, 1, 6, 2, 0, 8, -1, -1, 7, 4 }));
        nums.back().setOutput(CreateTree({ 2, 7, 4 }));
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(CreateTree({ 3, 5, 1, 6, 2, 0, 8, 7, -1, -1, 4 }));
        nums.back().setOutput(CreateTree({ 5 }));
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(CreateTree({ 3, 5, 1, 6, 2, 0, 8, 7, -1, -1, 4,-1,-1,15 }));
        nums.back().setOutput(CreateTree({ 3 }));
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(CreateTree({ 1 }));
        nums.back().setOutput(CreateTree({ 1 }));
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(CreateTree({ 0,1,3,-1,2 }));
        nums.back().setOutput(CreateTree({ 2 }));
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(CreateTree({ 0,1,3,-1,2,4 }));
        nums.back().setOutput(CreateTree({ 0 }));
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(CreateTree({ 1, 2, 3, -1, 4, 6, -1, 15, 5, 10, -1, -1, -1, -1, 7, 11, -1, 8, 12, -1, -1, -1, 9, 13, 14 }));
        nums.back().setOutput(CreateTree({ 7 }));
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
        assert(result->val == nums[i].mExpected1->val);
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


