/*\

847. Shortest Path Visiting All Nodes
Hard

You have an undirected, connected graph of n nodes labeled from 0 to n - 1. You are given an array graph where graph[i] is a list of all the nodes connected with node i by an edge.

Return the length of the shortest path that visits every node. You may start and stop at any node, you may revisit nodes multiple times, and you may reuse edges.



Example 1:

Input: graph = [[1,2,3],[0],[0],[0]]
Output: 4
Explanation: One possible path is [1,0,2,0,3]

Example 2:

Input: graph = [[1],[0,2,4],[1,3,4],[2],[1,2]]
Output: 4
Explanation: One possible path is [0,1,4,2,3]



Constraints:

    n == graph.length
    1 <= n <= 12
    0 <= graph[i].length < n
    graph[i] does not contain i.
    If graph[a] contains b, then graph[b] contains a.
    The input graph is always connected.

Example 1:

Input: graph = [[1,2,3],[0],[0],[0]]
Output: 4
Explanation: One possible path is [1,0,2,0,3]



---

brute force

*/

#include <iostream>
#include <vector>
#include <queue>
#include <bits/stdc++.h>
#include <map>
#include <set>

using namespace std;
#define MAX_NODES 100
using EDGES = vector<vector<int>>;

class Info
{
public:
    int startNode;
    int mask;

    Info() : startNode(-1), mask(-1) {}
    Info(int sn, int m) : startNode(sn), mask(m) {}
};

class Graph
{
    EDGES &mEdges;
    int mNodeCnt;
    int resultMask;

    map<int, set<int>> processed;
    queue<Info> q;

public:
    Graph(EDGES &conn) : mEdges(conn), mNodeCnt(conn.size())
    {
        resultMask = (1 << mNodeCnt) - 1;
    }

    int findPathSize(int level)
    {
        int size = q.size();

        if (size == 0)
            return 0;

        while (size--)
        {
            Info cur = q.front();
            q.pop();
            int curNode = cur.startNode;
            for (int i = 0; i < mEdges[curNode].size(); i++)
            {
                int nextNode = mEdges[curNode][i];
                int newMask = cur.mask | (1 << nextNode);

                if (newMask == resultMask)
                    return level;

                if (processed[nextNode].count(newMask)) // already processed, skip for now.
                {
                    //cout << "Hit continue ..." << endl;
                    continue;
                }

                processed[nextNode].insert(newMask);
                q.push(Info(nextNode, newMask));
            }
        }
        return findPathSize(level + 1);
    }

    int findShortestPath()
    {
        for (int i = 0; i < mNodeCnt; i++)
        {
            q.push(Info(i, 1 << i));
        }
        return findPathSize(1);
    }
};

class Solution
{
public:
    int shortestPathLength(vector<vector<int>> &graph)
    {
        Graph g(graph);
        return g.findShortestPath();
    }
};

int main()
{
    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> g = {{1, 2, 3},
                                 {0},
                                 {0},
                                 {0}};
        int result = s1.shortestPathLength(g);

        cout << " output (expected 4): " << result << endl;
    }
}