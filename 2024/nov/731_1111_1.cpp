
/*731. My Calendar II
Medium
Topics
Companies
Hint

You are implementing a program to use as your calendar. We can add a new event if adding the event will not cause a triple booking.

A triple booking happens when three events have some non-empty intersection (i.e., some moment is common to all the three events.).

The event can be represented as a pair of integers startTime and endTime that represents a booking on the half-open interval [startTime, endTime), the range of real numbers x such that startTime <= x < endTime.

Implement the MyCalendarTwo class:

    MyCalendarTwo() Initializes the calendar object.
    boolean book(int startTime, int endTime) Returns true if the event can be added to the calendar successfully without causing a triple booking. Otherwise, return false and do not add the event to the calendar.



Example 1:

Input
["MyCalendarTwo", "book", "book", "book", "book", "book", "book"]
[[}, {10, 20}, {50, 60}, {10, 40}, {5, 15}, {5, 10}, {25, 55]]
Output
[null, true, true, true, false, true, true]

Explanation
MyCalendarTwo myCalendarTwo = new MyCalendarTwo();
myCalendarTwo.book(10, 20); // return True, The event can be booked.
myCalendarTwo.book(50, 60); // return True, The event can be booked.
myCalendarTwo.book(10, 40); // return True, The event can be double booked.
myCalendarTwo.book(5, 15);  // return False, The event cannot be booked, because it would result in a triple booking.
myCalendarTwo.book(5, 10); // return True, The event can be booked, as it does not use time 10 which is already double booked.
myCalendarTwo.book(25, 55); // return True, The event can be booked, as the time in [25, 40) will be double booked with the third event, the time [40, 50) will be single booked, and the time [50, 55) will be double booked with the second event.



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
#define __DEBUG_BIG_TEST
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
struct TestCaseStruct
{
    int mID;
    vector<string> mData1;
    INPUT_TYPE mData2;
    OUTPUT_TYPE mExpected;

    TestCaseStruct() {
        mID = inputID++;
    }
    void SetInput(vector<string> d1, INPUT_TYPE d2) {
        this->mData1 = d1;
        this->mData2 = d2;
    }
    void SetOutput(OUTPUT_TYPE o1) {
        this->mExpected = o1;
    }
};

class MyCalendarTwo {

    map<int, int> sb_map;
    map<int, int> db_map;
public:
    MyCalendarTwo() {
        sb_map.clear();
        db_map.clear();
    }

    void log() {
        LOG("Schedule  : " << sb_map);
        LOG("Double Sch: " << db_map);
    }
    bool book(int startTime, int endTime) {

        if (sb_map.size() == 0) {
            sb_map[startTime] = endTime;
            return true;
        }
        if (tripple_booked(startTime, endTime)) {
            LOG("Attempting for tripple booking");
            return false;
        }
        update_booking(startTime, endTime);
        return true;
    }

    bool tripple_booked(int startTime, int endTime) {
        if (db_map.size() == 0) return false;
        if (endTime <= db_map.begin()->first) return false;
        if (startTime >= db_map.rbegin()->second) return false;

        auto it = db_map.lower_bound(startTime);
        if (startTime == it->first) return true;
        if (endTime > it->first) return true;

        auto pit = prev(it);
        if (startTime < pit->second) return true;

        return false;
    }
    void update_booking(int startTime, int endTime) {

        auto it = sb_map.begin();
        if (endTime < it->first) {
            sb_map[startTime] = endTime;
            return;
        }
        if (endTime == it->first) {
            auto newEndTime = it->second;
            sb_map.erase(it);
            sb_map[startTime] = newEndTime;
            return;
        }

        it = prev(sb_map.end());
        if (startTime == it->second) {
            it->second = endTime;
            return;
        }
        if (startTime > it->second) {
            sb_map[startTime] = endTime;
            return;
        }

        auto newStartTime = startTime;
        auto newEndTime = endTime;
        it = sb_map.lower_bound(startTime);

        if (it != sb_map.begin()) {
            auto pit = prev(it);

            if (startTime == pit->second && endTime == it->first) {
                pit->second = it->second;
                sb_map.erase(it);
                return;
            }

            else if (startTime == pit->second) {
                newStartTime = pit->first;
                sb_map.erase(pit);
            }
            else if (startTime < pit->second) {
                if (endTime <= pit->second) {
                    update_db_map(startTime, endTime);
                    return;
                }
                update_db_map(startTime, pit->second);
                newStartTime = pit->first;
                sb_map.erase(pit);
            }
        }

        while (it != sb_map.end()) {
            if (endTime < it->first) {
                sb_map[newStartTime] = endTime;
                return;
            }
            else if (endTime == it->first) {
                sb_map[newStartTime] = it->second;
                sb_map.erase(it);
                return;
            }
            else if (endTime <= it->second) {
                update_db_map(it->first, min(endTime, it->second));
                newEndTime = it->second;
                sb_map.erase(it);
                sb_map[newStartTime] = newEndTime;
                return;
            }
            update_db_map(it->first, it->second);
            it = sb_map.erase(it);
        }
        sb_map[newStartTime] = newEndTime;
    }
    void update_db_map(int startTime, int endTime) {

        if (startTime == endTime) {
            return;
        }

        auto it = db_map.lower_bound(startTime);

        if (it != db_map.begin() && it != db_map.end()) {

            auto prev_it = prev(it);
            if (startTime > prev_it->second && endTime < it->first) {
                db_map[startTime] = endTime;
            }
            else if (startTime == prev_it->second && endTime == it->first) {
                prev_it->second = it->second;
                db_map.erase(it);
            }
            else if (startTime == prev_it->second) {
                prev_it->second = endTime;
            }
            else if (endTime == it->first) {
                db_map[startTime] = it->second;
                db_map.erase(it);
            }
            return;
        }

        if (it == db_map.begin()) {
            if (endTime == it->first) {
                db_map[startTime] = it->second;
                db_map.erase(it);
            }
            else
                db_map[startTime] = endTime;
        }
        else if (it == db_map.end()) {
            auto prev_it = prev(it);
            if (startTime == prev_it->second)
                prev_it->second = endTime;
            else
                db_map[startTime] = endTime;
        }
    }
};

class Solution {
public:

    OUTPUT_TYPE ProxyFunction(TestCaseStruct& nums) {
        vector<bool> result;
        MyCalendarTwo* obj = new MyCalendarTwo();
        for (int i = 0; i < (int)nums.mData1.size(); ++i) {
            //if (nums.mData2[i].size() > 0 && nums.mData2[i][0] == 5)
            obj->log();
            if (nums.mData1[i] != "book") {
                delete obj;
                obj = new MyCalendarTwo();
                result.push_back(false);
                continue;
            }
            LOG("@" << i << " Process: " << nums.mData2[i]);
            auto ret = obj->book(nums.mData2[i][0], nums.mData2[i][1]);
            LOG("@" << i << " Result: " << ret << " Expected: " << nums.mExpected[i]);
            assert(ret == nums.mExpected[i]);
            result.push_back(ret);
        }
        obj->log();
        return result;
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
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
    vector<TestCaseStruct> nums;

    {
        nums.push_back(TestCaseStruct());
        nums.back().SetInput(
            { "MyCalendar", "book", "book", "book","book", "book", "book" },
            { {}, {10, 20}, {50, 60}, {10, 40}, {5, 15}, {5, 10}, {25, 55} }
        );
        nums.back().SetOutput({ false, true, true, true, false, true, true });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().SetInput(
            { "MyCalendar", "book", "book", "book","book", "book", "book" },
            { {}, {10, 15}, {20, 25}, {30, 35}, {5, 60}, {5, 10}, {50, 60} }
        );
        nums.back().SetOutput({ false, true, true, true, true, true, true });
    }

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
            << result << " vs e:" << nums[i].mExpected << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected);
    }
}

#ifdef __DEBUG_BIG_TEST

vector<string> getTestString(int size) {
    vector<string> result(size, "book");
    result[0] = "MyCalendarTwo";
    return result;
}

template <typename T> vector<T> load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<TestCaseStruct> nums;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    {
        nums.push_back(TestCaseStruct());
        nums.back().SetOutput(load_data_v1<bool>(t1e1));
        int size = nums.back().mExpected.size();
        nums.back().SetInput(getTestString(size), load_data_v2(t1i2));
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    assert(result == nums[0].mExpected);
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


