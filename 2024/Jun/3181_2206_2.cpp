
/*
3181. Maximum Total Reward Using Operations II
Hard
Topics
Companies
Hint

You are given an integer array rewardValues of length n, representing the values of rewards.

Initially, your total reward x is 0, and all indices are unmarked. You are allowed to perform the following operation any number of times:

    Choose an unmarked index i from the range [0, n - 1].
    If rewardValues[i] is greater than your current total reward x, then add rewardValues[i] to x (i.e., x = x + rewardValues[i]), and mark the index i.

Return an integer denoting the maximum total reward you can collect by performing the operations optimally.



Example 1:

Input: rewardValues = [1,1,3,3]

Output: 4

Explanation:

During the operations, we can choose to mark the indices 0 and 2 in order, and the total reward will be 4, which is the maximum.

Example 2:

Input: rewardValues = [1,6,4,3,2]

Output: 11

Explanation:

Mark the indices 0, 2, and 1 in order. The total reward will then be 11, which is the maximum.



Constraints:

    1 <= rewardValues.length <= 5 * 104
    1 <= rewardValues[i] <= 5 * 104


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

#include <bitset>
#include <ranges>

#ifndef __DEBUG_LOG
#define LOG(x)
#define VLOG(x)
#endif

#define INVALID -1
#define MY_MAX_VAL INT32_MAX

#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

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

    int maxTotalReward(vector<int>& rewardValues) {

        map<int, bool> entries;
        for (auto r : rewardValues) {
            entries[r] = true;
        }

        vector<int> rewardOptions;
        for (auto r : entries) {
            rewardOptions.push_back(r.first);
        }

        bitset <100000> flagged{ 1 };

        for (auto r : rewardOptions) {
            // remove flags greater than r
            auto invalidNums = flagged.size() - r;
            auto validOptions = flagged << invalidNums >> invalidNums;
            // add r = shift numbers by r
            validOptions = validOptions << r;
            // keep original numbers
            flagged = flagged | validOptions;
        }

        for (int i = rewardOptions.back() * 2 - 1;i >= rewardOptions.back(); --i) {
            if (flagged.test(i)) {
                return i;
            }
        }
        LOG("@End: rewardOptions:" << rewardOptions);
        return rewardOptions.back();
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxTotalReward(nums.nums);
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

    nums.push_back(Input({ 1,1,3,3 }));
    expected.push_back(4);

    nums.push_back(Input({ 1,6,4,3,2 }));
    expected.push_back(11);

    nums.push_back(Input({ 1, 14, 13, 19 }));
    expected.push_back(34);

    nums.push_back(Input({ 5, 8, 9, 12 }));
    expected.push_back(21);

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


