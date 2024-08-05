
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


struct Task {

    TASK_ID id;
    WORK_SLOTS work;
    TIMESLOT_IDX startTime;
    TIMESLOT_IDX endTime;
    bool isCompleted;

    Task() : id(INVALID), work(0), startTime(0), endTime(0), isCompleted(false) {}

    void Set(TASK_ID id, WORK_SLOTS timeTaken) {
        this->id = id;
        this->work = timeTaken;
        this->startTime = 1;
        this->endTime = this->startTime + this->work;
    }
};


enum SLOT_STATE {
    SLOT_FREE = 0,
    SLOT_USED = 1
};


struct Data {
    vector<Task> tasks;
    vector<vector<TASK_ID>> ooptions;

    map < TIMESLOT_IDX, map<WORK_SLOTS, int>> mTasksToMark;
    vector<TASK_ID> mTasksToWork;

    vector<bool> mUsedSlots;
    map<TIMESLOT_IDX, TIMESLOT_IDX> cache;

    void clear() {
        tasks.clear();
        ooptions.clear();

        mTasksToMark.clear();
        mTasksToWork.clear();
        mUsedSlots.clear();
        cache.clear();
    }
    Data(int taskSize, int maxTimeSlots) {
        clear();
        tasks.resize(taskSize + 1);
        ooptions.resize(taskSize + 1);
        mUsedSlots.resize(maxTimeSlots + 1, SLOT_FREE);
    }
    void TaksPendingForMark(TIMESLOT_IDX eligibleTime, TASK_ID id, WORK_SLOTS work) {
        mTasksToMark[eligibleTime][work]++;
    }

    inline void AddOption(TIMESLOT_IDX time, TASK_ID idx) {
        ooptions[idx].push_back(time);
    }

    int process(TASK_ID idx) {
        Task& t = tasks[idx];
        if (t.work == 0) {
            ooptions[idx].clear();
            TaksPendingForMark(1, idx, t.work);
            return 0;
        }
        if (ooptions[idx].size() == 0) {
            mTasksToWork.push_back(idx);
            return t.work;
        }

        TIMESLOT_IDX timeslot = ooptions[idx][0];
        mUsedSlots[timeslot] = SLOT_USED;
        TaksPendingForMark(timeslot, idx, t.work);
        return 0;
    }

    int processMarkTasks(int& pendingTime, int pendingWork) {

        WORK_SLOTS slots_for_work = 0;
        TIMESLOT_IDX cur_ts_idx = 1;

        auto itr = mTasksToMark.begin();
        for (;cur_ts_idx < mUsedSlots.size() && itr != mTasksToMark.end();itr++) {

            TIMESLOT_IDX nextMarkTime = itr->first;

            auto cnt = 0;
            for (auto item : itr->second) {
                cnt += item.second;
            }

            LOG("cur_ts: " << cur_ts_idx << " nextMarkTime: " << nextMarkTime << " cnt:" << cnt);

            for (;cur_ts_idx < nextMarkTime; cur_ts_idx++) {
                if (mUsedSlots[cur_ts_idx] == SLOT_USED) continue;
                if (cur_ts_idx < nextMarkTime) slots_for_work++;
            }

            if (cur_ts_idx < nextMarkTime) return -1;

            for (;cnt > 0 && cur_ts_idx < mUsedSlots.size(); cur_ts_idx++) {
                if (mUsedSlots[cur_ts_idx] == SLOT_USED) continue;
                mUsedSlots[cur_ts_idx] = SLOT_USED;
                cnt--;
            }

            if (cnt != 0) return -1;
            cache[nextMarkTime] = cur_ts_idx - 1;
        }

        cur_ts_idx--;

        if (mTasksToWork.size() == 0) {
            return cur_ts_idx;
        }

        WORK_SLOTS tp_work = pendingWork + mTasksToWork.size();
        pendingTime -= tp_work;

        if (slots_for_work >= tp_work)
            return cur_ts_idx;

        cur_ts_idx += (tp_work - slots_for_work);
        return cur_ts_idx;
    }

    int optimize(TIMESLOT_IDX ts_idx, int& pendingTime) {

        auto new_ts_idx = ts_idx;
        LOG("cache is:" << cache);
        for (auto itr = mTasksToMark.rbegin();itr != mTasksToMark.rend();itr++) {
            auto old_ts_idx = new_ts_idx;
            int extrawork = 0;
            int taskCnt = 0;

            for (auto item : itr->second) {
                extrawork += (item.first * item.second);
                taskCnt += item.second;
            }

            int extraSlotsRequired = extrawork + taskCnt - (2 * taskCnt);
            if (extraSlotsRequired > pendingTime) return new_ts_idx;

            pendingTime -= extraSlotsRequired;
            TIMESLOT_IDX prev_ts_idx = prev(itr)->first;
            auto prevEndTime = cache[prev_ts_idx];
            new_ts_idx = prevEndTime + extraSlotsRequired;

            LOG("optimize: @" << itr->first << " with counts: " << itr->second << " new_ts_idx:" << new_ts_idx << " vs old_ts_idx:" << old_ts_idx);
            if (new_ts_idx > old_ts_idx)
                return old_ts_idx;

            LOG("broken successfully:" << itr->second);
        }
        return new_ts_idx;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    int earliestSecondToMarkIndices(vector<int>& nums, vector<int>& changeIndices) {

        int totalTasks = nums.size();
        int maxTimeSlots = changeIndices.size();
        int work_pending_cnt = 0;

        if (maxTimeSlots < totalTasks) return -1;

        Data data(totalTasks, maxTimeSlots);


        // at time i, you can update task id changeIndices[i]
        for (size_t i = 0;i < changeIndices.size();i++) {
            data.AddOption(i + 1, changeIndices[i]);
        }

        for (size_t i = 0;i < nums.size();i++) {

            WORK_SLOTS work = nums[i];
            auto task_id = i + 1;
            Task& t = data.tasks[task_id];
            t.Set(task_id, work);
            work_pending_cnt += data.process(task_id);
        }
        if (work_pending_cnt + totalTasks > maxTimeSlots) return -1;

        int available_ts_cnt = maxTimeSlots - (work_pending_cnt + totalTasks);

        LOG("State:" << data);
        LOG("bp - available_ts_cnt:" << available_ts_cnt << " pending work cnt:" << work_pending_cnt);

        TIMESLOT_IDX maxTSused = data.processMarkTasks(available_ts_cnt, work_pending_cnt);

        LOG("bo - available_ts_cnt:" << available_ts_cnt << " pending work cnt:" << work_pending_cnt << " maxTSused:" << maxTSused);

        maxTSused = data.optimize(maxTSused, available_ts_cnt);

        LOG("ao - available_ts_cnt:" << available_ts_cnt << " pending work cnt:" << work_pending_cnt << " maxTSused:" << maxTSused);

        if (maxTSused <= changeIndices.size()) return maxTSused;
        return -1;
    }


    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return earliestSecondToMarkIndices(nums.nums, nums.cidxs);
    }
};


/****************************************************************************/
ostream& operator<<(ostream& out, const Task& t) {

    if (t.id == INVALID) return out;

    out << "T:" << t.id << "(" << t.work << "," << t.startTime << "," << t.endTime << ")";
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
    out << "forMark:" << d.mTasksToMark << endl;
    out << "forWork:" << d.mTasksToWork << endl;
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

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
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


