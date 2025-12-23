
/*
3409. Longest Subsequence With Decreasing Adjacent Difference
Medium
Topics
premium lock iconCompanies
Hint

You are given an array of integers nums.

Your task is to find the length of the longest

seq of nums, such that the absolute differences between consecutive elements form a non-increasing sequence of integers. In other words, for a subsequence seq0, seq1, seq2, ..., seqm of nums, |seq1 - seq0| >= |seq2 - seq1| >= ... >= |seqm - seqm - 1|.

Return the length of such a subsequence.



Example 1:

Input: nums = [16,6,3]

Output: 3

Explanation:

The longest subsequence is [16, 6, 3] with the absolute adjacent differences [10, 3].

Example 2:

Input: nums = [6,5,3,4,2,1]

Output: 4

Explanation:

The longest subsequence is [6, 4, 2, 1] with the absolute adjacent differences [2, 2, 1].

Example 3:

Input: nums = [10,20,10,19,10,20]

Output: 5

Explanation:

The longest subsequence is [10, 20, 10, 19, 10] with the absolute adjacent differences [10, 10, 9, 9].



Constraints:

    2 <= nums.length <= 104
    1 <= nums[i] <= 300

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

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1) {
        this->mData1 = input1;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};


struct Trace {
    IDX mPrevVal;
    IDX mCurVal;
    int trSize;
    Trace() : mPrevVal(-1), mCurVal(-1), trSize(0) {}
    int getDiff() const {
        return abs(mPrevVal - mCurVal);
    }
    void Add(int newIdx) {
        mPrevVal = mCurVal;
        mCurVal = newIdx;
        trSize++;
    }
    void reset() {
        mPrevVal = -1;
        mCurVal = -1;
        trSize = 0;
    }
};
ostream& operator<<(ostream& out, const Trace& trace);

/*---------------------------------------------------------------------------*/

class Solution {
public:

    Solution() {}

    void updateMinMax(pair<int, int>& minMax, int value) {
        if (value < minMax.first) {
            minMax.first = value;
        }
        if (value > minMax.second) {
            minMax.second = value;
        }
    }

    int longestSubsequence(vector<int>& nums) {
        int n = nums.size();
        if (n <= 2) return n;

        vector<Trace> traces(n);
        int maxLength = 2;

        // first trace
        traces[0].Add(nums[0]);
        traces[1] = traces[0];
        traces[1].Add(nums[1]);

        pair<int, int> minMax = { nums[0], nums[0] };
        updateMinMax(minMax, nums[1]);

        for (int idx = 2; idx < n; idx++) {
            auto curNum = nums[idx];
            LOG("@" << idx << "=" << curNum << "\tTraces:" << traces);

            for (int i = 1; i < idx; i++) {

                if (traces[i].trSize + 1 < traces[idx].trSize) continue;

                int curDiff = traces[i].getDiff();
                int newDiff = abs(curNum - traces[i].mCurVal);

                if (newDiff > curDiff) continue;

                if (traces[idx].trSize < 2) {
                    traces[idx] = traces[i];
                    traces[idx].Add(curNum);
                    LOG("\tAdding trace " << traces[idx]);
                    continue;
                }

                int pastDiff = traces[idx].getDiff();
                if (traces[i].trSize + 1 == traces[idx].trSize && pastDiff >= newDiff) {
                    continue;
                }

                traces[idx] = traces[i];
                traces[idx].Add(curNum);
                LOG("\tUpdating trace using " << traces[i] << " to " << traces[idx]);
            }

            if (traces[idx].trSize <= 2) {
                auto prevnum = minMax.first;
                auto diff = abs(curNum - minMax.first);
                if (diff < abs(curNum - minMax.second)) {
                    prevnum = minMax.second;
                    diff = abs(curNum - minMax.second);
                }

                if (traces[idx].trSize < 2 || diff < traces[idx].getDiff()) {
                    traces[idx].reset();
                    traces[idx].Add(prevnum);
                    traces[idx].Add(curNum);
                    LOG("\tAdding new trace(2) " << traces[idx]);
                }
            }
            updateMinMax(minMax, curNum);
            maxLength = max(maxLength, traces[idx].trSize);
        }

        LOG("Final Traces:" << traces);
        LOG("Max subsequence length: " << maxLength);
        return maxLength;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return longestSubsequence(nums.mData1);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const Trace& trace) {
    if (trace.mPrevVal == -1 || trace.mCurVal == -1) {
        out << "{ - }";
        return out;
    }
    out << "{" << trace.mPrevVal;
    out << "," << trace.mCurVal;
    out << " =:" << trace.trSize;
    out << " }";
    return out;
}

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
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
        nums.back().setInput({ 1 });
        nums.back().setOutput(1);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1, 2 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 16,6,3 });
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 6,5,3,4,2,1 });
        nums.back().setOutput(4);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 10,20,10,19,10,20 });
        nums.back().setOutput(5);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 10,20,50,20,50,20,10 });
        nums.back().setOutput(6);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,6,10,2,10,9 });
        nums.back().setOutput(5);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 9,2,10,8,2,6 });
        nums.back().setOutput(5);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 53, 70, 60, 72, 73 });
        nums.back().setOutput(4);
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
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

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
        nums.back().setInput(load_data_v1<int>(t1i1), load_data_v2(t1i2));
        nums.back().setOutput(load_data_v1<int>(t1e1));
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.proxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    //assert(result == nums[0].mExpected1);
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


