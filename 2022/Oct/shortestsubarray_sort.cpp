
/*

1574. Shortest Subarray to be Removed to Make Array Sorted
Medium

1229

44

Add to List

Share
Given an integer array arr, remove a subarray (can be empty) from arr such that the remaining elements in arr are non-decreasing.

Return the length of the shortest subarray to remove.

A subarray is a contiguous subsequence of the array.



Example 1:

Input: arr = [1,2,3,10,4,2,3,5]
Output: 3
Explanation: The shortest subarray we can remove is [10,4,2] of length 3. The remaining elements after that will be [1,2,3,3,5] which are sorted.
Another correct solution is to remove the subarray [3,10,4].
Example 2:

Input: arr = [5,4,3,2,1]
Output: 4
Explanation: Since the array is strictly decreasing, we can only keep a single element. Therefore we need to remove a subarray of length 4, either [5,4,3,2] or [4,3,2,1].
Example 3:

Input: arr = [1,2,3]
Output: 0
Explanation: The array is already non-decreasing. We do not need to remove any elements.


Constraints:

1 <= arr.length <= 105
0 <= arr[i] <= 109

*/

// TODO:  heap heap buffer overflow error in the leetcode

#include <iostream>
#include <vector>

using namespace std;

class Solution
{
public:
    int findLengthOfShortestSubarray(vector<int> &arr)
    {

        int size = arr.size();

        int s = -1;
        int e = -1;

        for (int i = 0; i < size - 1; i++)
            if (arr[i] > arr[i + 1])
            {
                s = i;
                break;
            }
        int left = s+1;
        for (int j = s; j > 0; j--)
            if (arr[j] == arr[j - 1])
                s--;

        for (int i = size - 1; i > 0; i--)
            if (arr[i] < arr[i - 1])
            {
                e = i;
                break;
            }
        int right= size - e;

        for (int j = e; j < size; j++)
            if (arr[j] == arr[j + 1])
                e++;

        cout << "(" << s << "," << e << ") , lr:(" <<  left << "," << right << ")" << endl;

        // array s to e is invalid, can we shrink the array further?
        if (s == -1 && e == -1)
            return 0;
        else if (s == -1 || e == -1)
            return 1;

        int result = e - s + 1;

        if (result == size)
            return (left < right) ? right : left;
        return result;
    }
};

int main()
{
    //if (false)
    {
        cout << "Test 1:" << endl;
        Solution s;

        vector<int> data = {1, 2, 3, 10, 4, 2, 3, 5};
        int result = s.findLengthOfShortestSubarray(data);
        cout << "Result is (expected=3):" << result << endl;
    }
    if (false)
    {
        cout << "Test 2:" << endl;
        Solution s;

        vector<int> data = {1, 2, 3};
        int result = s.findLengthOfShortestSubarray(data);
        cout << "Result is (expected=0):" << result << endl;
    }
    if (false)
    {
        cout << "Test 3:" << endl;
        Solution s;

        vector<int> data = {5, 4, 3, 2, 1};
        int result = s.findLengthOfShortestSubarray(data);
        cout << "Result is (expected=4):" << result << endl;
    }
    //if (false)
    {
        cout << "Test 4:" << endl;
        Solution s;

        vector<int> data = {2, 2, 2, 1, 1, 1};
        int result = s.findLengthOfShortestSubarray(data);
        cout << "Result is (expected=3):" << result << endl;
    }
}