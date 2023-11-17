/*
1376. Time Needed to Inform All Employees
Medium

A company has n employees with a unique ID for each employee from 0 to n - 1. The head of the company is the one with headID.

Each employee has one direct manager given in the manager array where manager[i] is the direct manager of the i-th employee, manager[headID] = -1. Also, it is guaranteed that the subordination relationships have a tree structure.

The head of the company wants to inform all the company employees of an urgent piece of news. He will inform his direct subordinates, and they will inform their subordinates, and so on until all employees know about the urgent news.

The i-th employee needs informTime[i] minutes to inform all of his direct subordinates (i.e., After informTime[i] minutes, all his direct subordinates can start spreading the news).

Return the number of minutes needed to inform all the employees about the urgent news.



Example 1:

Input: n = 1, headID = 0, manager = [-1], informTime = [0]
Output: 0
Explanation: The head of the company is the only employee in the company.

Example 2:

Input: n = 6, headID = 2, manager = [2,2,-1,2,2,2], informTime = [0,0,1,0,0,0]
Output: 1
Explanation: The head of the company with id = 2 is the direct manager of all the employees in the company and needs 1 minute to inform them all.
The tree structure of the employees in the company is shown.



Constraints:

    1 <= n <= 105
    0 <= headID < n
    manager.length == n
    0 <= manager[i] < n
    manager[headID] == -1
    informTime.length == n
    0 <= informTime[i] <= 1000
    informTime[i] == 0 if employee i has no subordinates.
    It is guaranteed that all the employees can be informed.


*/

#include <iostream>
#include <vector>
#include <map>
#include <memory>

using namespace std;

class Node
{
public:
    int empid;
    //vector<int> reportees;
    int maxTime;

    Node() : empid(-1), maxTime(0) {}
    Node(int i, int t) : empid(i), maxTime(t) {}
    Node(const Node &n) : empid(n.empid), maxTime(n.maxTime) {}
    /*Node &operator=(const Node &n)
    {
        empid = n.empid;
        maxTime = n.maxTime;
        return *this;
    }*/
};
class Tree
{
    map<int, Node> mgr2sub;
    int rootID;
    vector<int> &list;
    vector<int> &informTime;
    int alltime;

public:
    Tree(int r, vector<int> &l, vector<int> &time) : rootID(r), list(l), informTime(time) {}

    void buildTree()
    {
        for (int i = 0; i < list.size(); i++)
        {
            if (mgr2sub.find(list[i]) != mgr2sub.end())
            {
                mgr2sub[list[i]].reportees.push_back(i);
                if(mgr2sub[list[i]].maxTime < informTime[i])
                    mgr2sub[list[i]].maxTime = informTime[i];
            }
            else
            {
                mgr2sub[list[i]] = Node(i,0);
            }
        }
    }

    computeTimeInTree(int id)
    {

    }
    int computeTime()
    {
        return computeTimeInTree(rootID);
    }
};

class Solution
{
public:
    int numOfMinutes(int n, int headID, vector<int> &manager, vector<int> &informTime)
    {
        Tree t(headID, manager, informTime);
        t.buildTree();
        return t.computeTime();
    }
};

int main()
{
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        int n = 6;
        int headID = 2;
        vector<int> mgr = {2, 2, -1, 2, 2, 2};
        vector<int> informTime = {0, 0, 1, 0, 0, 0};
        int result = s1.numOfMinutes(n, headID, mgr, informTime);
        cout << " output (expected 1): " << result << endl;
    }
    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        int n = 1;
        int headID = 0;
        vector<int> mgr = {-1};
        vector<int> informTime = {0};
        int result = s1.numOfMinutes(n, headID, mgr, informTime);
        cout << " output (expected 1): " << result << endl;
    }
}