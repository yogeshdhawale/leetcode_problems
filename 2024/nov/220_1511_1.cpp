
/*
220. Contains Duplicate III
Hard
Topics
Companies
Hint

You are given an integer array nums and two integers indexDiff and valueDiff.

Find a pair of indices (i, j) such that:

    i != j,
    abs(i - j) <= indexDiff.
    abs(nums[i] - nums[j]) <= valueDiff, and

Return true if such pair exists or false otherwise.



Example 1:

Input: nums = [1,2,3,1], indexDiff = 3, valueDiff = 0
Output: true
Explanation: We can choose (i, j) = (0, 3).
We satisfy the three conditions:
i != j --> 0 != 3
abs(i - j) <= indexDiff --> abs(0 - 3) <= 3
abs(nums[i] - nums[j]) <= valueDiff --> abs(1 - 1) <= 0

Example 2:

Input: nums = [1,5,9,1,5,9], indexDiff = 2, valueDiff = 3
Output: false
Explanation: After trying all the possible pairs (i, j), we cannot satisfy the three conditions, so we return false.

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
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 1);

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

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/****************************************************************************/
typedef V1Int INPUT_TYPE;
typedef bool OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    int mData2;
    int mData3;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1, int input2, int input3) {
        this->mData1 = input1;
        this->mData2 = input2;
        this->mData3 = input3;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/****************************************************************************/

struct Data {
    VALUEI value;
    set<IDX> idxs;
    Data(VALUEI v) : value(v) {
        idxs.clear();
    }
    void addIdx(IDX i) {
        idxs.insert(i);
    }
    void removeIdx(IDX i) {
        idxs.erase(i);
    }
    int getSize() {
        return idxs.size();
    }
    bool checkIdx(int indexDiff) {
        if (idxs.size() < 2) return false;

        auto idxnew = *(idxs.rbegin());
        auto prevIdx = *(next(idxs.rbegin()));

        if (idxnew - prevIdx <= indexDiff) {
            LOG("Index condition satisfied for value=" << value);
            return true;
        }
        return false;
    }
};

class Solution {
public:

    map<int, Data*> options;
    void clear() {
        options.clear();
    }
    void log() {
        for (auto [k, v] : options) {
            LOG("\tkey=" << k << " positions=" << v->idxs);
        }
    }
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {

        clear();
        IDX i = 0;
        int initialLoop = min(indexDiff + 1, (int)nums.size());
        for (; i < initialLoop; i++) {
            VALUEI curVal = nums[i];
            if (options.find(curVal) == options.end()) options.insert(make_pair(curVal, new Data(curVal)));
            options[curVal]->addIdx(i);

            if (options[curVal]->checkIdx(indexDiff)) return true;
            if (checkState(curVal, valueDiff)) return true;
        }

        IDX tail = 0;
        for (;i < nums.size();i++, tail++) {
            LOG("@" << i << "=" << nums[i] << " @tail(" << tail << ")=" << nums[tail]);
            //log();

            VALUEI curVal = nums[i];
            if (options.find(curVal) == options.end()) options.insert(make_pair(curVal, new Data(curVal)));
            options[curVal]->addIdx(i);

            VALUEI pastVal = nums[tail];
            options[pastVal]->removeIdx(tail);
            if (options[pastVal]->getSize() == 0) options.erase(pastVal);

            if (options[curVal]->checkIdx(indexDiff)) return true;
            if (checkState(curVal, valueDiff)) return true;
        }
        return false;
    }



    bool checkState(VALUEI curVal, int valueDiff) {
        auto itr = options.lower_bound(curVal);

        if (itr != options.begin()) {
            auto pr = prev(itr);
            if (abs(curVal - pr->first) <= valueDiff) {
                LOG("Value condition satisfied for " << pr->first << " and " << curVal);
                return true;
            }
        }

        auto ne = next(itr);
        if (ne != options.end())
            if (abs(ne->first - curVal) <= valueDiff) {
                LOG("Value condition satisfied for " << curVal << " and " << ne->first);
                return true;
            }
        return false;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return containsNearbyAlmostDuplicate(nums.mData1, nums.mData2, nums.mData3);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1 << " indexDiff:" << ipObj.mData2 << " valueDiff:" << ipObj.mData3 << " }";
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
        nums.back().setInput({ 1,2,3,1 }, 3, 0);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,5,9,1,5,9 }, 2, 3);
        nums.back().setOutput(false);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ -3,3 }, 2, 4);
        nums.back().setOutput(false);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ -3,3,-6 }, 2, 3);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 10,15,18,24 }, 3, 3);
        nums.back().setOutput(true);
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

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] =>\t"
            << result << " vs e:" << nums[i].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected1);
    }
}

/****************************************************************************/

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
        auto d2 = load_data_v1<int>(t1i2);
        nums.back().setInput(load_data_v1<int>(t1i1), d2[0], d2[1]);
        nums.back().setOutput(true);
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.proxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    assert(result == nums[0].mExpected1);
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


