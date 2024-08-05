/*

632. Smallest Range Covering Elements from K Lists
Hard
Topics
Companies

You have k lists of sorted integers in non-decreasing order. Find the smallest range that includes at least one number from each of the k lists.

We define the range [a, b] is smaller than range [c, d] if b - a < d - c or a < c if b - a == d - c.



Example 1:

Input: nums = [[4,10,15,24,26},{0,9,12,20},{5,18,22,30]]
Output: [20,24]
Explanation:
List 1: [4, 10, 15, 24,26], 24 is in range [20,24].
List 2: [0, 9, 12, 20], 20 is in range [20,24].
List 3: [5, 18, 22, 30], 22 is in range [20,24].

Example 2:

Input: nums = [[1,2,3},{1,2,3},{1,2,3]]
Output: [1,1]



Constraints:

    nums.length == k
    1 <= k <= 3500
    1 <= nums[i].length <= 50
    -105 <= nums[i][j] <= 10^5
    nums[i] is sorted in non-decreasing order.


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include <numeric>
#include <chrono>
#include <string_view>

using namespace std;

#define __DEBUG_PRINT_ITEMS 10

template <typename T>
ostream &operator<<(ostream &out, const vector<T> &nums)
{
    size_t size = nums.size();
    if (nums.size() > __DEBUG_PRINT_ITEMS)
        size = __DEBUG_PRINT_ITEMS;
    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << ",";
    }
    if (nums.size() > __DEBUG_PRINT_ITEMS)
    {
        out << "...";
    }
    return out;
}

template <typename T>
ostream &operator<<(ostream &out, vector<vector<T>> &nums)
{
    size_t size = nums.size();
    if (nums.size() > __DEBUG_PRINT_ITEMS)
        size = __DEBUG_PRINT_ITEMS;
    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << "::";
    }
    if (nums.size() > __DEBUG_PRINT_ITEMS)
    {
        out << "...";
    }
    return out;
}

template <typename T, typename U>
ostream &operator<<(ostream &out, const pair<T, U> &nums)
{
    out << "(" << nums.first << "," << nums.second << ")";
    return out;
}

template <typename T, typename U>
ostream &operator<<(ostream &out, const map<T, U> &nums)
{
    size_t size = nums.size();
    if (nums.size() > __DEBUG_PRINT_ITEMS)
        size = __DEBUG_PRINT_ITEMS;

    size_t i = 0;
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
        if (i++ > size)
            break;
        out << it->second << ", ";
        // out << it->first << ":" << it->second << ", ";
    }
    if (nums.size() > __DEBUG_PRINT_ITEMS)
    {
        out << "...";
    }
    return out;
}

template <typename T>
    requires(std::ranges::input_range<typename T::container_type>)
ostream &operator<<(ostream &out, const T &nums)
{

    struct Printer : T
    {
        ostream &print(ostream &out) const
        {
            out << "[" << this->c << "]";
            return out;
        }
    };

    static_cast<Printer const &>(nums).print(out);
    return out;
}

#define LOGDATE __TIMESTAMP__
#define LOG_MSG(msg) \
    clog << "[" << LOGDATE << "]\t" << msg << endl

#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2
#define __DEBUG_LOG_O 4

#define __DEBUG_LOG 4
#ifdef __DEBUG_LOG
#define LOG(x)                                                         \
    if (__DEBUG_LOG != __DEBUG_LOG_NO && __DEBUG_LOG != __DEBUG_LOG_O) \
    LOG_MSG(x)
#define LOGV(x)                        \
    if (__DEBUG_LOG == __DEBUG_LOG_VB) \
    LOG_MSG(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

#define RESULT_MOD 1000000007
#define MY_MAX_VAL 100001
typedef pair<int, int> INTPAIR;

class Solution
{
public:
    inline int get(vector<vector<int>> &nums, int listidx, vector<int> idx2d)
    {
        return nums[listidx][idx2d[listidx]];
    }

    constexpr bool operator()(INTPAIR &a, INTPAIR &b)
    {
        return a.first < b.first;
    }

    int getNextIdx(vector<int> &nums, int val, int idx, int size)
    {
        while (idx < size && nums[idx] == val)
        {
            idx++;
        }
        return idx;
    }
    vector<int> smallestRange(vector<vector<int>> &nums)
    {
        auto size = nums.size();

        priority_queue<INTPAIR, vector<INTPAIR>, greater<INTPAIR>> pq;
        int start = INT32_MAX;
        int end = INT32_MIN;

        vector<int> idx2d = vector<int>(size, 0);

        for (size_t i = 0; i < size; i++)
        {
            int listidx = i;
            pq.push({get(nums, listidx, idx2d), listidx});
            start = min(start, nums[i][0]);
            end = max(end, nums[i][0]);
        }

        LOG("Starting for size: " << size << " start: " << start << " end: " << end << " with pq " << pq);
        int rangestart = start;
        int rangeend = end;
        while (pq.size() == size)
        {
            LOG("processing pq: " << pq);

            rangestart = pq.top().first;
            int listidx = pq.top().second;
            LOG("element: @[" << listidx << ", " << idx2d[listidx] << "]=" << rangestart << " with range [" << start << ", " << end << "] vs [" << rangestart << ", " << rangeend << "]");
            pq.pop();

            if (rangeend - rangestart < end - start)
            {
                start = rangestart;
                end = rangeend;
                LOG("Updated ranges to " << start << "," << end);
            }

            idx2d[listidx] = getNextIdx(nums[listidx], rangestart, idx2d[listidx], nums[listidx].size());
            if (idx2d[listidx] == nums[listidx].size())
            {
                break;
            }
            int nextVal = get(nums, listidx, idx2d);
            rangeend = max(rangeend, nextVal);

            pq.push({get(nums, listidx, idx2d), listidx});
        }
        return {start, end};
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<vector<int>>>(
        {{{4, 10, 15, 24, 26}, {0, 9, 12, 20}, {5, 18, 22, 30}},
         {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}},
         {{1, 3, 5, 7, 9}, {2, 4, 6, 8, 10}},
         {{1, 4, 7, 10, 13}, {2, 5, 8, 11, 13}, {3, 6, 9, 12}},
         {{-5, -4, -3, -2, -1, 1}, {1, 2, 3, 4, 5}},
         {{11, 38, 83, 84, 84, 85, 88, 89, 89, 92}, {28, 61, 89}, {52, 77, 79, 80, 81}, {21, 25, 26, 26, 26, 27}, {9, 83, 85, 90}, {84, 85, 87}, {26, 68, 70, 71}, {36, 40, 41, 42, 45}, {-34, 21}, {-28, -28, -23, 1, 13, 21, 28, 37, 37, 38}, {-74, 1, 2, 22, 33, 35, 43, 45}, {54, 96, 98, 98, 99}, {43, 54, 60, 65, 71, 75}, {43, 46}, {50, 50, 58, 67, 69}, {7, 14, 15}, {78, 80, 89, 89, 90}, {35, 47, 63, 69, 77, 92, 94}}});

    auto expected = vector<vector<int>>({{20, 24}, {1, 1}, {1, 2}, {12, 13}, {1, 1}, {15, 84}});

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.smallestRange(nums[i]);
        cout << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{

    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{5, 42, 43, 48, 54, 64, 65}, {19, 20, 32, 32, 32, 33}, {6, 24, 42, 49, 53, 58, 58, 59}, {6, 14, 28, 28, 34}, {2, 39, 45, 46, 49, 50}, {7, 23, 23, 28, 43, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 45}, {70, 75, 77, 87, 97, 99, 99, 99, 100}, {-44, 7, 20, 20, 25}, {-3, 39, 62, 72, 72, 73}, {-5, 13, 15, 20}, {-18, -4, 37, 61, 64}, {0, 10, 17, 35, 38, 41}, {42, 58, 83, 86, 89, 92, 100}, {-7, 15, 16}, {-9, 83, 99}, {-75, 16, 24, 52, 61, 93, 94, 97, 97, 99, 99, 100}, {37, 42, 49, 56}, {-69, 92, 93, 95, 95, 95, 96}});
    auto expected = vector<int>({16, 92});
    cout << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.smallestRange(nums);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us" << endl;
    cout << result << " vs " << expected << endl;
    assert(result == expected);
}

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_VB
    test2(2);
#endif

    return 0;
}
