
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

Input: colors = [0,1,1,0,1], queries = [[2,1,0],[1,4]]

Output: [2]

Explanation:

First query:

Change colors[1] to 0.

Second query:

Count of the alternating groups with size 4:

Example 2:

Input: colors = [0,0,1,0,1,1], queries = [[1,3],[2,3,0],[1,5]]

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

enum COLOR {
    RED = 0,
    BLU = 1
};
enum QUERY_T {
    Q_COUNT = 1,
    Q_UPDATE = 2
};

class ColorList {
private:
    vector<IDX>& mColors;
    int mCLSize;

public:
    vector<int> mSeqFlag;
    ColorList(vector<IDX>& colors) : mColors(colors) {
        mCLSize = mColors.size();
        mSeqFlag.resize(mCLSize, INVALID);
    }

    inline vector<IDX>& getColors() {
        return mColors;
    }
    inline int getSize() {
        return mCLSize;
    }
    inline int getColor(IDX idx) {
        return mColors[idx];
    }
    inline void set(IDX idx, int val) {
        mColors[idx] = val;
    }
    inline int getNIdx(IDX idx) {
        if (idx == mCLSize - 1) return 0;
        return idx + 1;
    }
    inline int getPIdx(IDX idx) {
        if (idx == 0) return mCLSize - 1;
        return idx - 1;
    }
    inline int getDistance(IDX idx1, IDX idx2) {
        if (idx1 == idx2) return 0;
        if (idx1 < idx2) return idx2 - idx1 + 1;
        return mCLSize - idx1 + idx2 + 1;
    }
    void updateSeqFlagForRange(IDX start, IDX end, int flag) {
        for (int i = start; i != end; i = getNIdx(i))
            mSeqFlag[i] = flag;
        mSeqFlag[end] = flag;
    }
    void markInvalid(IDX start) {
        updateSeqFlagForRange(start, start, INVALID);
    }
    void markInvalid(IDX start, IDX end) {
        updateSeqFlagForRange(start, end, INVALID);
    }

};

class Solution {
public:

    // stores list of valid sequences of alt colors 
    map<IDX, IDX> mSequnceData;
    vector<int> numberOfAlternatingGroups(vector<int>& colors, vector<vector<int>>& queries) {
        OUTPUT_TYPE result;

        ColorList clist(colors);
        mSequnceData.clear();

        calculateSeqAtBeg(clist);

        for (auto q : queries) {

            LOG("Sequnce is: " << clist.getColors());
            LOG("mSequenceData:" << mSequnceData);
            switch (q[0])
            {
            case Q_COUNT: {
                LOG("count query:" << q);
                auto val = getCount(clist, q[1]);
                result.push_back(val);
            }
                        break;

            case Q_UPDATE: {
                LOG("updating color for q:" << q);
                updateColor(clist, q[1], q[2]);
            }
                         break;
            }
        }
        return result;
    }

    void addSqMpEntry(ColorList& clist, int key, int value) {
        //if (key == value) return;
        mSequnceData[key] = value;
        clist.updateSeqFlagForRange(key, value, key);
    }

    void calculateSeqAtBeg(ColorList& clist) {
        int size = clist.getSize();
        int i = 0;
        auto prevColor = clist.getColor(i);
        i = clist.getNIdx(i);
        int cnt = 1;
        for (;i < size; i++) {
            if (clist.getColor(i) == prevColor) break;
            prevColor = clist.getColor(i);
            cnt++;
        }

        // create sequence entries
        if (cnt == size)
            addSqMpEntry(clist, 1, 0);
        else
            calculateSeq(clist, i, clist.getPIdx(i));
    }

    /**
     * Calculates sequences of alternating colors in the given range of a ColorList.
     *
     * This function identifies contiguous sequences of alternating colors starting
     * from the given `start` index up to the `end` index in the provided `ColorList`.
     * Each sequence is represented as a pair of indices marking its start and end.
     * The function updates the sequence flags within the ColorList for each identified
     * sequence and returns a map where the keys are the starting indices of the sequences
     * and the values are the ending indices.
     *
     * @param clist
     * @param start starting index from which to begin the sequence search.
     * @param end ending index up to which to find the sequences.
     * @return A map where each key is the starting index of a sequence and each value
     *         is the corresponding ending index of that sequence.
     */
    void calculateSeq(ColorList& clist, IDX start, IDX end) {

        const auto endIdx = clist.getNIdx(end);

        auto prevIdx = start;
        auto prevColor = clist.getColor(prevIdx);
        auto patternStart = prevIdx;;

        for (int i = clist.getNIdx(prevIdx); i != endIdx; i = clist.getNIdx(i)) {
            auto curColor = clist.getColor(i);

            if (prevColor == curColor) {
                if (patternStart != prevIdx) addSqMpEntry(clist, patternStart, prevIdx);
                patternStart = i;
            }
            prevIdx = i;
            prevColor = curColor;
        }

        if (patternStart != end) addSqMpEntry(clist, patternStart, end);
    }


    int getCount(ColorList& clist, int size) {
        int result = 0;
        auto it = mSequnceData.begin();

        if (mSequnceData.size() == 1 &&
            it->first == clist.getNIdx(it->second) &&
            clist.getColor(it->first) != clist.getColor(it->second)) {
            return clist.getSize();
        }

        for (; it != mSequnceData.end(); it++) {
            auto distance = clist.getDistance(it->first, it->second);
            if (distance < size) continue;
            result += 1 + (distance - size);
        }
        return result;
    }

    /**
     * Updates the color of a given index in the ColorList and recalculates the valid sequenses.
     *
     * if updated color is same do nothing.
     *
     * If the current index is part of a sequence, it removes invalid sequenses and/or updates and/or adds new sequenses.
     * If current index is not part of sequense it tries to merge/extend existing sequenses nearby to input.
     *
     * @param clist The ColorList object to be updated.
     * @param index The index of the element to be updated.
     * @param color The new color to be assigned to the element.
     */
    void updateColor(ColorList& clist, int index, int color) {

        auto existingColor = clist.getColor(index);
        auto newColor = color;
        if (existingColor == newColor) {
            LOG("No update needed");
            return;
        }

        clist.set(index, newColor);
        auto prevIdx = clist.getPIdx(index);
        auto nextIdx = clist.getNIdx(index);

        if (clist.mSeqFlag[index] == INVALID)
            updateColorForInvalidIdx(clist, index, prevIdx, nextIdx);
        else
            updateColorForValidIdx(clist, index, prevIdx, nextIdx);
    }

    void updateColorForValidIdx(ColorList& clist, int& index, int prevIdx, int nextIdx)
    {
        // index is part of sequence, i.e. not INVALID

        if (clist.mSeqFlag[index] == index) {
            // at start of sequence
            auto itr = mSequnceData.find(index);
            auto newstart = clist.getNIdx(itr->first);
            const auto newend = itr->second;
            mSequnceData.erase(itr);
            clist.markInvalid(index);

            if (newstart == newend) {
                clist.markInvalid(newstart);
                auto itr = mSequnceData.lower_bound(newstart);
                if (itr == mSequnceData.end()) itr = mSequnceData.begin();
                extendAtStart(clist, itr->first);
            }
            else {
                addSqMpEntry(clist, newstart, newend);
            }

            if (clist.mSeqFlag[prevIdx] != INVALID)
                extendAtEnd(clist, clist.mSeqFlag[prevIdx]);
            else
                calculateSeq(clist, prevIdx, index);

            return;
        }

        auto prevSeqItr = mSequnceData.find(clist.mSeqFlag[index]);

        if (index == prevSeqItr->second) {
            // at end of sequence
            clist.markInvalid(index);
            prevSeqItr->second = prevIdx;
            if (prevSeqItr->first == prevSeqItr->second)
                mSequnceData.erase(prevSeqItr);

            if (clist.mSeqFlag[nextIdx] != INVALID)
                extendAtStart(clist, clist.mSeqFlag[nextIdx]);
            else
                calculateSeq(clist, index, nextIdx);

            return;
        }

        if (clist.mSeqFlag[prevIdx] == clist.mSeqFlag[nextIdx]) {
            // break sequence

            clist.markInvalid(index);
            auto e1Start = prevSeqItr->first;
            auto e1End = prevIdx;
            auto e2Start = nextIdx;
            auto e2End = prevSeqItr->second;

            mSequnceData.erase(prevSeqItr);

            auto d1 = clist.getDistance(e1Start, e1End);
            auto d2 = clist.getDistance(e2Start, e2End);
            if (d1 >= d2)
                splitProcessing1(clist, e1Start, e1End, e2Start, e2End);
            else
                splitProcessing2(clist, e2Start, e2End, e1Start, e1End);
            return;
        }
        LOG("\n\nDone update, is any condition missing?");
    }

    void updateColorForInvalidIdx(ColorList& clist, int index, int prevIdx, int nextIdx)
    {
        // index not part of sequence
        if (clist.mSeqFlag[prevIdx] == INVALID && clist.mSeqFlag[nextIdx] == INVALID) {
            calculateSeq(clist, prevIdx, nextIdx);
        }
        else if (clist.mSeqFlag[prevIdx] != INVALID && clist.mSeqFlag[nextIdx] != INVALID) {

            // merge two sequences
            if (clist.mSeqFlag[prevIdx] == clist.mSeqFlag[nextIdx]) {
                // its actually 1 sequence only
                auto itr = mSequnceData.find(clist.mSeqFlag[nextIdx]);
                itr->second = clist.getNIdx(itr->second);
                clist.mSeqFlag[index] = itr->first;
            }
            else {
                auto prevItr = mSequnceData.find(clist.mSeqFlag[prevIdx]);
                auto nextItr = mSequnceData.find(clist.mSeqFlag[nextIdx]);
                prevItr->second = nextItr->second;
                mSequnceData.erase(nextItr);
                clist.updateSeqFlagForRange(index, nextItr->second, prevItr->first);
            }
        }
        else if (clist.mSeqFlag[prevIdx] != INVALID) {
            extendAtEnd(clist, clist.mSeqFlag[prevIdx]);
        }
        else if (clist.mSeqFlag[nextIdx] != INVALID) {
            extendAtStart(clist, clist.mSeqFlag[nextIdx]);
        }
        return;
    }

    void splitProcessing2(ColorList& clist, int& e2Start, int e2End, int& e1Start, int& e1End)
    {
        addSqMpEntry(clist, e2Start, e2End);
        clist.markInvalid(e1Start, e1End);
        extendAtEnd(clist, clist.mSeqFlag[e2Start]);
        if (e2Start == mSequnceData[e2Start]) {
            mSequnceData.erase(e1Start);
            clist.markInvalid(e1Start);
        }
        while (clist.mSeqFlag[e1Start] != INVALID) {
            if (e1Start == e1End) return;
            e1Start = clist.getNIdx(e1Start);
        }
        addSqMpEntry(clist, e1Start, e1End);
        extendAtStart(clist, clist.mSeqFlag[e1End]);
        e1Start = clist.mSeqFlag[e1Start];
        // if k == v remove entry
        if (e1Start == mSequnceData[e1Start]) {
            mSequnceData.erase(e1Start);
            clist.markInvalid(e1Start);
        }
    }

    void splitProcessing1(ColorList& clist, int& e1Start, int e1End, int& e2Start, int& e2End)
    {
        addSqMpEntry(clist, e1Start, e1End);
        clist.markInvalid(e2Start, e2End);
        extendAtStart(clist, clist.mSeqFlag[e1Start]);
        e1Start = clist.mSeqFlag[e1Start];
        if (e1Start == mSequnceData[e1Start]) {
            mSequnceData.erase(e1Start);
            clist.markInvalid(e1Start);
        }
        while (clist.mSeqFlag[e2End] != INVALID) {
            if (e2Start == e2End) return;
            e2End = clist.getPIdx(e2End);
        }

        addSqMpEntry(clist, e2Start, e2End);
        extendAtEnd(clist, clist.mSeqFlag[e2End]);
        // if k == v remove entry
        if (e2Start == mSequnceData[e2Start]) {
            mSequnceData.erase(e1Start);
            clist.markInvalid(e1Start);
        }
    }

    void extendAtStart(ColorList& clist, int seqID) {

        auto itr = mSequnceData.find(seqID);
        auto newStart = itr->first;
        const auto newEnd = itr->second;

        auto i = clist.getPIdx(newStart);
        if (clist.mSeqFlag[i] != INVALID) return;

        mSequnceData.erase(itr);

        auto nextColor = clist.getColor(newStart);
        for (;i != newEnd;i = clist.getPIdx(i)) {
            if (clist.mSeqFlag[i] != INVALID) break;
            auto curColor = clist.getColor(i);
            if (curColor == nextColor) break;
            nextColor = curColor;
        }

        newStart = clist.getNIdx(i);
        mSequnceData[newStart] = newEnd;
        clist.updateSeqFlagForRange(newStart, newEnd, newStart);
    }
    void extendAtEnd(ColorList& clist, int seqID) {
        auto itr = mSequnceData.find(seqID);
        auto newEnd = itr->second;

        auto prevColor = clist.getColor(newEnd);
        auto i = clist.getNIdx(newEnd);
        for (;i != itr->first;i = clist.getNIdx(i)) {
            if (clist.mSeqFlag[i] != INVALID) break;
            auto curColor = clist.getColor(i);
            if (curColor == prevColor) break;
            prevColor = curColor;
        }

        newEnd = clist.getPIdx(i);
        itr->second = newEnd;
        clist.updateSeqFlagForRange(itr->second, newEnd, itr->first);
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


