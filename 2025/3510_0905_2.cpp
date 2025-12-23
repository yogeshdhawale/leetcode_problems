
/*
3510. Minimum Pair Removal to Sort Array II
Hard
Topics
Companies
Hint

Given an array nums, you can perform the following operation any number of times:

    Select the adjacent pair with the minimum sum in nums. If multiple such pairs exist, choose the leftmost one.
    Replace the pair with their sum.

Return the minimum number of operations needed to make the array non-decreasing.

An array is said to be non-decreasing if each element is greater than or equal to its previous element (if it exists).



Example 1:

Input: nums = [5,2,3,1]

Output: 2

Explanation:

    The pair (3,1) has the minimum sum of 4. After replacement, nums = [5,2,4].
    The pair (2,4) has the minimum sum of 6. After replacement, nums = [5,6].

The array nums became non-decreasing in two operations.

Example 2:

Input: nums = [1,2,2]

Output: 0

Explanation:

The array nums is already sorted.



Constraints:

    1 <= nums.length <= 105
    -109 <= nums[i] <= 109


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

using namespace std;

#define __DEBUG_LOG __DEBUG_LOG_OFF
#define __DEBUG_BIG_TEST
enum TestOptions {
    TEST_ALL = 10,
    TEST_SINGLE = 11,
};

struct TestConfig {
    TestOptions options;
    int testCaseID;
};
#include "log.h"
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 1);
int gCurTestId = -1;

/****************************************************************************/
/****************************************************************************/

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

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    INPUT_TYPE mData1;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(INPUT_TYPE input1) {
        this->mData1 = input1;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/

struct Sum2IdxInfo {
    long mSum;
    map<int, int> mIDXPairs;

    Sum2IdxInfo(int sum) {
        this->mSum = sum;
        mIDXPairs.clear();
    }
    void AddPair(int idx1, int idx2) {
        mIDXPairs[idx1] = idx2;
    }
    void RemovePair(int idx1, int idx2) {
        mIDXPairs.erase(idx1);
    }
    pair<int, int> removeFirstPair() {
        auto minPair = make_pair(mIDXPairs.begin()->first, mIDXPairs.begin()->second);
        mIDXPairs.erase(mIDXPairs.begin());
        return minPair;
    }
    int getPairCount() {
        return mIDXPairs.size();
    }
};

struct IdxNeighborInfo {
    int mIdx;
    long mval;
    int mPIdx;
    int mNIdx;

    IdxNeighborInfo(int myidx = -1, long val = 0) : mIdx(myidx), mval(val) {
        this->mPIdx = -1;
        this->mNIdx = -1;
    }
    void setIdx(int myidx) {
        this->mIdx = myidx;
    }
    void setVal(long val) {
        this->mval = val;
    }
    void reset() {
        this->mval = 0;
        this->mIdx = -1;
        this->mPIdx = -1;
        this->mNIdx = -1;
    }
};

class Solution {

    map<long, Sum2IdxInfo*> sum2Idx;
    vector<IdxNeighborInfo> idxNMap;

public:
    Solution() {}

    int minimumPairRemoval(vector<int> nums) {

        sum2Idx.clear();
        idxNMap.clear();

        initialize(nums);

        int opCnt = 0;

        bool sortCheckNeeded = true;
        pair<int, int> sortCheckFailedIdx;
        while (true) {

            if (sortCheckNeeded) {
                sortCheckFailedIdx = getSortCheckFailure(idxNMap);
                if (sortCheckFailedIdx.first == -1) return opCnt;
                sortCheckNeeded = false;
            }

            auto curSumItr = sum2Idx.begin();
            auto curSum = curSumItr->first;
            // get 1st min pair from left and remove it
            // pair a=cur, ...x-a-b-c... selected pair a+b=p ...x-p-c... 
            auto minIdxPair = curSumItr->second->removeFirstPair();
            if (sum2Idx[curSum]->getPairCount() == 0) {
                auto s2d = sum2Idx[curSum];
                sum2Idx.erase(curSum);
                delete s2d;
            }

            auto curIdx = minIdxPair.first;
            auto tobeDeletedIdx = minIdxPair.second;

            if (curSum != idxNMap[curIdx].mval + idxNMap[tobeDeletedIdx].mval) {
                // invalid pair, either curIdx or tobeDeletedIdx is changed by operation
                LOG("sum mismatch, {" << curIdx << "," << tobeDeletedIdx
                    << "} curSum=" << curSum << " nums[" << curIdx << "]=" << idxNMap[curIdx].mval
                    << " nums[" << tobeDeletedIdx << "]=" << idxNMap[tobeDeletedIdx].mval);
                continue;
            }

            if (idxNMap[curIdx].mIdx == -1 || idxNMap[tobeDeletedIdx].mIdx == -1) {
                LOG("invalid pair even though sum (" << curSum << ") is same, pair is invalid {" << curIdx
                    << ", " << tobeDeletedIdx << "}");
                continue;
            }

            //LOG("\tArray is: " << nums << " opCnt=" << opCnt);

            auto prevIdx = idxNMap[curIdx].mPIdx;
            auto nextIdx = idxNMap[tobeDeletedIdx].mNIdx;

            LOG("selected " << curSum << " idx {" << curIdx << "," << tobeDeletedIdx
                << "} values=" << idxNMap[curIdx].mval << "," << idxNMap[tobeDeletedIdx].mval);

            /*
                        // remove old sums with prev and next
                        if (prevIdx != -1) {
                            int sumprev = nums[prevIdx] + nums[curIdx];
                            sum2Idx[sumprev]->RemovePair(prevIdx, curIdx);
                            if (sum2Idx[sumprev]->getPairCount() == 0) {
                                auto s2d = sum2Idx[sumprev];
                                sum2Idx.erase(sumprev);
                                delete s2d;
                            }
                        }
                        if (nextIdx != -1) {
                            int sumnext = nums[tobeDeletedIdx] + nums[nextIdx];
                            sum2Idx[sumnext]->RemovePair(tobeDeletedIdx, nextIdx);
                            if (sum2Idx[sumnext]->getPairCount() == 0) {
                                auto s2d = sum2Idx[sumnext];
                                sum2Idx.erase(sumnext);
                                delete s2d;
                            }
                        }
            */
            // use cur for new sum and invalidate its next number
            idxNMap[curIdx].mval = curSum;
            idxNMap[tobeDeletedIdx].reset();

            // fix links:  ...curIdx-tobeDeletedIdx-nextIdx..., becomes ...curIdx-nextIdx...
            idxNMap[curIdx].mNIdx = nextIdx;
            if (nextIdx != -1) {
                idxNMap[nextIdx].mPIdx = curIdx;
            }

            // recalculate sum for prev and newNext
            if (prevIdx != -1) {
                auto newprevsum = idxNMap[prevIdx].mval + idxNMap[curIdx].mval;
                if (sum2Idx.find(newprevsum) == sum2Idx.end()) {
                    sum2Idx[newprevsum] = new Sum2IdxInfo(newprevsum);
                }
                sum2Idx[newprevsum]->AddPair(prevIdx, curIdx);
            }
            if (nextIdx != -1) {
                auto newnextsum = idxNMap[curIdx].mval + idxNMap[nextIdx].mval;
                if (sum2Idx.find(newnextsum) == sum2Idx.end()) {
                    sum2Idx[newnextsum] = new Sum2IdxInfo(newnextsum);
                }
                sum2Idx[newnextsum]->AddPair(curIdx, nextIdx);
            }
            opCnt++;

            sortCheckNeeded = checkForUpdateNeeded(sortCheckFailedIdx, prevIdx, curIdx, tobeDeletedIdx, nextIdx);
        }
        return opCnt;
    }

    void initialize(V1Int& nums)
    {
        int n = nums.size();

        idxNMap.resize(n);

        idxNMap[0].setIdx(0);
        idxNMap[0].setVal(nums[0]);
        idxNMap[0].mNIdx = 1;

        for (int i = 1; i < n; i++) {

            idxNMap[i].setIdx(i);
            idxNMap[i].setVal(nums[i]);
            idxNMap[i].mPIdx = i - 1;
            idxNMap[i].mNIdx = i + 1;

            long sum = nums[i] + nums[i - 1];
            if (sum2Idx.find(sum) == sum2Idx.end())  sum2Idx[sum] = new Sum2IdxInfo(sum);
            sum2Idx[sum]->AddPair(i - 1, i);
        }

        idxNMap[n - 1].mNIdx = -1;
    }

    pair<int, int> getSortCheckFailure(vector<IdxNeighborInfo>& idxNMap) {
        auto curIdx = 0;
        for (;idxNMap[curIdx].mIdx == -1; curIdx++);

        for (auto nextIdx = idxNMap[curIdx].mNIdx;
            nextIdx != -1;
            curIdx = nextIdx, nextIdx = idxNMap[curIdx].mNIdx
            ) {

            if (idxNMap[curIdx].mval > idxNMap[nextIdx].mval) {
                return make_pair(curIdx, nextIdx);
            }
        }
        return make_pair(-1, -1);
    }

    bool checkForUpdateNeeded(pair<int, int>& sortCheckFailedIdx, int a1, int a2, int a3, int a4) {

        if (sortCheckFailedIdx.first == a1 || sortCheckFailedIdx.second == a1) {
            return true;
        }
        if (sortCheckFailedIdx.first == a2 || sortCheckFailedIdx.second == a2) {
            return true;
        }
        if (sortCheckFailedIdx.first == a3 || sortCheckFailedIdx.second == a3) {
            return true;
        }
        if (sortCheckFailedIdx.first == a4 || sortCheckFailedIdx.second == a4) {
            return true;
        }
        return false;
    }

    OUTPUT_TYPE proxyFunction(TestCaseStruct& nums) {
        return minimumPairRemoval(nums.mData1);
    }
};


/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " }";
    return out;
}

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int generateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}


void test1(int testID)
{
    vector<TestCaseStruct> nums;
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,2,3,1 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,2,2 });
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,1,2,8,10 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,3,1,8,10 });
        nums.back().setOutput(4);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 1,1,-1,1,7 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 5,
            1,1,1,1,1,
            1,1,1,1,1 });
        nums.back().setOutput(10);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 8,1,-1,7 });
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 8,1,-1,9 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 8,-1,1,9 });
        nums.back().setOutput(2);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 8,0,-1,1,9 });
        nums.back().setOutput(3);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 8,0,-1,1,7 });
        nums.back().setOutput(4);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput({ 855290830,806237550,837256618,615703760,837256618,
            50623755,85529083,137256618,61570376,50623755,
            });
        nums.back().setOutput(8);
    }

    cout << endl << __LOGPREFIX << "test:" << testID << " sub-tests count (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        string strTestId = "test:" + to_string(testID) + "." + to_string(nums[i].mID) + " => ";
        cout << __LOGPREFIX << strTestId << nums[i] << endl;

        gCurTestId = nums[i].mID;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << strTestId << "returns:" << result << " vs e:" << nums[i].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected1);
    }
}

#ifdef __DEBUG_BIG_TEST
template <typename T> vector<T> load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<TestCaseStruct> nums;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(load_data_v1<int>(t1i1));
        nums.back().setOutput(598);
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.proxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    //assert(result == nums[0].mExpected1);
}
#endif

int main()
{
    cout << "^^^Start^^^" << endl;
    test1(1);
#ifdef __DEBUG_BIG_TEST
    test2(2);
#endif
    cout << "^^^END^^^" << endl;
    return 0;
}


