/*

802. Find Eventual Safe States
Medium

There is a directed graph of n nodes with each node labeled from 0 to n - 1. The graph is represented by a 0-indexed 2D integer array graph where graph[i] is an integer array of nodes adjacent to node i, meaning there is an edge from node i to each node in graph[i].

A node is a terminal node if there are no outgoing edges. A node is a safe node if every possible path starting from that node leads to a terminal node.

Return an array containing all the safe nodes of the graph. The answer should be sorted in ascending order.



Example 1:
Illustration of graph

Input: graph = [[1,2],[2,3],[5],[0],[5],[],[]]
Output: [2,4,5,6]
Explanation: The given graph is shown above.
Nodes 5 and 6 are terminal nodes as there are no outgoing edges from either of them.
Every path starting at nodes 2, 4, 5, and 6 all lead to either node 5 or 6.

Example 2:

Input: graph = [[1,2,3,4],[1,2],[3,4],[0,4],[]]
Output: [4]
Explanation:
Only node 4 is a terminal node, and every path starting at node 4 leads to node 4.



Constraints:

    n == graph.length
    1 <= n <= 104
    0 <= graph[i].length <= n
    0 <= graph[i][j] <= n - 1
    graph[i] is sorted in a strictly increasing order.
    The graph may contain self-loops.
    The number of edges in the graph will be in the range [1, 4 * 104].

*/

#include <iostream>
#include <vector>

using namespace std;

class Node
{
public:
    int nodeID;

    bool isLoop;
    bool isTerminal;
    bool isSafe;
    vector<int> &edges;

    Node(int id, vector<int> &e) : isLoop(false), isTerminal(false), isSafe(false), nodeID(id), edges(e) {}
};

class SafeStates
{
    vector<Node> nodes;

public:
    SafeStates() {}
    void setGraph(vector<vector<int>> &graph)
    {
        for (int i = 0; i < graph.size(); i++)
        {
            nodes.push_back(Node(i, graph[i]));
        }
    }

    bool findLoop(int id, vector<bool> &visited)
    {
        bool isLoop = false;
        vector<int> &edges = nodes[id].edges;

        visited[id] = true;
        for (auto &&endV : edges)
        {
            if (nodes[endV].isLoop || visited[endV])
            {
                // this is loop
                isLoop = true;
                for (int i = 0; i < visited.size(); i++)
                    if (visited[i])
                        nodes[i].isLoop = true;

                return true;
            }
            vector<bool> v;
            copy(visited.begin(), visited.end(), back_inserter(v));
            findLoop(endV, v);
        }
        return nodes[id].isLoop;
    }
    vector<int> getSafeStates()
    {
        // mark all terminals
        for (auto &&node : nodes)
        {
            if (node.edges.size() == 0)
            {
                node.isTerminal = true;
                node.isSafe = true;
            }
        }
        // for each node detect and mark loops
        for (auto &&node : nodes)
        {
            if (node.isTerminal)
                continue;

            vector<bool> visited(nodes.size(), false);
            findLoop(node.nodeID, visited);
            if (!node.isLoop)
                node.isSafe = true;
        }

        // after all loops are marked, get the result
        vector<int> result;
        for (auto &&node : nodes)
        {
            if (!node.isLoop)
                result.push_back(node.nodeID);
        }
        return result;
    }
};

class Solution
{
public:
    vector<int> eventualSafeNodes(vector<vector<int>> &graph)
    {
        SafeStates s;
        s.setGraph(graph);
        return s.getSafeStates();
    }
};

void PrintVector(vector<int> &v)
{
    for (auto &&i : v)
        cout << i << ",";
    cout << endl;
}

int main()
{
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> graph = {{1, 2},
                                     {2, 3},
                                     {5},
                                     {0},
                                     {5},
                                     {},
                                     {}};
        vector<int> result = s1.eventualSafeNodes(graph);

        cout << " output (expected 4): " << result.size() << endl;
        PrintVector(result);
    }

    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> graph = {{1, 2, 3, 4},
                                     {1, 2},
                                     {3, 4},
                                     {0, 4},
                                     {}};
        vector<int> result = s1.eventualSafeNodes(graph);
        cout << " output (expected 1): " << result.size() << endl;
        PrintVector(result);
    }
}