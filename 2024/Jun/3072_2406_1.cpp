
/*
3072. Distribute Elements Into Two Arrays II
Hard
Topics
Companies
Hint

You are given a 1-indexed array of integers nums of length n.

We define a function greaterCount such that greaterCount(arr, val) returns the number of elements in arr that are strictly greater than val.

You need to distribute all the elements of nums between two arrays arr1 and arr2 using n operations. In the first operation, append nums[1] to arr1. In the second operation, append nums[2] to arr2. Afterwards, in the ith operation:

    If greaterCount(arr1, nums[i]) > greaterCount(arr2, nums[i]), append nums[i] to arr1.
    If greaterCount(arr1, nums[i]) < greaterCount(arr2, nums[i]), append nums[i] to arr2.
    If greaterCount(arr1, nums[i]) == greaterCount(arr2, nums[i]), append nums[i] to the array with a lesser number of elements.
    If there is still a tie, append nums[i] to arr1.

The array result is formed by concatenating the arrays arr1 and arr2. For example, if arr1 == [1,2,3] and arr2 == [4,5,6], then result = [1,2,3,4,5,6].

Return the integer array result.



Example 1:

Input: nums = [2,1,3,3]
Output: [2,3,1,3]
Explanation: After the first 2 operations, arr1 = [2] and arr2 = [1].
In the 3rd operation, the number of elements greater than 3 is zero in both arrays. Also, the lengths are equal, hence, append nums[3] to arr1.
In the 4th operation, the number of elements greater than 3 is zero in both arrays. As the length of arr2 is lesser, hence, append nums[4] to arr2.
After 4 operations, arr1 = [2,3] and arr2 = [1,3].
Hence, the array result formed by concatenation is [2,3,1,3].

Example 2:

Input: nums = [5,14,3,1,2]
Output: [5,3,1,2,14]
Explanation: After the first 2 operations, arr1 = [5] and arr2 = [14].
In the 3rd operation, the number of elements greater than 3 is one in both arrays. Also, the lengths are equal, hence, append nums[3] to arr1.
In the 4th operation, the number of elements greater than 1 is greater in arr1 than arr2 (2 > 1). Hence, append nums[4] to arr1.
In the 5th operation, the number of elements greater than 2 is greater in arr1 than arr2 (2 > 1). Hence, append nums[5] to arr1.
After 5 operations, arr1 = [5,3,1,2] and arr2 = [14].
Hence, the array result formed by concatenation is [5,3,1,2,14].

Example 3:

Input: nums = [3,3,3,3]
Output: [3,3,3,3]
Explanation: At the end of 4 operations, arr1 = [3,3] and arr2 = [3,3].
Hence, the array result formed by concatenation is [3,3,3,3].



Constraints:

    3 <= n <= 105
    1 <= nums[i] <= 109



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
#include <complex>
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
#define MY_MAX_VAL INT32_MAX

#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef V1Int OUTPUT_TYPE;

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

struct seg_node {
    int val;

    seg_node* left;
    seg_node* right;
    seg_node* parent;

    int leftCount;
    int rightCount;
    int count;

    void Init() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        leftCount = 0;
        rightCount = 0;
        count = 1;
    }

    seg_node(int val) : val(val) {
        Init();
    }
    seg_node(int val, seg_node* parent) : seg_node(val) {
        this->parent = parent;
    }

    void Add(int num) {
        seg_node* curNode = this;
        seg_node* newNode = nullptr;
        while (curNode) {

            if (num == curNode->val) {
                curNode->count++;
                newNode = curNode;
                break;
            }

            if (num > curNode->val) {
                if (curNode->right == nullptr) {
                    curNode->right = new seg_node(num, curNode);
                    newNode = curNode->right;
                    break;
                }
                else
                    curNode = curNode->right;
            }
            else {
                if (curNode->left == nullptr) {
                    curNode->left = new seg_node(num, curNode);
                    newNode = curNode->left;
                    break;
                }
                else
                    curNode = curNode->left;
            }
        }

        // update parent counts
        seg_node* parentNode = newNode->parent;
        while (parentNode != nullptr) {
            if (parentNode->left == newNode) {
                parentNode->leftCount++;
            }
            else {
                parentNode->rightCount++;
            }
            newNode = parentNode;
            parentNode = parentNode->parent;
        }
    }

    int greaterCount(int num) {

        seg_node* curNode = this;
        while (curNode) {
            if (num == curNode->val)
                break;

            if (num > curNode->val) {
                if (curNode->right == nullptr) break;
                curNode = curNode->right;
            }
            else {
                if (curNode->left == nullptr) break;
                curNode = curNode->left;
            }
        }
        // add all rightCounts
        int count = 0;
        while (curNode != nullptr) {
            if (curNode->val > num)
                count += curNode->count + curNode->rightCount;
            else if (curNode->val == num)
                count += curNode->rightCount;
            else if (curNode->val < num) {}

            curNode = curNode->parent;
        }
        return count;
    }
};

struct Data {

    V1Int mResult;
    seg_node* mSegTreeRoot;
    void clear() {
        mResult.clear();
    }
    Data(int val) {
        clear();
        mSegTreeRoot = new seg_node(val);
        mResult.push_back(val);
    }

    void Add(int num) {
        mSegTreeRoot->Add(num);
        mResult.push_back(num);
    }

    int getSize() {
        return mResult.size();
    }
    int greaterCount(int num) {
        return mSegTreeRoot->greaterCount(num);
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    vector<int> resultArray(vector<int>& nums) {

        V1Int arr1;
        V1Int arr2;

        Data d1(nums[0]);
        Data d2(nums[1]);

        for (size_t i = 2; i < nums.size(); i++) {

            auto c1 = d1.greaterCount(nums[i]);
            auto c2 = d2.greaterCount(nums[i]);

            LOG("c1:" << c1 << " c2:" << c2);
            if (c1 != c2) {
                if (c1 > c2)
                    d1.Add(nums[i]);
                else
                    d2.Add(nums[i]);
                continue;
            }

            auto s1 = d1.getSize();
            auto s2 = d2.getSize();

            LOG("s1:" << s1 << " s2:" << s2);
            if (s1 != s2) {
                if (s1 < s2)
                    d1.Add(nums[i]);
                else
                    d2.Add(nums[i]);
                continue;
            }

            d1.Add(nums[i]);
        }

        LOG("d1:" << d1.mResult);
        LOG("d2:" << d2.mResult);

        d1.mResult.insert(d1.mResult.end(), d2.mResult.begin(), d2.mResult.end());
        return d1.mResult;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return resultArray(nums.nums);
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
/**
 * Generates an array of random integers within a specified range.
 *
 * @param seed The seed value for the random number generator.
 * @param size The size of the array to be generated.
 * @param max The maximum value for the random integers.
 *
 * @return A vector of random integers.
 */
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

    nums.push_back(Input({ 2,1,3,3 }));
    expected.push_back({ 2,3,1,3 });

    nums.push_back(Input({ 5,14,3,1,2 }));
    expected.push_back({ 5,3,1,2,14 });

    nums.push_back(Input({ 3,3,3,3 }));
    expected.push_back({ 3,3,3,3 });

    nums.push_back(Input({ 2, 38, 2 }));
    expected.push_back({ 2, 38, 2 });

    nums.push_back(Input({ 50,812,620 }));
    expected.push_back({ 50,812,620 });

    nums.push_back(Input(GenerateInput(5, pow(10, 5), pow(10, 9))));
    expected.push_back({ });

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


