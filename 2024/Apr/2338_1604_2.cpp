
/*
2338. Count the Number of Ideal Arrays
Hard
Topics
Companies
Hint

You are given two integers n and maxValue, which are used to describe an ideal array.

A 0-indexed integer array arr of length n is considered ideal if the following conditions hold:

    Every arr[i] is a value from 1 to maxValue, for 0 <= i < n.
    Every arr[i] is divisible by arr[i - 1], for 0 < i < n.

Return the number of distinct ideal arrays of length n. Since the answer may be very large, return it modulo 109 + 7.



Example 1:

Input: n = 2, maxValue = 5
Output: 10
Explanation: The following are the possible ideal arrays:
- Arrays starting with the value 1 (5 arrays): [1,1], [1,2], [1,3], [1,4], [1,5]
- Arrays starting with the value 2 (2 arrays): [2,2], [2,4]
- Arrays starting with the value 3 (1 array): [3,3]
- Arrays starting with the value 4 (1 array): [4,4]
- Arrays starting with the value 5 (1 array): [5,5]
There are a total of 5 + 2 + 1 + 1 + 1 = 10 distinct ideal arrays.

Example 2:

Input: n = 5, maxValue = 3
Output: 11
Explanation: The following are the possible ideal arrays:
- Arrays starting with the value 1 (9 arrays):
   - With no other distinct values (1 array): [1,1,1,1,1]
   - With 2nd distinct value 2 (4 arrays): [1,1,1,1,2], [1,1,1,2,2], [1,1,2,2,2], [1,2,2,2,2]
   - With 2nd distinct value 3 (4 arrays): [1,1,1,1,3], [1,1,1,3,3], [1,1,3,3,3], [1,3,3,3,3]
- Arrays starting with the value 2 (1 array): [2,2,2,2,2]
- Arrays starting with the value 3 (1 array): [3,3,3,3,3]
There are a total of 9 + 1 + 1 = 11 distinct ideal arrays.



Constraints:

    2 <= n <= 104
    1 <= maxValue <= 104


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
#define MOD(x) ((x) % RESULT_MOD)

#define MY_MAX_VAL INT32_MAX

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    int num;
    int maxVal;

    Input() {
        id = inputID++;
    }
    Input(int num, int maxVal) {
        id = inputID++;
        this->num = num;
        this->maxVal = maxVal;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data {

    int maxVal;
    int maxlen;

    vector<vector<int>> cache;
    void clear() {
        cache.clear();
    }
    Data(int maxVal, int maxlen) {
        clear();
        this->maxVal = maxVal;
        this->maxlen = maxlen;

        cache.resize(maxVal + 1);
        for (int i = 0; i <= maxVal; i++)
            cache[i].resize(maxlen + 1, INVALID);
    }
    int getOptions(int curNumber, int length) {

        int chainSize = maxVal / curNumber;

        if (length == 0) return 0;
        if (cache[curNumber][length] != INVALID) return cache[curNumber][length];

        LOG("getOptions curNumber=" << curNumber << " length=" << length << " chainSize=" << chainSize);

        if (length == 1) {
            int result = chainSize - 1;
            cache[curNumber][length] = result;
            LOG("getOptions curNumber=" << curNumber << " length=" << length << " chainSize=" << chainSize << " result=" << result);
            return result;
        }

        if (chainSize == 1) {
            cache[curNumber][length] = 0;
            return 0;
        }

        int result = MOD(length * (chainSize - 1));

        for (int i = 1;i < chainSize;i++) {
            auto nextNumber = curNumber * (i + 1);
            for (int j = 1;j < length;j++) {
                result = MOD(result + getOptions(nextNumber, j));
            }
        }

        LOG("getOptions curNumber=" << curNumber << " length=" << length << " chainSize=" << chainSize << " result=" << result);
        cache[curNumber][length] = result;
        return result;
    }

    int getIACount() {
        int result = 0;

        int startNumber = 0;
        for (int i = 1;i <= maxVal / 2;i++) {
            LOG("i=" << i << " result=" << result);
            startNumber = i;
            int ret = getOptions(startNumber, maxlen - 1);
            result = MOD(result + ret);
        }

        LOG("End loop=" << result);

        result = MOD(result + maxVal);

        LOG("End result=" << result);
        return result;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:
    int idealArrays(int n, int maxValue) {

        if (n == 1) return maxValue;
        if (maxValue == 1) return 1;

        Data data(maxValue, n);
        LOG("Created options :" << data);

        int res = data.getIACount();
        LOG("n=" << n << " and maxValue=" << maxValue << " returning " << res);
        return res;
    }


    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return idealArrays(nums.num, nums.maxVal);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.num;
    out << " maxVal:" << nums.maxVal;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    out << "maxVal:" << d.maxVal << " maxlen:" << d.maxlen;
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

    nums.push_back(Input(5, 3));
    expected.push_back(11);

    nums.push_back(Input(2, 5));
    expected.push_back(10);

    nums.push_back(Input(4, 7));
    expected.push_back(43);

    nums.push_back(Input(5, 9));
    expected.push_back(111);

    nums.push_back(Input(587, 290));
    expected.push_back(602300031);

    nums.push_back(Input(5878, 2900));
    expected.push_back(465040898);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = __D_CASE_ID - 1; i <= __D_CASE_ID; i++)
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


