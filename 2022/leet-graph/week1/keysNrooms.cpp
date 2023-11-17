
/*
841. Keys and Rooms
Medium

There are n rooms labeled from 0 to n - 1 and all the rooms are locked except for room 0. Your goal is to visit all the rooms. However, you cannot enter a locked room without having its key.

When you visit a room, you may find a set of distinct keys in it. Each key has a number on it, denoting which room it unlocks, and you can take all of them with you to unlock the other rooms.

Given an array rooms where rooms[i] is the set of keys that you can obtain if you visited room i, return true if you can visit all the rooms, or false otherwise.



Example 1:

Input: rooms = [[1],[2],[3],[]]
Output: true
Explanation:
We visit room 0 and pick up key 1.
We then visit room 1 and pick up key 2.
We then visit room 2 and pick up key 3.
We then visit room 3.
Since we were able to visit every room, we return true.

Example 2:

Input: rooms = [[1,3],[3,0,1],[2],[0]]
Output: false
Explanation: We can not enter room number 2 since the only key that unlocks it is in that room.



Constraints:

    n == rooms.length
    2 <= n <= 1000
    0 <= rooms[i].length <= 1000
    1 <= sum(rooms[i].length) <= 3000
    0 <= rooms[i][j] < n
    All the values of rooms[i] are unique.


*/

#include <iostream>
#include <vector>


using namespace std;

using GRAPH_STRUCT = vector<vector<int>>;

class Graph
{
    GRAPH_STRUCT &data;
    int nodeCnt;
    vector<bool> visited;

public:
    Graph(GRAPH_STRUCT &g) : data(g), nodeCnt(g.size())
    {
        visited.resize(nodeCnt, false);
    }

    void visit(int roomID)
    {
        if (visited[roomID] == true)
            return;

        visited[roomID] = true;
        for (int i = 0; i < data[roomID].size(); i++)
        {
            visit(data[roomID][i]);
        }
    }
    void visitRooms()
    {
        visit(0);
    }
    bool isAllvisited()
    {
        for (int i = 0; i < visited.size(); i++)
            if (visited[i] == false)
                return false;

        return true;
    }
};

class Solution
{
public:
    bool canVisitAllRooms(GRAPH_STRUCT &rooms)
    {
        // basically keys represent edges of rooms. if room i has key for room j, you can say i is connected to j.
        Graph g(rooms);
        g.visitRooms();
        return g.isAllvisited();
    }
};

int main()
{
    // if (false)
    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<int>> Graph1 =
            {
                {1},
                {2},
                {3},
                {}};
        bool result = s1.canVisitAllRooms(Graph1);
        cout << "Graph1 output (expected 1): " << result << endl;
    }
    // if (false)
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<int>> Graph1 =
            {
                {1},
                {2},
                {2},
                {}};
        bool result = s1.canVisitAllRooms(Graph1);
        cout << "Graph1 output (expected 0): " << result << endl;
    }
}