
/*
2790. Maximum Number of Groups With Increasing Length
Hard
Topics
Companies
Hint

You are given a 0-indexed array usageLimits of length n.

Your task is to create groups using numbers from 0 to n - 1, ensuring that each number, i, is used no more than usageLimits[i] times in total across all groups. You must also satisfy the following conditions:

    Each group must consist of distinct numbers, meaning that no duplicate numbers are allowed within a single group.
    Each group (except the first one) must have a length strictly greater than the previous group.

Return an integer denoting the maximum number of groups you can create while satisfying these conditions.



Example 1:

Input: usageLimits = [1,2,5]
Output: 3
Explanation: In this example, we can use 0 at most once, 1 at most twice, and 2 at most five times.
One way of creating the maximum number of groups while satisfying the conditions is:
Group 1 contains the number [2].
Group 2 contains the numbers [1,2].
Group 3 contains the numbers [0,1,2].
It can be shown that the maximum number of groups is 3.
So, the output is 3.

Example 2:

Input: usageLimits = [2,1,2]
Output: 2
Explanation: In this example, we can use 0 at most twice, 1 at most once, and 2 at most twice.
One way of creating the maximum number of groups while satisfying the conditions is:
Group 1 contains the number [0].
Group 2 contains the numbers [1,2].
It can be shown that the maximum number of groups is 2.
So, the output is 2.

Example 3:

Input: usageLimits = [1,1]
Output: 1
Explanation: In this example, we can use both 0 and 1 at most once.
One way of creating the maximum number of groups while satisfying the conditions is:
Group 1 contains the number [0].
It can be shown that the maximum number of groups is 1.
So, the output is 1.



Constraints:

    1 <= usageLimits.length <= 105
    1 <= usageLimits[i] <= 109


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


typedef int LIMIT;
typedef int COUNT;

struct Data {


};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    int maxIncreasingGroups(vector<int>& usageLimits) {

        map<LIMIT, COUNT, greater<COUNT>> levels;
        for (size_t i = 0; i < usageLimits.size(); i++) {
            levels[usageLimits[i]]++;
        }
        COUNT maxSize = 0;
        for (;true;maxSize++) {

            LOG("maxSize: " << maxSize);

            COUNT newSize = maxSize + 1;

            map<LIMIT, COUNT> changes;
            while (newSize > 0) {
                if (levels.empty()) return maxSize;
                auto it = levels.begin();

                LIMIT curlvl = it->first;
                COUNT cnt = it->second;

                LOG("processing level: " << curlvl << " with size: " << cnt << " and new size: " << newSize);

                if (cnt >= newSize) {

                    levels[curlvl] -= newSize;

                    if (levels[curlvl] == 0)
                        levels.erase(curlvl);

                    if (curlvl != 1)
                        levels[curlvl - 1] += newSize;

                    break;
                }

                levels.erase(curlvl);
                newSize -= cnt;
                changes[curlvl - 1] += cnt;
            }

            for (auto& n : changes) {
                if (n.first == 0) continue;
                levels[n.first] += n.second;
            }
        }
        return maxSize;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxIncreasingGroups(nums.nums);
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


void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 1,2,5 }));
    expected.push_back(3);

    nums.push_back(Input({ 2,1,2 }));
    expected.push_back(2);

    nums.push_back(Input({ 1,1 }));
    expected.push_back(1);

    nums.push_back(Input({ 5,6,5 }));
    expected.push_back(3);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "*****Time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
            << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
            << endl;
        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
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
    cout << TEXT_LOG_PREFIX << "Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;

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


