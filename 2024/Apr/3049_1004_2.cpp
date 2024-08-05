
/*
3049. Earliest Second to Mark Indices II
Hard
Topics
Companies
Hint

You are given two 1-indexed integer arrays, nums and, changeIndices, having lengths n and m, respectively.

Initially, all indices in nums are unmarked. Your task is to mark all indices in nums.

In each second, s, in order from 1 to m (inclusive), you can perform one of the following operations:

    Choose an index i in the range [1, n] and decrement nums[i] by 1.
    Set nums[changeIndices[s]] to any non-negative value.
    Choose an index i in the range [1, n], where nums[i] is equal to 0, and mark index i.
    Do nothing.

Return an integer denoting the earliest second in the range [1, m] when all indices in nums can be marked by choosing operations optimally, or -1 if it is impossible.



Example 1:

Input: nums = [3,2,3], changeIndices = [1,3,2,2,2,2,3]
Output: 6
Explanation: In this example, we have 7 seconds. The following operations can be performed to mark all indices:
Second 1: Set nums[changeIndices[1]] to 0. nums becomes [0,2,3].
Second 2: Set nums[changeIndices[2]] to 0. nums becomes [0,2,0].
Second 3: Set nums[changeIndices[3]] to 0. nums becomes [0,0,0].
Second 4: Mark index 1, since nums[1] is equal to 0.
Second 5: Mark index 2, since nums[2] is equal to 0.
Second 6: Mark index 3, since nums[3] is equal to 0.
Now all indices have been marked.
It can be shown that it is not possible to mark all indices earlier than the 6th second.
Hence, the answer is 6.

Example 2:

Input: nums = [0,0,1,2], changeIndices = [1,2,1,2,1,2,1,2]
Output: 7
Explanation: In this example, we have 8 seconds. The following operations can be performed to mark all indices:
Second 1: Mark index 1, since nums[1] is equal to 0.
Second 2: Mark index 2, since nums[2] is equal to 0.
Second 3: Decrement index 4 by one. nums becomes [0,0,1,1].
Second 4: Decrement index 4 by one. nums becomes [0,0,1,0].
Second 5: Decrement index 3 by one. nums becomes [0,0,0,0].
Second 6: Mark index 3, since nums[3] is equal to 0.
Second 7: Mark index 4, since nums[4] is equal to 0.
Now all indices have been marked.
It can be shown that it is not possible to mark all indices earlier than the 7th second.
Hence, the answer is 7.

Example 3:

Input: nums = [1,2,3], changeIndices = [1,2,3]
Output: -1
Explanation: In this example, it can be shown that it is impossible to mark all indices, as we don't have enough seconds.
Hence, the answer is -1.



Constraints:

    1 <= n == nums.length <= 5000
    0 <= nums[i] <= 109
    1 <= m == changeIndices.length <= 5000
    1 <= changeIndices[i] <= n


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

#include "log.h"

using namespace std;

/****************************************************************************/

#include <ranges>

#ifndef __DEBUG_LOG
#define LOG(x)
#define VLOG(x)
#endif

#define INVALID -1
#define RESULT_MOD 1000000007
#define MY_MAX_VAL INT32_MAX

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    V1Int nums;
    V1Int cidxs;

    Input() {
        id = inputID++;
    }
    Input(V1Int nums, V1Int cidxs) {
        this->nums = nums;
        this->cidxs = cidxs;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};


typedef int TASK_ID;
typedef size_t WORK_SLOTS;
typedef size_t TIMESLOT_IDX;


enum SLOT_STATE {
    SLOT_FREE = 0,
    SLOT_USED,
    SLOT_MARKED
};

struct Task {
    TASK_ID id;
    WORK_SLOTS work;
    bool processed;

    Task() : id(INVALID), work(INVALID), processed(false) {}
    void Set(TASK_ID id, WORK_SLOTS work) {
        this->id = id;
        this->work = work;
    }

};

struct Slot {
    TIMESLOT_IDX idx;
    SLOT_STATE state;

    WORK_SLOTS workCnt;
    WORK_SLOTS markCnt;

    int mcAvailable;
    int mcMpending;

    Slot() : idx(INVALID), state(SLOT_FREE), workCnt(0), markCnt(0), mcAvailable(0), mcMpending(0) {}
    Slot(TIMESLOT_IDX ts_idx, WORK_SLOTS w, int mark) : Slot() {
        idx = ts_idx;
        workCnt = w;
        markCnt = mark;
        state = SLOT_USED;
    }
    void Add(TIMESLOT_IDX ts_idx, WORK_SLOTS w, int mark) {
        idx = ts_idx;
        workCnt += w;
        markCnt += mark;
        state = SLOT_USED;
    }

    bool isUsed() const {
        return state == SLOT_USED;
    }
    void Set(SLOT_STATE s) {
        state = s;
    }
};

struct Data {
    vector<Task> tasks;
    vector<vector<TIMESLOT_IDX>> options;
    map<TIMESLOT_IDX, Slot> slots;

    Data(size_t totalTasks, size_t maxTimeSlots) {
        tasks.resize(totalTasks + 1);
        options.resize(totalTasks + 1);

        slots.clear();
        slots[0] = Slot(0, 0, 0);
        slots[0].Set(SLOT_USED);
    }

    void AddOption(TASK_ID task_id, TIMESLOT_IDX ts_idx) {
        if (options[task_id].empty())
            options[task_id].push_back(ts_idx);
    }
    void process(TASK_ID task_id) {
        Task& t = tasks[task_id];
        if (t.processed) return;
        t.processed = true;

        if (options[task_id].empty() || t.work < 2) {
            options[task_id].clear();
            slots[0].Add(0, t.work, 1);
            return;
        }

        TIMESLOT_IDX ts_idx = options[task_id][0];
        slots[ts_idx] = Slot(ts_idx, t.work, 1);
    }
};

ostream& operator<<(ostream& out, const Data& d);
ostream& operator<<(ostream& out, const Task& t);
ostream& operator<<(ostream& out, const Slot& s);

class Solution {
public:

    int earliestSecondToMarkIndices(vector<int>& nums, vector<int>& changeIndices) {

        int totalTasks = nums.size();
        int maxTimeSlots = changeIndices.size();

        if (maxTimeSlots < totalTasks) return -1;

        Data data(totalTasks, maxTimeSlots);

        // at time i, you can update task id changeIndices[i]
        for (size_t i = 0;i < changeIndices.size();i++) {
            data.AddOption(changeIndices[i], i + 1);
        }

        for (size_t i = 0;i < nums.size();i++) {
            auto task_id = i + 1;
            WORK_SLOTS work = nums[i];

            Task& t = data.tasks[task_id];
            t.Set(task_id, work);
            data.process(task_id);
        }

        int freeSlotsCnt = 0;
        int markCntPending = 0;
        int elasticWorkPending = data.slots[0].workCnt + data.slots[0].markCnt;
        if (markCntPending > maxTimeSlots) return -1;

        int lastTimeSlotUpdated = 0;
        TIMESLOT_IDX lastIDX = data.slots.rbegin()->first;
        for (int i = 1;i <= lastIDX; i++) {

            if (data.slots.find(i) == data.slots.end()) {
                if (markCntPending) {
                    markCntPending--;
                    lastTimeSlotUpdated = i;
                }
                else
                    freeSlotsCnt++;
                continue;
            }

            Slot& slot = data.slots[i];
            int totalWork = slot.workCnt;
            if (totalWork > freeSlotsCnt || i == lastIDX) {
                slot.mcAvailable = freeSlotsCnt;
                slot.mcMpending = markCntPending;
                markCntPending += slot.markCnt;
                lastTimeSlotUpdated = i;
                continue;
            }

            LOG("break in loop: freeSlotsCnt:" << freeSlotsCnt << " totalWork:" << totalWork << " i:" << i);
            freeSlotsCnt -= totalWork;
            markCntPending += slot.markCnt;
            slot.Set(SLOT_FREE);
            data.slots.erase(i);
            if (markCntPending) {
                slot.Set(SLOT_MARKED);
                markCntPending--;
                lastTimeSlotUpdated = i;
            }
            else
                freeSlotsCnt++;
        }

        int finaltimeSlotUsed = lastTimeSlotUpdated + markCntPending;
        if (freeSlotsCnt < elasticWorkPending) {
            finaltimeSlotUsed += (elasticWorkPending - freeSlotsCnt);
        }

        LOG(data << " lastTimeSlotUpdated:" << lastTimeSlotUpdated << " markCntPending:" << markCntPending << " finaltimeSlotUsed:" << finaltimeSlotUsed);

        if (data.slots.size() == 1) return (finaltimeSlotUsed <= maxTimeSlots) ? finaltimeSlotUsed : -1;

        auto itr = data.slots.rbegin();

        Slot& lastSlot = itr->second;
        finaltimeSlotUsed = lastSlot.idx + lastSlot.mcMpending + lastSlot.markCnt;
        if (freeSlotsCnt < (elasticWorkPending)) {
            finaltimeSlotUsed += (elasticWorkPending - freeSlotsCnt);
        }

        LOG("\n\t1st result:" << " finaltimeSlotUsed:" << finaltimeSlotUsed);

        itr++;
        freeSlotsCnt = itr->second.mcAvailable;
        int runningWork = 0;

        itr = data.slots.rbegin();
        while (itr != data.slots.rend()) {

            Slot& breakSlot = itr->second;
            itr++;
            Slot& curSlot = itr->second;
            LOG("breakSlot:" << breakSlot << " curSlot:" << curSlot);

            runningWork += breakSlot.workCnt;
            runningWork += breakSlot.markCnt;
            LOG("runningWork:" << runningWork << " freeSlotsCnt:" << freeSlotsCnt);

            int pendingWork = runningWork - freeSlotsCnt;
            if (pendingWork < 0) pendingWork = 0;

            if (runningWork < freeSlotsCnt) freeSlotsCnt -= runningWork;
            else freeSlotsCnt = 0;

            int newfinaltimeSlotUsed = curSlot.idx + 1 + curSlot.mcMpending + curSlot.markCnt + pendingWork;

            if (curSlot.idx == 0) newfinaltimeSlotUsed += curSlot.workCnt;

            if (freeSlotsCnt < elasticWorkPending) {
                newfinaltimeSlotUsed += (elasticWorkPending - freeSlotsCnt);
            }

            LOG("new result :" << newfinaltimeSlotUsed << " vs old result:" << finaltimeSlotUsed);

            if (newfinaltimeSlotUsed <= finaltimeSlotUsed) finaltimeSlotUsed = newfinaltimeSlotUsed;
            else break;
        }

        return (finaltimeSlotUsed > maxTimeSlots) ? -1 : finaltimeSlotUsed;
    }


    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return earliestSecondToMarkIndices(nums.nums, nums.cidxs);
    }
};


/****************************************************************************/


ostream& operator<<(ostream& out, const Task& t) {
    if (t.id == INVALID) return out;
    out << "T:" << t.id << "(" << t.work << ")";
    return out;
}

ostream& operator<<(ostream& out, const Slot& s) {
    if (s.state != SLOT_USED) return out;
    out << "S:" << s.idx << "(" << s.state << "," << s.workCnt << "," << s.markCnt << ")-(" << s.mcMpending << "," << s.mcAvailable << " )";
    return out;
}

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    out << " cidxs:" << nums.cidxs;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */


ostream& operator<<(ostream& out, const Data& d) {

    out << "tasks:" << d.tasks << endl;
    out << "options:" << d.options << endl;
    out << "slots:(idx, state, work, mark, mcMpending, mcAvailable)\n" << d.slots << endl;
    return out;
}

/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << TEXT_LOG_PREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 3,2,3 }, { 1,3,2,2,2,2,3 }));
    expected.push_back(6);

    nums.push_back(Input({ 1,2,3 }, { 1,2,3 }));
    expected.push_back(-1);

    nums.push_back(Input({ 0,0,1,2 }, { 1,2,1,2,1,2,1,2 }));
    expected.push_back(7);

    nums.push_back(Input({ 2,3,4,5 }, { 1,1,1,1,1,4,3,2,1,1 }));
    expected.push_back(10);

    nums.push_back(Input({ 1, 0, 3, 1, 2 }, { 3,3,2,3,4 }));
    expected.push_back(-1);

    nums.push_back(Input({ 0, 0, 3, 0 }, { 2,2,2,2,2,2,3,1 }));
    expected.push_back(7);

    nums.push_back(Input({ 3,3 }, { 2,2,1 }));
    expected.push_back(-1);

    nums.push_back(Input({ 2,2 }, { 2,2,1 }));
    expected.push_back(-1);

    nums.push_back(Input({ 3,1 }, { 2,2,2,2,1,2,2 }));
    expected.push_back(6);


    nums.push_back(Input({ 0, 2, 4 }, { 1,2,1,1,2,1,3,3,1,1 }));
    expected.push_back(8);

    nums.push_back(Input({ 5,1,3,2,2,5 }, { 3,2,2,3,1,1,3,4,2,3,4,2,5,6,5,3,6,5,3 }));
    expected.push_back(15);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = __D_CASE_ID; i < __D_CASE_ID + 1; i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
        logtime("*****Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG

#include "data.h"

void test2(int testID)
{

    cout << TEXT_LOG_PREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input(d1));
    expected.push_back(o1);

    cout << TEXT_LOG_PREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << TEXT_LOG_PREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("*****Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG
    test2(2);
#endif

    return 0;
}


