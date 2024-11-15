
/*
3219. Minimum Cost for Cutting Cake II
Hard
Topics
Companies
Hint

There is an m x n cake that needs to be cut into 1 x 1 pieces.

You are given integers m, n, and two arrays:

    horizontalCut of size m - 1, where horizontalCut[i] represents the cost to cut along the horizontal line i.
    verticalCut of size n - 1, where verticalCut[j] represents the cost to cut along the vertical line j.

In one operation, you can choose any piece of cake that is not yet a 1 x 1 square and perform one of the following cuts:

    Cut along a horizontal line i at a cost of horizontalCut[i].
    Cut along a vertical line j at a cost of verticalCut[j].

After the cut, the piece of cake is divided into two distinct pieces.

The cost of a cut depends only on the initial cost of the line and does not change.

Return the minimum total cost to cut the entire cake into 1 x 1 pieces.



Example 1:

Input: m = 3, n = 2, horizontalCut = [1,3], verticalCut = [5]

Output: 13

Explanation:

    Perform a cut on the vertical line 0 with cost 5, current total cost is 5.
    Perform a cut on the horizontal line 0 on 3 x 1 subgrid with cost 1.
    Perform a cut on the horizontal line 0 on 3 x 1 subgrid with cost 1.
    Perform a cut on the horizontal line 1 on 2 x 1 subgrid with cost 3.
    Perform a cut on the horizontal line 1 on 2 x 1 subgrid with cost 3.

The total cost is 5 + 1 + 1 + 3 + 3 = 13.

Example 2:

Input: m = 2, n = 2, horizontalCut = [7], verticalCut = [4]

Output: 15

Explanation:

    Perform a cut on the horizontal line 0 with cost 7.
    Perform a cut on the vertical line 0 on 1 x 2 subgrid with cost 4.
    Perform a cut on the vertical line 0 on 1 x 2 subgrid with cost 4.

The total cost is 7 + 4 + 4 = 15.



Constraints:

    1 <= m, n <= 10^5
    horizontalCut.length == m - 1
    verticalCut.length == n - 1
    1 <= horizontalCut[i], verticalCut[i] <= 10^3


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

#include "log.h"

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
typedef pair<int, int> IDX2D;

typedef long long iCOSTt;

typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    int d1;
    int d2;
    V1Int hCut;
    V1Int vCut;

    Input() {
        mID = inputID++;
    }
    Input(int d1, int d2, V1Int h1, V1Int h2) : d1(d1), d2(d2), hCut(h1), vCut(h2) {
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

    void clear() {
    }
    Data() {
        clear();
    }
};

class Solution {
public:

    long long minimumCost(int m, int n, vector<int>& horizontalCut, vector<int>& verticalCut) {

        map<iCOSTt, pair<int, int>> cutInfo;

        if (m == 1) return accumulate(verticalCut.begin(), verticalCut.end(), 0);
        if (n == 1) return accumulate(horizontalCut.begin(), horizontalCut.end(), 0);

        for (size_t i = 0; i < horizontalCut.size(); i++) {
            auto cCost = horizontalCut[i];
            if (cutInfo.find(cCost) == cutInfo.end()) cutInfo[cCost] = make_pair(0, 0);
            cutInfo[cCost].first++;
        }
        for (size_t i = 0; i < verticalCut.size(); i++) {
            auto cCost = verticalCut[i];
            if (cutInfo.find(cCost) == cutInfo.end()) cutInfo[cCost] = make_pair(0, 0);
            cutInfo[cCost].second++;
        }

        iCOSTt totalCost = 0;
        pair<iCOSTt, iCOSTt> curCuts = make_pair(1, 1);
        for (auto& [cost, cnt] : cutInfo | std::views::reverse) {

            LOG("hCutMap:" << cost << " options: " << cnt);
            LOG("\t cur cuts:" << curCuts << " totalCost:" << totalCost);

            int min_cuts = min(cnt.first, cnt.second);
            cnt.first -= min_cuts;
            cnt.second -= min_cuts;

            if (min_cuts != 0) {
                iCOSTt mult = min_cuts;
                mult -= (curCuts.first * (curCuts.first - 1)) / 2;
                mult -= (curCuts.second * (curCuts.second - 1)) / 2;
                curCuts.first += min_cuts;
                curCuts.second += min_cuts;
                mult += (curCuts.first * (curCuts.first - 1)) / 2;
                mult += (curCuts.second * (curCuts.second - 1)) / 2;
                LOG("\t min_cuts:" << min_cuts << " mult:" << mult);

                totalCost += cost * mult;
            }

            totalCost += cost * curCuts.second * cnt.first;
            totalCost += cost * curCuts.first * cnt.second;
            curCuts.first += cnt.first;
            curCuts.second += cnt.second;

        }
        return totalCost;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return minimumCost(nums.d1, nums.d2, nums.hCut, nums.vCut);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "InputID:" << ipObj.mID;
    out << " " << ipObj.d1 << "," << ipObj.d2 << "," << ipObj.hCut << "," << ipObj.vCut;
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

    nums.push_back(Input({ 3,2,{1,3},{5} }));
    expected.push_back(13);

    nums.push_back(Input({ 2,2, {7}, {4} }));
    expected.push_back(15);

    nums.push_back(Input({ 5,5, {5,2,7,9}, {3,7,2,1} }));
    expected.push_back(80);

    nums.push_back(Input({ 1,7, {}, {2,1,2,1,2,1} }));
    expected.push_back(9);

    nums.push_back(Input({ 6,3, {2,3,2,3,1}, {1,2} }));
    expected.push_back(28);

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

        cout << TEXT_LOG_PREFIX << "test loop: [" << i << "] ==> r:" << result << " vs e:" << expected[i] << endl;
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


