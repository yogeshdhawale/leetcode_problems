
/*
3288. Length of the Longest Increasing Path
Hard
Topics
Companies
Hint

You are given a 2D array of integers coordinates of length n and an integer k, where 0 <= k < n.

coordinates[i] = [xi, yi] indicates the point (xi, yi) in a 2D plane.

An increasing path of length m is defined as a list of points (x1, y1), (x2, y2), (x3, y3), ..., (xm, ym) such that:

    xi < xi + 1 and yi < yi + 1 for all i where 1 <= i < m.
    (xi, yi) is in the given coordinates for all i where 1 <= i <= m.

Return the maximum length of an increasing path that contains coordinates[k].



Example 1:

Input: coordinates = [[3,1},{2,2},{4,1},{0,0},{5,3]], k = 1

Output: 3

Explanation:

(0, 0), (2, 2), (5, 3) is the longest increasing path that contains (2, 2).

Example 2:

Input: coordinates = [[2,1},{7,0},{5,6]], k = 2

Output: 2

Explanation:

(2, 1), (5, 6) is the longest increasing path that contains (5, 6).



Constraints:

    1 <= n == coordinates.length <= 105
    coordinates[i].length == 2
    0 <= coordinates[i][0], coordinates[i][1] <= 109
    All elements in coordinates are distinct.
    0 <= k <= n - 1


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

/****************************************************************************/
//#define __DEBUG_LOG __DEBUG_LOG_ON
//#define __DEBUG_BIG_TEST
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
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 6);

using namespace std;

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

typedef V2Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int mID;
    INPUT_TYPE mData1;
    int mData2;

    Input() {
        mID = inputID++;
    }
    Input(INPUT_TYPE d1, int d2) {
        this->mData1 = d1;
        this->mData2 = d2;
        mID = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data;
ostream& operator<<(ostream& out, const Data& d);

struct Data {

    void clear() {
    }
    Data() {
        clear();
    }
};

struct comparePoint {
    bool operator()(const IDX2D& a, const IDX2D& b) const {
        return a.first < b.first || (a.first == b.first && a.second < b.second);
    }
};
struct hashFunction {
    size_t operator()(const IDX2D& p) const {
        return std::hash<IDX>()(p.first) ^ std::hash<IDX>()(p.second);
    }
};

class Solution {
public:

    void clear() {
    }

    int maxPathLength(vector<vector<int>>& coordinates, int k) {

        map<int, map<int, bool>> cacheXYLower;
        map<int, map<int, bool>> cacheXYUpper;
        IDX2D selectedPoint = { coordinates[k][0], coordinates[k][1] };
        LOG("Point of interest is [" << selectedPoint.first << "," << selectedPoint.second << "]");

        for (size_t i = 0; i < coordinates.size(); i++) {
            IDX2D point = { coordinates[i][0], coordinates[i][1] };
            if (point.first < selectedPoint.first && point.second < selectedPoint.second) {
                cacheXYLower[point.first][point.second] = true;
            }
            else if (point.first > selectedPoint.first && point.second > selectedPoint.second) {
                cacheXYUpper[point.first][point.second] = true;
            }
            else {
                //LOG("Ignoring point [" << point.first << "," << point.second << "]");
            }
        }

        LOG("lowerbox points cache: " << cacheXYLower);
        LOG("upperbox points cache: " << cacheXYUpper);
        LOG("result upperlimit: 1+" << cacheXYLower.size() << "+" << cacheXYUpper.size());
        int pathLength = 1;

        pathLength += incrementViaXAxis(cacheXYLower);
        pathLength += incrementViaXAxis(cacheXYUpper);

        return pathLength;
    }

    int incrementViaXAxis(map<int, map<int, bool>>& cachexy) {
        int count = 0;
        if (cachexy.empty()) {
            LOG("returning count: " << count);
            return count;
        }
        if (cachexy.size() == 1) {
            count = 1;
            LOG("returning count: " << count);
            return count;
        }

        map<IDXC, int> forProcessing;

        auto itrx = cachexy.begin();
        forProcessing[itrx->second.begin()->first] = 1;
        itrx++;

        for (; itrx != cachexy.end(); ++itrx) {
            auto& mapy = itrx->second;
            auto itry = mapy.begin();
            auto ifp = forProcessing.begin();

            int prevCnt = 0;
            for (;itry != mapy.end() && ifp != forProcessing.end(); prevCnt = ifp->second, ifp++)
            {
                if (itry->first < ifp->first) {
                    forProcessing[itry->first] = prevCnt + 1;
                }
                else if (itry->first == ifp->first) {
                    forProcessing[itry->first] = max(forProcessing[itry->first], prevCnt + 1);
                }
                while (itry != mapy.end() && itry->first <= ifp->first) itry++;
            }
            if (ifp == forProcessing.end() && itry != mapy.end()) {
                forProcessing[itry->first] = prevCnt + 1;
            }

            auto itr = forProcessing.begin();
            auto nextItr = next(itr);
            while (nextItr != forProcessing.end()) {
                if (itr->second >= nextItr->second) {
                    nextItr = forProcessing.erase(nextItr);
                    continue;
                }
                itr = nextItr++;
            }
            //LOG("At end of row: " << itrx->first << " pathLength:" << forProcessing);
        }

        int retVal = 0;
        for (auto [_, val] : forProcessing) {
            retVal = max(retVal, val);
        }
        LOG("for procesing: " << forProcessing);
        LOG("returning count: " << retVal);
        return retVal;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return maxPathLength(nums.mData1, nums.mData2);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& ipObj) {
    out << "id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " k:" << ipObj.mData2;
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
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int GenerateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}

V2Int GenerateInput(int seed, IDX2D size, int max) {
    V2Int nums(size.first);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](V1Int& item) {item = GenerateInput(rand(), size.second, max);});
    return nums;
}
void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ {3,1},{2,2},{4,1},{0,0},{5,3} }, 1));
    expected.push_back(3);

    nums.push_back(Input({ {3,1},{2,2},{4,1},{0,0},{5,3} }, 2));
    expected.push_back(3);

    nums.push_back(Input({ {2,1},{7,0},{5,6} }, 2));
    expected.push_back(2);

    nums.push_back(Input({ {0,0},{1,1},{3,3}, {5,5} }, 2));
    expected.push_back(4);

    nums.push_back(Input({ {0,0},{0,1}, { 1,1 }, {1,2} ,{3,3}, {5,5} }, 4));
    expected.push_back(4);

    nums.push_back(Input({ {0,0},{0,10}, { 10,10 }, {11,10},{30,30}, {50,50} }, 4));
    expected.push_back(4);

    nums.push_back(Input({ {0,25},{10,10},{15,15}, { 20,20 }, {30,30}, {50,50} }, 4));
    expected.push_back(5);

    nums.push_back(Input({ { 8, 8},{8, 4},{5, 4},{0, 0},{6, 3},{1, 6},{2, 1 } }, 3));
    expected.push_back(4);

    nums.push_back(Input({ { 3, 0},{2, 7},{6, 8},{5, 1},{8, 5},{0, 9},{0, 6},{9, 7},{9, 0 } }, 0));
    expected.push_back(4);

    nums.push_back(Input({ {0, 1},{1, 0},{8, 4},{3, 6},{4, 6},{9, 7},{4, 8 } }, 1));
    expected.push_back(3);

    nums.push_back(Input(GenerateInput(1, { 1000000,2 }, 100000), 1));
    expected.push_back(515);

    cout << endl << __LOGPREFIX << "test:" << testID << " cases (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        cout << __LOGPREFIX << "test loop_id: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        auto result = s.ProxyFunction(nums[i]);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << "test loop_id: " << i << " ==>\t" << result << " vs e:" << expected[i] << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#ifdef __DEBUG_BIG_TEST

V1Int load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    nums.push_back(
        Input(load_data_v2(t1i1), load_data_int(t1i2))
    );
    expected.push_back(load_data_int(t1e1));

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
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


