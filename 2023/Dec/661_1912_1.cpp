/*
661. Image Smoother
Easy
Topics
Companies

An image smoother is a filter of the size 3 x 3 that can be applied to each cell of an image by rounding down the average of the cell and the eight surrounding cells (i.e., the average of the nine cells in the blue smoother). If one or more of the surrounding cells of a cell is not present, we do not consider it in the average (i.e., the average of the four cells in the red smoother).

Given an m x n integer matrix img representing the grayscale of an image, return the image after applying the smoother on each cell of it.



Example 1:

Input: img = [[1,1,1],[1,0,1],[1,1,1]]
Output: [[0,0,0],[0,0,0],[0,0,0]]
Explanation:
For the points (0,0), (0,2), (2,0), (2,2): floor(3/4) = floor(0.75) = 0
For the points (0,1), (1,0), (1,2), (2,1): floor(5/6) = floor(0.83333333) = 0
For the point (1,1): floor(8/9) = floor(0.88888889) = 0

Example 2:

Input: img = [[100,200,100],[200,50,200],[100,200,100]]
Output: [[137,141,137],[141,138,141],[137,141,137]]
Explanation:
For the points (0,0), (0,2), (2,0), (2,2): floor((100+200+200+50)/4) = floor(137.5) = 137
For the points (0,1), (1,0), (1,2), (2,1): floor((200+200+50+200+100+100)/6) = floor(141.666667) = 141
For the point (1,1): floor((50+200+200+200+200+100+100+100+100)/9) = floor(138.888889) = 138



Constraints:

    m == img.length
    n == img[i].length
    1 <= m, n <= 200
    0 <= img[i][j] <= 255

*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <list>

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
void printVector(vector<vector<int>> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        printVector(nums[i]);
    }
}

class Solution
{
public:
    bool valid(vector<vector<int>> &img, int i, int j)
    {
        if (i < 0 || i >= (int)img.size() || j < 0 || j >= (int)img[i].size())
        {
            return false;
        }
        return true;
    }

    int get(vector<vector<int>> &img, int i, int j)
    {
        return img[i][j];
    }

    pair<int, int> getColSum(vector<vector<int>> &img, int i, int j)
    {
        int sum = 0;
        float count = 0;
        if (valid(img, i - 1, j))
        {
            sum += get(img, i - 1, j);
            count++;
        }
        if (valid(img, i, j))
        {
            sum += get(img, i, j);
            count++;
        }
        if (valid(img, i + 1, j))
        {
            sum += get(img, i + 1, j);
            count++;
        }
        if (__DEBUG_LOG)
        {
            cout << "For i = " << i << " j = " << j << "-> sum = " << sum << " count = " << count << endl;
        }
        return make_pair(sum, count);
    }

    pair<int, int> getRowSum(vector<vector<int>> &img, int i, int j)
    {
        int sum = 0;
        float count = 0;
        if (valid(img, i, j - 1))
        {
            sum += get(img, i, j - 1);
            count++;
        }
        if (valid(img, i, j))
        {
            sum += get(img, i, j);
            count++;
        }
        if (valid(img, i, j + 1))
        {
            sum += get(img, i, j + 1);
            count++;
        }
        return make_pair(sum, count);
    }

    vector<vector<int>> imageSmoother(vector<vector<int>> &img)
    {
        vector<vector<int>> result(img.size(), vector<int>(img[0].size()));

        for (int i = 0; i < (int)img.size(); i++)
        {
            pair<int, int> r1 = make_pair(0, 0);
            pair<int, int> r2 = getColSum(img, i, 0);
            pair<int, int> r3 = make_pair(0, 0);
            for (int j = 0; j < (int)img[i].size(); j++)
            {
                r3 = getColSum(img, i, j + 1);
                result[i][j] = (r1.first + r2.first + r3.first) / (r1.second + r2.second + r3.second);
                if (__DEBUG_LOG)
                {
                    cout << "*** Setting result for -> i = " << i << " j = " << j << "-> result = " << result[i][j] << endl;
                }
                r1 = r2;
                r2 = r3;
            }
        }
        return result;
    }
};

void test1(int testID)
{

    cout << "test" << testID << endl;
    vector<vector<int>> nums = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    vector<vector<int>> expected = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    Solution s;

    auto result = s.imageSmoother(nums);
    printVector(result);

    assert(result == expected);
}

void test2(int testID)
{

    cout << "test" << testID << endl;
    vector<vector<int>> nums = {{100, 200, 100}, {200, 50, 200}, {100, 200, 100}};
    vector<vector<int>> expected = {{137, 141, 137}, {141, 138, 141}, {137, 141, 137}};
    Solution s;

    auto result = s.imageSmoother(nums);
    printVector(result);

    assert(result == expected);
}

int main()
{

    test1(1);
    test2(2);

    return 0;
}