
/*
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
#include <cmath>

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
/****************************************************************************/
typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1) {
        this->mData1 = input1;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/****************************************************************************/

class Solution
{
    vector<vector<map<int, int>>> cacheCount;
    map<long, int> oddEvenCache;

public:
    int numberOfBeautifulIntegers(int low, int high, int k)
    {
        if (low < 10)
            low = 10;

        if (low > high)
            return 0;

        auto maxRange = createCache({ low, high }, k);

        if (low == high)
            return IsBeautifulNumber(low, k) ? 1 : 0;

        return getMatchingNumbers({ low, high }, maxRange, k);
    }

    int getMatchingNumbers(pair<long, long> range, pair<long, long> maxRange, int k)
    {
        LOG("Processing range : " << range << " with maxRange = " << maxRange << " k = " << k);
        return process(to_string(range.second).size(), maxRange, range, 0, k, 0);
    }

    inline int getReminderFor(int num, int k)
    {
        return num % k;
    }
    inline bool IsBeautifulNumber(int num, int k, int oecnt = 0, int rem = 0)
    {
        if (getOddEvenCnt(num, oecnt) != 0)
            return false;
        return getReminderFor(rem + getReminderFor(num, k), k) == 0;
    }
    /*
         * return 1 if the odd even count is zero
        * return -1 otherwize
    */
    inline int getOddEvenCnt(int num, int oecnt = 0)
    {
        int curOEcnt = 0;
        auto len = to_string(num).size();
        if (len == 1)
            return oecnt + (getReminderFor(num, 2) == 0) ? 1 : -1;

        for (;len > 0;len--)
        {
            int lastDigit = getReminderFor(num, 10);
            curOEcnt += oddEvenCache[lastDigit];
            num /= 10;
        }
        return curOEcnt + oecnt;
    }


    long getCountForSmallRange(pair<long, long> range, int oecnt, int k, int rem)
    {
        long result = 0;
        long i = range.first;
        rem = getReminderFor(rem + getReminderFor(i, k), k);
        if (rem != 0)
        {
            i += k - rem;
        }

        for (; i <= range.second; i = i + k)
        {
            result += (getOddEvenCnt(i, oecnt) == 0) ? 1 : 0;
        }
        return result;
    }

    pair<long, long> createCache(pair<long, long> range, int k = 1)
    {
        // cache structure
        // cache-k[digit][rem][oecnt] = count
        // cache-d[digit] = reminder

        cacheCount.clear();
        oddEvenCache.clear();

        size_t highlen = to_string(range.second).size();
        string start(highlen, '0');
        string end(highlen, '9');
        start[0] = '1';
        long retstart = stol(start);
        long retend = stol(end);

        cacheCount.resize(highlen + 1);

        for (size_t i = 0; i <= highlen; i++)
        {
            cacheCount[i].resize(k + 1, map<int, int>());

            int startRange = pow(10, i);
            oddEvenCache[startRange] = getOddEvenCnt(startRange, 0);
        }

        // processing digits of length 1
        for (int i = 1; i < 10; i++)
        {
            cacheCount[1][getReminderFor(i, k)][getOddEvenCnt(i)]++;
            oddEvenCache[i] = getOddEvenCnt(i);
        }

        LOG("Cache digit @1" << cacheCount[1]);

        for (size_t d = 2; d < highlen; d++)
        {
            //for digit of size d=3
            auto prevCache = cacheCount[d - 1];
            int multiplier = pow(10, d - 1);

            for (int i = 1; i < 10; i++)
            {
                int rem = getReminderFor(i * multiplier, k);
                int oecnt = getOddEvenCnt(i, 0);
                for (int j = 0; j < k; j++)
                {
                    int newRem = (rem + j) % k;
                    for (auto itr = prevCache[j].begin(); itr != prevCache[j].end(); itr++) {
                        cacheCount[d][newRem][oecnt + itr->first] += itr->second;
                    }
                }
            }
            LOG("Cache digit @" << d << cacheCount[d]);

        }

        return make_pair(retstart, retend);
    }

    /**
 * A function to process the input range and return the result.
 *
 * @param len the length of the input range
 * @param maxRange the maximum range
 * @param range the input range
 * @param oecnt count of odd/even numbers
 * @param k the divisor
 * @param rem reminder
 *
 * @return the result of processing the input range
 *
 * @throws ErrorType description of error
 */
    long process(int len, pair<long, long> maxRange, pair<long, long> range, int oecnt, int k, int rem)
    {
        if (range.first > range.second)
        {
            return 0;
        }
        if (range.first == range.second)
        {
            return IsBeautifulNumber(range.first, k, oecnt, rem);
        }
        if (len <= 2)
        {
            long result = getCountForSmallRange(range, oecnt, k, rem);
            LOG("\t\t\tfor small range:" << range << "with (" << oecnt << "," << rem << ") result = " << result);
            return result;
        }
        bool isPrefixPreprocessed = false;
        bool isSuffixPreprocessed = false;

        if (maxRange.first == range.first)
        {
            isPrefixPreprocessed = true;
        }
        if (maxRange.second == range.second)
        {
            isSuffixPreprocessed = true;
        }

        if (isPrefixPreprocessed && isSuffixPreprocessed)
        {
            if (cacheCount[len][k][oecnt] > 0)
                return cacheCount[len][k][oecnt];
        }

        // 250-780 => 250-299, 300-699, 700-780
        // 252-254 => 252-254
        auto curRange = make_pair(range.first / maxRange.first, (range.second / maxRange.first));
        if (!isPrefixPreprocessed && getReminderFor(range.first, maxRange.first) != 0)
        {
            curRange.first++;
        }
        if (isSuffixPreprocessed)
        {
            curRange.second++;
        }

        LOG("\trange : input:" << range << " maxRange:" << maxRange << " with (" << oecnt << ", " << rem << ") loopRange:" << curRange);
        long result = 0;
        auto prevLevel = cacheCount[len - 1];
        for (auto i = curRange.first; i < curRange.second; i++)
        {
            auto curOEcnt = -1 * getOddEvenCnt(i, oecnt);
            auto curRem = (k - 1) - getReminderFor(rem + getReminderFor(i * maxRange.first, k), k);
            if (prevLevel[curRem].find(curOEcnt) != prevLevel[curRem].end())
                result += cacheCount[len - 1][curRem][curOEcnt];
        }

        if (curRange.first == curRange.second) {
            result += IsBeautifulNumber(curRange.first, k, oecnt, rem);
        }
        if (isPrefixPreprocessed && isSuffixPreprocessed)
            return result;

        long p1 = 0;
        long p2 = 0;

        curRange.first *= maxRange.first;
        curRange.second *= maxRange.first;

        auto prange = make_pair(range.first % maxRange.first, (curRange.first - 1) % maxRange.first);
        auto srange = make_pair(0, range.second - curRange.second);

        if (curRange.second == srange.second)
        {
            isSuffixPreprocessed = true;
        }

        LOG("process:(" << len << ")" << range << " with oecnt = " << oecnt << " split:" << prange << curRange << "=" << result << srange);

        if (!isPrefixPreprocessed)
        {
            int currem = rem;
            auto newlen = len - 1;
            auto newmxrange = make_pair(maxRange.first / 10, maxRange.second / 10);
            auto prefixOEcnt = oecnt;
            if (range.first > maxRange.first) {
                prefixOEcnt += getOddEvenCnt(curRange.first / maxRange.first);
                currem = getReminderFor(currem + getReminderFor(curRange.first, k), k);
            }

            int curLen = to_string(prange.second).size();
            if (curLen < newlen) {
                LOG("Reducing len from " << len << " to " << curLen);
                while (newlen > curLen) {
                    if (curRange.first > newmxrange.first) {
                        prefixOEcnt += oddEvenCache[0];
                        currem = getReminderFor(currem + getReminderFor(curRange.first, k), k);
                    }
                    newlen--;
                    newmxrange = { newmxrange.first / 10, newmxrange.second / 10 };
                }
            }
            LOG("\tprefix(" << newlen << "): " << prange << " maxRange:" << newmxrange << " with (" << prefixOEcnt << ", " << currem << ")");

            p1 = process(newlen, newmxrange, prange, prefixOEcnt, k, currem);
        }
        if (!isSuffixPreprocessed)
        {
            auto newlen = len - 1;
            auto newmxrange = make_pair(maxRange.first / 10, maxRange.second / 10);

            auto suffixOEcnt = getOddEvenCnt(curRange.second / maxRange.first, oecnt);
            auto currem = getReminderFor(rem + getReminderFor(curRange.second, k), k);

            int curLen = to_string(srange.second).size();
            if (curLen < newlen) {
                LOG("Reducing len from " << len << " to " << curLen);
                while (newlen > curLen) {
                    suffixOEcnt += oddEvenCache[0];
                    newlen--;
                    newmxrange = { maxRange.first / 10, maxRange.second / 10 };
                    currem = getReminderFor(currem + getReminderFor(newmxrange.first, k), k);
                }
            }
            LOG("\tsuffix(" << newlen << "): " << srange << " maxRange:" << newmxrange << " with (" << suffixOEcnt << ", " << currem << ")");

            p2 = process(newlen, newmxrange, srange, suffixOEcnt, k, currem);
        }
        LOG("-Returing for " << range << " with oecnt = " << oecnt << "\t\t{" << p1 << ", " << result << ", " << p2 << "} = " << result + p1 + p2);
        return result + p1 + p2;
    }
    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return numberOfBeautifulIntegers(nums.mData1[0], nums.mData1[1], nums.mData1[2]);
    }
};

/**/

/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1 << " }";
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
        nums.back().setInput({ 10,20,3 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,10,1 });
        nums.back().setOutput(1);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,5,2 });
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 42,58,3 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 481623,11729930,18 });
        nums.back().setOutput(37857);
    }
    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << " loop_id: [" << i << "] =>\t"
            << result << " vs e:" << nums[i].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected1);
    }
}

/****************************************************************************/
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
        auto d2 = load_data_v1<int>(t1i2);
        nums.back().setInput(load_data_v1<int>(t1i1), d2[0], d2[1]);
        nums.back().setOutput(false);
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.proxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    assert(result == nums[0].mExpected1);
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


