
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

struct charInfo {
    char c;
    int count;
    int firstOccurance;
    int lastOccurance;

    charInfo(char ch) : c(ch), count(0), firstOccurance(0), lastOccurance(0) {}
    void reset(char ch) {
        c = ch;
        count = 0;
        firstOccurance = 0;
        lastOccurance = 0;
    }
    void addOccurance(int pos) {
        if (count == 0) firstOccurance = pos;
        lastOccurance = pos;
        count++;
    }
};

ostream& operator<<(ostream& out, const charInfo& ipObj);


class Solution {


    vector<charInfo> info;
public:

    Solution() {
        info.resize(26, charInfo('a'));
        for (int i = 0; i < 26; i++) info[i].reset('a' + i);
    }

    bool maxSubstringLength(string s, int k) {

        int strlen = s.length();

        if (k == 0) return true;
        if (strlen == 1 || strlen < k) return false;

        for (int cur_pos = 0; cur_pos < strlen; cur_pos++) {
            int cidx = s[cur_pos] - 'a';
            info[cidx].addOccurance(cur_pos);
        }

        int totalDistinctChars = 0;
        int totalSingleCharSpecialSubstrings = 0;
        vector<int> substrRanges(strlen, -1);

        for (int i = 0;i < 26;i++) {
            if (info[i].count == 0) continue;
            LOG(info[i]);
            totalDistinctChars++;

            if (info[i].count == 1) {
                totalSingleCharSpecialSubstrings++;
                int pos = info[i].firstOccurance;
                substrRanges[pos] = pos;
                LOG("single char '" << info[i].c << "' at pos:" << pos << " is special substring");
                continue;
            }

            int start = info[i].firstOccurance;
            int end = extendEnd(s, start);

            if (end != -1) {
                if (start == 0 && end == strlen - 1) {
                    // substring cannot be considered as special as its whole string
                    LOG("Note: whole string cannot be considered as special substring");
                }
                else {
                    substrRanges[start] = end;
                    LOG("for char '" << info[i].c << "' special substring from " << start << " to " << end);
                }
            }
        }

        if (totalDistinctChars == 1) {
            // whole string cannot be considered as special substring
            LOG("Note:only one char sequence");
            return false;
        }

        if (totalSingleCharSpecialSubstrings >= k) {
            LOG("Note:enough single char special substrings");
            return true;
        }

        int maxCount = 0;

        map<int, int> counts; // end -> count of max special substrings till this end

        for (int i = 0; i < strlen; i++) {
            int start = i;
            int end = substrRanges[start];
            if (end == -1) continue;

            if (counts.find(end) == counts.end()) {
                counts[end] = 1;
                maxCount = max(maxCount, counts[end]);
            }

            while (true) {
                auto it = counts.begin();
                int prevEnd = it->first;
                int prevCount = it->second;

                if (prevEnd >= start) {
                    // this and all further ends after, ie. not disjoint
                    break;
                }
                if (counts.find(end) == counts.end())  counts[end] = prevCount + 1;
                else counts[end] = max(counts[end], prevCount + 1);

                maxCount = max(maxCount, counts[end]);
                counts.erase(it);
            }
            LOG("threads:" << counts.size() << " ->" << counts);
            LOG("After range:[" << start << "," << end << "] maxSpecialSubStrings:" << maxCount);
        }

        LOG("At End: maxSpecialSubStrings:" << maxCount);
        if (maxCount >= k) {
            return true;
        }
        return false;
    }

    int extendEnd(string& s, int start) {

        // ensure occurances of all chars between start and end are within the range.
        //  only then it is a special substring

        auto cidx = s[start] - 'a';

        if (info[cidx].firstOccurance < start) {
            return -1;
        }
        int end = info[cidx].lastOccurance;

        for (int i = start + 1; i < end; i++) {
            int cidx = s[i] - 'a';
            if (info[cidx].firstOccurance < start) {
                return -1;
            }
            end = max(end, info[cidx].lastOccurance);
        }
        return end;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return maxSubstringLength(nums.mData1, nums.mData2);
    }
};


/****************************************************************************/
/****************************************************************************/


ostream& operator<<(ostream& out, const charInfo& ipObj) {
    out << ipObj.c << ":" << ipObj.count;
    out << " [" << ipObj.firstOccurance << ", " << ipObj.lastOccurance << "] ";
    return out;
}

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " k:" << ipObj.mData2;
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

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("aabaacaadaaeaafaagaahaai", 8); // "c...c" substring is special
        nums.back().setOutput(true);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("aabbaacaadaaeaafaagaahaai", 8); // "c...c" substring is special
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput("abcdefghiabcdefghi", 2); // "c...c" substring is special
        nums.back().setOutput(false);
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


