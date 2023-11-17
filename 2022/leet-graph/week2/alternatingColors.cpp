/*
1129. Shortest Path with Alternating Colors
Medium

You are given an integer n, the number of nodes in a directed graph where the nodes are labeled from 0 to n - 1. Each edge is red or blue in this graph, and there could be self-edges and parallel edges.

You are given two arrays redEdges and blueEdges where:

    redEdges[i] = [ai, bi] indicates that there is a directed red edge from node ai to node bi in the graph, and
    blueEdges[j] = [uj, vj] indicates that there is a directed blue edge from node uj to node vj in the graph.

Return an array answer of length n, where each answer[x] is the length of the shortest path from node 0 to node x such that the edge colors alternate along the path, or -1 if such a path does not exist.



Example 1:

Input: n = 3, redEdges = [[0,1],[1,2]], blueEdges = []
Output: [0,1,-1]

Example 2:

Input: n = 3, redEdges = [[0,1]], blueEdges = [[2,1]]
Output: [0,1,-1]

*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

using EDGES = vector<vector<int>>;
#define MAX_NODE_LEN 100
enum COLOR
{
    RED = 0,
    BLUE = 1
};

void PrintVector(vector<int> &v)
{
    for (auto &&i : v)
        cout << i << ",";
    cout << endl;
}

class labels
{
public:
    bool blue;
    bool red;

    labels() : blue(false), red(false) {}

    void set(COLOR c)
    {
        if (c == BLUE)
            blue = true;
        else
            red = true;
    }

    bool isSet(COLOR c)
    {
        if (c == BLUE)
            return blue;
        return red;
    }
};

class Graph
{
    EDGES mBlue;
    EDGES mRed;
    int mNodeCnt;

    queue<int> q;
    vector<int> mPathInfo;

public:
    Graph(int cnt, EDGES &reds, EDGES &blues) : mNodeCnt(cnt)
    {
        mBlue.resize(cnt);
        mRed.resize(cnt);

        for (int i = 0; i < cnt; i++)
        {
            mBlue[i].resize(cnt, 0);
            mRed[i].resize(cnt, 0);
        }
        mPathInfo.resize(cnt, MAX_NODE_LEN);

        for (auto b : blues)
            mBlue[b[0]][b[1]] = 1;
        for (auto r : reds)
            mRed[r[0]][r[1]] = 1;
    }

    void processQueue(COLOR c)
    {
        vector<vector<labels>> processed;
        processed.resize(mNodeCnt);
        for (int i = 0; i < processed.size(); i++)
            processed[i].resize(mNodeCnt);

        int cnt = -1;
        while (true)
        {
            cnt++;
            int size = q.size();

            if (size == 0)
                break;

            c = (c == BLUE) ? RED : BLUE;

            for (int qs = 0; qs < size; qs++)
            {
                int node = q.front();
                q.pop();
                EDGES &e = (c == BLUE) ? mBlue : mRed;
                if (mPathInfo[node] > cnt)
                    mPathInfo[node] = cnt;

                for (int i = 0; i < mNodeCnt; i++)
                {
                    if (e[node][i] == 0)
                        continue;
                    if (processed[node][i].isSet(c))
                        continue;

                    processed[node][i].set(c);
                    q.push(i);
                }
            }
        }
    }
    vector<int> findPaths(int startNode)
    {
        mPathInfo[startNode] = 0;

        // process reds
        COLOR c = BLUE;
        q.push(startNode);
        processQueue(c);

        vector<int> ans1;
        copy(mPathInfo.begin(), mPathInfo.end(), back_inserter(ans1));

        // process blue
        c = RED;
        q.push(startNode);
        processQueue(c);

        for (int i = 0; i < mNodeCnt; i++)
        {
            if (ans1[i] < mPathInfo[i])
                mPathInfo[i] = ans1[i];
            if (mPathInfo[i] == MAX_NODE_LEN)
                mPathInfo[i] = -1;
        }
        return mPathInfo;
    }
};

class Solution
{
public:
    vector<int> shortestAlternatingPaths(int n, vector<vector<int>> &redEdges, vector<vector<int>> &blueEdges)
    {
        Graph g(n, redEdges, blueEdges);
        return g.findPaths(0);
    }
};

int main()
{
    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        int cnt = 3;
        vector<vector<int>> red = {{0, 1},
                                   {1, 2}};
        vector<vector<int>> blue = {};
        vector<int> result = s1.shortestAlternatingPaths(cnt, red, blue);

        cout << " output (expected 3): " << result.size() << endl;
        PrintVector(result);
    }
    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        int cnt = 3;
        vector<vector<int>> red = {{0, 1}};
        vector<vector<int>> blue = {{1, 2}};
        vector<int> result = s1.shortestAlternatingPaths(cnt, red, blue);

        cout << " output (expected 3): " << result.size() << endl;
        PrintVector(result);
    }

    if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        int cnt = 3;
        vector<vector<int>> red = {{0, 1}, {0, 2}};
        vector<vector<int>> blue = {{1, 0}};
        vector<int> result = s1.shortestAlternatingPaths(cnt, red, blue);

        cout << " output (expected 3): " << result.size() << endl;
        PrintVector(result);
    }
}