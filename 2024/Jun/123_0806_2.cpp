
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



class Solution {
public:
    int maxProfit(vector<int>& prices) {

        int n = prices.size();
        if (n < 2) return 0;

        int i = 1;
        while (prices[i - 1] > prices[i]) {
            ++i;
            if (i == n) return 0;
        }


        int buy = prices[i - 1];
        int profit = prices[i] - buy;

        int secondBuy = buy;
        int secondProfit = 0;

        for (; i < n; ++i) {
            LOG("@" << i << ":" << prices[i] << " @:" << buy << " profit:" << profit << " @:" << secondBuy << " secondProfit:" << secondProfit);

            if (prices[i] < buy)
                buy = prices[i];

            if (profit < prices[i] - buy)
                profit = prices[i] - buy;

            secondBuy = min(secondBuy, prices[i] - profit);
            secondProfit = max(secondProfit, prices[i] - secondBuy);

        }

        LOG("@end:" << " @:" << buy << " profit:" << profit << " @:" << secondBuy << " secondProfit:" << secondProfit);
        return secondProfit;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxProfit(nums.nums);
    }
};

/*
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        int n = prices.size();
        if (n < 2) return 0;

        int firstBuy = prices[0];
        int secondBuy = prices[0];
        int firstProfit = 0;
        int secondProfit = 0;
        for (int i = 0; i < n; ++i) {

            LOG("@i:" << i << ":" << prices[i] << " firstBuy:" << firstBuy << " secondBuy:" << secondBuy << " firstProfit:" << firstProfit << " secondProfit:" << secondProfit);
            firstBuy = min(firstBuy, prices[i]);
            firstProfit = max(firstProfit, prices[i] - firstBuy);

            secondBuy = min(secondBuy, prices[i] - firstProfit);
            secondProfit = max(secondProfit, prices[i] - secondBuy);
        }

        LOG("@end:" << " firstBuy:" << firstBuy << " secondBuy:" << secondBuy << " firstProfit:" << firstProfit << " secondProfit:" << secondProfit);
        return secondProfit;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxProfit(nums.nums);
    }
};
*/

/****************************************************************************/



ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */


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

    nums.push_back(Input({ 0,4,2,5,2,12,6,9 }));
    expected.push_back(15);

    nums.push_back(Input({ 0,4,2,5,2,12,6,10 }));
    expected.push_back(16);

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


