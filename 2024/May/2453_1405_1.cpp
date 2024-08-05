
/*
2453. Destroy Sequential Targets
Medium
Topics
Companies
Hint

You are given a 0-indexed array nums consisting of positive integers, representing targets on a number line. You are also given an integer space.

You have a machine which can destroy targets. Seeding the machine with some nums[i] allows it to destroy all targets with values that can be represented as nums[i] + c * space, where c is any non-negative integer. You want to destroy the maximum number of targets in nums.

Return the minimum value of nums[i] you can seed the machine with to destroy the maximum number of targets.



Example 1:

Input: nums = [3,7,8,1,1,5], space = 2
Output: 1
Explanation: If we seed the machine with nums[3], then we destroy all targets equal to 1,3,5,7,9,...
In this case, we would destroy 5 total targets (all except for nums[2]).
It is impossible to destroy more than 5 targets, so we return nums[3].

Example 2:

Input: nums = [1,3,5,2,4,6], space = 2
Output: 1
Explanation: Seeding the machine with nums[0], or nums[3] destroys 3 targets.
It is not possible to destroy more than 3 targets.
Since nums[0] is the minimal integer that can destroy 3 targets, we return 1.

Example 3:

Input: nums = [6,2,5], space = 100
Output: 2
Explanation: Whatever initial seed we select, we can only destroy 1 target. The minimal seed is nums[1].



Constraints:

    1 <= nums.length <= 105
    1 <= nums[i] <= 109
    1 <= space <= 109


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

#include "log.h"

using namespace std;

/****************************************************************************/

#include <ranges>

#ifndef __DEBUG_LOG
#define LOG(x)
#define VLOG(x)
#endif

#define INVALID -1
#define RESULT_MOD 1000000007
#define MY_MAX_VAL INT32_MAX

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    V1Int nums;
    int space;

    Input() {
        id = inputID++;
    }
    Input(V1Int nums, int s) {
        this->nums = nums;
        this->space = s;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data {

    unordered_map<int, pair<int, int>> mCnt;
    int space;
    pair<int, int> result;

    void clear() {
    }
    Data(int space) {
        this->space = space;
        result = { INVALID, 0 };
        clear();
    }

    void Add(int val) {

        LOG("result: " << result << " \tmap:" << mCnt);

        int minSeed = val % space;
        if (mCnt.find(minSeed) == mCnt.end()) {
            mCnt[minSeed] = { val, 1 };
        }
        else {
            mCnt[minSeed].first = min(val, mCnt[minSeed].first);
            mCnt[minSeed].second++;
        }

        if (mCnt[minSeed].second > result.second) {
            result = mCnt[minSeed];
        }
        else if (mCnt[minSeed].second == result.second) {
            result.first = min(result.first, mCnt[minSeed].first);
        }
    }

    int getMaxResult() {
        LOG("@END result: " << result << " \tmap:" << mCnt);
        return result.first;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    int destroyTargets(vector<int>& nums, int space) {

        Data data(space);

        for (size_t i = 0; i < nums.size(); ++i) {
            LOG("@" << i << "=" << nums[i]);
            data.Add(nums[i]);
        }

        return data.getMaxResult();
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return destroyTargets(nums.nums, nums.space);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    return out;
}

/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << TEXT_LOG_PREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 3,7,8,1,1,5 }, 2));
    expected.push_back(1);

    nums.push_back(Input({ 1,3,5,2,4,6 }, 2));
    expected.push_back(1);

    nums.push_back(Input({ 6,2,5 }, 100));
    expected.push_back(2);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = __D_CASE_ID; i <= __D_CASE_ID; i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
        logtime("*****Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG

#include "data.h"

void test2(int testID)
{

    cout << TEXT_LOG_PREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input(d1));
    expected.push_back(o1);

    cout << TEXT_LOG_PREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << TEXT_LOG_PREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("*****Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG
    test2(2);
#endif

    return 0;
}


