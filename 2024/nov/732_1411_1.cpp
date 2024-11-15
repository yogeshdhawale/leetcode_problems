
/*
732. My Calendar III
Hard
Topics
Companies
Hint

A k-booking happens when k events have some non-empty intersection (i.e., there is some time that is common to all k events.)

You are given some events [startTime, endTime), after each given event, return an integer k representing the maximum k-booking between all the previous events.

Implement the MyCalendarThree class:

    MyCalendarThree() Initializes the object.
    int book(int startTime, int endTime) Returns an integer k representing the largest integer such that there exists a k-booking in the calendar.



Example 1:

Input
["MyCalendarThree", "book", "book", "book", "book", "book", "book"]
[[},{10, 20},{50, 60},{10, 40},{5, 15},{5, 10},{25, 55]]
Output
[null, 1, 1, 2, 3, 3, 3]

Explanation
MyCalendarThree myCalendarThree = new MyCalendarThree();
myCalendarThree.book(10, 20); // return 1
myCalendarThree.book(50, 60); // return 1
myCalendarThree.book(10, 40); // return 2
myCalendarThree.book(5, 15); // return 3
myCalendarThree.book(5, 10); // return 3
myCalendarThree.book(25, 55); // return 3



Constraints:

    0 <= startTime < endTime <= 109
    At most 400 calls will be made to book.

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
typedef vector<int> OUTPUT_TYPE;

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


class MyCalendarThree {

    vector<map<int, int>> mOverLapMap;
public:
    MyCalendarThree() {
        mOverLapMap.clear();
        mOverLapMap.reserve(500);
    }

    void log() {
        for (size_t i = 0; i < mOverLapMap.size(); i++) {
            LOG("\tDepth-" << i << " : " << mOverLapMap[i]);
        }
    }
    int book(int startTime, int endTime) {
        process(startTime, endTime, 0);
        log();
        return mOverLapMap.size();
    }
    void process(int startTime, int endTime, int depth) {
        if (startTime == endTime) return;
        LOG("\t\td=" << depth << " provided window:" << startTime << "," << endTime);
        if (depth >= (int)mOverLapMap.size()) {
            mOverLapMap.push_back(map<int, int>());
        }
        auto& curMap = mOverLapMap[depth];

        if (curMap.size() == 0) {
            curMap[startTime] = endTime;
            return;
        }

        auto itr = curMap.begin();
        if (endTime < itr->first) {
            curMap[startTime] = endTime;
            return;
        }
        itr = prev(curMap.end());
        if (startTime > itr->second) {
            curMap[startTime] = endTime;
            return;
        }

        vector<pair<int, int>> nextList;
        nextList.clear();
        auto newstart = startTime;
        auto newend = endTime;

        itr = curMap.lower_bound(startTime);
        if (itr == curMap.end()) {
            --itr;
            newstart = itr->first;
            newend = max(endTime, itr->second);
        }

        if (itr != curMap.begin())
        {
            auto prev_itr = prev(itr);
            if (startTime == prev_itr->second) {
                if (endTime == itr->first) {
                    prev_itr->second = itr->second;
                    curMap.erase(itr);
                    return;
                }
                if (endTime < itr->first) {
                    prev_itr->second = endTime;
                    return;
                }
            }
            if (startTime < prev_itr->second) {
                nextList.push_back({ startTime, min(endTime, prev_itr->second) });
                newstart = prev_itr->first;
                newend = max(endTime, prev_itr->second);
                curMap.erase(prev_itr);
            }
        }
        else if (itr == curMap.begin()) {
            if (startTime < itr->first) {
                nextList.push_back({ itr->first, min(endTime, itr->second) });
                curMap[startTime] = itr->second;
                itr = curMap.erase(itr);
            }
        }

        bool updateAtEnd = false;
        for (;itr != curMap.end();) {
            if (endTime <= itr->first) {
                curMap[newstart] = newend;
                updateAtEnd = false;
                break;
            }
            if (endTime <= itr->second) {
                nextList.push_back({ itr->first, endTime });
                newend = itr->second;
                itr = curMap.erase(itr);
                curMap[newstart] = newend;
                updateAtEnd = false;
                break;
            }
            nextList.push_back({ itr->first, itr->second });
            itr = curMap.erase(itr);
            updateAtEnd = true;
        }

        if (updateAtEnd) curMap[newstart] = newend;

        merge(curMap, newstart);

        LOG("\t\tnext list : " << nextList);
        for (auto item : nextList) {
            process(max(startTime, item.first), min(endTime, item.second), depth + 1);
        }
    }

    void merge(std::map<int, int>& curMap, int& newstart)
    {

        auto itr = curMap.find(newstart);
        auto pr = prev(itr);
        auto ne = next(itr);
        if (pr != curMap.end() && ne != curMap.end()) {
            if (pr->second == itr->first) {
                pr->second = itr->second;
                curMap.erase(itr);
                if (pr->second == ne->first) {
                    pr->second = ne->second;
                    curMap.erase(ne);
                }
            }
        }
        else if (pr != curMap.end()) {
            if (pr->second == itr->first) {
                pr->second = itr->second;
                curMap.erase(itr);
            }
        }
        else if (itr->second == ne->first) {
            itr->second = ne->second;
            curMap.erase(ne);
        }

    }

};

class Solution {
public:

    OUTPUT_TYPE ProxyFunction(TestCaseStruct& nums) {
        OUTPUT_TYPE result;
        MyCalendarThree* obj = new MyCalendarThree();
        for (int i = 0; i < (int)nums.mData2.size(); ++i) {
            if (nums.mData1[i] != "book") {
                delete obj;
                obj = new MyCalendarThree();
                result.push_back(0);
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

vector<string> getTestString(int size) {
    vector<string> result(size, "book");
    result[0] = "MyCalendarThree";
    return result;
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
        nums.back().SetOutput({ 0, 1,1,2,3,3,3 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().SetInput(
            { "MyCalendar", "book", "book", "book","book", "book", "book" },
            { {}, {10, 15}, {20, 25}, {30, 35}, {5, 60}, {5, 10}, {50, 60} }
        );
        nums.back().SetOutput({ 0, 1,1,1,2,2,2 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().SetInput(
            { "MyCalendar", "book", "book", "book","book", "book", "book" },
            { {}, {10, 15}, {20, 25}, {30, 35}, {5, 60}, {5, 15}, {50, 60} }
        );
        nums.back().SetOutput({ 0, 1,1,1,2,3,3 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().SetInput(
            getTestString(15),
            { {},{26, 35},{26, 32},{25, 32},{18, 26},{40, 45},{19, 26},{48, 50},{1, 6},{46, 50},{11, 18} }
        );
        nums.back().SetOutput({ 0, 1,2,3,3,3,3,3,3,3,3 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().SetInput(
            getTestString(15),
            { {},{24,40},{43,50},{27,43},{5,21},{30,40},{14,29},{3,19},{3,14},{25,39},{6,19 } }
        );
        nums.back().SetOutput({ 0, 1,1,2,2,3,3,3,3,4,4 });
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


