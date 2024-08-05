
/*
629. K Inverse Pairs Array
Hard
Topics
Companies

For an integer array nums, an inverse pair is a pair of integers [i, x] where 0 <= i < x < nums.length and nums[i] > nums[x].

Given two integers n and k, return the number of different arrays consisting of numbers from 1 to n such that there are exactly k inverse pairs. Since the answer can be huge, return it modulo 109 + 7.



Example 1:

Input: n = 3, k = 0
Output: 1
Explanation: Only the array [1,2,3] which consists of numbers from 1 to 3 has exactly 0 inverse pairs.

Example 2:

Input: n = 3, k = 1
Output: 2
Explanation: The array [1,3,2] and [2,1,3] have exactly 1 inverse pair.



Constraints:

    1 <= n <= 1000
    0 <= k <= 1000


    n=3, k=1 -> 2
    1, 2, 3

    3 2 1 = 3
    3 1 2 = 2
    2 3 1 = 2
    2 1 3 = 1 *
    1 3 2 = 1 *
    1 2 3 = 0


    start-end
        range = k:count
    1-1 = {0:1}
        1 = 0:1
    1-2 = {0:1, 1:1}
        1 2 = 0:1
        2 1 = 1:1

    1-3 = {0:1, 1:2, 2:2, 3:1}
        {} 3 {1-2} = 2 + {0:1, 1:1} = 2:1,3:1
        {1} 3 {2} = 0:1 + 1 + {0:1} = 1:1
        {2} 3 {1} = 1 + 1 + {0:1} = 2:1
        {1-2} 3 {} = 0:1, 1:1

    1-4 : [0]:1, [1]:4, [2]:4, [3]:6, [4]:2, [5]:2, [6]:1,
        {} 4 {1-3} = 3 + {0:1, 1:2, 2:3, 3:1} = {3:1,4:2, 5:3, 6:1}
        {1} 4 {2-3} = 0+ 2 + {0:1, 1:1} = {2:1, 3:1}
        {1-2} 4 {3} = {0:1, 1:1} + 1 + {0:1} = 1:1, 2:1
        {3} 4 {1-2} = 2 + 2 + {0:1, 1:1} = 4:1, 5:1
        {1,3} 4 {2} = {0:1, 1:1} + 1 + 1 + {0:1} = 2:1, 3:1
        {1-3} 4 {} = {0:1, 1:2, 2:3, 3:1}
        {1-2} 5 {3-4} = {0:1, 1:1} + 2 + {0:1, 1:1} => 2:1, 3:2, 4:1
                      = 0:1 + 2 + {0:1, 1:1} => 2:1, 3:1
                      = 1:1 + 2 + {0:1, 1:1} => 3:1, 4:1

        4 => {0:1, 1:3, 2:6, 3:6, 4:4, 5:3, 6:1}
        1234 0:1
        1324 1:1
        2134 1:1
        2314 2:1
        3124 2:1
        3214 3:1

        1243 1:1
        2143 2:1
        1342 2:1 x
        3142 3:1 x
        2341 3:1
        3241 4:1

        1423 2:1
        1432 3:1
        2413 3:1 x
        2431 3:1 x
        3412 4:1
        3421 5:1

        4123 3:1
        4132 4:1
        4213 4:1
        4231 5:1
        4312 5:1
        4321 6:1

        4 {1-3} = 3 + {0:1, 1:2, 2:2, 3:1}
                = 3:1, 4:2, 5:2, 6:1
---
        5 4 3 2 1 = 4+3+2+1

*/

#include <iostream>
#include <cassert>
#include <chrono>
#include <string_view>
#include <algorithm>
#include <vector>
#include <map>
#include <numeric>
#include <sstream>
#include <string.h>

using namespace std;
#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2
#define LOGDATE __TIMESTAMP__

#define __DEBUG_LOG 1
#define LOG_MSG(msg) \
    clog << "[" << LOGDATE << "]\t" << msg << endl

#ifdef __DEBUG_LOG
#define LOG(x)                         \
    if (__DEBUG_LOG != __DEBUG_LOG_NO) \
    LOG_MSG(x)
#define LOGV(x)                        \
    if (__DEBUG_LOG >= __DEBUG_LOG_VB) \
    LOG_MSG(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

#define RESULT_MOD 1000000007

template <typename T>
ostream &operator<<(ostream &out, const vector<T> &nums)
{
    size_t size = nums.size();
    if (nums.size() > 20)
        size = 20;
    for (size_t i = 0; i < size; i++)
    {
        if (nums[i] <= 0)
            continue;
        out << "[" << i << "]:" << nums[i] << ", ";
    }
    if (nums.size() > 20)
    {
        out << "...";
    }
    return out;
}

template <typename T>
void printArry(const T *nums)
{
    int size = 20;
    for (int i = 0; i < size; i++)
    {
        if (nums[i] <= 0)
            continue;
        cout << "[" << i << "]:" << nums[i] << ", ";
    }
    return;
}

class Solution
{

public:
    int kInversePairs(int n, int k)
    {
        if (n == 0)
        {
            return 0;
        }
        if (n == 1)
        {
            return k == 0 ? 1 : 0;
        }

        if (k == 0 || k == n * (n - 1) / 2)
        {
            return 1;
        }

        int size = n;

        int *prevcache = new int[k + 1];
        int *cache = new int[k + 1];

        memset(prevcache, 0, sizeof(int) * (k + 1));
        memset(cache, 0, sizeof(int) * (k + 1));

        cache[0] = 1;
        cache[1] = 1;

        for (int i = 3; i <= size; i++)
        {

            swap(prevcache, cache);
            memset(cache, 0, sizeof(int) * (k + 1));

            LOG("Build for " << i << " with prevcache :");
            printArry(prevcache);

            int maxIdx = i * (i - 1) / 2;
            cache[0] = 1;
            if (maxIdx <= k)
            {
                cache[maxIdx] = 1;
            }

            int x = 0;
            for (x = 1; x <= k; x++)
            {
                if (prevcache[x - 1] == 0)
                {
                    break;
                }

                long long incrVal = (cache[x - 1] + prevcache[x]) % RESULT_MOD;
                if (x < i)
                {
                    cache[x] = incrVal;
                    if (maxIdx - x <= k)
                        cache[maxIdx - x] = incrVal;
                }
                else
                {
                    incrVal -= (prevcache[x - i] % RESULT_MOD);
                    if (incrVal < 0)
                    {
                        incrVal += RESULT_MOD;
                    }
                    cache[x] = incrVal;
                    if (maxIdx - x <= k)
                        cache[maxIdx - x] = incrVal;
                }
                // LOG("\t1Build for " << x << " incr: " << incrVal << " with cache :" << cache->to_string());
            }
        }

        // LOG("Processed " << n << " " << cache->to_string());
        int ret = cache[k];
        delete[] prevcache;
        delete[] cache;
        return ret;
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({
        {3, 0},
        {3, 3},
        {3, 1},
        {3, 2},
        {10, 2},
        {10, 5},
        {10, 6},
    });

    auto expected = vector<int>({1, 1, 2, 2, 44, 1068, 2298});

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.kInversePairs(nums[i][0], nums[i][1]);
        cout << result << " vs " << expected[i] << " with " << std::boolalpha << (result == expected[i]) << endl;
        // assert(result == expected[i]);
    }
}
void test2(int testID)
{

    cout << "test" << testID << endl;

    Solution s;
    auto startime = std::chrono::high_resolution_clock::now();

    auto result = s.kInversePairs(1000, 1000);

    auto endtime = std::chrono::high_resolution_clock::now();

    cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - startime).count() << "ms" << endl;
    auto expected = 663677020;
    cout << result << " vs " << expected << endl;
    assert(result == expected);
}
int main()
{

    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG > __DEBUG_LOG_VB
    test2(2);
#endif
    // test3(3);

    return 0;
}
