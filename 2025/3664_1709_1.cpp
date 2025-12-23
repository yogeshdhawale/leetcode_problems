
/*

3664. Two-Letter Card Game
Medium
premium lock iconCompanies
Hint

You are given a deck of cards represented by a string array cards, and each card displays two lowercase letters.

You are also given a letter x. You play a game with the following rules:

    Start with 0 points.
    On each turn, you must find two compatible cards from the deck that both contain the letter x in any position.
    Remove the pair of cards and earn 1 point.
    The game ends when you can no longer find a pair of compatible cards.

Return the maximum number of points you can gain with optimal play.

Two cards are compatible if the strings differ in exactly 1 position.



Example 1:

Input: cards = ["aa","ab","ba","ac"], x = "a"

Output: 2

Explanation:

    On the first turn, select and remove cards "ab" and "ac", which are compatible because they differ at only index 1.
    On the second turn, select and remove cards "aa" and "ba", which are compatible because they differ at only index 0.

Because there are no more compatible pairs, the total score is 2.

Example 2:

Input: cards = ["aa","ab","ba"], x = "a"

Output: 1

Explanation:

    On the first turn, select and remove cards "aa" and "ba".

Because there are no more compatible pairs, the total score is 1.

Example 3:

Input: cards = ["aa","ab","ba","ac"], x = "b"

Output: 0

Explanation:

The only cards that contain the character 'b' are "ab" and "ba". However, they differ in both indices, so they are not compatible. Thus, the output is 0.



Constraints:

    2 <= cards.length <= 105
    cards[i].length == 2
    Each cards[i] is composed of only lowercase English letters between 'a' and 'j'.
    x is a lowercase English letter between 'a' and 'j'.



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
typedef vector<string> INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    char mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1, char input2) {
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

    inline int c2idx(char c) { return c - 'a'; }

    int score(vector<string>& cards, char x) {

        vector<int> firstCharCount(10, 0);
        vector<int> secondCharCount(10, 0);
        int twinCardCnt = 0;


        int firstTotal = 0;
        int maxInFirstTotal = 0;
        int secondTotal = 0;
        int maxInSecondTotal = 0;

        for (auto& card : cards) {

            auto firstChar = card[0];
            auto secondChar = card[1];

            if (firstChar != x && secondChar != x) continue;

            if (firstChar == secondChar) {
                twinCardCnt++;
                continue;
            }
            if (firstChar == x) {
                firstCharCount[c2idx(secondChar)]++;
                firstTotal++;
                maxInFirstTotal = max(maxInFirstTotal, firstCharCount[c2idx(secondChar)]);
            }
            if (secondChar == x) {
                secondCharCount[c2idx(firstChar)]++;
                secondTotal++;
                maxInSecondTotal = max(maxInSecondTotal, secondCharCount[c2idx(firstChar)]);
            }
        }

        LOG("firstCharCount:" << firstCharCount);
        LOG("secondCharCount:" << secondCharCount);
        LOG("twinCardCnt:" << twinCardCnt << " firstTotal:" << firstTotal << " secondTotal:" << secondTotal);

        int pendingCardsInFirst = firstTotal % 2;
        int pendingCardsInSecond = secondTotal % 2;

        if (maxInFirstTotal > (firstTotal / 2)) {
            pendingCardsInFirst = maxInFirstTotal - (firstTotal - maxInFirstTotal);
        }

        if (maxInSecondTotal > (secondTotal / 2)) {
            pendingCardsInSecond = maxInSecondTotal - (secondTotal - maxInSecondTotal);
        }


        LOG("pendingCardsInFirst:" << pendingCardsInFirst << " pendingCardsInSecond:" << pendingCardsInSecond);
        LOG("twinCardCnt:" << twinCardCnt << " total pending:" << pendingCardsInFirst + pendingCardsInSecond);


        if (twinCardCnt >= (firstTotal + secondTotal)) {
            LOG("too many twin cards return " << firstTotal + secondTotal);
            return firstTotal + secondTotal;
        }

        int firstPairedCardsCnt = firstTotal - pendingCardsInFirst;
        int secondPairedCardsCnt = secondTotal - pendingCardsInSecond;

        int result = firstPairedCardsCnt / 2 + secondPairedCardsCnt / 2;

        if (twinCardCnt == 0) {
            LOG("no twin cards return " << result);
            return result;
        }

        if (twinCardCnt <= (pendingCardsInFirst + pendingCardsInSecond)) {
            result += twinCardCnt;
            LOG("twinCardCnt used up for pending unpaired cards, return " << result);
            return result;
        }

        result += (pendingCardsInFirst + pendingCardsInSecond);
        twinCardCnt -= (pendingCardsInFirst + pendingCardsInSecond);
        result += twinCardCnt / 2;

        LOG("breaking pairs for pending twin cards " << twinCardCnt << " return " << result);

        return result;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return score(nums.mData1, nums.mData2);
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
        nums.back().setInput({ "aa","ab","ba","ac" }, 'a');
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "aa", "ab", "ba" }, 'a');
        nums.back().setOutput(1);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "aa", "ab", "ba", "ac" }, 'b');
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "aa", "aa", }, 'a');
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ "ba", "ba" }, 'b');
        nums.back().setOutput(0);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({
             "cb", "ec", "ad", "ee", "be", "ee", "eb", "ea", "ee", "cc", "ac", "de", "bb", "aa", "db", "cd", "be", "ac", "bc", "dc", "bb", "bc", "db", "ca", "ec", "ac", "ae", "ac", "ea", "de", "ec", "ba", "ce", "bd", "ca", "ad", "ec", "db", "ab", "ac", "ba", "ee", "dc", "eb", "ae", "ec", "ec", "dd", "ed", "bc"
            },
            'e');
        nums.back().setOutput(11);
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


