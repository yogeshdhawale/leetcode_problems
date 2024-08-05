/*
2136. Earliest Possible Day of Full Bloom
Hard
Topics
Companies
Hint

You have n flower seeds. Every seed must be planted first before it can begin to grow, then bloom. Planting a seed takes time and so does the growth of a seed. You are given two 0-indexed integer arrays plantTime and growTime, of length n each:

    plantTime[i] is the number of full days it takes you to plant the ith seed. Every day, you can work on planting exactly one seed. You do not have to work on planting the same seed on consecutive days, but the planting of a seed is not complete until you have worked plantTime[i] days on planting it in total.
    growTime[i] is the number of full days it takes the ith seed to grow after being completely planted. After the last day of its growth, the flower blooms and stays bloomed forever.

From the beginning of day 0, you can plant the seeds in any order.

Return the earliest possible day where all seeds are blooming.



Example 1:

Input: plantTime = [1,4,3], growTime = [2,3,1]
Output: 9
Explanation: The grayed out pots represent planting days, colored pots represent growing days, and the flower represents the day it blooms.
One optimal way is:
On day 0, plant the 0th seed. The seed grows for 2 full days and blooms on day 3.
On days 1, 2, 3, and 4, plant the 1st seed. The seed grows for 3 full days and blooms on day 8.
On days 5, 6, and 7, plant the 2nd seed. The seed grows for 1 full day and blooms on day 9.
Thus, on day 9, all the seeds are blooming.

Example 2:

Input: plantTime = [1,2,3,2], growTime = [2,1,2,1]
Output: 9
Explanation: The grayed out pots represent planting days, colored pots represent growing days, and the flower represents the day it blooms.
One optimal way is:
On day 1, plant the 0th seed. The seed grows for 2 full days and blooms on day 4.
On days 0 and 3, plant the 1st seed. The seed grows for 1 full day and blooms on day 5.
On days 2, 4, and 5, plant the 2nd seed. The seed grows for 2 full days and blooms on day 8.
On days 6 and 7, plant the 3rd seed. The seed grows for 1 full day and blooms on day 9.
Thus, on day 9, all the seeds are blooming.

Example 3:

Input: plantTime = [1], growTime = [1]
Output: 2
Explanation: On day 0, plant the 0th seed. The seed grows for 1 full day and blooms on day 2.
Thus, on day 2, all the seeds are blooming.



Constraints:

    n == plantTime.length == growTime.length
    1 <= n <= 105
    1 <= plantTime[i], growTime[i] <= 104


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
#define MY_MAX_VAL 100001
#define SKEW_VAL 4

struct Input
{
    static int id;
    vector<int> p;
    vector<int> g;

    Input(vector<int> p, vector<int> g) : p(p), g(g) {}
};
int Input::id = 1;
typedef int OUTPUT_TYPE;

typedef int PLANT_ID;

struct Data {
    PLANT_ID  id;
    int plantTime;
    int growTime;
    int totalTime;

    Data(PLANT_ID  id, int plantTime, int growTime) : id(id), plantTime(plantTime), growTime(growTime), totalTime(plantTime + growTime) {
    }

    /*
    * @return used days
    */
    int Plant(int days)
    {
        int usedDays = (plantTime < days) ? plantTime : days;
        plantTime -= usedDays;
        totalTime = plantTime + growTime;
        return usedDays;
    }
    void Grow(int days) {
        growTime -= days;
        totalTime = growTime;
    }
};

bool operator<(const Data& a, const Data& b) {

    int diff = abs(a.growTime - b.growTime);
    if (diff > SKEW_VAL) {
        return a.growTime < b.growTime;
    }

    diff = abs(a.plantTime - b.plantTime);
    if (diff > SKEW_VAL) {
        return a.plantTime < b.plantTime;
    }

    return a.growTime < b.growTime;
}
struct compareData {
    bool operator()(const Data* a, const Data* b) {
        return *a < *b;
    }
};

ostream& operator<<(ostream& out, const Input& nums);
ostream& operator<<(ostream& out, const Data* d);
ostream& operator<<(ostream& out, const Data& d);

class Solution {
    int totalptime = 0;
    int totalgtime = 0;

public:

    void clear() {
        totalptime = 0;
        totalgtime = 0;
    }

    int earliestFullBloom(vector<int>& plantTime, vector<int>& growTime) {

        clear();
        if (plantTime.size() == 0) {
            return 0;
        }
        if (plantTime.size() == 1) {
            return plantTime[0] + growTime[0];
        }

        vector<Data*> data;
        priority_queue<Data*, vector<Data*>, compareData> queue;

        for (size_t i = 0; i < plantTime.size(); i++) {

            Data* obj = new Data(i, plantTime[i], growTime[i]);
            data.push_back(obj);
            totalptime += plantTime[i];
            totalgtime += growTime[i];
            queue.push(obj);
        }

        int growthCounter = 0;


        int time = 0;
        for (; queue.size() > 0;time++) {

            LOG("\t@time: " << time << " total remaining:(p)=(" << totalptime << ") g: (" << totalgtime << ") growing:" << growthCounter);
            LOG(queue);

            Data* obj = queue.top();
            queue.pop();

            int cnt = 1;
            if (queue.size() == 0)
                cnt = obj->plantTime;
            else if (obj->plantTime > queue.top()->plantTime) {
                cnt = 1 + obj->plantTime - queue.top()->plantTime;
                if (cnt > SKEW_VAL)
                    cnt = SKEW_VAL;
            }

            LOG("processing plant with : " << *obj << " with cnt:" << cnt);

            if (growthCounter != 0)
                growthCounter -= min(growthCounter, cnt);

            if (processPlanting(obj, cnt))
                growthCounter = max(growthCounter, obj->growTime);
            else
                queue.push(obj);

            time += (cnt - 1);
        }

        LOG("Returning " << time << "+" << growthCounter);
        return time + growthCounter;
    }

    bool processPlanting(Data* obj, int cnt) {

        int ret = obj->Plant(cnt);
        totalptime -= ret;
        return obj->plantTime == 0;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return earliestFullBloom(nums.p, nums.g);
    }
};

/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " p:" << nums.p;
    out << " g:" << nums.g;
    return out;
}

ostream& operator<<(ostream& out, const Data& d) {
    out << "id: " << d.id << " {" << d.plantTime << "," << d.growTime << "," << d.totalTime << "}";
    return out;
}

ostream& operator<<(ostream& out, const Data* d) {
    out << *d;
    return out;
}

/****************************************************************************/

#define TEXT_LOG_PREFIX "___________\t"

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 1, 4, 3 }, { 2, 3, 1 }));
    expected.push_back(9);

    nums.push_back(Input({ 1, 2, 3, 2 }, { 2, 1, 2, 1 }));
    expected.push_back(9);

    nums.push_back(Input({ 1 }, { 1 }));
    expected.push_back(2);

    nums.push_back(Input({ 3,11,29,4,4,26,26,12,13,10,30,19,27,2,10 }, { 10,13,22,17,18,15,21,11,24,14,18,23,1,30,6 }));
    expected.push_back(227);

    nums.push_back(Input({ 15,29,24,8,14,26,12,15,27,2,5,10,7,17,9,5,9,21,11,13,13,2,1,17,11 }, { 26,20,10,9,8,27,1,19,13,22,10,10,21,14,17,14,17,30,3,3,14,16,7,12,25 }));
    expected.push_back(324);

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "\ttest for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.ProxyFunction(nums[i]);
        cout << TEXT_LOG_PREFIX << "\t==> " << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{

    cout << TEXT_LOG_PREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 6,8,7,22,30,5,17,18 }, { 30,15,1,21,5,13,5,11 }));
    expected.push_back(114);

    cout << TEXT_LOG_PREFIX << "\ttest for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << TEXT_LOG_PREFIX << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us" << endl;
    cout << TEXT_LOG_PREFIX << "\t==> " << result << " vs " << expected[0] << endl;
    assert(result == expected[0]);
}

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_VB
    test2(2);
#endif

    return 0;
}


