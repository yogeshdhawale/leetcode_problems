
/*
3458. Select K Disjoint Special Substrings
Medium
Topics
premium lock iconCompanies
Hint

Given a string s of length n and an integer k, determine whether it is possible to select k disjoint special substrings.

A special substring is a

where:

    Any character present inside the substring should not appear outside it in the string.
    The substring is not the entire string s.

Note that all k substrings must be disjoint, meaning they cannot overlap.

Return true if it is possible to select k such disjoint special substrings; otherwise, return false.



Example 1:

Input: s = "abcdbaefab", k = 2

Output: true

Explanation:

    We can select two disjoint special substrings: "cd" and "ef".
    "cd" contains the characters 'c' and 'd', which do not appear elsewhere in s.
    "ef" contains the characters 'e' and 'f', which do not appear elsewhere in s.

Example 2:

Input: s = "cdefdc", k = 3

Output: false

Explanation:

There can be at most 2 disjoint special substrings: "e" and "f". Since k = 3, the output is false.

Example 3:

Input: s = "abeabe", k = 0

Output: true



Constraints:

    2 <= n == s.length <= 5 * 104
    0 <= k <= 26
    s consists only of lowercase English letters.



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
typedef string INPUT_TYPE;
typedef bool OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    int mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1, int input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/

class Solution {
public:

    Solution() {}

    bool maxSubstringLength(string s, int k) {

        if (k == 0) return true;
        if (s.size() == 1) return false;

        if (s.size() < k) return false;

        vector<int> charCount(26, 0);
        vector<int> prevPosition(26, -2);
        vector<int> isOnlyAdjCharStr(26, 0);

        for (int cur_pos = 0; cur_pos < s.size(); cur_pos++) {

            int cidx = s[cur_pos] - 'a';
            charCount[cidx]++;

            if (charCount[cidx] == 1) {
                // first occurance
            }
            else if (prevPosition[cidx] + 1 == cur_pos && isOnlyAdjCharStr[cidx] != -1) {
                // continuing adj char sequence
                isOnlyAdjCharStr[cidx] = 1;
                //LOG("@i:" << i << " char:" << s[i] << " setting isOnlyAdjCharStr");
            }
            else {
                // char appeared at many places and not contineous
                isOnlyAdjCharStr[cidx] = -1;
                //LOG("@i:" << i << " char:" << s[i] << " invalidating isOnlyAdjCharStr");
            }
            prevPosition[cidx] = cur_pos;
        }

        LOG("charCount: " << charCount);
        LOG("isOnlyAdjCharStr: " << isOnlyAdjCharStr);

        int totalPossibilities = 0;
        int totalDistinctChars = 0;
        for (int i = 0; i < 26; i++) {

            if (charCount[i] == 0) continue;

            totalDistinctChars++;
            if (charCount[i] == 1) {
                totalPossibilities++;
            }
            if (isOnlyAdjCharStr[i] == 1) {
                totalPossibilities++;
            }
            LOG("\t=>char: " << (char)('a' + i) << " tp:" << totalPossibilities);
        }


        if (totalDistinctChars == 1) {
            // whole string cannot be considered as special substring
            LOG("Note:only one char sequence");
            totalPossibilities--;
        }
        LOG("@End: dist_chars:" << totalDistinctChars << "\t tp:" << totalPossibilities << " and k:" << k);

        if (totalDistinctChars == 1) return false;
        if (totalPossibilities >= k) return true;
        return false;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return maxSubstringLength(nums.mData1, nums.mData2);
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
        nums.back().setInput("abcdbaefab", 2);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("cdefdc", 3);
        nums.back().setOutput(false);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("abeabe", 0);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("aabb", 2);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("aabbab", 2);
        nums.back().setOutput(false);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("dd", 1);
        nums.back().setOutput(false);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("eee", 1);
        nums.back().setOutput(false);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("aaaaccaaaaaadaaaaaaaa", 2);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("aaaaccbibibibdaaaaaaaa", 3);
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("acdbdbca", 1); // "c...c" substring is special
        nums.back().setOutput(true);
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


