
/*
3394. Check if Grid can be Cut into Sections
Medium
Topics
Companies
Hint

You are given an integer n representing the dimensions of an n x n grid, with the origin at the bottom-left corner of the grid. You are also given a 2D array of coordinates rectangles, where rectangles[i] is in the form [startx, starty, endx, endy], representing a rectangle on the grid. Each rectangle is defined as follows:

    (startx, starty): The bottom-left corner of the rectangle.
    (endx, endy): The top-right corner of the rectangle.

Note that the rectangles do not overlap. Your task is to determine if it is possible to make either two horizontal or two vertical cuts on the grid such that:

    Each of the three resulting sections formed by the cuts contains at least one rectangle.
    Every rectangle belongs to exactly one section.

Return true if such cuts can be made; otherwise, return false.



Example 1:

Input: n = 5, rectangles = [[1,0,5,2},{0,2,2,4},{3,2,5,3},{0,4,4,5]]

Output: true

Explanation:

The grid is shown in the diagram. We can make horizontal cuts at y = 2 and y = 4. Hence, output is true.

Example 2:

Input: n = 4, rectangles = [[0,0,1,1},{2,0,3,4},{0,2,2,3},{3,0,4,3]]

Output: true

Explanation:

We can make vertical cuts at x = 2 and x = 3. Hence, output is true.

Example 3:

Input: n = 4, rectangles = [[0,2,2,4},{1,0,3,2},{2,2,3,4},{3,0,4,2},{3,2,4,4]]

Output: false

Explanation:

We cannot make two horizontal or two vertical cuts that satisfy the conditions. Hence, output is false.



Constraints:

    3 <= n <= 109
    3 <= rectangles.length <= 105
    0 <= rectangles[i][0] < rectangles[i][2] <= n
    0 <= rectangles[i][1] < rectangles[i][3] <= n
    No two rectangles overlap.


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
typedef V2Int INPUT_TYPE;
typedef bool OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    int mData1;
    INPUT_TYPE mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(int input1, INPUT_TYPE input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    bool operator<(const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

class Solution {

public:
    map<int, int> hOptions;
    map<int, int> vOptions;

    Solution() {}

    bool checkValidCuts(int n, vector<vector<int>>& rectangles) {

        hOptions.clear();
        vOptions.clear();

        for (auto& rect : rectangles) {

            auto bottomLeft = Point(rect[0], rect[1]);
            auto topRight = Point(rect[2], rect[3]);
            AddInHOptions(bottomLeft, topRight);
            AddInVOptions(bottomLeft, topRight);
        }

        Log();

        if (hOptions.size() < 3 && vOptions.size() < 3) {
            LOG("Not enough options");
            return false;
        }
        int cutCnt = 0;
        auto itr_hc = hOptions.begin();
        int hcutPos = itr_hc->second;
        for (;itr_hc != hOptions.end();++itr_hc) {
            if (itr_hc->first < hcutPos) {
                hcutPos = max(hcutPos, itr_hc->second);
                continue;
            }
            cutCnt++;
            LOG("Horizontal Cut at : " << hcutPos);
            hcutPos = itr_hc->second;
            if (cutCnt == 2) break;
        }
        if (cutCnt == 2) return true;

        cutCnt = 0;
        auto itr_vc = vOptions.begin();
        int vcutPos = itr_vc->second;
        for (;itr_vc != vOptions.end();++itr_vc) {
            if (itr_vc->first < vcutPos) {
                vcutPos = max(vcutPos, itr_vc->second);
                continue;
            }
            cutCnt++;
            LOG("Vertical Cut at : " << vcutPos);
            vcutPos = itr_vc->second;
            if (cutCnt == 2) break;
        }
        if (cutCnt == 2) return true;
        return false;
    }

    void Log()
    {
        LOG("HOptions size: " << hOptions.size());
        LOG("map: " << hOptions);
        LOG("VOptions size: " << vOptions.size());
        LOG("map: " << vOptions);
    }

    void AddInHOptions(Point& bottomLeft, Point& topRight) {
        if (hOptions.find(bottomLeft.y) == hOptions.end())
            hOptions[bottomLeft.y] = topRight.y;
        else
            hOptions[bottomLeft.y] = max(hOptions[bottomLeft.y], topRight.y);
    }
    void AddInVOptions(Point& bottomLeft, Point& topRight) {
        if (vOptions.find(bottomLeft.x) == vOptions.end())
            vOptions[bottomLeft.x] = topRight.x;
        else
            vOptions[bottomLeft.x] = max(vOptions[bottomLeft.x], topRight.x);
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return checkValidCuts(nums.mData1, nums.mData2);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " rects:" << ipObj.mData2;
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
        nums.back().setInput(5, { {1, 0, 5, 2},{0, 2, 2, 4},{3, 2, 5, 3},{0, 4, 4, 5} });
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(4, { {0,0,1,1},{2,0,3,4},{0,2,2,3},{3,0,4,3} });
        nums.back().setOutput(true);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(4, { {0,2,2,4},{1,0,3,2},{2,2,3,4},{3,0,4,2},{3,2,4,4} });
        nums.back().setOutput(false);
    }

    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test:" << testID << "." << nums[i].mID << " " << nums[i] << endl;
        gCurTestId = nums[i].mID;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test:" << testID << "." << nums[i].mID << " returns: \t"
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


