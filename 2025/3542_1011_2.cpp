
/*

3542. Minimum Operations to Convert All Elements to Zero
Medium
Topics
premium lock iconCompanies
Hint

You are given an array nums of size n, consisting of non-negative integers. Your task is to apply some (possibly zero) operations on the array so that all elements become 0.

In one operation, you can select a

[i, j] (where 0 <= i <= j < n) and set all occurrences of the minimum non-negative integer in that subarray to 0.

Return the minimum number of operations required to make all elements in the array 0.



Example 1:

Input: nums = [0,2]

Output: 1

Explanation:

    Select the subarray [1,1] (which is [2]), where the minimum non-negative integer is 2. Setting all occurrences of 2 to 0 results in [0,0].
    Thus, the minimum number of operations required is 1.

Example 2:

Input: nums = [3,1,2,1]

Output: 3

Explanation:

    Select subarray [1,3] (which is [1,2,1]), where the minimum non-negative integer is 1. Setting all occurrences of 1 to 0 results in [3,0,2,0].
    Select subarray [2,2] (which is [2]), where the minimum non-negative integer is 2. Setting all occurrences of 2 to 0 results in [3,0,0,0].
    Select subarray [0,0] (which is [3]), where the minimum non-negative integer is 3. Setting all occurrences of 3 to 0 results in [0,0,0,0].
    Thus, the minimum number of operations required is 3.

Example 3:

Input: nums = [1,2,1,2,1,2]

Output: 4

Explanation:

    Select subarray [0,5] (which is [1,2,1,2,1,2]), where the minimum non-negative integer is 1. Setting all occurrences of 1 to 0 results in [0,2,0,2,0,2].
    Select subarray [1,1] (which is [2]), where the minimum non-negative integer is 2. Setting all occurrences of 2 to 0 results in [0,0,0,2,0,2].
    Select subarray [3,3] (which is [2]), where the minimum non-negative integer is 2. Setting all occurrences of 2 to 0 results in [0,0,0,0,0,2].
    Select subarray [5,5] (which is [2]), where the minimum non-negative integer is 2. Setting all occurrences of 2 to 0 results in [0,0,0,0,0,0].
    Thus, the minimum number of operations required is 4.



Constraints:

    1 <= n == nums.length <= 105
    0 <= nums[i] <= 105

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
#include <unordered_map>
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

class Solution {
public:

    Solution() {}


    int minOperations(vector<int>& nums) {

        int n = nums.size();
        int ret_ops = 0;

        map<int, set<int>> numIndices;
        map<int, int> validRanges;

        for (int i = 0; i < n; i++) {
            auto curNum = nums[i];
            numIndices[curNum].insert(i);
        }

        if (numIndices.find(0) == numIndices.end()) {
            validRanges[0] = n - 1;
        }
        else {
            auto& zeroIndices = numIndices[0];
            int prevIdx = -1;
            for (auto idx : zeroIndices) {
                if (prevIdx + 1 <= idx - 1) {
                    validRanges[prevIdx + 1] = idx - 1;
                }
                prevIdx = idx;
            }
            if (prevIdx + 1 <= n - 1) {
                validRanges[prevIdx + 1] = n - 1;
            }
        }

        numIndices.erase(0); // zeroes are already done

        LOG("Unique nums to process: " << numIndices.size());
        LOG("Valid ranges to process: " << validRanges.size());

        while (!numIndices.empty()) {

            auto it = numIndices.begin();
            int num = it->first;
            auto& indices = it->second;
            LOG("valid ranges cnt:" << validRanges.size() << " values: " << validRanges);
            LOG("\tProcessing number: " << num << " cnt: " << indices.size()
                << " range:(" << *indices.begin() << "," << *prev(indices.end()) << ")");

            int ranges_to_update = processBreaks(indices, validRanges);
            ret_ops += ranges_to_update;
            numIndices.erase(it);

            LOG("\tprocessed number " << num << " with operations:" << ranges_to_update << " total so far:" << ret_ops);
        }

        LOG("Total operations: " << ret_ops);
        return ret_ops;
    }

    int processBreaks(set<int>& breaksAt, map<int, int>& validRanges) {

        int retVal = 0;

        while (!breaksAt.empty()) {

            int curIdx = *breaksAt.begin();
            auto rangeIt = validRanges.lower_bound(curIdx);

            if (rangeIt == validRanges.begin()) {
            }
            else if (rangeIt == validRanges.end() || rangeIt->first > curIdx)
                rangeIt--;

            // remove current range as it is broken, add valid parts back
            int rangeStart = rangeIt->first;
            int rangeEnd = rangeIt->second;
            validRanges.erase(rangeIt);
            AddNewValidRanges(breaksAt, rangeStart, rangeEnd, validRanges);
            retVal++;
        }

        return retVal;
    }

    void AddNewValidRanges(std::set<int>& indices, int& rangeStart, int rangeEnd, std::map<int, int>& validRanges)
    {
        while (*(indices.begin()) <= rangeEnd && !indices.empty()) {
            int curIdx = *(indices.begin());
            indices.erase(indices.begin());

            if (rangeStart <= curIdx - 1) {
                validRanges[rangeStart] = curIdx - 1;
            }
            rangeStart = curIdx + 1;
        }
        if (rangeStart <= rangeEnd)
            validRanges[rangeStart] = rangeEnd;

    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return minOperations(nums.mData1);
    }
};


/****************************************************************************/
/****************************************************************************/

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
        nums.back().setInput({ 0,1 });
        nums.back().setOutput(1);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 3,1,2,1 });
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1, 2, 1, 2, 1, 2 });
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
        nums.back().setInput(load_data_v1<int>(t1i1));
        nums.back().setOutput(61389);
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


