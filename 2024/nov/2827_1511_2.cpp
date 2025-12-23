
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
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 9);

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
    // cache structure
    // oddEvenCache[number] = odd even count
    // cacheCount[digit_length][reminder][oe_cnt+MAX_DigitCnt] = count
    vector<vector<vector<long>>> ccPartCnt;
    unordered_map<long, int> oddEvenCache;
    unordered_map<long, int> fromStartTill_DL;
    int MAX_DL;

public:

    inline int getReminderFor(long num, int k) {
        return num % k;
    }
    inline int fixReminder(int rem, int k) {
        return (rem + k) % k;
    }

    int getOddEvenCnt(string num) {
        int cnt = 0;
        for (auto ch : num) {
            cnt += oddEvenCache[ch - '0'];
        }
        return cnt;
    }
    int getOddEvenCnt(long num, int prefix_oecnt) {
        if (oddEvenCache.find(num) != oddEvenCache.end()) {
            return oddEvenCache[num] + prefix_oecnt;
        }
        auto v = getOddEvenCnt(to_string(num));
        oddEvenCache[num] = v;
        return v + prefix_oecnt;
    }

    void setOddEvenCnt() {
        //if digit is even set it to 1 (0, 2, 4, 6, 8)
        //if digit is odd set it to -1 (1, 3, 5, 7, 9)
        // when total of all the digits in number is zero, it satisfies our cond.
        // 354 = -1-1+1 = -1
        // 3546 = -1-1+1+1 = 0

        auto cnt = 1;
        for (int i = 0; i < 10; i++) {
            oddEvenCache[i] = cnt;
            cnt = -cnt;
        }
        for (int i = 0; i <= MAX_DL; i++)
        {
            long startRange = pow(10, i);
            oddEvenCache[startRange] = getOddEvenCnt(startRange, 0);
        }
    }
    inline bool IsBeautifulNumber(long num, int k, int prefix_oecnt = 0, int prefix_rem = 0)
    {
        if (getOddEvenCnt(num, prefix_oecnt) != 0)
            return false;
        return getReminderFor(prefix_rem + getReminderFor(num, k), k) == 0;
    }

    string getStartNumString(long num) {
        auto len = to_string(num).size();
        string strStartD(len, '0');
        strStartD[0] = '1';
        return strStartD;
    }
    string getEndNumString(long num) {
        auto len = to_string(num).size();
        string strEndD(len, '9');
        return strEndD;
    }



    void createCache(long endNum, int k)
    {
        string strEndD = getEndNumString(endNum);
        MAX_DL = strEndD.length();
        LOG("Max Digit Count : " << MAX_DL);
        ccPartCnt.resize(MAX_DL);

        for (int i = 0; i < MAX_DL; i++)
            ccPartCnt[i].resize(k, vector<long>(MAX_DL * 2 + 1, 0));

        // processing digits of length 1 - (0 to 9)
        for (int i = 0; i < 10; i++) {
            auto rem = getReminderFor(i, k);
            auto oecnt = oddEvenCache[i] + MAX_DL;
            ccPartCnt[1][rem][oecnt]++;
        }
        for (int i = 0; i < k; i++)
            LOG("cacheCount[1][" << i << "] :\t" << ccPartCnt[1][i]);
        LOG("");



        for (int cur_dl_itr = 2; cur_dl_itr < MAX_DL; cur_dl_itr++)
        {
            auto& curCache = ccPartCnt[cur_dl_itr];
            long multiplier = pow(10, cur_dl_itr - 1);
            for (int fpDigit = 1; fpDigit < 10; fpDigit++)
            {
                long startNum = fpDigit * multiplier;
                int rem = getReminderFor(startNum, k);
                int oecnt = getOddEvenCnt(startNum, 0);

                for (int small_dl_itr = 1;small_dl_itr < cur_dl_itr;small_dl_itr++) {
                    auto& prevCache = ccPartCnt[small_dl_itr];
                    for (int l_rem = 0; l_rem < k; l_rem++)
                    {
                        int newRem = fixReminder(rem + l_rem, k);
                        int newOEcnt = oecnt - small_dl_itr * oddEvenCache[0];
                        for (int l_oe_cnt = 0; l_oe_cnt < MAX_DL * 2; l_oe_cnt++) {
                            // adding maxDigitCnt in newOEcnt and sub from l_oe_cnt cancels out
                            if (prevCache[l_rem][l_oe_cnt] == 0) continue;
                            int updatedOEcnt = l_oe_cnt + newOEcnt;
                            if (updatedOEcnt < 0 || updatedOEcnt >= MAX_DL * 2) continue;
                            curCache[newRem][updatedOEcnt] += prevCache[l_rem][l_oe_cnt];
                        }
                    }
                }
            }
            fromStartTill_DL[cur_dl_itr] = curCache[0][MAX_DL];
        }

        for (auto cdl = 2; cdl < MAX_DL; cdl++) {
            auto& prevCC = ccPartCnt[cdl - 1];
            auto& curCC = ccPartCnt[cdl];
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < -1 + MAX_DL * 2; j++) {
                    curCC[i][j + 1] += prevCC[i][j];
                }
            }
            int tsum = 0;
            for (int i = 0; i < k; i++) {
                long sum = accumulate(curCC[i].begin(), curCC[i].end(), 0);
                tsum += sum;
                LOG("cacheCount[" << cdl << "][" << i << "] :\t" << curCC[i]
                    << "\t sum:" << sum);
            }
            LOG("cacheCount[" << cdl << "] total sum:" << tsum);
        }
        LOG("oddEvenCache : " << oddEvenCache);
        LOG("fromStartTill_DL : " << fromStartTill_DL);
        return;
    }
    int numberOfBeautifulIntegers(int low, int high, int k)
    {
        if (high < 10) return 0;
        if (low > high) return 0;

        ccPartCnt.clear();
        oddEvenCache.clear();
        fromStartTill_DL.clear();

        setOddEvenCnt();

        if (low == high)
            return IsBeautifulNumber(low, k) ? 1 : 0;
        createCache(high, k);
        return getMatchingNumbers(low, high, k);
    }

    int getMatchingNumbers(long low, long high, int k)
    {
        // e.g. 23, 67, k=3
        // cnt(0,67) - cnt(0,22)
        int highCnt = 0, lowCnt = 0;
        highCnt = getCountFor(high, k);
        if (low >= 10) lowCnt = getCountFor(low, k);
        if (IsBeautifulNumber(low, k)) lowCnt--;
        LOG("Result = " << highCnt << " - " << lowCnt << " = " << highCnt - lowCnt);
        return highCnt - lowCnt;
    }
    int getCountFor(long num, int k) {
        // e.g. 1204, k=3 => dlen = 4
        auto strNum = to_string(num);
        int dl = strNum.size();

        auto ret = 0;
        for (int i = 0;i < dl;i++) ret += fromStartTill_DL[i];
        LOG("Initial Count for " << num << " = " << ret);
        if (dl % 2 == 1) return ret;

        /*
            for 1204
            get cout till 999 - max range of (num/10)
            loop for - max range of num/20
                1000 - 1099
                1100 - 1199
                loop for - ... max range 2nd last digit
                    1200 - 1200
                    calculate for last digit
                        1201 - 1204
         */
        string strStartD = getStartNumString(num);
        for (int i = 0;i < dl - 1;i++) {
            auto& prevCache = ccPartCnt[dl - i - 1];
            strStartD[0] = '1';
            int rend = strNum[i] - '0';

            int j = 0;
            if (i == 0) j = 1;
            for (;j < rend;j++) {
                auto curPrefixStr = strNum.substr(0, i);
                int prefixOEcnt = getOddEvenCnt(curPrefixStr);
                prefixOEcnt += getOddEvenCnt(j, 0);

                strStartD[0] = '0' + j;
                curPrefixStr += strStartD;
                int rem = getReminderFor(stol(curPrefixStr), k);
                rem = fixReminder(k - rem, k);

                auto oe_cnt_needed = -prefixOEcnt + MAX_DL;

                LOG("\ttill <" << curPrefixStr << "...> ret=" << ret
                    << "\t i,j=" << i << "," << j << " cache for:" << dl - i - 1 << "," << rem << ","
                    << -prefixOEcnt << " adding: " << prevCache[rem][oe_cnt_needed]);
                ret += prevCache[rem][oe_cnt_needed];
            }
            strStartD.pop_back();
            if (strStartD.length() == 0) break;
        }

        long lastNumber = (num / 10) * 10;
        LOG("\tcount till " << lastNumber << " = " << ret);
        auto prefix_rem = getReminderFor(lastNumber, k);
        auto prefix_oe = getOddEvenCnt(lastNumber / 10, 0);
        for (int i = 0; i < (num % 10) + 1; i++) {
            if (IsBeautifulNumber(i, k, prefix_oe, prefix_rem)) {
                LOG("\tincreasing count for " << lastNumber + i << " by 1");
                ret++;
            }
        }
        LOG("Returning Count for " << num << " = " << ret);
        return ret;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return numberOfBeautifulIntegers(nums.mData1[0], nums.mData1[1], nums.mData1[2]);
    }
};


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
        nums.back().setInput({ 123,567,1 });
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 42,58,3 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1234,2134,50 });
        nums.back().setOutput(8);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 10,20,3 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 36,60,3 });
        nums.back().setOutput(3);
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
        nums.back().setInput({ 1,1000,17 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,100,12 });
        nums.back().setOutput(4);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,79,12 });
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1148, 1258, 3 });
        nums.back().setOutput(13);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 481, 11729, 18 });
        nums.back().setOutput(240);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 481623,11729930,18 });
        nums.back().setOutput(37857);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 10000000 , 1000000000 , 1 });
        nums.back().setOutput(24609375);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 17 , 17 , 17 });
        nums.back().setOutput(0);
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


