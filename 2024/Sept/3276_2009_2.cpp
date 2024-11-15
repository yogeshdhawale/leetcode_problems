
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
#include <bitset>

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
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 18);

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
    int maskId;
    int totalScore;
    int currentNumber;
    Data(int maskId, int score, int currentNumber) {
        this->maskId = maskId;
        this->totalScore = score;
        this->currentNumber = currentNumber;
    }
    Data(const Data& d) {
        this->maskId = d.maskId;
        this->totalScore = d.totalScore;
        this->currentNumber = d.currentNumber;
    }
};

class Solution {
public:

    int mFinalMask;
    unordered_map<int, Data> mMaskCache;
    map<VALUEI, map<IDXR, bool> > dataCache;
    queue<int> mNewPath;

    void clear() {
        mFinalMask = 0;
        mMaskCache.clear();
        dataCache.clear();
        mNewPath = queue<int>();
    }

    void set_bit(int& mask, int i) {
        mask |= (1 << i);
    }
    bool is_set(int mask, int i) {
        return (mask & (1 << i)) != 0;
    }
    int maxScore(vector<vector<int>>& grid) {

        clear();
        vector< map<VALUEI, bool>> rowCache;
        map<VALUEI, IDXR> resultVals;
        size_t totalRows = grid.size();
        int rowMask = 0;
        mFinalMask = (1 << totalRows) - 1;
        rowCache.resize(totalRows);

        LOG("total rows: " << totalRows);

        for (size_t i = 0;i < totalRows;i++) {
            for (size_t j = 0;j < grid[i].size();j++) {
                int curVal = grid[i][j];
                rowCache[i][curVal] = true;
                dataCache[curVal][i] = true;
            }

            if (rowCache[i].size() == 1) {
                // only one option in this row
                int curVal = rowCache[i].begin()->first;
                resultVals[curVal] = i;
                set_bit(rowMask, i);
            }
        }

        int curTotal = 0;
        for_each(resultVals.begin(), resultVals.end(), [&](auto& item) {
            dataCache.erase(item.first);
            curTotal += item.first;
            });

        if (dataCache.empty()) return curTotal;
        if (dataCache.size() == 1) return curTotal + dataCache.begin()->first;

        int result = 0;
        // special case where we want to process 1st element, set the Data.currentNumber to highest number + 1
        Data curData(rowMask, curTotal, dataCache.rbegin()->first + 1);
        LOG("At start: mask:" << rowMask << " total:" << curTotal);
        generate_options(curData);

        while (!mNewPath.empty()) {
            int curRowMask = mNewPath.front();
            mNewPath.pop();
            auto itr = mMaskCache.find(curRowMask);
            if (itr == mMaskCache.end()) continue;
            result = max(result, itr->second.totalScore);
            generate_options(itr->second);
        }
        LOG("final result:" << result);
        return result;
    }

    void generate_options(Data& curData) {
        int curRowMask = curData.maskId;
        if (curRowMask == mFinalMask) {
            return;
        }

        auto itr = dataCache.lower_bound(curData.currentNumber);
        if (dataCache.begin() == itr) return;

        if (itr == dataCache.end()) --itr;
        if (itr->first == curData.currentNumber) --itr;

        for (bool found = false;!found && itr != dataCache.end(); --itr) {
            auto curMaxVal = itr->first;
            auto curOptions = itr->second;
            for (auto it = curOptions.begin(); it != curOptions.end(); it++) {
                if (is_set(curRowMask, it->first)) continue;

                found = true;
                auto newMask = curRowMask;
                set_bit(newMask, it->first);
                auto itr = mMaskCache.find(newMask);
                if (itr == mMaskCache.end()) {
                    Data d(newMask, curData.totalScore + curMaxVal, curMaxVal);
                    mMaskCache.insert({ newMask, d });
                    mNewPath.push(newMask);
                    LOG("Generating options for: " << curMaxVal << " mask: " << d.maskId << " score:" << d.totalScore);
                }
                else {
                    if (itr->second.totalScore < curData.totalScore + curMaxVal) {
                        itr->second.totalScore = curData.totalScore + curMaxVal;
                        itr->second.currentNumber = curMaxVal;
                        mNewPath.push(newMask);
                        LOG("updating options for: " << curMaxVal << " mask: " << itr->second.maskId << " score:" << itr->second.totalScore);
                    }
                    else
                        LOG("Skipping options for: " << curMaxVal << " mask: " << itr->second.maskId << " score:" << itr->second.totalScore);
                }
            }
        }
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxScore(nums.mData1);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "input_id:" << ipObj.mID;
    out << " data:" << ipObj.mData1;
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

    nums.push_back(Input({ {1,2,3,4,5} , {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5} }));
    expected.push_back(15);

    nums.push_back(Input({ {5},{7},{19},{5} }));
    expected.push_back(31);

    nums.push_back(Input({ {13,17},{17,17},{13,17} }));
    expected.push_back(30);

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
        Input(load_data_v2(t1i1))
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


