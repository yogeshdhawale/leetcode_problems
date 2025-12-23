
/*
3346. Maximum Frequency of an Element After Performing Operations I
Medium
Topics
Companies
Hint

You are given an integer array nums and two integers k and numOperations.

You must perform an operation numOperations times on nums, where in each operation you:

    Select an index i that was not selected in any previous operations.
    Add an integer in the range [-k, k] to nums[i].

Return the maximum possible

of any element in nums after performing the operations.



Example 1:

Input: nums = [1,4,5], k = 1, numOperations = 2

Output: 2

Explanation:

We can achieve a maximum frequency of two by:

    Adding 0 to nums[1]. nums becomes [1, 4, 5].
    Adding -1 to nums[2]. nums becomes [1, 4, 4].

Example 2:

Input: nums = [5,11,20,20], k = 5, numOperations = 1

Output: 2

Explanation:

We can achieve a maximum frequency of two by:

    Adding 0 to nums[1].



Constraints:

    1 <= nums.length <= 105
    1 <= nums[i] <= 105
    0 <= k <= 105
    0 <= numOperations <= nums.length

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

/*---------------------------------------------------------------------------*/

class Solution {
public:

    Solution() {}

    int maxFrequency(vector<int>& nums, int k, int numOperations) {

        int n = nums.size();
        map<int, int> freqMap;

        for (int i = 0; i < n; i++) {
            freqMap[nums[i]]++;
        }

        int maxCount = 0;

        auto startItr = freqMap.begin();
        auto curNum = startItr->first;

        if (startItr->second > numOperations) {
            LOG("At start, returning " << curNum << " max:" << startItr->second);
            return numOperations + 1;
        }

        auto curCount = startItr->second;
        auto pendingOps = numOperations + 1 - startItr->second;
        auto curItr = next(startItr);

        while (curItr != freqMap.end()) {

            if (pendingOps > 0) {
                if (curNum >= (curItr->first - k)) {
                    if (pendingOps >= curItr->second) {
                        curCount += curItr->second;
                        pendingOps -= curItr->second;
                    }
                    else {
                        LOG("At " << curNum << "Not enough operations left to process whole group");
                        curCount += pendingOps;
                        pendingOps = 0;
                    }
                    curItr++;
                    continue;
                }

                if ((curNum + k) < (curItr->first - k)) {
                    LOG("At " << curNum << " can't extend");
                }
                else {
                    auto extendedWItr = curItr;
                    pendingOps--;
                    while (extendedWItr != freqMap.end() && (curNum + k) >= (extendedWItr->first - k)) {
                        if (pendingOps >= extendedWItr->second) {
                            curCount += extendedWItr->second;
                            pendingOps -= extendedWItr->second;
                        }
                        else {
                            LOG("At " << curNum << " not enough operations left with extendedWItr");
                            curCount += pendingOps;
                            pendingOps = 0;
                            break;
                        }
                        extendedWItr++;
                    }
                }
            }

            // exhausted operations for this set
            if (curCount > maxCount) {
                maxCount = curCount;
                LOG("At " << curNum << " after processing " << curItr->first << " resMax:" << maxCount);
                if (maxCount > numOperations) {
                    LOG("At " << curNum << " max:" << maxCount);
                    return numOperations + 1;
                }
            }
            // start with next set
            startItr++;
            curNum = startItr->first;
            pendingOps = numOperations + 1 - startItr->second;
            curCount = startItr->second;
            if (startItr == curItr) {
                curItr++;
            }
            else {
                for (auto tmp = next(startItr);tmp != curItr; tmp++) {
                    pendingOps -= tmp->second;
                }
                pendingOps -= curItr->second;
            }
        }
        if (curCount > maxCount) {
            maxCount = curCount;
            LOG("At end " << curNum << " max:" << maxCount);
            if (maxCount > numOperations) {
                LOG("At end returning max");
                return numOperations + 1;
            }
        }

        return maxCount;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return maxFrequency(nums.mData1, nums.mData2, nums.mData3);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " k:" << ipObj.mData2;
    out << " numOperations:" << ipObj.mData3;
    out << " e:" << ipObj.mExpected1;
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
        nums.back().setInput({ 1,4,5 }, 1, 2);
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,11,20,20 }, 5, 1);
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 3,3,3, 5,5,7,7,7,7 ,9,9 }, 1, 3);
        nums.back().setOutput(4);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 3,3,3,5,5,7,7,9,9 }, 1, 3);
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 88,53 }, 27, 2);
        nums.back().setOutput(2);
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


