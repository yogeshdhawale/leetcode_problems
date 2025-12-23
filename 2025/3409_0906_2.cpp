
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

/*---------------------------------------------------------------------------*/


struct trace {
    int endNumber[2];
    int diff;
    int length;
    trace(int num = -1, int d = 0) : diff(0), length(d) {
        endNumber[0] = num;
        endNumber[1] = num; // no second option
    }
    void set(int curNum, int diff, int len) {
        this->endNumber[0] = curNum;
        this->endNumber[1] = curNum;
        this->diff = diff;
        this->length = len;
    }

    void addOption(int newNum) {
        endNumber[0] = min(endNumber[0], newNum);
        endNumber[1] = max(endNumber[1], newNum);
    }
    int getDiffWith(int num) const {
        return min(abs(endNumber[0] - num), abs(endNumber[1] - num));
    }
};
ostream& operator<<(ostream& out, const trace& t);


class Solution {
public:


    pair<int, int> mMMPair;
    Solution() {}


    void updateMinMaxPair(int num) {
        mMMPair.first = min(mMMPair.first, num);
        mMMPair.second = max(mMMPair.second, num);
    }
    int longestSubsequence(vector<int>& nums) {

        int n = nums.size();
        if (n <= 2) return n;

        vector<trace> dp(n + 1, trace(-1, 0));

        dp[2].set(nums[1], abs(nums[0] - nums[1]), 2);

        mMMPair.first = nums[0];
        mMMPair.second = nums[0];
        updateMinMaxPair(nums[1]);

        int maxLength = 2;

        for (int currIdx = 2;currIdx < n;currIdx++) {

            auto curNum = nums[currIdx];
            LOG("\t@:" << currIdx << "=" << curNum << " with:" << dp);

            for (int curLen = maxLength; curLen > 1; curLen--) {

                int prevDiff = dp[curLen].diff;
                int newDiff = dp[curLen].getDiffWith(curNum);

                if (dp[curLen + 1].endNumber[0] == -1) {
                    if (newDiff <= prevDiff) {
                        dp[curLen + 1].set(curNum, newDiff, curLen + 1);
                        maxLength = max(maxLength, curLen + 1);
                        LOG("New dp[" << curLen + 1 << "] to " << dp[curLen + 1]);
                    }
                    continue;
                }
                else  if (dp[curLen + 1].diff < newDiff) {
                    dp[curLen + 1].set(curNum, newDiff, curLen + 1);
                    LOG("update dp[" << curLen + 1 << "] to " << dp[curLen + 1]);
                }
                else if (dp[curLen + 1].diff == newDiff) {
                    dp[curLen + 1].addOption(curNum);
                }
            }
            {
                auto diffAt2 = dp[2].diff;
                auto diff1 = abs(curNum - mMMPair.first);
                auto diff2 = abs(curNum - mMMPair.second);
                auto maxDiff = max(diff1, diff2);

                if (maxDiff > diffAt2) {
                    dp[2].set(curNum, maxDiff, 2);
                    LOG("update dp[2] to " << dp[2]);
                }
                else if (maxDiff == diffAt2) {
                    dp[2].addOption(curNum);
                }
                updateMinMaxPair(curNum);
            }
        }

        LOG("At end:" << dp);
        LOG("Max subsequence length: " << maxLength);
        return maxLength;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return longestSubsequence(nums.mData1);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const trace& t) {
    if (t.endNumber[0] == -1) {
        out << "{ - }";
        return out;
    }

    out << "{" << t.length;
    out << ",->" << t.endNumber[0];
    if (t.endNumber[0] != t.endNumber[1]) out << "&" << t.endNumber[1];
    out << ",!" << t.diff;
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
        // remove 10 
        // 9 -> 2(7) -> 8(6) -> 2(4) -> 6(4)
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 53, 70, 60, 72, 73 });
        nums.back().setOutput(4);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 9,9,8,4,8,1 });
        nums.back().setOutput(3);
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
        nums.back().setInput(load_data_v1<int>(t1i1));
        nums.back().setOutput(0);
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


