
/*
Q3. Subrectangle Queries
Medium
Topics
premium lock iconCompanies
Hint

Implement the class SubrectangleQueries which receives a rows x cols rectangle as a matrix of integers in the constructor and supports two methods:

1. updateSubrectangle(int row1, int col1, int row2, int col2, int newValue)

    Updates all values with newValue in the subrectangle whose upper left coordinate is (row1,col1) and bottom right coordinate is (row2,col2).

2. getValue(int row, int col)

    Returns the current value of the coordinate (row,col) from the rectangle.



Example 1:

Input
["SubrectangleQueries","getValue","updateSubrectangle","getValue","getValue","updateSubrectangle","getValue","getValue"]
[[[[1,2,1],[4,3,4],[3,2,1],[1,1,1]]],[0,2],[0,0,3,2,5],[0,2],[3,1],[3,0,3,2,10],[3,1],[0,2]]
Output
[null,1,null,5,5,null,10,5]
Explanation
SubrectangleQueries subrectangleQueries = new SubrectangleQueries([[1,2,1],[4,3,4],[3,2,1],[1,1,1]]);
// The initial rectangle (4x3) looks like:
// 1 2 1
// 4 3 4
// 3 2 1
// 1 1 1
subrectangleQueries.getValue(0, 2); // return 1
subrectangleQueries.updateSubrectangle(0, 0, 3, 2, 5);
// After this update the rectangle looks like:
// 5 5 5
// 5 5 5
// 5 5 5
// 5 5 5
subrectangleQueries.getValue(0, 2); // return 5
subrectangleQueries.getValue(3, 1); // return 5
subrectangleQueries.updateSubrectangle(3, 0, 3, 2, 10);
// After this update the rectangle looks like:
// 5   5   5
// 5   5   5
// 5   5   5
// 10  10  10
subrectangleQueries.getValue(3, 1); // return 10
subrectangleQueries.getValue(0, 2); // return 5

Example 2:

Input
["SubrectangleQueries","getValue","updateSubrectangle","getValue","getValue","updateSubrectangle","getValue"]
[[[[1,1,1],[2,2,2],[3,3,3]]],[0,0],[0,0,2,2,100],[0,0],[2,2],[1,1,2,2,20],[2,2]]
Output
[null,1,null,100,100,null,20]
Explanation
SubrectangleQueries subrectangleQueries = new SubrectangleQueries([[1,1,1],[2,2,2],[3,3,3]]);
subrectangleQueries.getValue(0, 0); // return 1
subrectangleQueries.updateSubrectangle(0, 0, 2, 2, 100);
subrectangleQueries.getValue(0, 0); // return 100
subrectangleQueries.getValue(2, 2); // return 100
subrectangleQueries.updateSubrectangle(1, 1, 2, 2, 20);
subrectangleQueries.getValue(2, 2); // return 20



Constraints:

    There will be at most 500 operations considering both methods: updateSubrectangle and getValue.
    1 <= rows, cols <= 100
    rows == rectangle.length
    cols == rectangle[i].length
    0 <= row1 <= row2 < rows
    0 <= col1 <= col2 < cols
    1 <= newValue, rectangle[i][j] <= 10^9
    0 <= row < rows
    0 <= col < cols


    */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include<list>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

using namespace std;

#define __DEBUG_LOG __DEBUG_LOG_ON
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
#include <unordered_set>
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
typedef vector<long long > V1LL;

typedef vector<string> V1Str;
typedef vector<V1Str> V2Str;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef vector<int> OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    V1Str mData1;
    V2Int mData2;
    V2Int mData3;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(V1Str input1, V2Int input2, V2Int input3) {
        this->mData1 = input1;
        this->mData2 = input2;
        this->mData3 = input3;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

class SubrectangleQueries {
public:

    vector<vector<int>>& data;
    SubrectangleQueries(vector<vector<int>>& rectangle) : data(rectangle) {

    }

    void updateSubrectangle(int row1, int col1, int row2, int col2, int newValue) {
        for (int i = row1;i <= row2;i++) {
            for (int j = col1;j <= col2;j++) {
                data[i][j] = newValue;
            }
        }
    }

    int getValue(int row, int col) {
        return data[row][col];
    }
};

/**
 * Your SubrectangleQueries object will be instantiated and called as such:
 * SubrectangleQueries* obj = new SubrectangleQueries(rectangle);
 * obj->updateSubrectangle(row1,col1,row2,col2,newValue);
 * int param_2 = obj->getValue(row,col);
 */

class Solution {
public:
    OUTPUT_TYPE proxyFunction(TestCaseStruct& tc) {


        SubrectangleQueries* obj = nullptr;
        OUTPUT_TYPE result;

        obj = new SubrectangleQueries(tc.mData2);

        for (size_t i = 1; i < tc.mData1.size(); i++) {

            string cmd = tc.mData1[i];
            LOG("\t" << cmd << " with data:" << tc.mData3[i - 1]);

            if (cmd == "updateSubrectangle") {
                obj->updateSubrectangle(tc.mData3[i - 1][0], tc.mData3[i - 1][1], tc.mData3[i - 1][2], tc.mData3[i - 1][3], tc.mData3[i - 1][4]);
            }
            if (cmd == "getValue") {
                auto ret = obj->getValue(tc.mData3[i - 1][0], tc.mData3[i - 1][1]);
                result.push_back(ret);
                LOG("\treturns:" << ret << endl);
            }
            else {
                cout << "Unknown command:" << cmd << endl;
            }
        }
        return result;
    }
};

/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " data2:" << ipObj.mData2;
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
        nums.back().setInput(
            { "SubrectangleQueries","getValue","updateSubrectangle","getValue","getValue","updateSubrectangle","getValue" },
            { { 1, 1, 1 }, { 2,2,2 }, { 3,3,3 } },
            { { 0,0 }, { 0,0,2,2,100 }, { 0,0 }, { 2,2 }, { 1,1,2,2,20 }, { 2,2 } }
        );
        nums.back().setOutput({ 1, 100,100,20 });
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
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        Solution s;
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
    vector<string> testcases_str = { "test1" };

    for (auto& tc : testcases_str)
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            load_data_v1_string(TEST_DIR + tc + "-i1.txt"),
            load_data_v2(TEST_DIR + tc + "-i2.txt")
        );
        nums.back().setOutput({ 0 });
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


