
/*
3245. Alternating Groups III
Hard
Topics
Companies
Hint

There are some red and blue tiles arranged circularly. You are given an array of integers colors and a 2D integers array queries.

The color of tile i is represented by colors[i]:

    colors[i] == 0 means that tile i is red.
    colors[i] == 1 means that tile i is blue.

An alternating group is a contiguous subset of tiles in the circle with alternating colors (each tile in the group except the first and last one has a different color from its adjacent tiles in the group).

You have to process queries of two types:

    queries[i] = [1, sizei], determine the count of alternating groups with size sizei.
    queries[i] = [2, indexi, colori], change colors[indexi] to colori.

Return an array answer containing the results of the queries of the first type in order.

Note that since colors represents a circle, the first and the last tiles are considered to be next to each other.



Example 1:

Input: colors = [0,1,1,0,1], queries = [[2,1,0},{1,4]]

Output: [2]

Explanation:

First query:

Change colors[1] to 0.

Second query:

Count of the alternating groups with size 4:

Example 2:

Input: colors = [0,0,1,0,1,1], queries = [[1,3},{2,3,0},{1,5]]

Output: [2,0]

Explanation:

First query:

Count of the alternating groups with size 3:

Second query: colors will not change.

Third query: There is no alternating group with size 5.



Constraints:

    4 <= colors.length <= 5 * 104
    0 <= colors[i] <= 1
    1 <= queries.length <= 5 * 104
    queries[i][0] == 1 or queries[i][0] == 2
    For all i that:
        queries[i][0] == 1: queries[i].length == 2, 3 <= queries[i][1] <= colors.length - 1
        queries[i][0] == 2: queries[i].length == 3, 0 <= queries[i][1] <= colors.length - 1, 0 <= queries[i][2] <= 1


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
typedef vector<int> OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    V1Int mData1;
    INPUT_TYPE mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(V1Int input1, V2Int input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/


enum QUERY_T {
    Q_COUNT = 1,
    Q_UPDATE = 2
};

class ColorList {
    vector<int>& mColors;
    int size;
public:
    ColorList(vector<int>& colors) : mColors(colors), size(colors.size()) {}
    int getSize() {
        return size;
    }
    int getColor(int i) {
        return mColors[i];
    }
    void setColor(int i, int color) {
        mColors[i] = color;
    }
    vector<int>& getList() {
        return mColors;
    }
    int getNIdx(int idx) {
        return (++idx == size) ? 0 : idx;
    }
    int getPIdx(int idx) {
        return (idx + size - 1) % mColors.size();
    }
    int getDistance(int start, int end) {
        return 1 + (end - start + size) % size;
    }
};

class Data {
public:
    ColorList& list;
    unordered_map<int, int> mSeqMap;
    vector<int> mSeqID;
    vector<int> mCount;
    Data(ColorList& l) : list(l) {
        mSeqMap.clear();
        mSeqID.resize(list.getSize(), INVALID);
        mCount.resize(list.getSize() + 1, 0);
        processColors();
    }

    bool isInfiniteSeq() {
        if (mSeqMap.size() != 1) return false;
        if (mCount.back() != 1) return false;

        int start = mSeqMap.begin()->first;
        int end = mSeqMap.begin()->second;
        if (start != list.getNIdx(end)) return false;
        if (list.getColor(start) == list.getColor(end)) return false;
        return true;
    }
    void removeSeqFromMap(int key) {
        //if (mSeqID.find(mSeqID[key]) == mSeqID.end()) return;
        int start = mSeqID[key];
        int end = mSeqMap[key];
        for (int i = start; i != end; i = list.getNIdx(i)) {
            mSeqID[i] = INVALID;
        }
        mSeqID[end] = INVALID;
        mSeqMap.erase(start);
        mCount[list.getDistance(start, end)]--;
    }
    void addSeqToMap(int start, int end) {
        if (start == end) {
            if (mSeqID[start] != INVALID) removeSeqFromMap(start);
            mSeqID[start] = INVALID;
            return;
        }
        int id = start;
        for (int i = start; i != end; i = list.getNIdx(i)) {
            if (mSeqID[i] != INVALID) removeSeqFromMap(i);
            mSeqID[i] = id;
        }
        if (mSeqID[end] != INVALID) removeSeqFromMap(end);
        mSeqID[end] = id;

        mSeqMap[start] = end;
        mCount[list.getDistance(start, end)]++;
    }
    void processColors() {

        auto prevIdx = 0;
        if (list.getColor(0) == list.getColor(1)) prevIdx = 1;
        else
        {
            int startSeq = 0;
            prevIdx = list.getNIdx(startSeq);
            int curIdx = list.getNIdx(prevIdx);

            for (;list.getColor(curIdx) != list.getColor(prevIdx);curIdx = list.getNIdx(curIdx)) {
                if (curIdx == startSeq) {
                    addSeqToMap(0, list.getPIdx(0));
                    return;
                }
                prevIdx = curIdx;
            }
            prevIdx = curIdx;
        }

        auto startSeq = prevIdx;
        auto prevColor = list.getColor(prevIdx);
        for (int curIdx = list.getNIdx(prevIdx), cnt = 1; cnt < list.getSize(); curIdx = list.getNIdx(curIdx), cnt++) {
            auto curColor = list.getColor(curIdx);
            if (prevColor == curColor) {
                if (startSeq != prevIdx) addSeqToMap(startSeq, prevIdx);
                startSeq = curIdx;
            }
            prevColor = curColor;
            prevIdx = curIdx;
        }
        if (startSeq != prevIdx) addSeqToMap(startSeq, prevIdx);
    }
    int extendSeqAtDirection(int prevIdx, bool direction = true) {
        auto EndIdx = prevIdx;
        auto prevColor = list.getColor(prevIdx);
        auto incrementor = direction ? &ColorList::getNIdx : &ColorList::getPIdx;
        auto curIdx = (list.*incrementor)(prevIdx);

        for (;mSeqID[curIdx] == INVALID; curIdx = (list.*incrementor)(prevIdx))
        {
            auto curColor = list.getColor(curIdx);

            if (EndIdx == curIdx) return prevIdx;
            if (curColor == prevColor) break;

            prevIdx = curIdx;
            prevColor = curColor;
        }
        return prevIdx;
    }

    void newSeq(int start) {
        if (mSeqID[start] != INVALID) return;

        auto retIdx = extendSeqAtDirection(start);
        if (retIdx == INVALID || start == retIdx) return;

        addSeqToMap(start, retIdx);
        merge(start);
    }
    void extendSeqAppend(int idx) {
        auto start = mSeqID[idx];
        auto end = mSeqMap[start];

        auto retIdx = extendSeqAtDirection(end);
        if (retIdx == INVALID || end == retIdx) return;

        removeSeqFromMap(start);
        addSeqToMap(start, retIdx);
        merge(start);
    }
    void extendSeqPrepend(int idx) {
        auto start = mSeqID[idx];
        auto end = mSeqMap[start];

        auto retIdx = extendSeqAtDirection(start, false);
        if (retIdx == INVALID || start == retIdx) return;

        removeSeqFromMap(start);
        addSeqToMap(retIdx, end);

        auto prevIdx = list.getPIdx(retIdx);
        if (mSeqID[prevIdx] == INVALID) return;
        if (mSeqID[prevIdx] == mSeqID[retIdx]) return;
        merge(prevIdx);
    }

    void merge(int idx) {
        int start = mSeqID[idx];
        int end = mSeqMap[start];
        int next = list.getNIdx(end);
        if (mSeqID[next] == INVALID) return;

        if (list.getColor(end) == list.getColor(next)) return;
        auto newend = mSeqMap[next];
        removeSeqFromMap(next);
        addSeqToMap(start, newend);
    }
    void updateColor(int index, int color) {
        list.setColor(index, color);
        int prevIdx = list.getPIdx(index);
        int nextIdx = list.getNIdx(index);

        if (mSeqID[index] == INVALID) {
            if (mSeqID[prevIdx] == mSeqID[nextIdx]) {
                if (mSeqID[prevIdx] == INVALID) newSeq(prevIdx);
                else {
                    extendSeqAppend(prevIdx);
                    extendSeqPrepend(nextIdx);
                }
            }
            else {
                if (mSeqID[prevIdx] != INVALID) extendSeqAppend(prevIdx);
                if (mSeqID[nextIdx] != INVALID) extendSeqPrepend(nextIdx);
            }
            return;
        }

        auto start = mSeqID[index];
        if (mSeqID[prevIdx] == mSeqID[nextIdx] && mSeqID[prevIdx] == start) {
            auto end = mSeqMap[start];
            if (start == index) {
                // index starts sequence
                removeSeqFromMap(start);
                addSeqToMap(nextIdx, end);
                extendSeqAppend(mSeqID[prevIdx]);
            }
            else if (index == end) {
                // index ends sequence
                removeSeqFromMap(start);
                addSeqToMap(start, prevIdx);
                extendSeqPrepend(nextIdx);
            }
            else {
                if (isInfiniteSeq()) {
                    removeSeqFromMap(start);
                    addSeqToMap(nextIdx, prevIdx);
                    return;
                }
                removeSeqFromMap(start);
                addSeqToMap(start, prevIdx);
                addSeqToMap(nextIdx, end);
            }
            return;
        }

        if (mSeqID[prevIdx] == mSeqID[index]) {
            removeSeqFromMap(start);
            addSeqToMap(start, prevIdx);
            if (mSeqID[nextIdx] == INVALID)
                newSeq(index);
            else
                extendSeqPrepend(nextIdx);
        }
        else if (mSeqID[nextIdx] == mSeqID[index]) {
            auto end = mSeqMap[start];
            removeSeqFromMap(start);
            if (start != index) addSeqToMap(start, prevIdx);
            if (end != index) addSeqToMap(nextIdx, end);

            if (mSeqID[prevIdx] == INVALID)
                newSeq(prevIdx);
            else
                extendSeqAppend(mSeqID[prevIdx]);
        }
    }
};
class Solution {
public:

    vector<int> numberOfAlternatingGroups(vector<int>& colors, vector<vector<int>>& queries) {
        OUTPUT_TYPE result;

        ColorList l(colors);
        Data data(l);
        for (auto q : queries) {
            LOG("list is:" << l.getList());
            LOG("\tmap is:" << data.mSeqMap);
            LOG("\tseq id is:" << data.mSeqID);
            switch (q[0])
            {
            case Q_COUNT: {
                LOG("count query:" << q);
                auto val = getCount(data, q[1]);
                result.push_back(val);
            }
                        break;

            case Q_UPDATE: {
                LOG("updating color for q:" << q);
                updateColor(data, q[1], q[2]);
            }
                         break;
            }
        }
        return result;
    }

    int getCount(Data& data, int minSize) {
        LOG("\tmCount:" << data.mCount);

        if (data.isInfiniteSeq())
            return data.list.getSize();

        int result = 0;
        int cnt = 1;
        for (int i = minSize;i < (int)data.mCount.size();i++, cnt++) {
            if (data.mCount[i] <= 0) continue;
            result += data.mCount[i] * cnt;
        }
        return result;
    }

    void updateColor(Data& data, int index, int color) {
        if (data.list.getColor(index) == color) {
            LOG("No update required");
            return;
        }
        data.updateColor(index, color);
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return numberOfAlternatingGroups(nums.mData1, nums.mData2);
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
        nums.back().setInput(
            { 0, 0, 1, 0, 1,1 },
            { {1,3},{2,3,0},{1,5} }
        );
        nums.back().setOutput({ 2,0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0, 1, 1, 0, 1 },
            { {2,1,0},{1,4} }
        );
        nums.back().setOutput({ 2 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0, 0, 0, 1 },
            { {2, 1, 1}, {1, 3}, {2, 1, 1}, {2, 0, 1} }
        );
        nums.back().setOutput({ 4 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0, 1, 0, 1 },
            { {1, 3}, {2, 2, 1}, {1, 3}, {1, 3} }
        );
        nums.back().setOutput({ 4,1,1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1, 1, 0, 1 },
            { {1, 3}, {2,0,0}, {1,3} }
        );
        nums.back().setOutput({ 1, 4 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0, 0, 1,0 },
            { {1, 3}, {2,3,0}, {2,1,1},{2,2,0},{2,0,1},{1,3}, }
        );
        nums.back().setOutput({ 1, 0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1, 1, 1,0 },
            { {2,3,1}, {1,3}, {1,3},{2,0,1},{1,3}, { 2,3,0 }, {1,3} }
        );
        nums.back().setOutput({ 0, 0,0,1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,0,0,1 },
            { {2, 0, 1}, {2, 2, 0}, {1, 3}, {1, 3} }
        );
        nums.back().setOutput({ 0, 0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1,0,1,0 },
            { {2, 3, 1}, {1, 3}, {1, 3} }
        );
        nums.back().setOutput({ 1,1, });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,0,1,1 },
            { { 2,2,1},{2,0,1},{1,3},{2,1,0} }
        );
        nums.back().setOutput({ 0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,1,0 },
            { { 2,1,1},{1,3},{2,3,1},{2,0,1},{2,3,0 } }
        );
        nums.back().setOutput({ 0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,0,0 },
            { {2, 2, 1},{2, 0, 1},{1, 3},{1, 3},{2, 0, 1},{2, 2, 1} }
        );
        nums.back().setOutput({ 1,1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,0,1 },
            { {2, 1, 0},{2, 2, 1},{1, 3},{2, 0, 1},{1,3},{2, 3, 0} }
        );
        nums.back().setOutput({ 0,1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,0,1,1,0,0 },
            { {1, 4},{1, 5},{2, 0, 1},{1, 3},{2, 3, 0} });
        nums.back().setOutput({ 0,0,2 });
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


