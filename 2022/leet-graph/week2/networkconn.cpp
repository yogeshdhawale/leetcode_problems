
#include <iostream>
#include <vector>

using namespace std;

class Graph
{
    vector<vector<int>> &data;
    int row;

    vector<vector<int>> adjList;
    vector<bool> visited;
    int nodes;

public:
    Graph(vector<vector<int>> &g) : data(g), row(g.size())
    {
        nodes = row;
    }
    void SetNodes(int n)
    {
        nodes = n;
        visited.resize(nodes, false);
        adjList.resize(nodes);
    }

    void createAdjList()
    {
        for (int i = 0; i < data.size(); i++)
        {
            int n1 = data[i][0];
            int n2 = data[i][1];
            adjList[n1].push_back(n2);
            adjList[n2].push_back(n1);
        }

        // for(int i=0;i<adjList.size();i++)
        //     cout<< i << ":" << adjList[i].size() << endl;
    }

    void visit(int n)
    {
        if (visited[n])
            return;
        visited[n] = true;
        for (int i = 0; i < adjList[n].size(); i++)
        {
            int node = adjList[n][i];
                visit(node);
        }
    }

    int findConnections()
    {
        if (data.size() < (nodes - 1))
            return -1;

        createAdjList();
        int totalPartitions = 0;
        for (int i = 0; i < nodes; i++)
        {
            if (visited[i])
                continue;
            visit(i);
            totalPartitions++;
        }
        return totalPartitions - 1;
    }
};

class Solution
{
public:
    int makeConnected(int n, vector<vector<int>> &connections)
    {
        Graph m(connections);
        m.SetNodes(n);
        return m.findConnections();
    }
};

int main()
{
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 1},
                {0, 2},
                {1, 2}};
        int result = s1.makeConnected(4, grid1);
        cout << " output (expected 1): " << result << endl;
    }
    if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {
                {0, 1},
                {0, 2},
                {0, 3},
                {1, 2},
                {1, 3}};
        int result = s1.makeConnected(6, grid1);
        cout << " output (expected 2): " << result << endl;
    }

    // if (false)
    {
        cout << "*** Starting test 3" << endl;

        Solution s1;
        vector<vector<int>> grid1 =
            {{1, 4},
             {0, 3},
             {1, 3},
             {3, 7},
             {2, 7},
             {0, 1},
             {2, 4},
             {3, 6},
             {5, 6},
             {6, 7},
             {4, 7},
             {0, 7},
             {5, 7}};
        int result = s1.makeConnected(11, grid1);
        cout << " output (expected 3): " << result << endl;
    }
}