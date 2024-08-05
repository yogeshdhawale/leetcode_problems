
/*
41. First Missing Positive
Hard
Topics
Companies
Hint

Given an unsorted integer array nums. Return the smallest positive integer that is not present in nums.

You must implement an algorithm that runs in O(n) time and uses O(1) auxiliary space.



Example 1:

Input: nums = [1,2,0]
Output: 3
Explanation: The numbers in the range [1,2] are all in the array.

Example 2:

Input: nums = [3,4,-1,1]
Output: 2
Explanation: 1 is in the array but 2 is missing.

Example 3:

Input: nums = [7,8,9,11,12]
Output: 1
Explanation: The smallest positive integer 1 is missing.



Constraints:

    1 <= nums.length <= 105
    -231 <= nums[i] <= 231 - 1


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

struct Data {

    map<int, int> m;
    void clear() {
    }
    Data() {
        clear();
    }

    void insertFirst(int n) {
        m[n] = n;
    }
    void insert(int n) {
        if (n < 1)
            return;

        auto itr = m.lower_bound(n);
        if (itr == m.end()) {
            if (m.rbegin()->second == n - 1)
                m.rbegin()->second = n;
            else
                m[n] = n;
            return;
        }

        itr = prev(itr);
        if (itr->second == n - 1) {
            itr->second = n;
        }
        else {
            m[n] = n;
        }
    }
    int getMissing() {

        int prev = 0;
        for (auto itr = m.begin();itr != m.end(); ++itr) {
            if (itr->first != prev + 1)
                return prev + 1;
            prev = itr->second;
        }
        return prev + 1;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {

        Data data;
        size_t i = 0;
        for (;i < nums.size();i++) {
            if (nums[i] <= 0) continue;
            data.insertFirst(nums[i]);
            break;
        }
        i++;
        for (;i < nums.size(); i++) {
            LOG(data.m);
            data.insert(nums[i]);
        }
        LOG(data.m);
        return data.getMissing();
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return firstMissingPositive(nums.nums);
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

    nums.push_back(Input({ 1,2,0 }));
    expected.push_back(3);

    nums.push_back(Input({ 3,4,-1,1 }));
    expected.push_back(2);

    nums.push_back(Input({ 3,4,-1 }));
    expected.push_back(1);

    nums.push_back(Input({ 1,2,4,3,6 }));
    expected.push_back(5);



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


