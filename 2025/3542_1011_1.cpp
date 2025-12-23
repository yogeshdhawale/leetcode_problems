
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

        vector<bool> updated(n, false);
        map<int, IDX2D> nums2Idxs;

        {
            unordered_map<int, IDX2D> _nums2Idxs_uo;
            for (int i = 0; i < n; i++) {

                if (nums[i] == 0) {
                    updated[i] = true;
                    continue;
                }

                if (_nums2Idxs_uo.find(nums[i]) == _nums2Idxs_uo.end()) {
                    _nums2Idxs_uo[nums[i]] = { i, i };
                }
                else {
                    _nums2Idxs_uo[nums[i]].second = i;
                }
            }

            for (const auto& [num, idxs] : _nums2Idxs_uo) {
                nums2Idxs[num] = idxs;
            }
        }

        LOG("Nums to process: " << nums2Idxs.size());

        while (!nums2Idxs.empty()) {

            auto it = nums2Idxs.begin();
            int ranges_to_update = updateRangesInterval(nums, it->second.first, it->second.second, updated);
            ret_ops += ranges_to_update;
            nums2Idxs.erase(it);
        }

        LOG("Total operations: " << ret_ops);
        return ret_ops;
    }

    int updateRangesInterval(const vector<int>& nums, int start, int end, vector<bool>& updated) {

        int retVal = 0;

        int curNum = nums[start];

        if (start == end) {
            updated[start] = true;
            LOG("Only number " << curNum << " in range [" << start << "," << end << "]");
            return 1;
        }

        int cnt = 0;
        for (int i = start; i <= end; i++) {

            if (!updated[i]) {
                if (nums[i] == curNum) {
                    updated[i] = true;
                    cnt++;
                }
                continue;
            }

            if (cnt > 0) {
                retVal++;
                cnt = 0;
            }
        }

        if (cnt > 0)
            retVal++;

        LOG("Updating number " << curNum << " in range [" << start << "," << end << "] requires operations:" << retVal);
        return retVal;
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


