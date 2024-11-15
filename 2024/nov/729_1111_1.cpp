
/*
729. My Calendar I
Medium
Topics
Companies
Hint

You are implementing a program to use as your calendar. We can add a new event if adding the event will not cause a double booking.

A double booking happens when two events have some non-empty intersection (i.e., some moment is common to both events.).

The event can be represented as a pair of integers startTime and endTime that represents a booking on the half-open interval [startTime, endTime), the range of real numbers x such that startTime <= x < endTime.

Implement the MyCalendar class:

    MyCalendar() Initializes the calendar object.
    boolean book(int startTime, int endTime) Returns true if the event can be added to the calendar successfully without causing a double booking. Otherwise, return false and do not add the event to the calendar.



Example 1:

Input
["MyCalendar", "book", "book", "book"]
[[], [10, 20], [15, 25], [20, 30]]
Output
[null, true, false, true]

Explanation
MyCalendar myCalendar = new MyCalendar();
myCalendar.book(10, 20); // return True
myCalendar.book(15, 25); // return False, It can not be booked because time 15 is already booked by another event.
myCalendar.book(20, 30); // return True, The event can be booked, as the first event takes every time less than 20, but not including 20.



Constraints:

    0 <= start < end <= 109
    At most 1000 calls will be made to book.


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include<list>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

/****************************************************************************/
//#define __DEBUG_LOG __DEBUG_LOG_ON
//#define __DEBUG_BIG_TEST
enum TestOptions {
    TEST_ALL = 10,
    TEST_SINGLE = 11,
};

struct TestConfig {
    TestOptions options;
    int testCaseID;
};
#include "log.h"
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 1);

using namespace std;

/****************************************************************************/

#ifndef __DEBUG_LOG
#define LOG(x)
#define LOGV(x)
#endif

#define INVALID -1
#define MY_MAX_VAL INT32_MAX
#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;

typedef V2Int INPUT_TYPE;
typedef vector<bool> OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    vector<string> mData1;
    INPUT_TYPE mData2;

    Input() {
        mID = inputID++;
    }
    Input(vector<string> d1, INPUT_TYPE d2) {
        mID = inputID++;
        this->mData1 = d1;
        this->mData2 = d2;
    }
};

class MyCalendar {

    map<int, int> sb_map;
public:
    MyCalendar() {
        sb_map.clear();
    }

    void log() {
        LOG("Schedule: " << sb_map);
    }
    bool book(int startTime, int endTime) {
        if (sb_map.size() == 0) {
            sb_map[startTime] = endTime;
            return true;
        }

        auto cur_sch = sb_map.lower_bound(startTime);
        if (cur_sch == sb_map.begin()) {
            if (startTime == cur_sch->first || endTime > cur_sch->first) {
                return false;
            }
            if (endTime == cur_sch->first) {
                auto new_e_time = cur_sch->second;
                sb_map.erase(cur_sch);
                sb_map[startTime] = new_e_time;
            }
            else
                sb_map[startTime] = endTime;
            return true;
        }
        auto prev_sch = prev(cur_sch);
        if (cur_sch == sb_map.end()) {
            if (startTime < prev_sch->second) return false;
            if (startTime == prev_sch->second)
                prev_sch->second = endTime;
            else
                sb_map[startTime] = endTime;
            return true;
        }

        if (startTime == prev_sch->second && endTime == cur_sch->first) {
            prev_sch->second = cur_sch->second;
            sb_map.erase(cur_sch);
            return true;
        }

        if (startTime >= prev_sch->second && endTime <= cur_sch->first) {
            if (startTime == prev_sch->second) prev_sch->second = endTime;
            else if (endTime == cur_sch->first) {
                auto new_e_time = cur_sch->second;
                sb_map.erase(cur_sch);
                sb_map[startTime] = new_e_time;
            }
            else
                sb_map[startTime] = endTime;
            return true;
        }
        return false;
    }
};

class Solution {
public:

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        vector<bool> result;
        MyCalendar* obj = new MyCalendar();
        for (int i = 0; i < (int)nums.mData1.size(); ++i) {
            if (nums.mData1[i] != "book") {
                obj->log();
                delete obj;
                obj = new MyCalendar();
                result.push_back(false);
                continue;
            }
            result.push_back(
                obj->book(nums.mData2[i][0], nums.mData2[i][1])
            );
        }
        obj->log();
        return result;
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData2 << " }";
    return out;
}


/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int GenerateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ "MyCalendar", "book", "book", "book" }, { {}, {10,20}, {15,25}, {20,30} }));
    expected.push_back({ false, true, false, true });

    nums.push_back(Input({ "MyCalendar", "book", "book", "book" }, { {}, {10,20}, {22,25}, {25,30} }));
    expected.push_back({ false, true, true, true });

    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.ProxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] =>\t"
            << result << " vs e:" << expected[i] << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#ifdef __DEBUG_BIG_TEST


V1Int load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    nums.push_back(
        Input(load_data_v2(t1i1), load_data_int(t1i2))
    );
    expected.push_back(load_data_int(t1e1));

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "^^^Start^^^" << endl;
    test1(1);
#ifdef __DEBUG_BIG_TEST
    test2(2);
#endif
    cout << "^^^END^^^" << endl;
    return 0;
}


