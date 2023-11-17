/*

797. All Paths From Source to Target
Medium

Given a directed acyclic graph (DAG) of n nodes labeled from 0 to n - 1, find all possible paths from node 0 to node n - 1 and return them in any order.

The graph is given as follows: graph[i] is a list of all nodes you can visit from node i (i.e., there is a directed edge from node i to node graph[i][j]).



Example 1:

Input: graph = [[1,2],[3],[3],[]]
Output: [[0,1,3],[0,2,3]]
Explanation: There are two paths: 0 -> 1 -> 3 and 0 -> 2 -> 3.

Example 2:

Input: graph = [[4,3,1],[3,2,4],[3],[4],[]]
Output: [[0,4],[0,3,4],[0,1,3,4],[0,1,2,3,4],[0,1,4]]



Constraints:

    n == graph.length
    2 <= n <= 15
    0 <= graph[i][j] < n
    graph[i][j] != i (i.e., there will be no self-loops).
    All the elements of graph[i] are unique.
    The input graph is guaranteed to be a DAG.


*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define LIMIT 100001
#define QUEUED 100002
#define WALL '+'

using GRAPH_STRUCT = vector<vector<int>>;

class Graph
{
    GRAPH_STRUCT &data;
    int nodeCnt;
    vector<vector<int>> paths;
    pair<int, int> srcDest;

public:
    Graph(GRAPH_STRUCT &g) : data(g), nodeCnt(g.size())
    {
    }
    void setSrcDest()
    {
        srcDest.first = 0;
        srcDest.second = nodeCnt - 1;
    }


    void travel(int node, vector<int> path)
    {
        for(int i=0;i<data[node].size();i++)
        {
            vector<int> newPath = path;
            //copy(path.begin(), path.end(), back_inserter(newPath));
            newPath.push_back(node);

            if (data[node][i] == srcDest.second) {
                newPath.push_back(data[node][i]);
                paths.push_back(newPath);
                continue;
            }
            travel(data[node][i], newPath);
        }
    }

    vector<vector<int>> findAllPaths()
    {
        travel(srcDest.first, {});
        return paths;
    }
};

class Solution
{
public:
    vector<vector<int>> allPathsSourceTarget(GRAPH_STRUCT &graph)
    {
        Graph g(graph);
        g.setSrcDest();
        return g.findAllPaths();
    }
};

int main()
{
    //if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> Graph1 =
            {
                {1, 2},
                {3},
                {3},
                {}};
        vector<vector<int>> result = s1.allPathsSourceTarget(Graph1);
        cout << "Graph1 output (expected 2): " << result.size() << endl;
    }
}