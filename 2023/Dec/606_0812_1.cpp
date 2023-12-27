/*

606. Construct String from Binary Tree
Easy
Topics
Companies

Given the root of a binary tree, construct a string consisting of parenthesis and integers from a binary tree with the preorder traversal way, and return it.

Omit all the empty parenthesis pairs that do not affect the one-to-one mapping relationship between the string and the original binary tree.



Example 1:

Input: root = [1,2,3,4]
Output: "1(2(4))(3)"
Explanation: Originally, it needs to be "1(2(4)())(3()())", but you need to omit all the unnecessary empty parenthesis pairs. And it will be "1(2(4))(3)"

Example 2:

Input: root = [1,2,3,null,4]
Output: "1(2()(4))(3)"
Explanation: Almost the same as the first example, except we cannot omit the first parenthesis pair to break the one-to-one mapping relationship between the input and the output.



Constraints:

    The number of nodes in the tree is in the range [1, 104].
    -1000 <= Node.val <= 1000



*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 0

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution
{
private:
    string result;

public:
    string tree2str(TreeNode *root)
    {
        preorder_traverse(root);
        return result;
    }
    void preorder_traverse(TreeNode *root)
    {
        if (root == nullptr)
        {
            return;
        }

        if (__DEBUG_LOG)
        {
            cout << "preorder_traverse: val:" << root->val << "\ttree2str:" << result << endl;
        }
        result += to_string(root->val);

        if (root->left == nullptr && root->right == nullptr)
        {
            return;
        }

        result += '(';
        preorder_traverse(root->left);
        result += ')';

        if (root->right != nullptr)
        {
            result += '(';
            preorder_traverse(root->right);
            result += ')';
        }
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto root = new TreeNode(1,
                             new TreeNode(2, new TreeNode(4), nullptr),
                             new TreeNode(3, nullptr, nullptr));

    auto expected = "1(2(4))(3)";

    auto result = s.tree2str(root);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{

    Solution s;
    cout << "test" << testID << endl;

    auto root = new TreeNode(1,
                             new TreeNode(2, nullptr, new TreeNode(4)),
                             new TreeNode(3, nullptr, nullptr));

    auto expected = "1(2()(4))(3)";

    auto result = s.tree2str(root);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    test2(2);
    // test3(3);

    return 0;
}