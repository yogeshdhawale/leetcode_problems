
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
typedef vector<string> V1Str;

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

    Solution() {}

    int x(vector<int> nums) {

    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return x(nums.mData1);
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
        nums.back().setInput({ 0, 0, 1, 0, 1,1 });
        nums.back().setOutput(0);
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

vector<string> load_data_v1_string(string fileName);
vector<vector<string>> load_data_v2_string(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "big test" << testID << endl;

    vector<TestCaseStruct> nums;
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            load_data_v1_string(TEST_DIR + "test1" + "-i1.txt"),
            load_data_v2_string(TEST_DIR + "test1" + "-i2.txt")
        );
        nums.back().setOutput(0);
    }

    cout << __LOGPREFIX << "big test for: " << nums.size() << endl;

    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << __LOGPREFIX << "big test: " << testID << "." << nums[i].mID << " ==> " << nums[i] << endl;

        Solution s;

        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "big test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        //assert(result == nums[i].mExpected1);
    }

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


