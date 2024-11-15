

/*
3134. Find the Median of the Uniqueness Array
Attempted
Hard
Topics
Companies
Hint

You are given an integer array nums. The uniqueness array of nums is the sorted array that contains the number of distinct elements of all the
subarrays
of nums. In other words, it is a sorted array consisting of distinct(nums[i..j]), for all 0 <= i <= j < nums.length.

Here, distinct(nums[i..j]) denotes the number of distinct elements in the subarray that starts at index i and ends at index j.

Return the median of the uniqueness array of nums.

Note that the median of an array is defined as the middle element of the array when it is sorted in non-decreasing order. If there are two choices for a median, the smaller of the two values is taken.



Example 1:

Input: nums = [1,2,3]

Output: 1

Explanation:

The uniqueness array of nums is [distinct(nums[0..0]), distinct(nums[1..1]), distinct(nums[2..2]), distinct(nums[0..1]), distinct(nums[1..2]), distinct(nums[0..2])] which is equal to [1, 1, 1, 2, 2, 3]. The uniqueness array has a median of 1. Therefore, the answer is 1.

Example 2:

Input: nums = [3,4,3,4,5]

Output: 2

Explanation:

The uniqueness array of nums is [1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3]. The uniqueness array has a median of 2. Therefore, the answer is 2.

Example 3:

Input: nums = [4,3,5,4]

Output: 2

Explanation:

The uniqueness array of nums is [1, 1, 1, 1, 2, 2, 2, 3, 3, 3]. The uniqueness array has a median of 2. Therefore, the answer is 2.



Constraints:

    1 <= nums.length <= 105
    1 <= nums[i] <= 105


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

typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    INPUT_TYPE mData1;

    Input() {
        mID = inputID++;
    }
    Input(INPUT_TYPE d1) {
        this->mData1 = d1;
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

    int medianOfUniquenessArray(vector<int>& nums) {

        if (nums.size() == 1)
            return 1;

        vector<pair<int, int>> uniqArrWithFr;
        map<int, int> resultFreqMap;
        int totalOptions = (nums.size() * (nums.size() + 1)) / 2;
        totalOptions = (totalOptions + 1) / 2;

        LOG("@start totalOptions=" << totalOptions << " freqMap=" << resultFreqMap);

        for (size_t i = 0; i < nums.size(); i++) {
            uniqArrWithFr.push_back({ nums[i], 1 });
            auto j = i + 1;
            for (; j < nums.size() && nums[i] == nums[j];uniqArrWithFr.back().second++, j++) {}
            i = j - 1;
            auto curOptions = (uniqArrWithFr.back().second * (uniqArrWithFr.back().second + 1)) / 2;
            resultFreqMap[1] += curOptions;
            totalOptions -= curOptions;
        }
        LOG("freqArr=" << uniqArrWithFr);
        LOG("@ws:1 totalOptions=" << totalOptions << " freqMap=" << resultFreqMap);

        int maxFreq = uniqArrWithFr.size();
        for (size_t windowSize = 2; windowSize < uniqArrWithFr.size(); windowSize += 1) {
            if (totalOptions <= 0) {
                LOG("break @ws:" << windowSize << " totalOptions=" << totalOptions << " freqMap = " << resultFreqMap);
                break;
            }

            LOG("freqArr=" << uniqArrWithFr);
            LOG("@ws:" << windowSize << " totalOptions=" << totalOptions << " freqMap = " << resultFreqMap);
            int startIdx = 0;
            auto endIdx = startIdx;
            map<int, int> curWindowUniqNos;

            for (;endIdx < maxFreq;) {
                // if (totalOptions <= 0) break;
                if (curWindowUniqNos.size() == windowSize) {
                }
                else {
                    while (endIdx < maxFreq) {
                        curWindowUniqNos[uniqArrWithFr[endIdx].first]++;
                        if (curWindowUniqNos.size() == windowSize) break;
                        if (endIdx == maxFreq - 1) break;
                        endIdx++;
                    }
                }
                if (curWindowUniqNos.size() < windowSize) break;
                LOG("\twindow found @[" << startIdx << "," << endIdx << "] freqMap = " << resultFreqMap);

                do {
                    auto startoptions = uniqArrWithFr[startIdx].second - 1;
                    auto endoptions = uniqArrWithFr[endIdx].second;
                    if (uniqArrWithFr[startIdx].first == uniqArrWithFr[endIdx].first) {
                        startoptions--;
                        endoptions++;
                    }

                    auto currOptions = 1 + (startoptions * endoptions);
                    resultFreqMap[windowSize] += currOptions;
                    totalOptions -= currOptions;

                    if (endIdx == maxFreq - 1 ||
                        curWindowUniqNos.find(uniqArrWithFr[endIdx + 1].first) == curWindowUniqNos.end()) {
                        break;
                    }
                    endIdx++;
                    curWindowUniqNos[uniqArrWithFr[endIdx].first]++;

                } while (true);

                if (curWindowUniqNos.size() < windowSize) break;
                LOG("\texpanded to" << "[" << startIdx << "," << endIdx << "] freqMap = " << resultFreqMap);
                //if (totalOptions <= 0) break;

                do {
                    LOG("current unique numbers with cnt:" << curWindowUniqNos);
                    curWindowUniqNos[uniqArrWithFr[startIdx].first]--;
                    if (curWindowUniqNos[uniqArrWithFr[startIdx].first] == 0) {
                        curWindowUniqNos.erase(uniqArrWithFr[startIdx].first);
                        startIdx++;
                        endIdx++;
                        break;
                    }
                    startIdx++;
                    if (startIdx >= endIdx) break;

                    auto currOptions = 0;
                    if (uniqArrWithFr[startIdx].first == uniqArrWithFr[endIdx].first) {
                        currOptions = (uniqArrWithFr[startIdx].second * uniqArrWithFr[endIdx].second);
                        if (curWindowUniqNos[uniqArrWithFr[startIdx].first] > 2)
                            currOptions += 1;
                    }
                    else {
                        currOptions = (uniqArrWithFr[startIdx].second * (uniqArrWithFr[endIdx].second));
                        if (curWindowUniqNos[uniqArrWithFr[startIdx].first] > 1) {
                            currOptions += 1;
                        }
                        if (curWindowUniqNos[uniqArrWithFr[endIdx].first] > 1) {
                            currOptions += 1;
                        }
                    }

                    resultFreqMap[windowSize] += currOptions;
                    totalOptions -= currOptions;


                } while (curWindowUniqNos.size() == windowSize);
                LOG("\t shrinked to" << "[" << startIdx << "," << endIdx << "] freqMap = " << resultFreqMap);
            }
        }

        LOG("At end: freqMap=" << resultFreqMap);
        return resultFreqMap.rbegin()->first;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return medianOfUniquenessArray(nums.mData1);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
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

    nums.push_back(Input({ 1,2,3 }));
    expected.push_back(1);

    nums.push_back(Input({ 3,4,3,4 }));
    expected.push_back(2);

    nums.push_back(Input({ 3,4,3,4,5 }));
    expected.push_back(2);

    nums.push_back(Input({ 4,3,5,4 }));
    expected.push_back(2);

    nums.push_back(Input({ 1,2,3,4,5 }));
    expected.push_back(2);

    nums.push_back(Input({ 90, 26, 34, 90, 90, 90, 90, 90 }));
    expected.push_back(1);

    nums.push_back(Input({ 57, 40, 55, 49, 49, 49, 49 }));
    expected.push_back(2);

    nums.push_back(Input({ 91, 64, 76, 18, 61, 55, 46, 93, 65, 99 }));
    expected.push_back(4);

    nums.push_back(Input({ 15, 86, 69, 20, 20, 69, 86, 20, 86 }));
    expected.push_back(3);

    nums.push_back(Input({ 99, 73, 14, 84, 14, 73, 14, 14, 14 }));
    expected.push_back(2);

    nums.push_back(Input({ 39, 23, 10, 81, 91, 10, 81, 10, 23, 91, 10, 10, 91 }));
    expected.push_back(3);

    nums.push_back(Input({ 94, 36, 69, 69, 36, 69, 94, 36, 94 }));
    expected.push_back(2);

    nums.push_back(Input({ 30, 23, 61, 12, 23, 23, 30 }));
    expected.push_back(2);

    //nums.push_back(Input(GenerateInput(1, 1000, 1000)));
    //expected.push_back(255);

    cout << endl << TEXT_LOG_PREFIX << "test:" << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_ST
    for (size_t i = __D_CASE_ID; i <= __D_CASE_ID; i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "test loop_id: " << i << " ==>\t" << result << " vs e:" << expected[i] << endl;
        logtime("Time: ", starttime, endtime);
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
    cout << "***Start***" << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG
    test2(2);
#endif

    cout << "***END***" << endl;
    return 0;
}



