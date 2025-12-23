
/*
2901. Longest Unequal Adjacent Groups Subsequence II
Medium
Topics
Companies
Hint

You are given a string array words, and an array groups, both arrays having length n.

The hamming distance between two strings of equal length is the number of positions at which the corresponding characters are different.

You need to select the longest

from an array of indices [0, 1, ..., n - 1], such that for the subsequence denoted as [i0, i1, ..., ik-1] having length k, the following holds:

    For adjacent indices in the subsequence, their corresponding groups are unequal, i.e., groups[ij] != groups[ij+1], for each j where 0 < j + 1 < k.
    words[ij] and words[ij+1] are equal in length, and the hamming distance between them is 1, where 0 < j + 1 < k, for all indices in the subsequence.

Return a string array containing the words corresponding to the indices (in order) in the selected subsequence. If there are multiple answers, return any of them.

Note: strings in words may be unequal in length.



Example 1:

Input: words = ["bab","dab","cab"], groups = [1,2,2]

Output: ["bab","cab"]

Explanation: A subsequence that can be selected is [0,2].

    groups[0] != groups[2]
    words[0].length == words[2].length, and the hamming distance between them is 1.

So, a valid answer is [words[0],words[2]] = ["bab","cab"].

Another subsequence that can be selected is [0,1].

    groups[0] != groups[1]
    words[0].length == words[1].length, and the hamming distance between them is 1.

So, another valid answer is [words[0],words[1]] = ["bab","dab"].

It can be shown that the length of the longest subsequence of indices that satisfies the conditions is 2.

Example 2:

Input: words = ["a","b","c","d"], groups = [1,2,3,4]

Output: ["a","b","c","d"]

Explanation: We can select the subsequence [0,1,2,3].

It satisfies both conditions.

Hence, the answer is [words[0],words[1],words[2],words[3]] = ["a","b","c","d"].

It has the longest length among all subsequences of indices that satisfy the conditions.

Hence, it is the only answer.



Constraints:

    1 <= n == words.length == groups.length <= 1000
    1 <= words[i].length <= 10
    1 <= groups[i] <= n
    words consists of distinct strings.
    words[i] consists of lowercase English letters.

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

typedef vector<string> V1String;
typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, GID, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1String INPUT_TYPE;
typedef V1String OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    V1Int mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1, V1Int input2) {
        mData1 = input1;
        mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};


/*---------------------------------------------------------------------------*/
bool IsHammingNeighbor(const string& str1, const string& str2) {
    int count = 0;
    for (int i = 0; i < (int)str1.length(); i++) {
        if (str1[i] != str2[i]) {
            count++;
            if (count == 2) return false;
        }
    }
    return count == 1;
}




class Solution {
public:

    Solution() {}

    vector<string> getWordsInLongestSubsequence(vector<string>& words, vector<int>& groups) {

        int n = words.size();
        if (n == 0) {
            return {};
        }
        if (n == 1) {
            return { words[0] };
        }

        vector<IDX> hammingFlags(n, 1);
        vector<IDX> traceIdx(n, -1);
        int maxVal = 1;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (groups[i] == groups[j]) continue;
                if (words[i].length() != words[j].length()) continue;
                if (!IsHammingNeighbor(words[i], words[j])) continue;

                if (hammingFlags[i] < hammingFlags[j] + 1) {
                    LOG("Updating for: " << words[j] << " and " << words[i]);
                    hammingFlags[i] = hammingFlags[j] + 1;
                    traceIdx[i] = j;
                    maxVal = max(maxVal, hammingFlags[i]);
                }
            }
        }

        LOG("Hamming Flags: " << hammingFlags);
        LOG("trace : " << traceIdx);
        LOG("Max Value: " << maxVal);

        vector<string> result(maxVal, "");
        int idx = 0;
        for (;hammingFlags[idx] < maxVal; idx++) {}

        int update_pos = maxVal - 1;
        while (idx != -1) {
            result[update_pos--] = words[idx];
            idx = traceIdx[idx];
        }

        return result;
    }


    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return getWordsInLongestSubsequence(nums.mData1, nums.mData2);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " groups:" << ipObj.mData2;
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
        nums.back().setInput({ "bab","dab","cab" }, { 1,2,2 });
        nums.back().setOutput({ "bab", "dab" });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "a","b","c","d" }, { 1,2,3,4 });
        nums.back().setOutput({ "a","b","c","d" });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "abc","aqc","abd","abe" }, { 1,2,3,4 });
        nums.back().setOutput({ "abc","abd","abe" });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "abc","aqc","abd","abe" }, { 1,2,3,3 });
        nums.back().setOutput({ "abc","aqc" });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "abc", "abcde" }, { 1,2 });
        nums.back().setOutput({ "abc" });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "abc", "abcde", "abe" }, { 1,2,1 });
        nums.back().setOutput({ "abc" });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "bd","dbd","dcc","cb","ac","bb","abd" }, { 1,3,3,6,6,4,5 });
        nums.back().setOutput({ "bd", "bb" });
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


