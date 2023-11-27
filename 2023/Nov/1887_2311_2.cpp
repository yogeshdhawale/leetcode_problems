/*

1887. Reduction Operations to Make the Array Elements Equal
Medium
Topics
Companies
Hint

Given an integer array nums, your goal is to make all elements in nums equal. To complete one operation, follow these steps:

    Find the largest value in nums. Let its index be i (0-indexed) and its value be largest. If there are multiple elements with the largest value, pick the smallest i.
    Find the next largest value in nums strictly smaller than largest. Let its value be nextLargest.
    Reduce nums[i] to nextLargest.

Return the number of operations to make all elements in nums equal.



Example 1:

Input: nums = [5,1,3]
Output: 3
Explanation: It takes 3 operations to make all elements in nums equal:
1. largest = 5 at index 0. nextLargest = 3. Reduce nums[0] to 3. nums = [3,1,3].
2. largest = 3 at index 0. nextLargest = 1. Reduce nums[0] to 1. nums = [1,1,3].
3. largest = 3 at index 2. nextLargest = 1. Reduce nums[2] to 1. nums = [1,1,1].

Example 2:

Input: nums = [1,1,1]
Output: 0
Explanation: All elements in nums are already equal.

Example 3:

Input: nums = [1,1,2,2,3]
Output: 4
Explanation: It takes 4 operations to make all elements in nums equal:
1. largest = 3 at index 4. nextLargest = 2. Reduce nums[4] to 2. nums = [1,1,2,2,2].
2. largest = 2 at index 2. nextLargest = 1. Reduce nums[2] to 1. nums = [1,1,1,2,2].
3. largest = 2 at index 3. nextLargest = 1. Reduce nums[3] to 1. nums = [1,1,1,1,2].
4. largest = 2 at index 4. nextLargest = 1. Reduce nums[4] to 1. nums = [1,1,1,1,1].



Constraints:

    1 <= nums.length <= 5 * 104
    1 <= nums[i] <= 5 * 104


*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 0

void printVector(vector<int> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }
    cout << endl;
}

class Solution
{
private:
    map<int, int> nodes; // int and its occurences

public:
    int reductionOperations(vector<int> &nums)
    {
        for (auto x : nums)
        {
            nodes[x]++;
        }

        int result = 0;
        auto top = nodes.rbegin();
        auto next = top;
        next++;

        for (; next != nodes.rend(); next++)
        {
            int diff = top->first - next->first;

            if (__DEBUG_LOG)
                cout << "Processing " << top->first << " " << next->first << " with diff=" << diff << endl;

            result += top->second;
            next->second += top->second;
            top = next;
        }
        return result;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({5, 1, 3});

    auto expected = 3;

    auto result = s.reductionOperations(nums);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

void test2(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto nums = vector<int>({1, 1, 2, 2, 3});

    auto expected = 4;

    auto result = s.reductionOperations(nums);

    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{

    cout << "1887. Reduction Operations to Make the Array Elements Equal" << endl;
    test1(1);
    test2(2);
    // test3(3);

    return 0;
}