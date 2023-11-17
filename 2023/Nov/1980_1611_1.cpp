/*
1980. Find Unique Binary String
Medium
Topics
Companies
Hint

Given an array of strings nums containing n unique binary strings each of length n, return a binary string of length n that does not appear in nums. If there are multiple answers, you may return any of them.



Example 1:

Input: nums = ["01","10"]
Output: "11"
Explanation: "11" does not appear in nums. "00" would also be correct.

Example 2:

Input: nums = ["00","01"]
Output: "11"
Explanation: "11" does not appear in nums. "10" would also be correct.

Example 3:

Input: nums = ["111","011","001"]
Output: "101"
Explanation: "101" does not appear in nums. "000", "010", "100", and "110" would also be correct.



Constraints:

    n == nums.length
    1 <= n <= 16
    nums[i].length == n
    nums[i] is either '0' or '1'.
    All the strings of nums are unique.


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

#define __DEBUG_LOG 0

struct node
{
    char val;
    node *left;
    node *right;
    node *parent;
    bool isfull;

    node(char val)
    {
        this->val = val;
        this->isfull = false;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
    node(char val, node *parent) : node(val)
    {
        this->parent = parent;
    }
};

class Solution
{
public:
    string findDifferentBinaryString(vector<string> &nums)
    {
        int n = nums.size();
        int len = nums[0].size();

        node *root = new node(-1);
        for (auto x : nums)
        {

            node *cur = root;
            for (int i = 0; i < len; i++)
            {
                if (x[i] == '0')
                {
                    if (cur->left == nullptr)
                    {
                        cur->left = new node('0', cur);
                    }
                    cur = cur->left;
                }
                else
                {
                    if (cur->right == nullptr)
                    {
                        cur->right = new node('1', cur);
                    }
                    cur = cur->right;
                }
            }
            cur->isfull = true;
            node *parent = cur->parent;
            while (parent != nullptr)
            {
                if (parent->left == nullptr || parent->right == nullptr)
                    break;

                if (parent->left->isfull && parent->right->isfull)
                {
                    parent->isfull = true;
                    parent = parent->parent;
                }
                else
                    break;
            }
        }

        string result = processTree(root, "");
        int res_len = result.size();
        while (res_len < len)
        {
            result += '0';
            res_len++;
        }

        if (__DEBUG_LOG)
            cout << "returning: " << result << endl;

        return result;
    }

    string processTree(node *root, string prefix)
    {

        if (root == nullptr)
        {
            return prefix;
        }
        if (root->isfull)
        {
            return "";
        }
        
        if (root->left == nullptr)
        {
            return prefix + '0';
        }
        if (root->right == nullptr)
        {
            return prefix + '1';
        }

        if (!root->left->isfull)
        {
            string leftresult = processTree(root->left, prefix + '0');
            if (leftresult != "")
                return leftresult;
        }
        if (!root->right->isfull)
        {
            string rightresult = processTree(root->right, prefix + '1');
            if (rightresult != "")
                return rightresult;
        }
        return "";
    }
};

void test1(Solution &s, int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<string>({"01", "10"});
    auto result = s.findDifferentBinaryString(nums);

    for (auto x : nums)
    {
        assert(x != result);
    }
}

void test2(Solution &s, int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<string>({"00"});
    auto result = s.findDifferentBinaryString(nums);

    for (auto x : nums)
    {
        assert(x != result);
    }
}

void test3(Solution &s, int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<string>({"01", "10", "00"});
    auto result = s.findDifferentBinaryString(nums);

    for (auto x : nums)
    {
        assert(x != result);
    }
}

int main()
{

    cout << "1980. Find Unique Binary String" << endl;
    Solution s;

    test1(s, 1);
    test2(s, 2);
    test3(s, 3);

    return 0;
}