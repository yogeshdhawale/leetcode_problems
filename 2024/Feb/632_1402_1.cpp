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
#define __DEBUG_LOG 1

#define LOGDATE __TIMESTAMP__
#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2

#define RESULT_MOD 1000000007
#define MY_MAX_VAL 100001

#define LOG_MSG(msg) \
    clog << "[" << LOGDATE << "]\t" << msg << endl

#ifdef __DEBUG_LOG
#define LOG(x)                         \
    if (__DEBUG_LOG != __DEBUG_LOG_NO) \
    LOG_MSG(x)
#define LOGV(x)                        \
    if (__DEBUG_LOG >= __DEBUG_LOG_VB) \
    LOG_MSG(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

template <typename T>
ostream &operator<<(ostream &out, const vector<T> &nums)
{
    size_t size = nums.size();
    if (nums.size() > 20)
        size = 20;
    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << ",";
    }
    if (nums.size() > 20)
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
    if (nums.size() > 20)
        size = 20;

    size_t i = 0;
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
        if (i++ > size)
            break;
        out << it->second << ", ";
        // out << it->first << ":" << it->second << ", ";
    }
    if (nums.size() > 20)
    {
        out << "...";
    }
    return out;
}

struct Data
{
    int start;
    int end;
    int diff;
    bool carryForward;

public:
    Data() : start(-1), end(-1), diff(-1), carryForward(false) {}
    Data(int s, int e) : start(s), end(e), diff(e - s), carryForward(false) {}
    Data(int s) : Data(s, s) {}

    int getDistance(int x)
    {
        if (x < start)
            return start - x;
        if (x > end)
            return x - end;

        return 0;
    }
    Data(const Data &d) : start(d.start), end(d.end), diff(d.diff)
    {
        carryForward = false;
    }
    Data operator=(const Data &d)
    {
        start = d.start;
        end = d.end;
        diff = d.diff;
        carryForward = false;
        return *this;
    }
    Data getNewData(int x)
    {
        if (x < start)
            return Data(x, end);
        if (x > end)
            return Data(start, x);
        return *this;
    }
    bool overlaps(const Data &d)
    {
        if (start <= d.start && d.end <= end)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &out, const Data &d)
    {
        if (d.carryForward)
            out << "[" << d.start << "," << d.end << "]=>" << d.diff << ", ";
        else
            out << "[" << d.start << "," << d.end << "]=<" << d.diff << ", ";
        return out;
    }
};

class Solution
{
public:
    vector<int> smallestRange(vector<vector<int>> &nums)
    {
        int size = nums.size();
        // vector<int> idx = vector<int>(listCnt, 0);

        map<int, Data> *mp = new map<int, Data>();
        map<int, Data> *prevmp = new map<int, Data>();
        auto &firstList = nums[0];
        for (int i = 0; i < (int)firstList.size(); i++)
        {
            mp->insert({firstList[i], Data(firstList[i])});
        }
        for (int i = 1; i < size; i++)
        {
            auto &list = nums[i];

            LOG("List " << i << " : " << list);
            delete prevmp;
            prevmp = mp;
            mp = new map<int, Data>();
            LOG("Current Map : " << *prevmp);
            for (size_t j = 0; j < list.size(); j++)
            {
                int cur = list[j];
                if (i != 0 && cur == list[j - 1])
                    continue;
                auto ret = processElement(cur, prevmp);
                auto newd = ret.second.getNewData(cur);
                LOGV("\t\tupdating @[" << i << "," << j << "] " << cur << " entry:" << newd);
                mp->insert({newd.start, newd});
            }

            LOG("\tresutl map" << *mp);
            LOG("\tpending items : (cnt:" << prevmp->size() << ") : " << *prevmp);

            for (auto it = prevmp->begin(); it != prevmp->end();)
            {
                if (!it->second.carryForward)
                {
                    it = prevmp->erase(it);
                    continue;
                }
                Data d = it->second;
                it = prevmp->erase(it);

                if (d.end < mp->begin()->first || d.start > mp->rbegin()->first)
                {
                    mp->insert({d.start, d});
                    continue;
                }

                mergeRange(mp, d);
            }
            LOG("\tAt end of list processing " << i << " : " << *mp);
        }

        int mindiff = INT32_MAX;
        int minstart = 0;
        int minend = 0;

        LOG("Final Map : " << *mp);
        for (auto it = mp->begin(); it != mp->end(); it++)
        {
            if (it->second.diff < mindiff)
            {
                mindiff = it->second.diff;
                minstart = it->first;
                minend = it->second.end;
            }
        }

        delete mp;
        delete prevmp;
        return {minstart, minend};
    }

    void mergeRange(map<int, Data> *mp, Data &d)
    {
        auto itr = mp->lower_bound(d.start);
        if (itr == mp->end())
        {
            mp->insert({d.end, d});
            return;
        }
        if (itr->second.overlaps(d))
        {
            return;
        }
        if (distance(mp->begin(), itr) == 0)
        {
            Data newd(min(d.start, itr->first), max(d.end, itr->second.end));
            mp->erase(itr->first);
            mp->insert({newd.start, newd});

            return;
        }
        auto previtr = prev(itr);

        int prevcost = max(d.end, previtr->second.end) - min(d.start, previtr->first);
        int curcost = max(d.end, itr->second.end) - min(d.start, itr->first);

        if (previtr->second.end < d.end && d.start < itr->second.start)
        {
            mp->insert({d.start, d});
            return;
        }

        if (curcost < prevcost)
        {
            Data newd(min(d.start, itr->first), max(d.end, itr->second.end));
            mp->erase(itr->first);
            mp->insert({newd.start, newd});

            return;
        }
        else if (curcost > prevcost)
        {
            Data newd(min(d.start, previtr->first), max(d.end, previtr->second.end));
            mp->erase(previtr->first);
            mp->insert({newd.start, newd});
            return;
        }
    }

    pair<int, Data> processElement(int cur, map<int, Data> *prevmp)
    {
        auto itrfirst = prevmp->lower_bound(cur);
        auto itrlast = prevmp->upper_bound(cur);
        if (itrlast != prevmp->end())
        {
            itrlast = next(itrlast);
        }

        if (itrfirst == prevmp->end())
        {
            itrfirst = prevmp->begin();
        }
        int diff = INT32_MAX;
        auto itrpos = itrfirst;
        // LOG("\tprocessing " << cur << " -> itrfirst = " << itrfirst->first << ", itrlast = " << itrlast->first);

        if (distance(prevmp->begin(), itrfirst) > 0)
        {
            auto itprev = prev(itrfirst);
            itrpos = itprev;
            diff = itprev->second.getDistance(cur) + itprev->second.diff;
            if (itprev->second.getDistance(cur) == 0)
            {
                itprev->second.carryForward = true;
                diff = 0;
            }
        }

        if (itrfirst != itrlast)
        {
            for (auto it = itrfirst; it != itrlast; it++)
            {
                if (it->second.getDistance(cur) == 0)
                {
                    it->second.carryForward = true;
                    if (diff > 0)
                    {
                        diff = 0;
                        itrpos = it;
                    }
                }
                else if (it->second.getDistance(cur) + it->second.diff < diff)
                {
                    diff = it->second.getDistance(cur) + it->second.diff;
                    itrpos = it;
                }
            }
        }

        return {itrpos->first, itrpos->second};
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
    auto result = s.smallestRange(nums);
    cout << result << " vs " << expected << endl;
    assert(result == expected);
}

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG > __DEBUG_LOG_VB
    test2(2);
#endif

    return 0;
}
