/* 1743. Restore the Array From Adjacent Pairs
Medium
Topics
Companies
Hint

There is an integer array nums that consists of n unique elements, but you have forgotten it. However, you do remember every pair of adjacent elements in nums.

You are given a 2D integer array adjacentPairs of size n - 1 where each adjacentPairs[i] = [ui, vi] indicates that the elements ui and vi are adjacent in nums.

It is guaranteed that every adjacent pair of elements nums[i] and nums[i+1] will exist in adjacentPairs, either as [nums[i], nums[i+1]] or [nums[i+1], nums[i]]. The pairs can appear in any order.

Return the original array nums. If there are multiple solutions, return any of them.



Example 1:

Input: adjacentPairs = [[2,1],[3,4],[3,2]]
Output: [1,2,3,4]
Explanation: This array has all its adjacent pairs in adjacentPairs.
Notice that adjacentPairs[i] may not be in left-to-right order.

Example 2:

Input: adjacentPairs = [[4,-2],[1,4],[-3,1]]
Output: [-2,4,1,-3]
Explanation: There can be negative numbers.
Another solution is [-3,1,4,-2], which would also be accepted.

Example 3:

Input: adjacentPairs = [[100000,-100000]]
Output: [100000,-100000]



Constraints:

    nums.length == n
    adjacentPairs.length == n - 1
    adjacentPairs[i].length == 2
    2 <= n <= 105
    -105 <= nums[i], ui, vi <= 105
    There exists some nums that has adjacentPairs as its pairs.

*/

#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <algorithm>
#include <iterator>
#include <chrono>

#define __DEBUG_LOG 0

using namespace std;

class Solution
{
public:
    vector<int> restoreArray(vector<vector<int>> &);
};

class Segment
{
    int segment_id;
    vector<int> data;
    static int id;

public:
    Segment(int start, int end)
    {
        segment_id = Segment::id++;
        data.reserve(106);
        data.push_back(start);
        data.push_back(end);
    }

    Segment(Segment &obj) : segment_id(obj.segment_id), data(obj.data)
    {
    }
    /*
    ~Segment()
    {
        cout << "Segment " << segment_id << " deleted" << endl;
    }*/

    int size()
    {
        return data.size();
    }

    vector<int> getData()
    {
        return data;
    }

    pair<int, int> getEnds()
    {
        return make_pair(data.front(), data.back());
    }
    int getSegmentId()
    {
        return segment_id;
    }

    void addToSegment(int ancher, int new_value)
    {
        if (data.front() == ancher)
            data.insert(data.begin(), new_value);
        else
            data.push_back(new_value);
    }
    friend Segment &MergeSegments(Segment &, Segment &, int a, int b);

    void print(string message)
    {
        if (__DEBUG_LOG != 1)
            return;

        cout << "Segment " << segment_id << " (" << message << ") :";
        for (auto x : data)
        {
            cout << " " << x;
        }
        cout << endl;
    }
};

int Segment::id = 1;

Segment &MergeSegments(Segment &s_a, Segment &s_b, int a, int b)
{
    if (s_a.size() < s_b.size())
    {
        return MergeSegments(s_b, s_a, b, a);
    }

    Segment &big = s_a;
    Segment &small = s_b;

    if (big.data.back() == a || big.data.back() == b)
    {
        if (small.data.front() == a || small.data.front() == b)
            move(small.data.begin(), small.data.end(), back_inserter(big.data));
        else
            move(small.data.rbegin(), small.data.rend(), back_inserter(big.data));
    }
    else
    {
        if (small.data.front() == a || small.data.front() == b)
            big.data.insert(big.data.begin(), small.data.rbegin(), small.data.rend());
        else
            big.data.insert(big.data.begin(), small.data.begin(), small.data.end());
    }
    return big;
}

vector<int>
Solution::restoreArray(vector<vector<int>> &adjacentPairs)
{
    map<int, int> s_ends;
    map<int, Segment> segments;

    for (size_t i = 0; i < adjacentPairs.size(); i++)
    {
        int a = adjacentPairs[i][0];
        int b = adjacentPairs[i][1];
        if (__DEBUG_LOG)
            cout << "Processing (" << a << ", " << b << ")\t";

        if (s_ends.find(a) == s_ends.end() && s_ends.find(b) == s_ends.end())
        {
            // new segment
            Segment current_segment = *(new Segment(a, b));
            segments.insert_or_assign(current_segment.getSegmentId(), current_segment);

            s_ends[a] = current_segment.getSegmentId();
            s_ends[b] = current_segment.getSegmentId();
            current_segment.print("new");
        }
        else if (s_ends.find(a) != s_ends.end() && s_ends.find(b) != s_ends.end())
        {
            // merge two segments
            int segment_id_a = s_ends[a];
            int segment_id_b = s_ends[b];
            Segment &s_a = segments.find(segment_id_a)->second;
            Segment &s_b = segments.find(segment_id_b)->second;

            s_a.print("M1");
            s_b.print("M2");

            s_ends.erase(s_a.getEnds().first);
            s_ends.erase(s_a.getEnds().second);
            s_ends.erase(s_b.getEnds().first);
            s_ends.erase(s_b.getEnds().second);

            Segment &result = MergeSegments(s_a, s_b, a, b);
            result.print("merged");

            s_ends[result.getEnds().first] = result.getSegmentId();
            s_ends[result.getEnds().second] = result.getSegmentId();

            if (result.getSegmentId() == s_a.getSegmentId())
            {
                segments.erase(s_b.getSegmentId());
            }
            else
            {
                segments.erase(s_a.getSegmentId());
            }
        }
        else
        {
            // s_end.find(a) != s_ends.end()
            int anchor = a;
            int newvalue = b;

            if (s_ends.find(a) == s_ends.end())
            {
                anchor = b;
                newvalue = a;
            }

            // add to segment
            int segment_id = s_ends[anchor];
            Segment &s = segments.find(s_ends[anchor])->second;
            s.addToSegment(anchor, newvalue);
            s_ends[newvalue] = segment_id;
            s_ends.erase(anchor);
            s.print("extended");
        }
    }

    if (segments.size() != 1)
    {
        return {};
    }
    else
    {
        Segment &result = segments.begin()->second;
        return result.getData();
    }
}

void test1(Solution &s, int testID)
{
    cout << "test" << testID << endl;

    vector<vector<int>> adjacentPairs = {{2, 1}, {3, 4}, {3, 2}};
    auto expected = vector<int>{1, 2, 3, 4};
    auto result = s.restoreArray(adjacentPairs);
    assert(expected.size() == result.size());
}

void test2(Solution &s, int testID)
{
    cout << "test" << testID << endl;

    vector<vector<int>> adjacentPairs = {{4, -2}, {1, 4}, {-3, 1}};
    auto expected = vector<int>{4, -2, 1, -3};
    auto result = s.restoreArray(adjacentPairs);
    assert(expected.size() == result.size());
}

void test3(Solution &s, int testID)
{
    cout << "test" << testID << endl;

    vector<vector<int>> adjacentPairs = {{1000, -110009}};
    auto expected = vector<int>{1000, -110009};
    auto result = s.restoreArray(adjacentPairs);
    assert(expected.size() == result.size());
}

void test4(Solution &s, int testID)
{
    cout << "test" << testID << endl;

    vector<vector<int>> adjacentPairs = {{-3, -9}, {-5, 3}, {2, -9}, {6, -3}, {6, 1}, {5, 3}, {8, 5}, {-5, 1}, {7, 2}};
    auto expected = vector<int>{-3, -9, -5, 3, 2, 6, 1, 5, 8, 7};
    auto result = s.restoreArray(adjacentPairs);
    assert(expected.size() == result.size());
}

void test5(Solution &s, int testID)
{
    cout << "test" << testID << endl;

    vector<vector<int>> adjacentPairs = {{91207, 59631}, {581, 91207}, {51465, 20358}, {-66119, 94118}, {-7392, 72809}, {51465, -7392}, {-98504, -29411}, {-98504, 35968}, {35968, 59631}, {94118, 20358}, {72809, 581}, {34348, 43653}, {43653, -29411}};
    auto result = s.restoreArray(adjacentPairs);
    assert(14 == result.size());
}

int main()
{

    cout << "1743. Restore the Array From Adjacent Pairs" << endl;

    Solution s;
    auto start = std::chrono::system_clock::now();

    /**
    test1(s, 1);
    test2(s, 2);
    test3(s, 3);
    test4(s, 4);
    */
    test5(s, 5);
    auto end = std::chrono::system_clock::now();
    cout << "Time taken : " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << endl;

    return 0;
}
