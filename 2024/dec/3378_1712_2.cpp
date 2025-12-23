
/*
3378. Count Connected Components in LCM Graph
Hard
Topics
Companies
Hint

You are given an array of integers nums of size n and a positive integer threshold.

There is a graph consisting of n nodes with the ith node having a value of nums[i]. Two nodes i and j in the graph are connected via an undirected edge if lcm(nums[i], nums[j]) <= threshold.

Return the number of connected components in this graph.

A connected component is a subgraph of a graph in which there exists a path between any two vertices, and no vertex of the subgraph shares an edge with a vertex outside of the subgraph.

The term lcm(a, b) denotes the least common multiple of a and b.



Example 1:

Input: nums = [2,4,8,3,9], threshold = 5

Output: 4

Explanation:



The four connected components are (2, 4), (3), (8), (9).

Example 2:

Input: nums = [2,4,8,3,9,12], threshold = 10

Output: 2

Explanation:

The two connected components are (2, 3, 4, 8, 9), and (12).



Constraints:

    1 <= nums.length <= 105
    1 <= nums[i] <= 109
    All elements of nums are unique.
    1 <= threshold <= 2 * 105


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
#define __DEBUG_BIG_TEST
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
typedef int OUTPUT_TYPE;

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

class Solution {
public:

    map<int, vector<int>> mGroups;

    Solution() {}
    int countComponents(vector<int>& nums, int threshold) {

        mGroups.clear();
        sort(nums.begin(), nums.end());


        int result = 0;
        int end = nums.size() - 1;
        for (; end > 0;end--) {
            if (nums[end] <= threshold) break;
            result++;
        }

        LOG("Numbers greater than threshold " << threshold << " : " << result);

        auto firstEntry = nums[0];
        mGroups[firstEntry] = { firstEntry };
        LOG("Adding new group for " << firstEntry << ", distinct group count : " << mGroups.size());

        for (int i = 1;i <= end; i++) {
            auto curVal = nums[i];
            if (!addToGroup(curVal, threshold)) {
                mGroups[curVal] = { curVal };
                LOG("Adding new group for " << curVal << ", distinct group count : " << mGroups.size());
            }
        }

        log();
        return result + mGroups.size();
    }

    void log()
    {
        LOG("groups count:" << mGroups.size());
        for (auto g : mGroups) {
            if (g.second.size() != 1)
                LOG("{: " << g.first << " : " << g.second.back() << "}_" << g.second.size() << ", ");
        }
    }

    bool addToGroup(int otherVal, int threshold) {

        vector<int> toBeMerged;
        bool addEntry = true;
        for (auto gitr = mGroups.begin(); gitr != mGroups.end(); gitr++) {
            for (auto curVal : gitr->second) {
                double ret = lcm((long long)otherVal, (long long)curVal);
                if (ret > threshold) continue;
                LOG("\t for " << otherVal << " and " << curVal << ", LCM is " << ret << ", to be merged : " << gitr->first);
                if (ret == otherVal) addEntry = false;
                toBeMerged.push_back(gitr->first);
                break;
            }
        }
        if (toBeMerged.size() == 0) return false;

        auto itr = toBeMerged.begin();
        auto& firstList = mGroups[*itr];
        if (addEntry) {

            firstList.push_back(otherVal);
            sort(firstList.begin(), firstList.end());
            //LOG("\tAdding entry in group " << *itr << " for " << otherVal);
        }
        itr++;

        if (itr == toBeMerged.end()) return true;

        LOG("Merging groups for " << otherVal << " : " << toBeMerged);
        for (; itr != toBeMerged.end(); itr++) {
            firstList.insert(firstList.end(), mGroups[*itr].begin(), mGroups[*itr].end());
            mGroups.erase(*itr);
        }
        sort(firstList.begin(), firstList.end());
        return true;
    }
    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return countComponents(nums.mData1, nums.mData2);
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
        nums.back().setInput({ 2,4,8,3,9 }, 5);
        nums.back().setOutput(4);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 2,4,8,3,9,12 }, 10);
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 7,15,18,24 }, 100);
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 7,15,18,24 }, 80);
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 7,15,18,24 }, 70);
        nums.back().setOutput(4);
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
        nums.back().setInput(load_data_v1<int>(t1i1), 111111);
        nums.back().setOutput(1246);
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


