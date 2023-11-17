/*
1306. Jump Game III
Medium

Given an array of non-negative integers arr, you are initially positioned at start index of the array. When you are at index i, you can jump to i + arr[i] or i - arr[i], check if you can reach to any index with value 0.

Notice that you can not jump outside of the array at any time.



Example 1:

Input: arr = [4,2,3,0,3,1,2], start = 5
Output: true
Explanation:
All possible ways to reach at index 3 with value 0 are:
index 5 -> index 4 -> index 1 -> index 3
index 5 -> index 6 -> index 4 -> index 1 -> index 3

Example 2:

Input: arr = [4,2,3,0,3,1,2], start = 0
Output: true
Explanation:
One possible way to reach at index 3 with value 0 is:
index 0 -> index 4 -> index 1 -> index 3

Example 3:

Input: arr = [3,0,2,1,2], start = 2
Output: false
Explanation: There is no way to reach at index 1 with value 0.



Constraints:

    1 <= arr.length <= 5 * 104
    0 <= arr[i] < arr.length
    0 <= start < arr.length

*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph
{
    vector<int> &nodes;
    int mNodeCnt;

    vector<bool> visited;

public:
    Graph(vector<int> &n) : nodes(n), mNodeCnt(n.size())
    {
        visited.resize(mNodeCnt, false);
    }

    bool visitNode(int i)
    {
        if (i < 0 || i >= mNodeCnt)
            return false;

        if (visited[i])
            return false;

        int value = nodes[i];

        if (value == 0)
            return true;

        visited [i] = true;

        if (visitNode(i - value))
            return true;
        if (visitNode(i + value))
            return true;

        return false;
    }
    bool CheckPathTo0(int startNode)
    {
        return visitNode(startNode);
        return false;
    }
};

class Solution
{
public:
    bool canReach(vector<int> &arr, int start)
    {
        Graph g(arr);
        return g.CheckPathTo0(start);
    }
};

int main()
{
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<int> g = {4, 2, 3, 0, 3, 1, 2};
        int startNode = 5;
        bool result = s1.canReach(g, startNode);

        cout << " output (expected 1): " << result << endl;
    }

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<int> g = {3,0,2,1,2};
        int startNode = 2;
        bool result = s1.canReach(g, startNode);

        cout << " output (expected 0): " << result << endl;
    }
}