/*

4. Median of Two Sorted Arrays
Solved
Hard
Topics
Companies

Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays.

The overall run time complexity should be O(log (m+n)).



Example 1:

Input: nums1 = [1,3], nums2 = [2]
Output: 2.00000
Explanation: merged array = [1,2,3] and median is 2.

Example 2:

Input: nums1 = [1,2], nums2 = [3,4]
Output: 2.50000
Explanation: merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.



Constraints:

    nums1.length == m
    nums2.length == n
    0 <= m <= 1000
    0 <= n <= 1000
    1 <= m + n <= 2000
    -106 <= nums1[i], nums2[i] <= 106



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

#define __DEBUG_LOG 1

/****************************************************************************/

#define LOGDATE __TIMESTAMP__
#define LOG_MSG(msg) \
    clog << "[" << LOGDATE << "]\t" << msg << endl

#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2
#define __DEBUG_LOG_O 4

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

struct Data
{
};

class Solution
{
public:
    inline double getMedian(vector<int> &nums, size_t startidx, size_t size)
    {
        int skipAtStart = startidx;
        int range = size - skipAtStart;
        int pos = startidx + range / 2;
        LOG("getMedian: @[" << startidx << "," << size - 1 << "] pos=" << pos << " with nums:" << nums);
        if (range % 2 == 0)
        {
            return (nums[pos] + nums[pos - 1]) / 2.0;
        }
        else
        {
            return nums[pos];
        }
    }

    double getMedianWith(vector<int> &nums, size_t startidx, size_t size, int other)
    {
        int skipAtStart = startidx;
        int range = size - skipAtStart;
        int pos = startidx + range / 2;
        LOG("getMedian: @[" << startidx << "," << size - 1 << "] pos=" << pos << " other=" << other << " with nums:" << nums);

        int n1 = nums[pos - 1];
        int n2 = other;
        int n3 = nums[pos];

        if (n2 < n1)
            swap(n1, n2);

        if (range % 2 != 0)
        {
            return (n2 + n3) / 2.0;
        }
        return n2;
    }
    double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2)
    {
        auto len1 = nums1.size();
        auto len2 = nums2.size();

        if (len1 == 0 || len2 == 0)
        {
            if (len1 == 0)
                return getMedian(nums2, 0, len2);
            return getMedian(nums1, 0, len1);
        }
        size_t medianPos = (len1 + len2) / 2;
        bool isOdd = (len1 + len2) % 2;
        LOG("medianPos: " << medianPos << " isOdd: " << isOdd);

        vector<int> *first = &nums1;
        vector<int> *second = &nums2;
        size_t curpos = 0;
        size_t i = 0;
        size_t j = 0;

        while (i < len1 && j < len2)
        {
            if (first->at(i) > second->at(j))
            {
                swap(first, second);
                swap(i, j);
                swap(len1, len2);
            }
            LOG(" @[" << i << "," << j << "] : " << first->at(i) << " vs " << second->at(j));

            if (first->at(i) > second->at(j))
                j = lower_bound(second->begin() + j, second->end(), first->at(i)) - second->begin() + j;

            if (j >= len2)
            {
                break;
            }

            LOG(" At[" << i << "," << j << "] : " << first->at(i) << " vs " << second->at(j));
            curpos = i + j + 1;

            if (curpos != medianPos)
            {
                if (curpos > medianPos)
                    break;
                i++;
                continue;
            }
            LOG("Found @[" << i << "," << j << "] : " << first->at(i) << " vs " << second->at(j) << " isodd:" << isOdd);
            if (isOdd)
            {
                if (i != len1 - 1)
                    return min(first->at(i + 1), second->at(j));
                else
                    return second->at(j);
            }
            else
            {
                int n1 = first->at(i);
                int n2 = second->at(j);
                if (i != len1 - 1)
                {
                    n2 = min(n2, first->at(i + 1));
                }
                return double(n1 + n2) / 2.0;
            }
        }

        curpos = i + j - 1;
        if (i == len1)
        {
            return getMedianWith(*second, j, len2 - curpos, first->at(i - 1));
        }

        if (j == len2)
        {
            return getMedianWith(*first, i, len1 - curpos, second->at(j - 1));
        }

        LOG(" @[" << i << "," << j << "] : " << first->at(i) << " vs " << second->at(j));
        if (isOdd)
        {
            if (i != len1 - 1)
                return min(first->at(i + 1), second->at(j));
            else
                return second->at(j);
        }
        else
        {
            int n1 = first->at(i);
            int n2 = second->at(j);
            if (i != len1 - 1)
            {
                n2 = min(n2, first->at(i + 1));
            }
            return double(n1 + n2) / 2.0;
        }
        return 0.0;
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<vector<int>>>(
        {
            {{1, 3}, {2}},
            {{1, 2}, {3, 4}},
            {{1, 2}, {7, 8, 9}},
            {{1, 2}, {3, 4, 5, 6}},
            {{1}, {3, 4, 5, 6}},
            {{1, 3}, {2, 7}},
            {{3}, {-2, -1}},
            {{1, 2}, {-1, 3}},
            {{4}, {1, 2, 3, 5}},
            {{2, 2, 2}, {2, 2, 2, 2}},
            {{2}, {1, 3, 4}},
            {{2, 3, 4}, {1}},
        });

    auto expected = vector<double>({2, 2.5, 7, 3.5, 4, 2.5, -1.0, 1.5, 3, 2, 2.5, 2.5});

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.findMedianSortedArrays(nums[i][0], nums[i][1]);
        cout << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{

    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{2}, {1, 3, 4, 5, 6}});
    auto expected = 3.5;
    cout << "test for: " << nums << endl;

    Solution s;
    auto startidxtime = std::chrono::high_resolution_clock::now();
    auto result = s.findMedianSortedArrays(nums[0], nums[1]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - startidxtime).count() << "us" << endl;
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
