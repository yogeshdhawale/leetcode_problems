
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

using namespace std;
#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2
#define LOGDATE __TIMESTAMP__

#define __DEBUG_LOG 3
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

class Data
{
    vector<int> pairCounts;
    size_t k;
    void setData(const Data &other)
    {
        for (size_t i = 0; i <= k; i++)
        {
            pairCounts[i] = other.pairCounts[i];
        }
    }

public:
    /*
     * k = inverse pairs value
     * count = number of times you can get k inverse pairs
     */
    Data(int k) : k(k)
    {
        pairCounts.resize(k + 1, 0);
    }

    inline int GetCnt(size_t i) const
    {
        return pairCounts[i];
    }
    void MultPairCnt(size_t i, long long count)
    {
        if (i > k)
        {
            return;
        }
        SetPairCnt(i, (GetCnt(i) * count) % RESULT_MOD);
    }
    void IncrPairCnt(size_t i, long long count)
    {
        if (i > k)
        {
            return;
        }
        SetPairCnt(i, (GetCnt(i) + count) % RESULT_MOD);
    }
    void SetPairCnt(size_t i, int count)
    {
        if (i > k)
        {
            return;
        }
        pairCounts[i] = count;
    }

    void Merge(const Data &other, size_t start = 0, size_t count = 1)
    {
        for (size_t i = 0; i <= k; i++)
        {
            if ((i + start) > k || other.GetCnt(i) == 0)
                break;

            // MultPairCnt(i + start, other.GetCnt(i) * count);
            IncrPairCnt(i + start, (other.GetCnt(i) * count));
        }
    }

    ostream &operator<<(ostream &out) const
    {
        out << pairCounts;
        return out;
    }
    friend ostream &operator<<(ostream &out, const Data &data);
    string to_string() const
    {
        stringstream ss;
        ss << pairCounts;
        return ss.str();
    }
};

ostream &operator<<(ostream &out, const Data &data)
{
    out << "Pair Counts: ";
    out << data.pairCounts;
    return out;
}
class Solution
{
    Data *prevcache;
    Data *cache;

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

        prevcache = new Data(k);
        cache = new Data(k);
        cache->SetPairCnt(0, 1);
        cache->SetPairCnt(1, 1);

        for (int i = 3; i <= size; i++)
        {

            delete prevcache;
            prevcache = cache;
            cache = new Data(k);
            LOG("Build for " << i << " with prevcache :" << prevcache->to_string());

            for (int j = 0; j < i / 2; j++)
            {
                cache->Merge(*prevcache, j);
                cache->Merge(*prevcache, i - j - 1);
                // LOG("Build for @" << j << " with cache :" << cache->to_string());
            }
            if (i % 2 == 1)
            {
                cache->Merge(*prevcache, (i - 1) / 2);
                // LOG("Build for @@ with cache :" << cache->to_string());
            }
        }

        LOG("Processed " << n << " " << cache->to_string());
        int ret = cache->GetCnt(k);
        delete cache;
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
    });

    auto expected = vector<int>({1, 1, 2, 2, 44});

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

    auto result = s.kInversePairs(10, 10);

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
