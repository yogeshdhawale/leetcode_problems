
/*
2659. Make Array Empty
Hard
Topics
Companies
Hint

You are given an integer array nums containing distinct numbers, and you can perform the following operations until the array is empty:

    If the first element has the smallest value, remove it
    Otherwise, put the first element at the end of the array.

Return an integer denoting the number of operations it takes to make nums empty.



Example 1:

Input: nums = [3,4,-1]
Output: 5

Operation	Array
1	[4, -1, 3]
2	[-1, 3, 4]
3	[3, 4]
4	[4]
5	[]

Example 2:

Input: nums = [1,2,4,3]
Output: 5

Operation	Array
1	[2, 4, 3]
2	[4, 3]
3	[3, 4]
4	[4]
5	[]

Example 3:

Input: nums = [1,2,3]
Output: 3

Operation	Array
1	[2, 3]
2	[3]
3	[]



Constraints:

    1 <= nums.length <= 105
    -109 <= nums[i] <= 109
    All values in nums are distinct.


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

static int inputID = 1;
struct Input
{
    int id;
    vector<int> nums;

    Input() {
        id = inputID++;
    }
    Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }
    Input(vector<int> nums) {
        this->nums = nums;
        id = inputID++;
    }
};

typedef long long OUTPUT_TYPE;
typedef int VALUE;
typedef size_t POSITION;


struct DataMap {
    POSITION startRange;
    POSITION endRange;
    POSITION count;
    bool isfull;
    map<POSITION, bool> idx;

    DataMap() : startRange(0), endRange(0), count(0), isfull(false) {

    }

    void setRange(int startRange, int endRange) {
        this->startRange = startRange;
        this->endRange = endRange;
    }
    int getCount(POSITION pos) {
        if (count == 0 || pos > endRange)
            return count;
        if (isfull) {
            return count - (endRange - pos + 1);
        }
        if (pos < idx.begin()->first)
            return 0;

        auto itr = idx.lower_bound(pos);
        if (itr == idx.end())
            return count;
        return distance(idx.begin(), itr);
    }
    void AddIdx(POSITION pos) {
        idx[pos] = true;
        count++;
        if (count == endRange - startRange + 1) {
            isfull = true;
            idx.clear();
        }
    }
};
struct ProcessedData {
    int totalSize;
    const int RANGE_SIZE = 1000;
    vector<DataMap> dataMap;
    void clear() {
        dataMap.clear();
    }
    ProcessedData(int totalSize) : totalSize(totalSize) {
        dataMap.resize(totalSize / RANGE_SIZE + 1);
        for (size_t i = 0; i < dataMap.size(); i++) {
            dataMap[i].setRange(i * RANGE_SIZE, (i + 1) * RANGE_SIZE - 1);
        }
        dataMap[dataMap.size() - 1].setRange((dataMap.size() - 1) * RANGE_SIZE, totalSize - 1);
    }
    void AddIdx(POSITION pos) {
        int idx = pos / RANGE_SIZE;
        dataMap[idx].AddIdx(pos);
    }
    OUTPUT_TYPE getCount(POSITION pos) {
        OUTPUT_TYPE cnt = 0;
        int idx = pos / RANGE_SIZE;
        for (int i = 0; i <= idx; i++) {
            cnt += dataMap[i].getCount(pos);
        }
        return cnt;
    }
};

struct Data {
    map<VALUE, POSITION> v2p;
    int rotation;
    ProcessedData pdata;

    void clear() {
        v2p.clear();
        rotation = 0;
    }
    Data(int totalSize) : rotation(0), pdata(totalSize) {
        clear();
        pdata.clear();
    }

    long long process(VALUE val, POSITION pos) {

        int size = v2p.size();
        int deletedCnt = pdata.getCount(pos);

        int curRotNeeded = pos - deletedCnt - rotation;
        if (curRotNeeded < 0)
            curRotNeeded = curRotNeeded + size;

        LOG("processing @" << pos << "=" << val << "\tcurpos=" << curRotNeeded << " deletedCnt=" << deletedCnt << " rotation=" << rotation);

        v2p.erase(val);
        pdata.AddIdx(pos);
        rotation = (rotation + curRotNeeded) % size;

        LOG("v2p size=" << size - 1 << " rotation=" << rotation);
        return curRotNeeded + 1;
    }

    Data(const Data& data) = delete;
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    long long countOperationsToEmptyArray(vector<int>& nums) {
        if (nums.size() == 0) return 0;
        if (nums.size() == 1) return 1;

        Data d(nums.size());
        for (size_t i = 0; i < nums.size(); i++) {
            d.v2p[nums[i]] = i;
        }

        long long result = 0;

        while (d.v2p.size() > 1) {
            result += d.process(d.v2p.begin()->first, d.v2p.begin()->second);
            LOG("result=" << result);
        }
        return result + 1;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return countOperationsToEmptyArray(nums.nums);
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

    nums.push_back(Input({ 3,4,-1 }));
    expected.push_back(5);

    nums.push_back(Input({ 1,2,4,3 }));
    expected.push_back(5);

    nums.push_back(Input({ 1,2,3 }));
    expected.push_back(3);

    nums.push_back(Input({ -14,-16,-17,10 }));
    expected.push_back(9);

    nums.push_back(Input(100000));
    expected.push_back(100000);

    nums.push_back(Input({ -18,7,6,-17,14 }));
    expected.push_back(10);

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


