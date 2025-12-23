
/*

Editorial
Editorial
Solutions
Solutions
Submissions
Submissions
Code
Testcase
Test Result
Test Result
3266. Final Array State After K Multiplication Operations II
Hard
Topics
premium lock iconCompanies
Hint

You are given an integer array nums, an integer k, and an integer multiplier.

You need to perform k operations on nums. In each operation:

    Find the minimum value x in nums. If there are multiple occurrences of the minimum value, select the one that appears first.
    Replace the selected minimum value x with x * multiplier.

After the k operations, apply modulo 109 + 7 to every value in nums.

Return an integer array denoting the final state of nums after performing all k operations and then applying the modulo.



Example 1:

Input: nums = [2,1,3,5,6], k = 5, multiplier = 2

Output: [8,4,6,5,6]

Explanation:
Operation	Result
After operation 1	[2, 2, 3, 5, 6]
After operation 2	[4, 2, 3, 5, 6]
After operation 3	[4, 4, 3, 5, 6]
After operation 4	[4, 4, 6, 5, 6]
After operation 5	[8, 4, 6, 5, 6]
After applying modulo	[8, 4, 6, 5, 6]



// power with mod
for num 22, 22 * 22 = 484
with mod 7
    22 -> 1:3
    484 -> 1:69

with mod 5
    22 -> 2:4       -> 2 + 4*5 = 2+20 = 22
    484 -> 4:96     -> 4+ 96*5 = 4+480=484
    nu -> x:y       -> x + y*5
                    -> 5*y + x


with mod 5, num1 -> x:y, num2 -> a:b
    num1 * num2
    = (5*y+x) * (5*b+a)
    = 25*y*b + 5*x*b + 5*a*y + x*a
    = 5*(5*y*b + x*b + a*y) + x*a

for 7 * 8 = 56 with mod 5
    7 -> 2:1
    8 -> 3:1
    56 -> 1:11
    val = 2*3 = 6 % 5 = 1
    mod = 5*1*1 + 1*3 + 2*1 = 10 + 6/5 = 11


for 22 * 22 = 484 with mod 5
    22 -> 2:4
    484 -> 4:96
    val = 2*2 = 4 % 5 = 4
    mod = 5 * 4*4 + 4*2 + 2*4 = 80 + 8 + 8 = 96 + 4/5 = 96

for 22 * 22 = 484 with mod 7
    22 -> 1:3
    484 -> 1:69
    val = 1*1 = 1 % 7 = 1
    mod = 7 * 3*3 + 1*3 + 1*3 = 63 + 3 + 3 = 69 + 1/7 = 69

for 3 * 4 = 12 with mod 5
    3 -> 3:0
    4 -> 4:0
    12 -> 2:2
    val = 3*4 = 12 % 5 = 2
    mod = 5 * 0*0 + 3*0 + 4*0 = 0 + 12/5 = 2

for 22^3 = 10648 with mod 5
    22 -> 2:4
    22 * 22 = 484 -> 4:96
    22 * 22 * 22 = 484 * 22 = 10648 -> 3:2129
    val = 2*2*2 = 8 % 5 = 3
    mod = (4*96)*5 + 96*2 + 4*4 = 1920 + 192 + 16 = 2128 + 8/5 = 2129



---
mu:10^6
2*10^3, 10^6
2*10^27, 10^24

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
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef V1Int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    int mData2;
    int mData3;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1, int input2, int input3) {
        this->mData1 = input1;
        this->mData2 = input2;
        this->mData3 = input3;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/


struct ModRepresentation {
    int index;
    long long value;
    long double modCnt;

    ModRepresentation() : index(INVALID), value(0), modCnt(0) {}
    ModRepresentation(int i, int v) : index(i), value(0), modCnt(0) {
        modCnt = v / RESULT_MOD;
        value = GET_MOD(v);
    }
    /*
    ModRepresentation(const ModRepresentation& other) {
        index = other.index;
        value = other.value;
        modCnt = other.modCnt;
    }

    ModRepresentation& operator=(const ModRepresentation& other) {
        index = other.index;
        value = other.value;
        modCnt = other.modCnt;
        return *this;
    }

    bool operator>(const ModRepresentation& other) const {
        if (modCnt != other.modCnt) {
            return modCnt > other.modCnt;
        }
        if (value != other.value) {
            return value > other.value;
        }
        return index > other.index;
    }*/
    bool operator<(const ModRepresentation& other) const {
        if (modCnt != other.modCnt) {
            return modCnt < other.modCnt;
        }
        if (value != other.value) {
            return value < other.value;
        }
        return index < other.index;
    }
    void multiply(ModRepresentation& multiplier) {
        long long newValue = GET_MOD(value * multiplier.value);
        long double newMod = (long long)modCnt * multiplier.modCnt;
        newMod += (long long)(value * multiplier.modCnt) / RESULT_MOD;
        newMod += (long long)(modCnt * multiplier.value) / RESULT_MOD;
        newMod += (long long)(value * multiplier.value) / RESULT_MOD;

        //LOG("\tmultiply " << value << ":" << modCnt << " * " << multiplier.value << ":" << multiplier.modCnt
        //    << " = " << newValue << ":" << newMod);

        value = newValue;
        modCnt = newMod;
    }

    void getPower(int expectedPower) {

        if (expectedPower == 1) return;

        ModRepresentation result(0, 1);
        ModRepresentation curPower = *this;

        for (;expectedPower != 0;expectedPower >>= 1) {
            if (expectedPower & 1) result.multiply(curPower);
            curPower.multiply(curPower);
        }
        value = result.value;
        modCnt = result.modCnt;
    }
};



class Solution {

public:

    Solution() {

    }

    vector<int> getFinalState(vector<int>& nums, int k, int multiplier) {

        set <ModRepresentation, less<ModRepresentation>> elements; // first will be min element
        int n = nums.size();

        if (n == 0) return nums;

        if (multiplier == 1 || k == 0) {
            for (int i = 0; i < n; i++) {
                nums[i] = GET_MOD((long long int)nums[i]);
            }
            return nums;
        }

        ModRepresentation multNu(0, multiplier);

        if (n == 1) {
            ModRepresentation onlyNum(0, nums[0]);
            auto multNuWithPower = multNu;
            multNuWithPower.getPower(k);
            onlyNum.multiply(multNuWithPower);

            nums[0] = onlyNum.value;
            LOG("Only one num after k ops:" << onlyNum.value << ":" << onlyNum.modCnt);
            return nums;
        }

        LOG("For nums = " << n << " k:" << k << " mul:" << multiplier);

        for (int i = 0; i < n; i++) {
            auto cur = ModRepresentation(i, nums[i]);
            elements.insert(cur);
        }

        for (;k > 0;--k) {

            auto cur = *(elements.begin());
            auto maxNumber = *(elements.rbegin());

            LOG("At k:" << k
                << " cur:@" << cur.index << "=" << cur.value << ":" << cur.modCnt
                << "\tmaxNum:@" << maxNumber.index << "=" << maxNumber.value << ":" << maxNumber.modCnt
            );


            cur.multiply(multNu);

            if (k < n || cur < maxNumber) {
                // can process current element only
                elements.erase(elements.begin());
                elements.insert(cur);
                continue;
            }

            // as cur >= maxNumber, so all elements will be changed in this round
            auto rounds = k / n;
            {
                set<ModRepresentation, less<ModRepresentation>> newSet;
                auto multNuWithPower = multNu;
                multNuWithPower.getPower(rounds);
                LOG("Making " << rounds << " rounds Multiplying each by " << multNuWithPower.value << ":" << multNuWithPower.modCnt);
                for (auto itr = elements.begin(); itr != elements.end(); itr++) {
                    auto item = *itr;
                    item.multiply(multNuWithPower);
                    newSet.insert(item);
                }
                elements.clear();
                elements = newSet;
            }
            k -= n * rounds;
            k++; // as we processed whole nums instead of single element, so add 1 back to offset the --k in for loop
        }

        LOG("At end of k ops");
        for (auto item : elements) {
            nums[item.index] = item.value;
            LOG("@:" << item.index << " val:" << item.value << ":" << item.modCnt);
        }
        return nums;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return getFinalState(nums.mData1, nums.mData2, nums.mData3);
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
        nums.back().setInput({ 2,1,3,5,6 }, 5, 2);
        nums.back().setOutput({ 8, 4, 6, 5, 6 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,2,3,4,5 }, 10, 20);
        nums.back().setOutput({ 400, 800, 1200, 1600, 2000 });
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 100000, 2000 }, 2, 1000000);
        nums.back().setOutput({ 999999307,999999993 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 100000, 2000 }, 7, 1000000);
        nums.back().setOutput({ 4900000,999999321 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1, 2, 3,4,5,6,7,8,9 }, 25, 1000);
        nums.back().setOutput(
            { 1000000000,999999993,999999986,999999979,999999972,999999965,999999958,8000000,9000000 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1, 2, 3,4,5,6,7,8,9 }, 25, 10000);
        nums.back().setOutput(
            { 999993007,999986007,999979007,999972007,999965007,999958007,999951007,800000000,900000000, });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 161209470 }, 56851412, 39846);
        nums.back().setOutput(
            { 198168519 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 66307295, 441787703, 589039035, 322281864 }, 900900704, 641725);
        nums.back().setOutput(
            { 664480092,763599523,886046925,47878852 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 66307295, 441787703, 589039035, 322281864 }, 900900704, 99641725);
        nums.back().setOutput(
            { 259453246, 925904898, 581589761, 743627809 });
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 66307295,441787703,589039035,322281864 },
            900900704, 641725);
        nums.back().setOutput(
            { 664480092,763599523,886046925,47878852 });
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
    /*
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
    */
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


