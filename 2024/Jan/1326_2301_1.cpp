/*
1326. Minimum Number of Taps to Open to Water a Garden
Hard
Topics
Companies
Hint

There is a one-dimensional garden on the x-axis. The garden starts at the point 0 and ends at the point n. (i.e., the length of the garden is n).

There are n + 1 taps located at points [0, 1, ..., n] in the garden.

Given an integer n and an integer array ranges of length n + 1 where ranges[i] (0-indexed) means the i-th tap can water the area [i - ranges[i], i + ranges[i]] if it was open.

Return the minimum number of taps that should be open to water the whole garden, If the garden cannot be watered return -1.



Example 1:

Input: n = 5, ranges = [3,4,1,1,0,0]
Output: 1
Explanation: The tap at point 0 can cover the interval [-3,3]
The tap at point 1 can cover the interval [-3,5]
The tap at point 2 can cover the interval [1,3]
The tap at point 3 can cover the interval [2,4]
The tap at point 4 can cover the interval [4,4]
The tap at point 5 can cover the interval [5,5]
Opening Only the second tap will water the whole garden [0,5]

Example 2:

Input: n = 3, ranges = [0,0,0,0]
Output: -1
Explanation: Even if you activate all the four taps you cannot water the whole garden.



Constraints:

    1 <= n <= 104
    ranges.length == n + 1
    0 <= ranges[i] <= 100


*/

#include <iostream>

#include <vector>
#include <map>

#include <algorithm>
#include <cassert>

using namespace std;

#define __DEBUG_LOG 0

template <typename T>
void printVector(vector<T> &nums, size_t end = -1)
{
    size_t size = nums.size();
    if (end >= 0 && end < size)
        size = end;

    for (size_t i = 0; i < size; i++)
    {
        cout << nums[i] << " ";
    }
    if (end == size)
    {
        cout << "...";
    }
    cout << endl;
}

void printMap(int i, const map<int, int> &nums)
{
    cout << "for i[" << i << "] map size: " << nums.size() << "\t";
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
        cout << it->first << " " << it->second << ", ";
    }
    cout << endl;
}

void addToSorted(vector<int> &nums, int end)
{
    size_t index = lower_bound(nums.begin(), nums.end(), end) - nums.begin();
    if (index < nums.size() && nums[index] == end)
        return;
    nums.insert(nums.begin() + index, end);
}
class Solution
{
    vector<vector<int>> positions;
    map<int, int> cache;

public:
    int minTaps(int n, vector<int> &ranges)
    {
        if (n == 0)
            return 0;

        int minstart = INT32_MAX;
        int maxend = INT32_MIN;

        positions.clear();
        positions.resize(n + 1, vector<int>());

        for (int i = 0; i <= n; i++)
        {
            if (ranges[i] > 0)
            {
                int start = max(0, i - ranges[i]);
                int end = min(n, i + ranges[i]);

                if (start == 0 && end == n)
                {
                    return 1;
                }
                if (__DEBUG_LOG)
                {
                    cout << "New range: [" << start << ", " << end << "]" << endl;
                }
                for (int j = start; j < end; j++)
                {
                    addToSorted(positions[j], end);
                }
                minstart = min(minstart, start);
                maxend = max(maxend, end);
            }
        }

        if (__DEBUG_LOG)
        {
            for (int i = 0; i <= n; i++)
            {
                cout << "position: " << i << ", end vector: ";
                printVector(positions[i]);
            }
        }
        if (minstart != 0 || maxend != n)
            return -1;

        return processCurrentIndex(n, 0);
    }

    int processCurrentIndex(int n, size_t idx)
    {
        if (positions[idx].size() == 0)
        {
            return -1;
        }

        if (cache.find(idx) != cache.end())
        {
            return cache[idx];
        }

        int minVal = INT32_MAX;
        vector<int> &cur = positions[idx];

        if (cur.back() == n)
        {
            if (__DEBUG_LOG)
            {
                cout << "@idx: " << idx << " minVal: 1" << endl;
            }
            cache[idx] = 1;
            return 1;
        }

        for (auto it2 = cur.rbegin(); it2 != cur.rend(); it2++)
        {
            minVal = min(minVal, processCurrentIndex(n, *it2));
        }

        if (minVal == -1 || minVal == INT32_MAX)
        {
            cache[idx] = -1;
            return -1;
        }

        if (__DEBUG_LOG)
        {
            cout << "@idx: " << idx << " minVal: " << minVal + 1 << endl;
        }
        cache[idx] = minVal + 1;
        return cache[idx];
    }
};

void test1(int testID)
{
    cout << "###test" << testID << endl;

    auto nums = vector<pair<int, vector<int>>>({
        {5, {3, 4, 1, 1, 0, 0}},
        {3, {0, 0, 0, 0}},
        {8, {4, 0, 0, 0, 0, 0, 0, 0, 4}},
    });
    auto expected = vector<int>({1, -1, 2});

#if __DEBUG_LOG == 3
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        Solution s;

        cout << "\n###loop: " << i << " test for: size: " << nums[i].first << " vector size: " << nums[i].second.size() << endl;
        printVector(nums[i].second);
        auto result = s.minTaps(nums[i].first, nums[i].second);

        cout << "result " << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{
    cout << "###test" << testID << endl;
    vector<int> data = {1, 2, 1, 0, 2, 1, 0, 1};

    cout << " test for: size: " << data.size() - 1 << endl;
    printVector(data, 10);

    Solution s;
    auto result = s.minTaps(7, data);
    cout << "result " << result << endl;
    assert(result == 3);
}

int main()
{
    test1(1);
    test2(2);
    return 0;
}