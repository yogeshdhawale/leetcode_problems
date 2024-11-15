
/*
3187. Peaks in Array
Hard
Topics
Companies
Hint

A peak in an array arr is an element that is greater than its previous and next element in arr.

You are given an integer array nums and a 2D integer array queries.

You have to process queries of two types:

    queries[i] = [1, li, ri], determine the count of peak elements in the
    subarray
    nums[li..ri].
    queries[i] = [2, indexi, vali], change nums[indexi] to vali.

Return an array answer containing the results of the queries of the first type in order.

Notes:

    The first and the last element of an array or a subarray cannot be a peak.



Example 1:

Input: nums = [3,1,4,2,5], queries = [[2,3,4],[1,0,4]]

Output: [0]

Explanation:

First query: We change nums[3] to 4 and nums becomes [3,1,4,4,5].

Second query: The number of peaks in the [3,1,4,4,5] is 0.

Example 2:

Input: nums = [4,1,4,2,1,5], queries = [[2,2,4],[1,0,2],[1,0,4]]

Output: [0,1]

Explanation:

First query: nums[2] should become 4, but it is already set to 4.

Second query: The number of peaks in the [4,1,4] is 0.

Third query: The second 4 is a peak in the [4,1,4,2,1].



Constraints:

    3 <= nums.length <= 105
    1 <= nums[i] <= 105
    1 <= queries.length <= 105
    queries[i][0] == 1 or queries[i][0] == 2
    For all i that:
        queries[i][0] == 1: 0 <= queries[i][1] <= queries[i][2] <= nums.length - 1
        queries[i][0] == 2: 0 <= queries[i][1] <= nums.length - 1, 1 <= queries[i][2] <= 105


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

/****************************************************************************/

#define __DEBUG_BIG_TEST
#define __DEBUG_LOG             __DEBUG_LOG_OFF
#include "log.h"
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 5);

/****************************************************************************/

#include <ranges>

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
typedef pair<int, int> IDX2D;

typedef V1Int INPUT_TYPE;
typedef V1Int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    V1Int mData1;
    V2Int mData2;

    Input() {
        mID = inputID++;
    }
    Input(V1Int d1, V2Int d2) {
        this->mData1 = d1;
        this->mData2 = d2;
        mID = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data;
ostream& operator<<(ostream& out, const Data& d);

struct Data {

    void clear() {
    }
    Data() {
        clear();
    }
};

class Solution {
public:

    map<int, int> mp;
    vector<int> countOfPeaks(vector<int>& nums, vector<vector<int>>& queries) {

        mp.clear();
        for (size_t i = 1; i < nums.size() - 1; i++) {
            if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) {
                mp[i] = nums[i];
            }
        }

        vector<int> result;
        for (auto& q : queries) {
            LOG("mp: " << mp);
            switch (q[0])
            {
            case 1:
                LOG("Query count:" << q[1] << " " << q[2]);
                result.push_back(getCount(q[1], q[2]));
                break;
            case 2:
                LOG("update:" << q[1] << " " << q[2]);
                updateNumber(nums, q[1], q[2]);
                break;
            default:
                break;
            }
        }
        return result;
    }

    void updateNumber(vector<int>& nums, int index, int value) {
        if (nums[index] == value) return;

        if (mp.find(index) != mp.end()) {
            if (nums[index] < value) {
                nums[index] = value;
                return;
            }
            else mp.erase(index);
        }

        if (mp.find(index - 1) != mp.end()) {
            mp.erase(index - 1);
        }
        if (mp.find(index + 1) != mp.end()) {
            mp.erase(index + 1);
        }
        nums[index] = value;
        int startIdx = max(1, index - 1);
        int endIdx = min((int)nums.size() - 1, index + 1);
        for (int i = startIdx; i < endIdx; i++) {
            if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) {
                mp[i] = nums[i];
            }
        }
    }

    int getCount(int left, int right) {
        if (mp.size() == 0) return 0;

        auto l = mp.lower_bound(left + 1);
        if (l == mp.end() || l->first >= right) return 0;

        auto r = mp.lower_bound(right - 1);
        if (r == mp.end()) return distance(l, mp.end());

        while (l != r) {
            if (r->first >= right) r--;
            else break;
        }
        if (l == r) return 1;
        return distance(l, r) + 1;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return countOfPeaks(nums.mData1, nums.mData2);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " queries:" << ipObj.mData2;
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
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int GenerateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 3, 1, 4, 2, 5 }, { {2,3,4}, {1,0,4} }));
    expected.push_back({ 0 });

    nums.push_back(Input({ 4,1,4,2,1,5 }, { {2,2,4}, {1,0,2}, {1,0,4} }));
    expected.push_back({ 0,1 });

    nums.push_back(Input({ 4,7,3,8,2,9 }, { { 1,0,2 }, { 1,1,3 }, { 1, 0,5 },{ 1, 1,5 } }));
    expected.push_back({ 1,0,2,1 });

    nums.push_back(Input({ 4,7,3,8,2,9 }, { { 1,0,2 }, { 1,1,3 }, { 1, 0,5 }, {2,3,2}, { 1, 1,5 } }));
    expected.push_back({ 1,0,2,0 });

    nums.push_back(Input({ 4,7,3,8,2,9,3,4,5,2 }, { { 1, 0,5 },{ 1, 1,5 },{1, 5,8},{1, 5,9} }));
    expected.push_back({ 2,1,0,1 });

    nums.push_back(Input({ 3,6,9 }, { {1,1,1}, {1,2,2}, {2,2,3} }));
    expected.push_back({ 0,0 });

    cout << endl << __LOGPREFIX << "test:" << testID << " - (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test loop_id: " << i << " ==>\t" << result << " vs e:" << expected[i] << endl;
        logtime("Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#ifdef __DEBUG_BIG_TEST

extern vector<int> load_data_v1(string fileName);
extern vector<vector<int>> load_data_v2(string fileName);

void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input(load_data_v1("test_data/test1-i1.txt"), load_data_v2("test_data/test1-i2.txt")));
    expected.push_back(load_data_v1("test_data/test1-e.txt"));

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("*****Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "***Start***" << endl;
    test1(1);
#ifdef __DEBUG_BIG_TEST
    test2(2);
#endif
    cout << "***END***" << endl;
    return 0;
}


