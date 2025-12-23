
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
    int nextIdx(int idx) {
        return (++idx == size) ? 0 : idx;
    }
    int prevIdx(int idx) {
        return (idx + size - 1) % mColors.size();
    }
    int getDistance(int start, int end) {
        return 1 + (end - start + size) % size;
    }
};

class SeqManager {
public:

    map<IDX, IDX> mSequnceData;
    SeqManager() {
        mSequnceData.clear();
    }
    void clear() {
        mSequnceData.clear();
    }
    void addSeq(int start, int end) {
        mSequnceData[start] = end;
    }
    void removeSeq(int start, int end) {
        mSequnceData.erase(start);
    }

    map<int, int>::iterator getNextSeq(map<int, int>::iterator itr) {
        itr++; if (itr == mSequnceData.end()) itr = mSequnceData.begin();
        return itr;
    }
    map<int, int>::iterator getPrevSeq(map<int, int>::iterator itr) {
        if (itr == mSequnceData.begin()) itr = prev(mSequnceData.end()); else itr--;
        return itr;
    }

    bool isForwardRange(IDX2D range) {
        return range.first < range.second;
    }

    bool isWithIn(IDX2D range, int idx) {
        if (range.first == idx || range.second == idx) return true;
        if (isForwardRange(range)) {
            if (range.first < idx && idx < range.second) return true;
        }
        else {
            if (range.first > idx && range.second < idx) return false;
            return true;
        }
        return false;
    }
    IDX2D getRangeFor(int idx)
    {
        // 5:7, 10:2 
        // for 1, return 10:2
        // for 3, return INVALID,INVALID
        if (mSequnceData.size() == 0) return { INVALID, INVALID };

        auto curItr = mSequnceData.lower_bound(idx);
        if (curItr != mSequnceData.end() && curItr->first == idx)
            return { curItr->first, curItr->second };

        if (curItr == mSequnceData.end()) curItr = prev(mSequnceData.end());
        else curItr = getPrevSeq(curItr);

        if (isWithIn(*curItr, idx)) return *curItr;
        return { INVALID,INVALID };
    }
    IDX2D getSeqFollow(int idx) {
        if (mSequnceData.size() == 0) return { INVALID,INVALID };

        auto nextItr = mSequnceData.lower_bound(idx);
        if (nextItr == mSequnceData.end()) nextItr = mSequnceData.begin();
        else if (nextItr->first == idx) nextItr = getNextSeq(nextItr);
        return { nextItr->first, nextItr->second };
    }

    IDX2D getSeqPrior(int idx) {
        if (mSequnceData.size() == 0) return { INVALID,INVALID };

        auto prevItr = mSequnceData.lower_bound(idx);
        if (prevItr == mSequnceData.end()) prevItr = prev(mSequnceData.end());
        else prevItr = getPrevSeq(prevItr);

        return *prevItr;
    }

    IDX2D getOnlyRange() {
        if (mSequnceData.size() != 1) return { INVALID, INVALID };
        auto itr = mSequnceData.begin();
        return *itr;
    }

    void getSeqRangesFullyWithIn(int& start, int& end, vector<IDX2D> seq)
    {
        // return ranges within start:end, exclude edge ranges
        if (mSequnceData.size() == 0) return;

        auto itr = mSequnceData.lower_bound(start);

        if (itr != mSequnceData.end()) {
            if (isWithIn(*itr, start)) itr = getNextSeq(itr);
        }
        if (itr == mSequnceData.end()) itr = mSequnceData.begin();
        while (!isWithIn(*itr, end)) {
            seq.push_back(*itr);
            int mstart = itr->second;
            itr = getNextSeq(itr);
            int mend = itr->first;
            if (isWithIn({ mstart, mend }, end)) break;
        }
    }
};

class Solution {
public:

    ColorList* clist;
    map<int, int> mCountTable;
    map<int, int> mcache;
    map<int, int> mCurUpdates;
    SeqManager mSeqMgr;

    Solution() {
        clist = nullptr;
    }

    vector<int> numberOfAlternatingGroups(vector<int>& colors, vector<vector<int>>& queries) {
        OUTPUT_TYPE result;

        delete clist;
        clist = new ColorList(colors);
        initSeq();
        LOG("@start: Sequnce is: " << clist->getList());
        LOG("@start: SequenceData:" << mSeqMgr.mSequnceData);
        LOG("@start: count:" << mCountTable);

        for (auto q : queries) {

            switch (q[0])
            {
            case Q_COUNT: {
                auto val = getCount(q[1]);
                result.push_back(val);
            }
                        break;

            case Q_UPDATE: {
                updateColor(q[1], q[2]);
            }
                         break;
            }
        }
        return result;
    }

    void incrementCount(int dist) {
        mCountTable[dist]++;
    }
    void reduceCount(int dist) {
        mCountTable[dist]--;
        if (mCountTable[dist] == 0) mCountTable.erase(dist);
    }
    void addEntryInSeqData(int idx) {
        mSeqMgr.clear();
        mSeqMgr.addSeq(idx, clist->prevIdx(idx));
        incrementCount(clist->getSize());
    }
    void addEntryInSeqData(int start, int end) {
        if (start == end) {
            if (mSeqMgr.mSequnceData.empty()) return;
            // current entry is invalid
            // if prevSeq or nextSeq exists, try to extend it.
            auto prevSeq = mSeqMgr.getSeqPrior(start);
            if (prevSeq.first != 1) extendSeqAtEnd(prevSeq);
            auto nextSeq = mSeqMgr.getSeqFollow(end);
            if (nextSeq.first != 1) extendSeqAtStart(nextSeq);
            return;
        }

        auto dist = clist->getDistance(start, end);
        incrementCount(dist);
        mSeqMgr.addSeq(start, end);
        LOG("\t\tAdding sequence : (" << start << " - " << end << ") dist:" << dist);
        while (MergeWithNext(start)) {
            auto ret = mSeqMgr.getRangeFor(start);
            end = ret.second;
        }
    }

    IDX2D removeEntryInSeqData(int idx) {
        if (mSeqMgr.mSequnceData.empty()) return make_pair(INVALID, INVALID);
        auto ret = mSeqMgr.getRangeFor(idx);
        auto dist = clist->getDistance(ret.first, ret.second);
        reduceCount(dist);
        mSeqMgr.removeSeq(ret.first, ret.second);
        return ret;
    }

    void updateEntryInSeqData(int oldstart, int oldend, int newstart, int newend) {

        if (oldstart == newstart && oldend == newend) return;

        auto olddist = clist->getDistance(oldstart, oldend);
        auto newdist = clist->getDistance(newstart, newend);

        mSeqMgr.removeSeq(oldstart, oldend);
        reduceCount(olddist);

        if (newstart == newend) return;
        mSeqMgr.addSeq(newstart, newend);
        incrementCount(newdist);
    }

    bool MergeWithNext(int start)
    {
        if (mSeqMgr.mSequnceData.size() < 2) return false;
        auto curSeq = mSeqMgr.getRangeFor(start);
        if (curSeq.first == INVALID) return false;

        auto xstart = curSeq.first;
        auto xend = curSeq.second;
        auto ystart = clist->nextIdx(curSeq.second);
        auto yend = INVALID;
        if (clist->getColor(xend) == clist->getColor(ystart)) return false;

        auto nextSeq = mSeqMgr.getRangeFor(ystart);
        if (nextSeq.first != ystart) return false;

        ystart = nextSeq.first;
        yend = nextSeq.second;

        mSeqMgr.removeSeq(xstart, xend);
        mSeqMgr.removeSeq(ystart, yend);
        reduceCount(clist->getDistance(xstart, xend));
        reduceCount(clist->getDistance(ystart, yend));

        mSeqMgr.addSeq(xstart, yend);
        incrementCount(clist->getDistance(xstart, yend));

        LOG("\t\tMerging: (" << xstart << " - " << xend << ") with (" << ystart << " - " << yend << ")");
        return true;
    }


    void initSeq() {

        mCurUpdates.clear();
        mCountTable.clear();
        mSeqMgr.clear();
        mcache.clear();

        int i = 0;
        auto prevColor = clist->getColor(clist->prevIdx(i));
        for (;i < clist->getSize();i++) {
            if (prevColor == clist->getColor(i)) break;
            prevColor = clist->getColor(i);
        }
        if (i == clist->getSize()) {
            addEntryInSeqData(0, clist->prevIdx(0));
            return;
        }

        addNewSeqInRange(i, clist->prevIdx(i));
    }

    void addNewSeqInRange(int start, int end)
    {
        end = clist->nextIdx(end);
        auto startSeq = start;
        auto prevIdx = start;
        auto prevColor = clist->getColor(prevIdx);
        for (int i = clist->nextIdx(prevIdx); i != end;i = clist->nextIdx(i)) {
            if (clist->getColor(i) == prevColor) {
                addEntryInSeqData(startSeq, prevIdx);
                startSeq = i;
            }
            prevColor = clist->getColor(i);
            prevIdx = i;
        }
        addEntryInSeqData(startSeq, prevIdx);
    }
    bool isInfiniteSeq() {
        if (mCountTable.size() != 1) return false;

        auto ret = mSeqMgr.getOnlyRange();
        if (ret.first == INVALID) return false;
        auto start = ret.first;
        auto end = ret.second;
        if (clist->prevIdx(start) != end) return false;
        if (clist->getColor(start) == clist->getColor(end)) return false;
        return true;
    }
    int getCount(int minSize) {

        if (!mCurUpdates.empty()) {
            mcache.clear();
            processPendingUpdate();
        }

        LOG("===> count query: " << minSize);

        if (mCountTable.size() == 0) return 0;
        if (mcache.find(minSize) != mcache.end()) return mcache[minSize];
        if (isInfiniteSeq()) return  clist->getSize();

        int result = 0;
        auto itr = mCountTable.lower_bound(minSize);
        for (;itr != mCountTable.end(); itr++) {
            int mult = itr->first - minSize + 1;
            result += itr->second * mult;
        }
        mcache[minSize] = result;
        return result;
    }

    void updateColor(int index, int color) {
        if (clist->getColor(index) == color) {
            mCurUpdates.erase(index);
            return;
        }
        mCurUpdates[index] = color;
    }
    void processPendingUpdate() {

        auto start = mCurUpdates.begin()->first;
        auto end = mCurUpdates.rbegin()->first;

        if (start == 0 && end == clist->getSize() - 1) {
            for (auto update : mCurUpdates) {
                LOG("updating color for :" << update);
                clist->setColor(update.first, update.second);
            }

            initSeq();
            return;
        }

        end = start;
        for (auto itr = mCurUpdates.begin(); itr != mCurUpdates.end(); itr++) {

            int idx = itr->first;
            if (idx < end + 3) {
                end = idx;
                continue;
            }

            if (start == end) processUpdate(start, itr->second);
            else processUpdateForRange(start, end);

            start = idx;
            end = INVALID;
        }

        end = mCurUpdates.rbegin()->first;
        if (start == end) processUpdate(start, mCurUpdates.rbegin()->second);
        else processUpdateForRange(start, end);
        mCurUpdates.clear();

        LOG("\nSequnce is: " << clist->getList());
        LOG("SequenceData:" << mSeqMgr.mSequnceData);
        LOG("count:" << mCountTable);
    }

    void processUpdateForRange(int start, int end) {

        vector<IDX2D> seq;
        mSeqMgr.getSeqRangesFullyWithIn(start, end, seq);

        for (auto item : seq) {
            removeEntryInSeqData(item.first);
        }

        bool partOfSameRange = false;
        auto rangeStart = mSeqMgr.getRangeFor(start);
        if (rangeStart.first != INVALID) {

            removeEntryInSeqData(rangeStart.first);
            if (mSeqMgr.isWithIn(rangeStart, end)) {
                partOfSameRange = true;
                if (rangeStart.first == start && rangeStart.second == end) {
                    // seq is exactly equal to start and end
                }
                else {
                    if (rangeStart.first == start)
                        addEntryInSeqData(clist->nextIdx(end), rangeStart.second);
                    else if (rangeStart.second == end)
                        addEntryInSeqData(rangeStart.first, clist->prevIdx(start));
                    else {
                        addEntryInSeqData(rangeStart.first, clist->prevIdx(start));
                        addEntryInSeqData(clist->nextIdx(end), rangeStart.second);
                    }
                }
            }
            else {
                if (rangeStart.first == start) {}
                else
                    addEntryInSeqData(rangeStart.first, clist->prevIdx(start));
            }
        }
        if (!partOfSameRange) {
            auto rangeEnd = mSeqMgr.getRangeFor(end);
            if (rangeEnd.first != INVALID) {
                removeEntryInSeqData(rangeEnd.first);
                if (rangeEnd.second == end) {}
                else
                    addEntryInSeqData(clist->nextIdx(end), rangeEnd.second);
            }
        }

        LOG("updating color for :" << mCurUpdates);
        for (auto update : mCurUpdates) {
            clist->setColor(update.first, update.second);
        }

        if (clist->getDistance(start, end) >= clist->getSize() - 2) {
            initSeq();
            return;
        }
        start = clist->prevIdx(start);
        end = clist->nextIdx(end);

        rangeStart = mSeqMgr.getRangeFor(start);
        if (rangeStart.first != INVALID) {
            auto ret = extendSeqAtEnd(rangeStart);
            start = clist->nextIdx(ret.second);
        }
        rangeStart = mSeqMgr.getRangeFor(end);
        if (rangeStart.first != INVALID) {
            auto ret = extendSeqAtStart(rangeStart);
            start = clist->nextIdx(ret.second);
        }

        addNewSeqInRange(start, end);
        return;
    }


    void processUpdate(int index, int color) {

        auto prevIdx = clist->prevIdx(index);
        auto nextIdx = clist->nextIdx(index);

        if (mSeqMgr.mSequnceData.empty()) {
            clist->setColor(index, color);
            addNewSeq(prevIdx);
            return;
        }

        if (isInfiniteSeq()) {
            clist->setColor(index, color);
            auto ret = mSeqMgr.getOnlyRange();
            removeEntryInSeqData(ret.first);
            addEntryInSeqData(nextIdx, prevIdx);
            return;
        }

        LOG("updating color for :" << index << " " << color);
        clist->setColor(index, color);

        auto ret = mSeqMgr.getRangeFor(index);

        if (ret.first == INVALID) {
            // not part of range
            auto prevSeq = mSeqMgr.getSeqPrior(index);
            auto nextSeq = mSeqMgr.getSeqFollow(index);

            if (prevSeq.second == prevIdx && nextSeq.first == nextIdx) {
                // we can merge
                if (prevSeq.first == nextSeq.first) {
                    // same seq at start and end, becomes full seq  10 0 010
                    removeEntryInSeqData(prevSeq.first);
                    addEntryInSeqData(nextIdx, index);
                    return;
                }
                removeEntryInSeqData(prevSeq.first);
                removeEntryInSeqData(nextSeq.first);
                addEntryInSeqData(prevSeq.first, nextSeq.second);
                return;
            }
            else if (prevSeq.second != prevIdx && nextSeq.first != nextIdx)
                addNewSeqInRange(prevIdx, nextIdx);
            if (prevSeq.second != INVALID) extendSeqAtEnd(prevSeq);
            if (nextSeq.first != INVALID) extendSeqAtStart(nextSeq);
        }
        else {
            // part of some range, make it not part of range
            removeEntryInSeqData(ret.first);
            if (ret.first == index) {
                addEntryInSeqData(nextIdx, ret.second);
                addNewSeqInRange(prevIdx, index);
            }
            else if (ret.second == index) {
                addEntryInSeqData(ret.first, prevIdx);
            }
            else {
                addEntryInSeqData(ret.first, prevIdx);
            }
        }

        // can we extend any seq?
        auto prevSeq = mSeqMgr.getSeqPrior(index);
        auto nextSeq = mSeqMgr.getSeqFollow(index);
        if (prevSeq.first != INVALID) prevSeq = extendSeqAtEnd(prevSeq);
        if (nextSeq.first != INVALID) nextSeq = extendSeqAtStart(nextSeq);

        auto prevRange = mSeqMgr.getRangeFor(prevIdx);
        auto indexRange = mSeqMgr.getRangeFor(prevIdx);
        auto nextRange = mSeqMgr.getRangeFor(prevIdx);
        if (prevRange.first == -1 && (indexRange.first == -1 || nextRange.first == -1)) {
            if (nextRange.first == -1) addNewSeqInRange(prevIdx, nextIdx);
            else addNewSeqInRange(prevIdx, index);
        }
        if (indexRange.first == -1 && nextRange.first == -1)
            addNewSeqInRange(index, nextIdx);
        MergeWithNext(prevIdx);
        MergeWithNext(index);
        MergeWithNext(nextIdx);
    }

    IDX2D addNewSeq(int index) {
        int start = index;
        int prevColor = clist->getColor(start);
        auto end = clist->nextIdx(start);
        for (; end != start; end = clist->nextIdx(end)) {
            if (clist->getColor(end) == prevColor) break;
            prevColor = clist->getColor(end);
        }
        end = clist->prevIdx(end);
        addEntryInSeqData(start, end);
        return make_pair(start, end);
    }
    IDX2D extendSeqAtEnd(IDX2D range) {
        auto prevIdx = range.second;
        auto endCond = range.first;
        auto nrange = mSeqMgr.getSeqFollow(clist->nextIdx(prevIdx));
        if (nrange.first != INVALID) endCond = nrange.first;

        auto prevColor = clist->getColor(prevIdx);
        auto end = clist->nextIdx(prevIdx);
        for (;end != endCond;end = clist->nextIdx(end)) {
            if (clist->getColor(end) == prevColor) break;
            prevColor = clist->getColor(end);
        }

        end = clist->prevIdx(end);
        if (end != endCond)
            updateEntryInSeqData(range.first, range.second, range.first, end);
        return make_pair(range.first, end);
    }
    IDX2D extendSeqAtStart(IDX2D range) {
        auto start = range.first;
        auto endCond = range.second;
        auto prange = mSeqMgr.getSeqPrior(clist->prevIdx(range.first));
        if (prange.first != INVALID) endCond = prange.second;

        auto prevColor = clist->getColor(start);
        start = clist->prevIdx(start);
        for (; start != endCond; start = clist->prevIdx(start)) {
            if (clist->getColor(start) == prevColor) break;
            prevColor = clist->getColor(start);
        }

        start = clist->nextIdx(start);
        if (start != endCond)
            updateEntryInSeqData(range.first, range.second, start, range.second);
        return make_pair(start, range.second);
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
            { {1,3}, {2,3,1}, {1,3}, {1,3},{2,0,1},{1,3}, { 2,3,0 }, {1,3} }
        );
        nums.back().setOutput({ 1,0,0,0,1 });
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
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,1,1 },
            { {1, 3},{2, 3, 0},{2, 3, 0},{1, 3},{1, 3} });
        nums.back().setOutput({ 1,0,0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1,1,0,1 },
            { {2, 1, 0},{2, 2, 1},{1, 3},{2, 2, 0},{1, 3} });
        nums.back().setOutput({ 1,0 });
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0, 1, 0, 0, 1 },
            { {2, 0, 1},{2, 2, 0},{1, 3},{1, 3} });
        nums.back().setOutput({ 0,0 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1, 0, 1, 0 },
            { {2, 3, 1},{2, 0, 0},{2, 0, 0},{2, 1, 1},{1, 3},{2, 0, 0} });
        nums.back().setOutput({ 1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1, 0, 1, 1 },
            { {2, 1, 1},{2, 3, 0},{1, 3} });
        nums.back().setOutput({ 1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1, 0, 1, 1,1 },
            { {1, 4 }, { 1, 3 }, { 2, 3, 0 }, { 1, 3 }, { 1, 4 } });
        nums.back().setOutput({ 0,1,3,2 });
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,0,1,1,0 },
            { {2, 5, 1},{1, 4},{2, 4, 0},{2, 2, 1} });
        nums.back().setOutput({ 2 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 1,1,1,0 },
            { {1,3} });
        nums.back().setOutput({ 1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,0,0,0 },
            { {2, 1, 1},{1, 3},{1, 3},{2, 1, 0},{2, 2, 1},{1, 3},{1, 3} });
        nums.back().setOutput({ 1,1,1,1 });
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { 0,1,0,1 },
            { {2, 0, 1},{1, 3},{2, 1, 0},{1, 3},{2, 3, 0},{2, 3, 0},{1, 3} });
        nums.back().setOutput({ 1,0,1 });
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

        cout << __LOGPREFIX << "test:" << testID << " =>\t"
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


