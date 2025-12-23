
/*
3317. Find the Number of Possible Ways for an Event
Hard
Topics
Companies
Hint

You are given three integers n, x, and y.

An event is being held for n performers. When a performer arrives, they are assigned to one of the x stages. All performers assigned to the same stage will perform together as a band, though some stages might remain empty.

After all performances are completed, the jury will award each band a score in the range [1, y].

Return the total number of possible ways the event can take place.

Since the answer may be very large, return it modulo 109 + 7.

Note that two events are considered to have been held differently if either of the following conditions is satisfied:

    Any performer is assigned a different stage.
    Any band is awarded a different score.



Example 1:

Input: n = 1, x = 2, y = 3

Output: 6

Explanation:

    There are 2 ways to assign a stage to the performer.
    The jury can award a score of either 1, 2, or 3 to the only band.

Example 2:

Input: n = 5, x = 2, y = 1

Output: 32

Explanation:

    Each performer will be assigned either stage 1 or stage 2.
    All bands will be awarded a score of 1.

Example 3:

Input: n = 3, x = 3, y = 4

Output: 684



Constraints:

    1 <= n, x, y <= 1000


---

distribute a across b
    => a identical, b identical
        (a+b-1)C(b-1)
    => a unique, b unique
        (a+b-1)C(b-1) * (a)! * (b-1)!
    => a unique, b identical
        (a+b-1)C(b-1) * (a)!
    => a identical, b unique
        (a+b-1)C(b-1) * (b-1)!



1 across 3
(1+3-1)C(3-1) = 3C2 = 3
i,i = 3
u,u = 3 * 2 * 1 = 6
u,i = 3 * 1 = 3
i,u = 3 * 2 = 6

2 across 3
(2+3-1)C(3-1) = 4C2 = 6
i,i = 6
u,u = 6 * 2 * 2 = 24
u,i = 6 * 2 = 12
i,u = 6 * 2 = 12

3 across 2
(3+2-1)C(2-1) = 5C1 = 5
i,i = 6
u,u = 6 * 2 * 2 = 24
u,i = 6 * 2 = 12
i,u = 6 * 2 = 12

5 across 2
6C1 = 6
i,i = 6
u,u = 6 * 1 * 1 = 6
u,i = 6 * 1 = 6
i,u = 6 * 1 = 6

Input: players = 3(n), stages(x) = 3, score = 4(y)
players 3 => a,b,c
stagess 3 => s1,s2,s3
result = 684
        684/2=342/2=171
        684/3=228/3=76
        684/4=171
score combos:
[1-4] [1-4] [1-4] = 64
itr_0: 0 empty stages, 3 used stages = 4^x = 64
        distribute x players to occupy all = x!
        distribute n-x players = 0
        [a-c] [a-c] [a-c] 1 combo       -> 1 * 3! * 64  = 384
itr_1: 1 empty stage, 2 used stages = 4^(x-1) * 3c2 = 48
        [a-c] [a-c] []  = 3*2*1         -> 6 *48        = 288
itr_last: x-1 empty stage = 4^1 * 3c1 = 12
        all are in one stage            -> 1 * 12       =  12
                                            total       = 684

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
//                 -123456789

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------*/

class Solution {
public:

    vector<long> factorialTable;
    vector<vector<long>> stirlingNo2;
    vector<long> powerTableForScores;
    Solution() {}

    long getMOD(double d) {
        if (d == RESULT_MOD) return d;
        long ret_val = lround(d) % (long)RESULT_MOD;
        return ret_val;
    }
    int numberOfWays(int n, int x, int y) {
        int performers = n;
        int stages = x;
        int scores = y;

        prepFactorialTable(stages);
        prepSn2Table(performers, stages);
        preparePowerTable(scores, stages);

        LOG("factorial table:" << factorialTable);
        //LOG("stirling no. of 2nd kind:[" << sn2Table.size() << "," << sn2Table[0].size() << "]");
        //for (int i = 0;i < sn2Table.size();i++) LOG("@i:" << i << "=" << sn2Table[i]);

        LOG("performers:" << performers << " stages:" << stages << " scores:" << scores);


        /*
        * calculations are:
                performers = n, stages = x, scores = y

            for each stage usage combination [ i out of x ]
            {
                1. choose i stages to use out of x stages = xCi = C(x,i)
                2. scores for i used stages = y^i
                3. distribute performers to i used stages = stirlingNo2(n,i)
                4. assign performers buckets/group created in (3.) to used stages: i!
            }
            -> as xCi = (xPi / i!) we can do: (1.) * (4.) = xCi * (i!) = xPi
        */

        int result = 0;
        int loopCnt = min(stages, performers);
        for (int i = loopCnt; i > 0; i--) {
            int unusable_st = stages - i;
            int remaining_perf = performers - i;
            LOG("At split stages:[" << i << ", " << unusable_st << "], performers:[" << performers - remaining_perf << ", " << remaining_perf << "]");

            auto stage_combos = nPr(stages, i);
            auto scoring_combos = powerTableForScores[i];
            auto perf_combos = stirlingNo2[performers][i];
            //long perf_combos = sn2(performers, i);

            long cur_score = stage_combos;
            cur_score = getMOD(cur_score * scoring_combos);
            cur_score = getMOD(cur_score * perf_combos);
            //cur_score = getMOD(cur_score * getMOD(factorialTable[i]));

            LOG("\t stage_c:" << stage_combos << " score_c:" << scoring_combos << " perf_c: " << perf_combos
                << "\tcur_score:" << cur_score);
            result = getMOD(result + cur_score);
        }
        LOG("Returing result:" << result);
        return result;
    }
    void prepFactorialTable(int n) {
        factorialTable.clear();
        factorialTable.resize(n + 1, 1);
        for (int i = 1;i <= n;i++) {
            factorialTable[i] = getMOD(factorialTable[i - 1] * i);
        }
    }

    void prepSn2Table(int s, int k) {
        //Stirling number of the second kind
        stirlingNo2.clear();

        stirlingNo2.resize(s + 1);
        for (int i = 0;i <= s;i++)
            stirlingNo2[i].resize(k + 1, 0);

        for (int i = 1;i <= s;i++) stirlingNo2[i][1] = 1;
        for (int i = 1;i <= min(s, k);i++) stirlingNo2[i][i] = 1;

        for (int i = 1;i <= s;i++)
            for (int j = 2;j <= k;j++) {
                long part1 = stirlingNo2[i - 1][j - 1];
                long part2 = getMOD(j * stirlingNo2[i - 1][j]);
                stirlingNo2[i][j] = getMOD(part1 + part2);
            }
    }

    void preparePowerTable(int value, int exponent) {
        powerTableForScores.clear();
        powerTableForScores.resize(exponent + 1, 1);
        for (int i = 1;i <= exponent;i++)
            powerTableForScores[i] = getMOD(powerTableForScores[i - 1] * value);
    }

    int sn2(int s, int k) {
        //Stirling number of the second kind
        if (k > s || s == 0 || k == 0) return 0;
        if (k == 1 || s == k) {
            stirlingNo2[s][k] = 1;
            return 1;
        }
        int part1 = getMOD(k * sn2(s - 1, k));
        int part2 = sn2(s - 1, k - 1);
        int val = getMOD(part1 + part2);
        stirlingNo2[s][k] = val;
        return val;
    }

    long nCr(int n, int r) {
        int max_d = max(n - r, r);
        int min_d = min(n - r, r);

        double result = 1.0;
        for (int i = n;i > max_d;i--) {
            result = (result * i / min_d);
            if (min_d > 1) min_d--;
        }
        return getMOD(result);
    }
    long nPr(int n, int r) {
        long result = 1;
        long looptill = n - r;
        for (int i = n;i > looptill;i--)
            result = getMOD(result * i);
        return result;
    }
    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return numberOfWays(nums.mData1[0], nums.mData1[1], nums.mData1[2]);
    }
};


/****************************************************************************/
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
        nums.back().setInput({ 1,2,3 });
        nums.back().setOutput(6);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,2,1 });
        nums.back().setOutput(32);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 3,3,4 });
        nums.back().setOutput(684);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,5,1 });
        nums.back().setOutput(3125);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 76,31,194 });
        nums.back().setOutput(544840908);
    }

    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test:" << testID << " => " << nums[i] << endl;
        gCurTestId = nums[i].mID;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << " => " << nums[i].mID << " returns: \t"
            << result << " vs e:" << nums[i].mExpected1 << endl;
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


