
/*
1776. Car Fleet II
Hard
Topics
Companies
Hint

There are n cars traveling at different speeds in the same direction along a one-lane road. You are given an array cars of length n, where cars[i] = [positioni, speedi] represents:

    positioni is the distance between the ith car and the beginning of the road in meters. It is guaranteed that positioni < positioni+1.
    speedi is the initial speed of the ith car in meters per second.

For simplicity, cars can be considered as points moving along the number line. Two cars collide when they occupy the same position. Once a car collides with another car, they unite and form a single car fleet. The cars in the formed fleet will have the same position and the same speed, which is the initial speed of the slowest car in the fleet.

Return an array answer, where answer[i] is the time, in seconds, at which the ith car collides with the next car, or -1 if the car does not collide with the next car. Answers within 10-5 of the actual answers are accepted.



Example 1:

Input: cars = [[1,2],[2,1],[4,3],[7,2]]
Output: [1.00000,-1.00000,3.00000,-1.00000]
Explanation: After exactly one second, the first car will collide with the second car, and form a car fleet with speed 1 m/s. After exactly 3 seconds, the third car will collide with the fourth car, and form a car fleet with speed 2 m/s.

Example 2:

Input: cars = [[3,4],[5,4],[6,3],[9,1]]
Output: [2.00000,1.00000,1.50000,-1.00000]



Constraints:

    1 <= cars.length <= 105
    1 <= positioni, speedi <= 106
    positioni < positioni+1


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
typedef vector<double> OUTPUT_TYPE;

typedef pair<int, int> IDX2D;



static int inputID = 1;
struct Input
{
    int id;
    V2Int nums;

    Input() {
        id = inputID++;
    }
    Input(V2Int nums) {
        this->nums = nums;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};


int POS_IDX = 0;
int SPD_IDX = 1;

typedef int IDX;
typedef int POSInt;
typedef int SPDInt;


struct Data {

    vector<vector<int>>& cars;
    map < SPDInt, vector<IDX>> cMap;
    vector<POSInt> crossedPos;

    void clear() {
        cMap.clear();
    }
    Data(vector<vector<int>>& cars) : cars(cars) {
        clear();
        crossedPos.resize(cars.size(), INVALID);
    }

    inline SPDInt getSpeed(IDX i) {
        return cars[i][SPD_IDX];
    }
    inline POSInt getPosition(IDX i) {
        return cars[i][POS_IDX];
    }
    inline void Add(IDX i) {
        cMap[getSpeed(i)].push_back(i);
    }

    void checkIntersection(IDX scIdx, vector<double>& result) {

        LOG("crossed Positions: " << crossedPos);
        auto it_speed = cMap.lower_bound(getSpeed(scIdx));

        if (it_speed == cMap.end()) return;
        if (it_speed->first == getSpeed(scIdx)) it_speed++;
        if (it_speed == cMap.end()) return;

        while (it_speed != cMap.end()) {

            LOG("checkIntersection: " << it_speed->first << " with: " << it_speed->second);
            for (auto it_pos = it_speed->second.begin(); it_pos != it_speed->second.end(); ++it_pos) {

                auto fcIdx = *it_pos;

                if (crossedPos[fcIdx] != INVALID && crossedPos[fcIdx] <= getPosition(scIdx)) {
                    continue;
                }

                auto newResult = 1.0 * (getPosition(scIdx) - getPosition(fcIdx)) / (getSpeed(fcIdx) - getSpeed(scIdx));
                crossedPos[fcIdx] = getPosition(scIdx) + (newResult * getSpeed(scIdx));

                if (result[fcIdx] == -1) {
                    LOG("\t fasterCar:" << fcIdx << " result: " << newResult);
                    result[fcIdx] = newResult;
                }
                else {
                    LOG("\t fasterCar:" << fcIdx << " result: " << result[fcIdx] << " vs: " << newResult);
                    result[fcIdx] = min(result[fcIdx], newResult);
                }
                cMap[getSpeed(scIdx)].push_back(fcIdx);
            }

            it_speed = cMap.erase(it_speed);
        }
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:
    vector<double> getCollisionTimes(vector<vector<int>>& cars) {

        auto size = cars.size();
        vector<double> result(size, -1.0);
        Data data(cars);

        for (int i = 0; i < size; ++i) {

            LOG("@" << i << " processing: " << cars[i]);

            data.Add(i);
            data.checkIntersection(i, result);

            LOG("result: " << result << endl);
        }

        return result;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return getCollisionTimes(nums.nums);
    }
};


/****************************************************************************/



ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
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

    nums.push_back(Input({ {1, 2}, {2, 1}, {4, 3}, {7, 2} }));
    expected.push_back({ 1.00000, -1.00000, 3.00000, -1.00000 });

    nums.push_back(Input({ {3, 4}, {5, 4}, {6, 3}, {9, 1} }));
    expected.push_back({ 2.00000, 1.00000, 1.50000, -1.00000 });


    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = __D_CASE_ID; i <= __D_CASE_ID; i++)
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


