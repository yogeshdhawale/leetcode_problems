/*
630. Course Schedule III
Hard
Topics
Companies
Hint

There are n different online courses numbered from 1 to n. You are given an array courses where courses[i] = [durationi, lastDayi] indicate that the ith course should be taken continuously for durationi days and must be finished before or on lastDayi.

You will start on the 1st day and you cannot take two or more courses simultaneously.

Return the maximum number of courses that you can take.



Example 1:

Input: courses = [[100,200},{200,1300},{1000,1250},{2000,3200]]
Output: 3
Explanation:
There are totally 4 courses, but you can take 3 courses at most:
First, take the 1st course, it costs 100 days so you will finish it on the 100th day, and ready to take the next course on the 101st day.
Second, take the 3rd course, it costs 1000 days so you will finish it on the 1100th day, and ready to take the next course on the 1101st day.
Third, take the 2nd course, it costs 200 days so you will finish it on the 1300th day.
The 4th course cannot be taken now, since you will finish it on the 3300th day, which exceeds the closed date.

Example 2:

Input: courses = [[1,2]]
Output: 1

Example 3:

Input: courses = [[3,2},{4,3]]
Output: 0



Constraints:

    1 <= courses.length <= 104
    1 <= durationi, lastDayi <= 104


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
#define __DEBUG_LOG 0

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
        out << it->first << ":" << it->second << ", ";
    }
    if (nums.size() > 20)
    {
        out << "...";
    }
    return out;
}

struct Data
{
public:
    int duration;
    int lastDay;

    Data(int i, int s, int d, int ld) : duration(d), lastDay(ld)
    {
    }
    Data(const Data &d) : duration(d.duration), lastDay(d.lastDay) {}

    friend ostream &operator<<(ostream &out, const Data &d)
    {
        out << "\t @"
            << " [" << d.duration << " : " << d.lastDay << "]";
        return out;
    }

    bool isSchedulable(int starttime)
    {
        return (starttime + duration) <= lastDay;
    }
    bool isSchedulable(int starttime, int endtime)
    {
        return (starttime + duration) <= endtime;
    }
};

struct MData
{
    vector<Data *> v;

public:
    MData() {}
    void push(Data *d)
    {
        v.push_back(d);
    }
    friend ostream &operator<<(ostream &out, const MData &d)
    {
        out << "[" << d.v.size() << "] - ";
        for (auto it : d.v)
            out << *it << ", ";
        return out;
    }
};

class Solution
{
    int miSize;
    map<int, MData> courseByLastDay;

public:
    Solution() : miSize(0)
    {
    }

    /*
    int scheduleCourse(std::vector<std::vector<int>> &courses)
    {
        std::sort(courses.begin(), courses.end(), [](const std::vector<int> &a, const std::vector<int> &b)
                  { return a[1] < b[1]; });

        std::priority_queue<int> pq;
        int time = 0;
        for (const auto &course : courses)
        {
            time += course[0];
            pq.push(course[0]);
            if (time > course[1])
            {
                time -= pq.top();
                pq.pop();
            }
        }

        return pq.size();
    }*/
    int scheduleCourse(vector<vector<int>> &courses)
    {
        miSize = courses.size();
        int validCourses = 0;
        for (int i = 0; i < miSize; i++)
        {
            int maxStart = courses[i][1] - courses[i][0];
            if (maxStart < 0)
                continue;

            Data *d = new Data(i, maxStart, courses[i][0], courses[i][1]);
            courseByLastDay[d->lastDay].push(d);
            validCourses++;
        }

        if (courseByLastDay.size() == 0)
            return 0;

        if (validCourses == 1)
            return 1;

        auto it = courseByLastDay.begin();

        int maxCnt = 0;
        int processedCourses = 0;
        int takenCoursesCnt = 0;

        map<int, int> takenByDur;
        int currentDuration = 0;

        for (; it != courseByLastDay.end(); it++)
        {
            // LOG("Processing lastDay " << it->first << " size[" << (it->second.v.size()) << "] maxCnt:" << maxCnt << " done:" << processedCourses);

            vector<Data *> v = it->second.v;
            sort(v.begin(), v.end(), [](Data *a, Data *b)
                 { return a->duration < b->duration; });

            for (auto v : it->second.v)
            {
                if (processedCourses == validCourses)
                    break;

                processedCourses++;
                takenCoursesCnt++;

                currentDuration += v->duration;
                takenByDur[v->duration] += 1;

                LOG("for " << (*v) << " dur:" << currentDuration << " taken:" << takenCoursesCnt << " done:" << processedCourses);

                if (currentDuration <= v->lastDay)
                {
                    maxCnt = max(maxCnt, takenCoursesCnt);
                    continue;
                }

                while (currentDuration > v->lastDay)
                {
                    if (takenCoursesCnt == 0)
                    {
                        break;
                    }

                    currentDuration -= takenByDur.rbegin()->first;
                    takenCoursesCnt--;
                    LOG("\t substracted " << takenByDur.rbegin()->first << " new dur:" << currentDuration << " taken:" << takenCoursesCnt << " done:" << processedCourses);
                    takenByDur.rbegin()->second -= 1;
                    if (takenByDur.rbegin()->second == 0)
                        takenByDur.erase(takenByDur.rbegin()->first);
                }

                maxCnt = max(maxCnt, takenCoursesCnt);
                LOG("\t check " << (*v) << " dur:" << currentDuration << " taken:" << takenCoursesCnt << " done:" << processedCourses << " maxCnt:" << maxCnt);
            }
            // LOG("for-end dur:" << currentDuration << " taken:" << takenCourses.size() << " done:" << processedCourses << " maxCnt:" << maxCnt);
        }
        return maxCnt;
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<vector<int>>>(
        {{{1, 2}},
         {{3, 2}, {4, 3}},
         {{100, 200}, {200, 1300}, {1000, 1250}, {2000, 3200}},
         {{7, 16}, {2, 3}, {3, 12}, {3, 14}, {10, 19}, {10, 16}, {6, 8}, {6, 11}, {3, 13}, {6, 16}},
         {{5, 5}, {4, 6}, {2, 6}},
         {{5, 5}, {4, 6}, {2, 4}},
         {{7, 17}, {3, 12}, {10, 20}, {9, 10}, {5, 20}, {10, 19}, {4, 18}},
         {{9, 14}, {7, 12}, {1, 11}, {4, 7}},
         {{1, 2}, {2, 3}},
         {{1, 2}},
         {{3, 14}, {6, 18}, {10, 16}, {3, 4}, {4, 5}, {7, 15}, {2, 9}, {1, 6}, {10, 13}, {5, 16}}});

    auto expected = vector<int>({1, 0, 3, 4, 2, 2, 4, 3, 2, 1, 5});

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.scheduleCourse(nums[i]);
        cout << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{

    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{9, 20}, {4, 14}, {4, 10}, {6, 7}, {2, 14}, {8, 10}, {6, 6}, {5, 7}});
    auto expected = 4;
    cout << "test for: " << nums << endl;

    Solution s;
    auto result = s.scheduleCourse(nums);
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
