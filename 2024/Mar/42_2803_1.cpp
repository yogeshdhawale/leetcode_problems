
/*
42. Trapping Rain Water
Hard
Topics
Companies

Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it can trap after raining.



Example 1:

Input: height = [0,1,0,2,1,0,1,3,2,1,2,1]
Output: 6
Explanation: The above elevation map (black section) is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped.

Example 2:

Input: height = [4,2,0,3,2,5]
Output: 9



Constraints:

    n == height.length
    1 <= n <= 2 * 104
    0 <= height[i] <= 105


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

    void clear() {
    }
    Data() {
        clear();
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    int trap(vector<int>& height) {
        int size = height.size();
        int i = 0;
        for (; i < size - 1; i++) {
            if (height[i] <= height[i + 1]) {
                continue;
            }
            break;
        }

        for (int j = size - 1;j > i;j--) {
            if (height[j] <= height[j - 1]) {
                size--;
                continue;
            }
            break;
        }
        size--;

        LOG("out of " << height.size() << " starting from " << i << " to " << size);

        int result = 0;
        int wallIdx = i;

        for (i = wallIdx + 1;i < size;i++) {

            if (height[wallIdx] <= height[i]) {
                wallIdx = i;
                continue;
            }

            int sum = 0;
            int wallh = height[wallIdx];

            LOG("Wall is @" << wallIdx << " with height:" << wallh);
            for (;i < size && wallh >= height[i];i++) {
                sum += height[i];
                continue;
            }

            if (i == size && wallh > height[i]) {
                break;
            }

            int newh = min(wallh, height[i]);

            while (height[wallIdx + 1] > newh) {
                sum -= height[wallIdx + 1];
                wallIdx++;
            }
            result += calculate(wallIdx + 1, i, sum, newh);
            wallIdx = i;
        }

        vector<int> subarray;
        copy(height.begin() + wallIdx, height.begin() + size + 1, back_inserter(subarray));
        if (subarray.size() == 1) {
            return result;
        }
        reverse(subarray.begin(), subarray.end());

        result += trap(subarray);

        return result;
    }


    int calculate(int start, int end, int sum, int height) {
        LOG("start:" << start << " end:" << end << " sum:" << sum << " height:" << height << " result:" << ((end - start) * height) - sum);
        return ((end - start) * height) - sum;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return trap(nums.nums);
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


void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 0,1,0,2,1,0,1,3,2,1,2,1 }));
    expected.push_back(6);

    nums.push_back(Input({ 4,2,0,3,2,5 }));
    expected.push_back(9);

    nums.push_back(Input({ 2,1,0,2 }));
    expected.push_back(3);

    nums.push_back(Input({ 5,4,1,2 }));
    expected.push_back(1);

    nums.push_back(Input({ 4, 3, 3, 9, 3, 0, 9, 2, 8, 3 }));
    expected.push_back(23);

    nums.push_back(Input({ 8,2,8,9,0,1,7,7,9 }));
    expected.push_back(27);

    nums.push_back(Input({ 9,6,8,8,5,6,3 }));
    expected.push_back(3);

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

        cout << TEXT_LOG_PREFIX << "*****Time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
            << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
            << endl;
        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
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
    cout << TEXT_LOG_PREFIX << "Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;

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


