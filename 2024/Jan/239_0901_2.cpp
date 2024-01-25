/*

239. Sliding Window Maximum
Hard
Topics
Companies
Hint

You are given an array of integers nums, there is a sliding window of size k which is moving from the very left of the array to the very right. You can only see the k numbers in the window. Each time the sliding window moves right by one position.

Return the max sliding window.



Example 1:

Input: nums = [1,3,-1,-3,5,3,6,7], k = 3
Output: [3,3,5,5,6,7]
Explanation:
Window position                Max
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7

Example 2:

Input: nums = [1], k = 1
Output: [1]



Constraints:

    1 <= nums.length <= 105
    -104 <= nums[i] <= 104
    1 <= k <= nums.length


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <queue>

using namespace std;

#define __DEBUG_LOG 0

template <typename T>
void printVector(vector<T> &nums, string msg = "")
{
    cout << msg << ": ";
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << ", ";
    }
    cout << endl;
}

void LOG(string msg)
{
    if (__DEBUG_LOG)
        cout << msg << endl;
}

class Position
{
    vector<queue<int>> index;

public:
    Position()
    {
        index.resize(20002);
    }
    void setPosition(int value, int idx, int start)
    {
        index[value + 10000].push(idx);
        while (index[value + 10000].front() < start)
            index[value + 10000].pop();
    }
    int getPosition(int value, int start)
    {
        while (!index[value + 10000].empty())
        {
            if (index[value + 10000].front() < start)
            {
                index[value + 10000].pop();
            }
            else
                break;
        }
        return index[value + 10000].front();
    }
};

class Solution
{
private:
    Position position;
    vector<int> heap;

public:
    Solution()
    {
    }

    int getMaxElement(int start)
    {
        int max = INT32_MIN;
        while (true)
        {
            max = heap.front();
            int pos = position.getPosition(max, start);
            if (pos != -1)
                break;
            pop_heap(heap.begin(), heap.end());
            heap.pop_back();
        }
        return max;
    }
    vector<int> maxSlidingWindow(vector<int> &nums, int k)
    {
        int len = nums.size();

        if (k == 1 || len == 1)
        {
            copy(nums.begin(), nums.end(), back_inserter(heap));
            return heap;
        }

        for (int i = 0; i < k; i++)
        {
            position.setPosition(nums[i], i, 0);
            heap.push_back(nums[i]);
        }

        make_heap(heap.begin(), heap.end());

        vector<int> res;

        for (int i = k; i < len; i++)
        {
            res.push_back(getMaxElement(i - k));
            position.setPosition(nums[i], i, i - k);
            heap.push_back(nums[i]);
            push_heap(heap.begin(), heap.end());
        }
        res.push_back(getMaxElement(len - k));

        return res;
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<pair<vector<int>, int>>({
        {{1, 3, -1, -3, 5, 3, 6, 7}, 3},
        {{1}, 1},
        {{-7, -8, 7, 5, 7, 1, 6, 0}, 4},
        {{7, 2, 4}, 2},
        {{1, 3, 1, 2, 0, 5}, 3},
        {{-6, -10, -7, -1, -9, 9, -8, -4, 10, -5, 2, 9, 0, -7, 7, 4, -2, -10, 8, 7}, 7},
        {{7, 2, 4}, 2},
    });

    auto expected = vector<vector<int>>({
        {3, 3, 5, 5, 6, 7},
        {1},
        {7, 7, 7, 7, 7},
        {7, 4},
        {3, 3, 2, 5},
        {9, 9, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 8, 8},
        {7, 4},
    });

    for (size_t i = 0; i < nums.size(); i++)
    // for (size_t i = nums.size() - 1; i < nums.size(); i++)
    {
        Solution s;

        printVector(nums[i].first, "\ntest for :" + to_string(i));
        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.maxSlidingWindow(nums[i].first, nums[i].second);
        auto endtime = std::chrono::high_resolution_clock::now();
        cout << "Time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << endl;

        printVector(result, "result  ");
        printVector(expected[i], "expected");
        for (size_t j = 0; j < expected[i].size(); j++)
        {
            assert(result[j] == expected[i][j]);
        }
    }
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    // test2(2);
    // test3(3);

    return 0;
}