
/*
Q1. Range Module
Hard
Topics

A Range Module is a module that tracks ranges of numbers. Design a data structure to track the ranges represented as half-open intervals and query about them.

A half-open interval [left, right) denotes all the real numbers x where left <= x < right.

Implement the RangeModule class:

    RangeModule() Initializes the object of the data structure.
    void addRange(int left, int right) Adds the half-open interval [left, right), tracking every real number in that interval. Adding an interval that partially overlaps with currently tracked numbers should add any numbers in the interval [left, right) that are not already tracked.
    boolean queryRange(int left, int right) Returns true if every real number in the interval [left, right) is currently being tracked, and false otherwise.
    void removeRange(int left, int right) Stops tracking every real number currently being tracked in the half-open interval [left, right).



Example 1:

Input
["RangeModule", "addRange", "removeRange", "queryRange", "queryRange", "queryRange"]
[[], [10, 20], [14, 16], [10, 14], [13, 15], [16, 17]]
Output
[null, null, null, true, false, true]

Explanation
RangeModule rangeModule = new RangeModule();
rangeModule.addRange(10, 20);
rangeModule.removeRange(14, 16);
rangeModule.queryRange(10, 14); // return True,(Every number in [10, 14) is being tracked)
rangeModule.queryRange(13, 15); // return False,(Numbers like 14, 14.03, 14.17 in [13, 15) are not being tracked)
rangeModule.queryRange(16, 17); // return True, (The number 16 in [16, 17) is still being tracked, despite the remove operation)



Constraints:

    1 <= left < right <= 109
    At most 104 calls will be made to addRange, queryRange, and removeRange.


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include<list>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

using namespace std;

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
#include <unordered_set>
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 1);
int gCurTestId = -1;

/****************************************************************************/
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
typedef vector<long long > V1LL;

typedef vector<string> V1Str;
typedef vector<V1Str> V2Str;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef vector<bool> OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    V1Str mData1;
    vector<V1LL> mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(V1Str input1, vector < V1LL> input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

class RangeModule {

    map <int, bool> ranges;
public:
    RangeModule() {}

    void logRanges() {
        LOG("Ranges: " << ranges.size() << " -- " << ranges);
    }

    void addRange(int left, int right) {

        if (ranges.empty()) {
            ranges[left] = true;
            ranges[right] = false;
            return;
        }

        if (right < ranges.begin()->first || left > ranges.rbegin()->first) {
            ranges[left] = true;
            ranges[right] = false;
            return;
        }

        if (right == ranges.begin()->first) {
            ranges.erase(ranges.begin());
            ranges[left] = true;
            return;
        }
        if (left == ranges.rbegin()->first) {
            ranges.erase(prev(ranges.end()));
            ranges[right] = false;
            return;
        }

        ranges[left] = true;
        ranges[right] = false;

        auto itr_left = ranges.lower_bound(left);

        auto itr_right = next(itr_left);
        while (itr_right != ranges.end() && itr_right->first < right) {
            itr_right = ranges.erase(itr_right);
        }

        if (itr_left != ranges.begin()) {
            auto previtr = prev(itr_left);
            if (previtr->second == true) {
                ranges.erase(itr_left);
            }
        }

        if (itr_right != ranges.end()) {
            auto nextitr = next(itr_right);
            if (nextitr != ranges.end() && nextitr->second == false) {
                ranges.erase(itr_right);
            }
        }
    }


    bool queryRange(int left, int right) {

        if (ranges.empty()) {
            return false;
        }

        if (left < ranges.begin()->first) return false;
        if (right > ranges.rbegin()->first) return false;

        auto itr = ranges.lower_bound(left);

        if (left == itr->first && itr->second == true) {
            auto nextitr = next(itr);
            if (right <= nextitr->first) {
                return true;
            }
        }

        itr = prev(itr);
        if (left >= itr->first && itr->second == true) {
            auto nextitr = next(itr);
            if (right <= nextitr->first) {
                return true;
            }
        }

        return false;
    }

    void removeRange(int left, int right) {

        if (ranges.empty()) {
            return;
        }

        if (right < ranges.begin()->first || left > ranges.rbegin()->first) {
            // outside valid ranges
            return;
        }


        auto itr = ranges.lower_bound(left);
        if (itr != ranges.end() && right < itr->first && itr->second == true) {
            // in gap of valid ranges
            return;
        }

        ranges[left] = false;
        ranges[right] = true;

        auto itr_left = ranges.lower_bound(left);
        auto itr_right = next(itr_left);

        while (itr_right != ranges.end() && itr_right->first < right) {
            itr_right = ranges.erase(itr_right);
        }

        auto previtr = prev(itr_left);
        if (itr_left == ranges.begin() || previtr->second == false) {
            ranges.erase(itr_left);
        }

        auto nextitr = next(itr_right);
        if (nextitr == ranges.end() || nextitr->second == true) {
            ranges.erase(itr_right);
        }
    }
};

/**
 * Your RangeModule object will be instantiated and called as such:
 * RangeModule* obj = new RangeModule();
 * obj->addRange(left,right);
 * bool param_2 = obj->queryRange(left,right);
 * obj->removeRange(left,right);
 */

class Solution {
public:
    OUTPUT_TYPE proxyFunction(TestCaseStruct& tc) {


        RangeModule* obj = new RangeModule();
        OUTPUT_TYPE result;

        for (size_t i = 0; i < tc.mData1.size(); i++) {

            string cmd = tc.mData1[i];
            LOG("\t" << cmd << " with data:" << tc.mData2[i]);

            if (cmd == "RangeModule") {
                if (obj) delete obj;
                obj = new RangeModule();
            }
            else if (cmd == "addRange") {
                obj->addRange(tc.mData2[i][0], tc.mData2[i][1]);
            }
            else if (cmd == "queryRange") {
                auto ret = obj->queryRange(tc.mData2[i][0], tc.mData2[i][1]);
                result.push_back(ret);
                LOG("\treturns:" << ret << endl);
            }
            else if (cmd == "removeRange") {
                obj->removeRange(tc.mData2[i][0], tc.mData2[i][1]);
            }
            else {
                cout << "Unknown command:" << cmd << endl;
            }

            obj->logRanges();
        }
        return result;
    }
};

/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " data2:" << ipObj.mData2;
    out << " }";
    return out;
}

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int generateInput(int seed, int size, int max) {
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
        nums.back().setInput(
            { "RangeModule", "addRange", "removeRange", "queryRange", "queryRange", "queryRange" },
            { {}, {10, 20}, {14, 16}, {10, 14}, {13, 15}, {16, 17} }
        );
        nums.back().setOutput({ 1,0,1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "RangeModule", "addRange", "queryRange", "removeRange", "removeRange", "addRange", "queryRange", "addRange", "queryRange", "removeRange" },
            { {},{5,8},{3,4},{5,6},{3,6},{1,3},{2,3},{4,8},{2,3},{4,9} }
        );
        nums.back().setOutput({ 0,1,1 });
    }


    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "RangeModule", "addRange", "addRange", "addRange", "queryRange", "queryRange", "queryRange", "removeRange", "queryRange" },
            { {},{10,180},{150,200},{250,500},{50,100},{180,300},{600,1000},{50,150},{50,100} }
        );
        nums.back().setOutput({ 1,0,0,0 });
    }


    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "RangeModule", "addRange", "addRange", "addRange", "queryRange", "queryRange", "queryRange", "removeRange", "queryRange" },
            { {},{10,180},{150,200},{250,500},{50,100},{180,300},{600,1000},{50,150},{50,100} }
        );
        nums.back().setOutput({ 1, 0, 0, 0 });
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "RangeModule", "addRange", "removeRange", "removeRange", "addRange", "removeRange", "addRange", "queryRange", "queryRange", "queryRange" },
            { {},{6,8},{7,8},{8,9},{8,9},{1,3},{1,8},{2,4},{2,9},{4,6} }
        );
        nums.back().setOutput({ 1, 1, 1 });
    }



    cout << endl << __LOGPREFIX << "test:" << testID << " sub-tests count (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        string strTestId = "test:" + to_string(testID) + "." + to_string(nums[i].mID) + " => ";
        cout << __LOGPREFIX << strTestId << nums[i] << endl;

        gCurTestId = nums[i].mID;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        Solution s;
        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << strTestId << "returns:" << result << " vs e:" << nums[i].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected1);
    }
}

#ifdef __DEBUG_BIG_TEST

template <typename T> vector<T> load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

vector<string> load_data_v1_string(string fileName);
vector<vector<string>> load_data_v2_string(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "big test" << testID << endl;

    vector<TestCaseStruct> nums;
    vector<string> testcases_str = { "test1", "test2" };

    for (auto& tc : testcases_str)
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            load_data_v1_string(TEST_DIR + tc + "-i1.txt"),
            load_data_v2_string(TEST_DIR + tc + "-i2.txt")
        );
        nums.back().setOutput(0);
    }

    cout << __LOGPREFIX << "big test for: " << nums.size() << endl;

    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << __LOGPREFIX << "big test: " << testID << "." << nums[i].mID << " ==> " << nums[i] << endl;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();
        cout << __LOGPREFIX << "big test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        //assert(result == nums[i].mExpected1);
    }

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


