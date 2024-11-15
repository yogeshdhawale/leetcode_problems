
/*
3276. Select Cells in Grid With Maximum Score
Hard
Topics
Companies
Hint

You are given a 2D matrix grid consisting of positive integers.

You have to select one or more cells from the matrix such that the following conditions are satisfied:

    No two selected cells are in the same row of the matrix.
    The values in the set of selected cells are unique.

Your score will be the sum of the values of the selected cells.

Return the maximum score you can achieve.



Example 1:

Input: grid = [[1,2,3],[4,3,2],[1,1,1]]

Output: 8

Explanation:

We can select the cells with values 1, 3, and 4 that are colored above.

Example 2:

Input: grid = [[8,7,6],[8,3,2]]

Output: 15

Explanation:

We can select the cells with values 7 and 8 that are colored above.



Constraints:

    1 <= grid.length, grid[i].length <= 10
    1 <= grid[i][j] <= 100


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

/****************************************************************************/
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
//TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
TestConfig gTestConfig(TestOptions::TEST_SINGLE, 15);

using namespace std;

/****************************************************************************/

#include <ranges>

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

typedef V2Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    INPUT_TYPE mData1;

    Input() {
        mID = inputID++;
    }
    Input(INPUT_TYPE d1) {
        this->mData1 = d1;
        mID = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data;
ostream& operator<<(ostream& out, const Data& d);

struct Data {

};

class Solution {
public:

    map<VALUEI, map<IDXR, bool> > mDataCache;
    //multimap<VALUEI, IDXR> mDataCache;
    vector< map<VALUEI, bool>> mRowCache;
    map<VALUEI, IDXR> mResultVal;

    void clear() {
        mDataCache.clear();
        mRowCache.clear();
        mResultVal.clear();
    }
    int maxScore(vector<vector<int>>& grid) {

        clear();

        size_t totalRows = grid.size();
        mRowCache.resize(totalRows);

        for (size_t i = 0;i < totalRows;i++) {
            for (size_t j = 0;j < grid[i].size();j++) {
                int curVal = grid[i][j];
                mRowCache[i][curVal] = true;
                mDataCache[curVal][i] = true;
            }
            if (mRowCache[i].size() == 1) {
                // only one option
                mResultVal.insert({ mRowCache[i].begin()->first, i });
            }
        }

        for_each(mResultVal.begin(), mResultVal.end(), [&](auto& item) {
            clearOptionFrom(item.first, item.second);
            });

        LOG("tolal rows: " << totalRows << " start result:" << mResultVal);

        while (mResultVal.size() < totalRows) {

            handlOnlyOptionRows(totalRows);
            if (mDataCache.size() == 0) break;

            auto itr = mDataCache.rbegin();
            auto maxVal = itr->first;
            auto maxIdxR = getOptimalRowId(maxVal);
            if (maxIdxR == INVALID) {
                mDataCache.erase(maxVal);
                continue;
            }
            LOG("found option row:" << maxIdxR << " with value:" << maxVal);
            mResultVal[maxVal] = maxIdxR;
            clearOptionFrom(maxVal, maxIdxR);
        }
        LOG("At End datacache:" << mDataCache);
        LOG("final result:" << mResultVal);

        int retVal = accumulate(mResultVal.begin(), mResultVal.end(), 0,
            [](int a, pair<VALUEI, bool> b) {return a + b.first;});

        return retVal;
    }

    void clearOptionFrom(VALUEI val, IDXR rowId) {
        mDataCache.erase(val);
        if (rowId != INVALID) {
            mRowCache[rowId].clear();
            for_each(mDataCache.begin(), mDataCache.end(), [&](auto& item) {item.second.erase(rowId);});
        }
        for_each(mRowCache.begin(), mRowCache.end(), [&](auto& item) {item.erase(val);});
    }

    IDXR getOptimalRowId(VALUEI val, vector<IDXR>& rowOptions, size_t depth) {
        if (rowOptions.size() == 0) return INVALID;
        if (rowOptions.size() == 1) return rowOptions[0];

        map<VALUEI, vector<IDXR>> secondOptions;
        for (auto itr = rowOptions.begin(); itr != rowOptions.end(); ++itr) {
            auto curRowId = *itr;
            if (mRowCache[curRowId].size() <= depth) return curRowId;
            auto secondVal = next(mRowCache[curRowId].rbegin(), depth)->first;
            secondOptions[secondVal].push_back(curRowId);
        }
        if (secondOptions.size() == 0) return rowOptions[0];
        if (secondOptions.begin()->second.size() == 1) {
            // best option found, but wait, do we need to check if rows will be fully used.
            return secondOptions.begin()->second[0];
        }
        LOG("for val:" << val << " depth:" << depth << " second options:" << secondOptions);
        return getOptimalRowId(val, secondOptions.begin()->second, depth + 1);
    }

    IDXR getOptimalRowId(VALUEI val) {

        //transform(mDataCache[val].begin(), mDataCache[val].end(), back_inserter(rowOptions),
        //    [](pair<IDXR, bool> p) {return p.first;});
        auto ks = std::views::keys(mDataCache[val]);
        vector<IDXR> rowOptions{ ks.begin(), ks.end() };

        //vector<IDXR> rowOptions(from_range, views::keys(mDataCache[val]));

        return getOptimalRowId(val, rowOptions, 1);
    }

    void handlOnlyOptionRows(size_t totalRows) {
        if (mDataCache.size() == 0 || mResultVal.size() == totalRows) return;

        for (size_t i = 0;i < (totalRows - mResultVal.size() - 1);) {
            auto itr = next(mDataCache.rbegin(), i);
            if (itr == mDataCache.rend()) break;

            int curMax = itr->first;
            if (itr->second.size() == 0) {
                clearOptionFrom(curMax, INVALID);
            }
            else if (itr->second.size() == 1) {
                auto curIdxR = itr->second.begin()->first;
                LOG("found alone @" << i << " row: " << curIdxR << " with value:" << curMax);
                mResultVal[curMax] = curIdxR;
                clearOptionFrom(curMax, curIdxR);
                i = 0;
            }
            else i++;
        }
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxScore(nums.mData1);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    return out;
}

/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int GenerateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ {1,2,3}, {4,3,2}, {1,1,1} }));
    expected.push_back(8);

    nums.push_back(Input({ {8,7,6}, {8,3,2} }));
    expected.push_back(15);

    nums.push_back(Input({ {18,7,13}, {13,18,7} }));
    expected.push_back(31);

    nums.push_back(Input({ {18,7,13}, {7,13,18}, {13,18,7} }));
    expected.push_back(38);

    nums.push_back(Input({ {5,4,3}, {1,1,1}, {1,1,1} }));
    expected.push_back(6);

    nums.push_back(Input({ {8, 11, 3},{17, 7, 3},{13, 20, 3},{3, 17, 20} }));
    expected.push_back(61);

    nums.push_back(Input({ {8, 11, 3},{17, 7, 3},{13, 20, 3},{3, 17, 20},{1,1,1} }));
    expected.push_back(62);

    nums.push_back(Input({ {19, 18 }, { 1, 14 }, { 4, 14 } }));
    expected.push_back(37);

    nums.push_back(Input({ {8, 2},{7, 19},{4, 8},{6, 20} }));
    expected.push_back(51);

    nums.push_back(Input({ {16,14}, {5,4}, {7,16} }));
    expected.push_back(35);

    nums.push_back(Input({ {16,14}, {5,4}, {7,16}, {16,3} }));
    expected.push_back(42);

    nums.push_back(Input({ {16,14}, {5,4}, {7,16}, {16,8} }));
    expected.push_back(43);

    nums.push_back(Input({ {17, 19, 18},{2, 19, 18},{2, 11, 11},{17, 9, 13} }));
    expected.push_back(65);

    nums.push_back(Input({ {5, 19, 18},{2, 19, 18},{7, 19, 18},{10, 9, 13} }));
    expected.push_back(57);

    nums.push_back(Input({ {13,14,13},{14,13,14},{13,10,7} }));
    expected.push_back(37);

    nums.push_back(Input({ {18,15,18} , { 18,15,18 },{3,20,19},{18,5,6} }));
    expected.push_back(59);


    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test loop_id: " << i << " ==>\t" << result << " vs e:" << expected[i] << endl;
        logtime("Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#ifdef __DEBUG_BIG_TEST

V1Int load_data_v1(string fileName);
V2Int load_data_v2(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    nums.push_back(
        Input(load_data_v2(t1i2))
    );
    expected = load_data_v1(t1e1);

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("*****Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "***Start***" << endl;
    test1(1);
#ifdef __DEBUG_BIG_TEST
    test2(2);
#endif
    cout << "***END***" << endl;
    return 0;
}


