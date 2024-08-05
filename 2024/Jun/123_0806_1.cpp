
/*
123. Best Time to Buy and Sell Stock III
Hard
Topics
Companies

You are given an array prices where prices[i] is the price of a given stock on the ith day.

Find the maximum profit you can achieve. You may complete at most two transactions.

Note: You may not engage in multiple transactions simultaneously (i.e., you must sell the stock before you buy again).



Example 1:

Input: prices = [3,3,5,0,0,3,1,4]
Output: 6
Explanation: Buy on day 4 (price = 0) and sell on day 6 (price = 3), profit = 3-0 = 3.
Then buy on day 7 (price = 1) and sell on day 8 (price = 4), profit = 4-1 = 3.

Example 2:

Input: prices = [1,2,3,4,5]
Output: 4
Explanation: Buy on day 1 (price = 1) and sell on day 5 (price = 5), profit = 5-1 = 4.
Note that you cannot buy on day 1, buy on day 2 and sell them later, as you are engaging multiple transactions at the same time. You must sell before buying again.

Example 3:

Input: prices = [7,6,4,3,1]
Output: 0
Explanation: In this case, no transaction is done, i.e. max profit = 0.



Constraints:

    1 <= prices.length <= 105
    0 <= prices[i] <= 105


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

typedef int IDX;
typedef int PRICE;
typedef int PROFIT;

static int inputID = 1;
struct Input
{
    int id;
    V1Int nums;

    Input() {
        id = inputID++;
    }
    Input(V1Int nums) {
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

struct Data {

    vector<int>& prices;

    deque<IDX> minPriceIdxArray;
    map<PROFIT, map<IDX, IDX>> maxProfitMap;

    void clear() {
        minPriceIdxArray.clear();
        maxProfitMap.clear();
    }
    Data(vector<int>& prices) : prices(prices) {
        clear();
    }

    void AddMinPriceIdx(IDX minIdx) {

        if (prices[minIdx] > prices[minPriceIdxArray.front()])
            return;
        minPriceIdxArray.push_front(minIdx);
    }

    void AddOptions(IDX maxIdx, IDX minIdx) {
        LOG("AddOptions idx:[" << maxIdx << "," << minIdx << "] vals:("
            << prices[maxIdx] << ", " << prices[minIdx] << ") profit:" << prices[maxIdx] - prices[minIdx]);

        PROFIT profit = prices[maxIdx] - prices[minIdx];
        maxProfitMap[profit][maxIdx] = minIdx;

        AddMinPriceIdx(minIdx);

        for (int i = 0; i < minPriceIdxArray.size(); ++i) {
            PROFIT profit = prices[maxIdx] - prices[minPriceIdxArray[i]];
            maxProfitMap[profit][maxIdx] = minPriceIdxArray[i];
            if (i == 1) break;
        }
    }

    bool isRangeOverLaps(const pair<IDX, IDX>& range1, const pair<IDX, IDX>& range2) {
        return (range1.first <= range2.second && range2.first <= range1.second);
    }
    int getMaxProfit(int profit, int maxIdx, int minIdx) {
        return profit;
    }

    int getMaxProfit() {
        int maxProfit = 0;
        for (auto ritr = maxProfitMap.rbegin(); ritr != maxProfitMap.rend(); ++ritr) {
            auto profit = ritr->first;
            LOG("maxProfit:" << maxProfit << " profit:" << profit);
            for (auto& range : ritr->second) {
                auto curprofit = getMaxProfit(profit, range.first, range.second);
                if (curprofit > maxProfit) maxProfit = curprofit;
            }
        }
        return maxProfit;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:
    int maxProfit(vector<int>& prices) {

        int n = prices.size();
        if (n < 2) return 0;

        Data data(prices);
        int minPriceIdx = 0;
        int minPrice = prices[minPriceIdx];
        data.AddMinPriceIdx(minPriceIdx);

        int i = 1;

        int maxPriceIdx = minPriceIdx;
        int maxPrice = minPrice;

        for (; i < n; ++i) {

            LOG("@" << i << " price: " << prices[i] << " minPrice: " << minPrice << " minPriceIdx: " << minPriceIdx);
            LOG(data);

            maxPriceIdx = minPriceIdx;
            maxPrice = minPrice;

            for (;i < n;i++) {
                auto curPrice = prices[i];

                if (curPrice < maxPrice) break;

                if (maxPrice < curPrice) {
                    maxPrice = curPrice;
                    maxPriceIdx = i;
                }
            }

            if (i == n) break;

            data.AddOptions(maxPriceIdx, minPriceIdx);

            minPriceIdx = i;
            minPrice = prices[minPriceIdx];
        }

        data.AddOptions(maxPriceIdx, minPriceIdx);
        LOG("@end: " << data);

        int res = 0;
        res = data.getMaxProfit();
        return res;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxProfit(nums.nums);
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
    out << " minPriceIdxArray:" << d.minPriceIdxArray;
    out << " maxProfitMap:" << d.maxProfitMap;
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

    nums.push_back(Input({ 3,3,5,0,0,3,1,4 }));
    expected.push_back(6);

    nums.push_back(Input({ 1,2,4,3 }));
    expected.push_back(3);

    nums.push_back(Input({ 1,2,3,4,5 }));
    expected.push_back(4);

    nums.push_back(Input({ 7,6,4,3,1 }));
    expected.push_back(0);

    nums.push_back(Input({ 0,2,2,2,2,5 }));
    expected.push_back(5);

    nums.push_back(Input({ 0,5,4,6 }));
    expected.push_back(7);

    nums.push_back(Input({ 0,4,2,5,2 }));
    expected.push_back(7);

    nums.push_back(Input({ 0,4,2,5,2,12 }));
    expected.push_back(15);

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


