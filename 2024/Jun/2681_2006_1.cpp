
/*
2681. Power of Heroes
Hard
Topics
Companies
Hint

You are given a 0-indexed integer array nums representing the strength of some heroes. The power of a group of heroes is defined as follows:

    Let i0, i1, ... ,ik be the indices of the heroes in a group. Then, the power of this group is max(nums[i0], nums[i1], ... ,nums[ik])2 * min(nums[i0], nums[i1], ... ,nums[ik]).

Return the sum of the power of all non-empty groups of heroes possible. Since the sum could be very large, return it modulo 109 + 7.



Example 1:

Input: nums = [2,1,4]
Output: 141
Explanation:
1st group: [2] has power = 22 * 2 = 8.
2nd group: [1] has power = 12 * 1 = 1.
3rd group: [4] has power = 42 * 4 = 64.
4th group: [2,1] has power = 22 * 1 = 4.
5th group: [2,4] has power = 42 * 2 = 32.
6th group: [1,4] has power = 42 * 1 = 16.
​​​​​​​7th group: [2,1,4] has power = 42​​​​​​​ * 1 = 16.
The sum of powers of all groups is 8 + 1 + 64 + 4 + 32 + 16 + 16 = 141.

Example 2:

Input: nums = [1,1,1]
Output: 7
Explanation: A total of 7 groups are possible, and the power of each group will be 1. Therefore, the sum of the powers of all groups is 7.



Constraints:

    1 <= nums.length <= 105
    1 <= nums[i] <= 109


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
#define MY_MAX_VAL INT32_MAX

const int RESULT_MOD = (1e9 + 7);
int GET_MOD(int x) { return x % RESULT_MOD; }

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    V1Int nums;

    Input() {
        id = inputID++;
    }
    Input(V1Int nums) {
        this->nums = nums;
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

    void clear() {
    }
    Data() {
        clear();
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    int square(int x) {
        return GET_MOD(x * x);
    }
    int cube(int x) {
        return GET_MOD(square(x) * x);
    }
    int sumOfPower(vector<int>& nums) {

        sort(nums.rbegin(), nums.rend());
        LOG("input: " << nums);

        int cubes = 0;
        int partial = 0;
        int squareSum = 0;

        for (auto x : nums) {

            cubes = GET_MOD(cubes + cube(x));
            partial = GET_MOD(partial + GET_MOD(squareSum * x));
            squareSum = GET_MOD(GET_MOD(squareSum * 2) + square(x));
            LOG("@" << x << " squareSum:" << squareSum << " partial:" << partial << " cubes:" << cubes);
        }

        auto result = GET_MOD(partial + cubes);
        LOG("@end: result:" << result);
        return result;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return sumOfPower(nums.nums);
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

    nums.push_back(Input({ 4,1,2 }));
    expected.push_back(141);

    nums.push_back(Input({ 1,2,4 }));
    expected.push_back(141);

    nums.push_back(Input({ 2,4,1 }));
    expected.push_back(141);

    nums.push_back(Input({ 1,1,1 }));
    expected.push_back(7);

    nums.push_back(Input({ 76, 24, 96, 82, 97 }));
    expected.push_back(13928461);

    nums.push_back(Input({ 3,4,5 }));
    expected.push_back(514);

    nums.push_back(Input({ 1,2,3,4,5 }));
    expected.push_back(1091);

    nums.push_back(Input({ 658, 489, 777, 2418, 1893, 130, 2448, 178, 1128, 2149, 1059, 1495, 1166, 608, 2006, 713, 1906, 2108, 680, 1348, 860, 1620, 146, 2447, 1895, 1083, 1465, 2351, 1359, 1187, 906, 533, 1943, 1814, 1808, 2065, 1744, 254, 1988, 1889, 1206 }));
    expected.push_back(898673274);

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


