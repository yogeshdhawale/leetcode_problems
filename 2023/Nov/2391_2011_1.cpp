/*
2391. Minimum Amount of Time to Collect Garbage
Medium
Topics
Companies
Hint

You are given a 0-indexed array of strings garbage where garbage[i] represents the assortment of garbage at the ith house. garbage[i] consists only of the characters 'M', 'P' and 'G' representing one unit of metal, paper and glass garbage respectively. Picking up one unit of any type of garbage takes 1 minute.

You are also given a 0-indexed integer array travel where travel[i] is the number of minutes needed to go from house i to house i + 1.

There are three garbage trucks in the city, each responsible for picking up one type of garbage. Each garbage truck starts at house 0 and must visit each house in order; however, they do not need to visit every house.

Only one garbage truck may be used at any given moment. While one truck is driving or picking up garbage, the other two trucks cannot do anything.

Return the minimum number of minutes needed to pick up all the garbage.



Example 1:

Input: garbage = ["G","P","GP","GG"], travel = [2,4,3]
Output: 21
Explanation:
The paper garbage truck:
1. Travels from house 0 to house 1
2. Collects the paper garbage at house 1
3. Travels from house 1 to house 2
4. Collects the paper garbage at house 2
Altogether, it takes 8 minutes to pick up all the paper garbage.
The glass garbage truck:
1. Collects the glass garbage at house 0
2. Travels from house 0 to house 1
3. Travels from house 1 to house 2
4. Collects the glass garbage at house 2
5. Travels from house 2 to house 3
6. Collects the glass garbage at house 3
Altogether, it takes 13 minutes to pick up all the glass garbage.
Since there is no metal garbage, we do not need to consider the metal garbage truck.
Therefore, it takes a total of 8 + 13 = 21 minutes to collect all the garbage.

Example 2:

Input: garbage = ["MMM","PGM","GP"], travel = [3,10]
Output: 37
Explanation:
The metal garbage truck takes 7 minutes to pick up all the metal garbage.
The paper garbage truck takes 15 minutes to pick up all the paper garbage.
The glass garbage truck takes 15 minutes to pick up all the glass garbage.
It takes a total of 7 + 15 + 15 = 37 minutes to collect all the garbage.



Constraints:

    2 <= garbage.length <= 105
    garbage[i] consists of only the letters 'M', 'P', and 'G'.
    1 <= garbage[i].length <= 10
    travel.length == garbage.length - 1
    1 <= travel[i] <= 100


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define __DEBUG_LOG 0

enum GarbageTruckType
{
    METAL_TRUCK = 0,
    PAPER_TRUCK = 1,
    GLASS_TRUCK = 2
};
char getGarbageTruckTypeChar(GarbageTruckType type)
{
    switch (type)
    {
    case METAL_TRUCK:
        return 'M';
    case PAPER_TRUCK:
        return 'P';
    case GLASS_TRUCK:
        return 'G';
    default:
        return ' ';
    }
}

GarbageTruckType getGarbageTruckType(char c)
{
    switch (c)
    {
    case 'M':
        return METAL_TRUCK;
    case 'P':
        return PAPER_TRUCK;
    case 'G':
        return GLASS_TRUCK;
    default:
        return METAL_TRUCK;
    }
}

struct GarbageTruckTracking
{
    GarbageTruckType type;
    long unsigned int location;
    int time;
    int defesite;

public:
    GarbageTruckTracking(GarbageTruckType type)
    {
        this->type = type;
        this->location = 0;
        this->time = 0;
        this->defesite = 0;
    }

    void printInfo()
    {
        cout << getGarbageTruckTypeChar(type) << " L:" << location << " T:" << time << endl;
    }
};

class Solution
{
private:
    GarbageTruckTracking trucks[3];
    int size;

public:
    Solution() : trucks{GarbageTruckTracking(METAL_TRUCK), GarbageTruckTracking(PAPER_TRUCK), GarbageTruckTracking(GLASS_TRUCK)},
                 size(3)
    {
    }
    int garbageCollection(vector<string> &garbage, vector<int> &travel);
    void addDefesiteTime(int val)
    {
        for (int i = 0; i < size; i++)
        {
            trucks[i].defesite += val;
        }
    }

    void print()
    {
        for (int i = 0; i < size; i++)
        {
            trucks[i].printInfo();
        }
    }
};

int Solution::garbageCollection(vector<string> &garbage, vector<int> &travel)
{

    for (long unsigned int x = 0; x < garbage.size(); x++)
    {
        for (auto c : garbage[x])
        {
            GarbageTruckType t = getGarbageTruckType(c);

            if (trucks[t].location != x)
            {
                trucks[t].time += trucks[t].defesite;
                trucks[t].defesite = 0;
                trucks[t].location = x;
            }
            trucks[t].time += 1;
        }
        if (x < travel.size())
        {
            addDefesiteTime(travel[x]);
        }

        if (__DEBUG_LOG)
        {
            cout << "processing - " << garbage[x] << endl;
            print();
        }
    }

    int result = 0;

    for (int i = 0; i < size; i++)
    {
        result += trucks[i].time;
    }
    return result;
}

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto garbage = vector<string>({"G", "P", "GP", "GG"});
    auto travel = vector<int>({2, 4, 3});
    auto expected = 21;

    auto result = s.garbageCollection(garbage, travel);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto garbage = vector<string>({"MMM", "PGM", "GP"});
    auto travel = vector<int>({3, 10});
    auto expected = 37;

    auto result = s.garbageCollection(garbage, travel);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test3(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto garbage = vector<string>({"G", "M"});
    auto travel = vector<int>({1});
    auto expected = 3;

    auto result = s.garbageCollection(garbage, travel);
    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "2391. Minimum Amount of Time to Collect Garbage" << endl;
    test1(1);
    test2(2);
    test3(3);

    return 0;
}