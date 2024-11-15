
/*
3197. Find the Minimum Area to Cover All Ones II
Hard
Topics
Companies
Hint

You are given a 2D binary array grid. You need to find 3 non-overlapping rectangles having non-zero areas with horizontal and vertical sides such that all the 1's in grid lie inside these rectangles.

Return the minimum possible sum of the area of these rectangles.

Note that the rectangles are allowed to touch.



Example 1:

Input: grid = [[1,0,1],[1,1,1]]

Output: 5

Explanation:

    The 1's at (0, 0) and (1, 0) are covered by a rectangle of area 2.
    The 1's at (0, 2) and (1, 2) are covered by a rectangle of area 2.
    The 1 at (1, 1) is covered by a rectangle of area 1.

Example 2:

Input: grid = [[1,0,1,0],[0,1,0,1]]

Output: 5

Explanation:

    The 1's at (0, 0) and (0, 2) are covered by a rectangle of area 3.
    The 1 at (1, 1) is covered by a rectangle of area 1.
    The 1 at (1, 3) is covered by a rectangle of area 1.



Constraints:

    1 <= grid.length, grid[i].length <= 30
    grid[i][j] is either 0 or 1.
    The input is generated such that there are at least three 1's in grid.


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
#define LOGV(x)
#endif

#define INVALID -1
#define MY_MAX_VAL INT32_MAX

#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef V2Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    INPUT_TYPE nums;

    Input() {
        id = inputID++;
    }
    Input(INPUT_TYPE nums) {
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

struct POSITION;
struct Rectangle;
struct Data;
ostream& operator<<(ostream& out, const POSITION& p);
ostream& operator<<(ostream& out, const Rectangle& r);
ostream& operator<<(ostream& out, const Data& d);

struct POSITION {
    int x;
    int y;
    POSITION(int x, int y) : x(x), y(y) {
    }
    POSITION(const POSITION& p) : x(p.x), y(p.y) {
    }

    bool operator==(const POSITION& p) const {
        return this->x == p.x && this->y == p.y;
    }
};

struct Data {
    int row;
    int col;
    vector<map<int, bool>> rInfo;
    vector<map<int, bool>> cInfo;

    void clear() {
        rInfo.clear();
        cInfo.clear();
    }
    Data(int row, int col) : row(row), col(col) {
        clear();
        rInfo.resize(row);
        cInfo.resize(col);
    }

    void insert(int i, int j) {
        rInfo[i][j] = true;
        cInfo[j][i] = true;
    }
};

struct Rectangle {
    POSITION start;
    POSITION end;
    int area;
    Rectangle(POSITION start, POSITION end) : start(start), end(end) {
        computeArea();
    }

    bool operator==(const Rectangle& r) const {
        return this->start == r.start && this->end == r.end;
    }
    void computeArea() {
        area = (end.x - start.x + 1) * (end.y - start.y + 1);
    }

    void updateStart(int x, int y) {
        start = POSITION(x, y);
        computeArea();
    }
    void updateEnd(int x, int y) {
        end = POSITION(x, y);
        computeArea();
    }
    void Reduce(Data& data) {

        auto newStartX = end.x;
        auto newStartY = end.y;

        auto newEndX = start.x;
        auto newEndY = start.y;

        for (int i = start.x; i <= end.x; i++) {
            if (data.rInfo[i].empty()) continue;
            {
                auto it = data.rInfo[i].lower_bound(start.y);
                if (it != data.rInfo[i].end())
                    newStartY = min(newStartY, it->first);
            }
            {
                auto it = data.rInfo[i].upper_bound(end.y);
                if (it != data.rInfo[i].begin())
                    newEndY = max(newEndY, (--it)->first);
            }
        }

        for (int i = start.y; i <= end.y; i++) {
            if (data.cInfo[i].empty()) continue;
            {
                auto it = data.cInfo[i].lower_bound(start.x);
                if (it != data.cInfo[i].end())
                    newStartX = min(newStartX, it->first);
            }
            {
                auto it = data.cInfo[i].upper_bound(end.x);
                if (it != data.cInfo[i].begin())
                    newEndX = max(newEndX, (--it)->first);
            }
        }

        start = POSITION(newStartX, newStartY);
        end = POSITION(newEndX, newEndY);
        computeArea();
    }
};

struct RectangleHasher {
    size_t operator()(const Rectangle& r) const {
        return hash<int>()(r.start.x) ^ hash<int>()(r.start.y) ^ hash<int>()(r.end.x) ^ hash<int>()(r.end.y);
    }
};


class Solution {
public:


    unordered_map <Rectangle, int, RectangleHasher> hcache;
    unordered_map <Rectangle, int, RectangleHasher> vcache;

    Solution() {
        hcache.clear();
        vcache.clear();
    }
    int minimumSum(vector<vector<int>>& grid) {

        int rows = grid.size();
        int cols = grid[0].size();

        LOG("rows=" << rows << " cols=" << cols);

        Data data(rows, cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 0) continue;
                data.insert(i, j);
            }
        }

        LOG("getCount: rInfo" << data.rInfo);
        LOG("getCount: cInfo" << data.cInfo);

        Rectangle bigRect(POSITION(0, 0), POSITION(data.row - 1, data.col - 1));
        LOG("bigRect:" << bigRect);
        bigRect.Reduce(data);
        LOG("min bigRect:" << bigRect);

        return min(getHCount(data, bigRect, 1), getVCount(data, bigRect, 1));
    }
    int getHCount(Data& data, Rectangle& curRect, int depth) {

        LOG("getHCount:" << curRect << " depth:" << depth);
        if (curRect.area == 0) return 0;
        if (curRect.start.x == curRect.end.x) return curRect.area;

        if (hcache.find(curRect) != hcache.end()) {
            LOG("hcache hit!!!");
            return hcache[curRect];
        }

        int minhSplitResult = INT32_MAX;
        for (int i = curRect.start.x + 1;i <= curRect.end.x;i++) {

            LOG("hSplit @" << i << " minhSplitResult:" << minhSplitResult);

            Rectangle firstRect = curRect;
            Rectangle secondRect = curRect;

            firstRect.updateEnd(i - 1, firstRect.end.y);
            secondRect.updateStart(i, secondRect.start.y);
            firstRect.Reduce(data);
            secondRect.Reduce(data);
            LOGV("firstRect:" << firstRect);
            LOGV("secondRect:" << secondRect);

            if (depth == 1) {
                auto firstAreasplit = secondRect.area + getVCount(data, firstRect, depth + 1);
                auto secondAreasplit = firstRect.area + getVCount(data, secondRect, depth + 1);
                LOGV("firstAreasplit:" << firstAreasplit << " secondAreasplit:" << secondAreasplit);
                minhSplitResult = min(minhSplitResult, min(firstAreasplit, secondAreasplit));

                firstAreasplit = secondRect.area + getHCount(data, firstRect, depth + 1);
                secondAreasplit = firstRect.area + getHCount(data, secondRect, depth + 1);
                LOGV("firstAreasplit:" << firstAreasplit << " secondAreasplit:" << secondAreasplit);
                minhSplitResult = min(minhSplitResult, min(firstAreasplit, secondAreasplit));

            }
            else {
                minhSplitResult = min(minhSplitResult, firstRect.area + secondRect.area);
            }

            LOGV("minhSplitResult:" << minhSplitResult);
        }
        LOG("getHCount:" << curRect << " depth:" << depth << " minhSplitResult:" << minhSplitResult << endl);
        hcache[curRect] = minhSplitResult;
        return minhSplitResult;
    }

    int getVCount(Data& data, Rectangle& curRect, int depth) {

        LOG("getVCount:" << curRect << " depth:" << depth);
        if (curRect.area == 0) return 0;
        if (curRect.start.y == curRect.end.y) return curRect.area;

        if (vcache.find(curRect) != vcache.end()) {
            LOG("vcache hit!!!");
            return vcache[curRect];
        }

        int minVSplitResult = INT32_MAX;
        for (int i = curRect.start.y + 1;i <= curRect.end.y;i++) {

            LOG("VSplit @" << i << " minVSplitResult:" << minVSplitResult);

            Rectangle firstRect = curRect;
            Rectangle secondRect = curRect;

            firstRect.updateEnd(firstRect.end.x, i - 1);
            secondRect.updateStart(secondRect.start.x, i);
            firstRect.Reduce(data);
            secondRect.Reduce(data);
            LOGV("firstRect:" << firstRect);
            LOGV("secondRect:" << secondRect);

            if (depth == 1) {
                auto firstAreasplit = secondRect.area + getHCount(data, firstRect, depth + 1);
                auto secondAreasplit = firstRect.area + getHCount(data, secondRect, depth + 1);
                minVSplitResult = min(minVSplitResult, min(firstAreasplit, secondAreasplit));
                LOGV("firstAreasplit:" << firstAreasplit << " secondAreasplit:" << secondAreasplit);

                firstAreasplit = secondRect.area + getVCount(data, firstRect, depth + 1);
                secondAreasplit = firstRect.area + getVCount(data, secondRect, depth + 1);
                minVSplitResult = min(minVSplitResult, min(firstAreasplit, secondAreasplit));
                LOGV("firstAreasplit:" << firstAreasplit << " secondAreasplit:" << secondAreasplit);
            }
            else
                minVSplitResult = min(minVSplitResult, firstRect.area + secondRect.area);

            LOGV("minVSplitResult:" << minVSplitResult);
        }
        LOG("getVCount:" << curRect << " depth:" << depth << " minVSplitResult:" << minVSplitResult << endl);
        vcache[curRect] = minVSplitResult;
        return minVSplitResult;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return minimumSum(nums.nums);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    return out;
}

ostream& operator<<(ostream& out, const POSITION& p) {
    out << "(" << p.x << "," << p.y << ")";
    return out;
}

ostream& operator<<(ostream& out, const Rectangle& r) {
    out << "Rect:{" << r.start << "," << r.end << " area:" << r.area << "}";
    return out;
}

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

V1Int GenerateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}

V2Int GenerateInput2(int seed, int size, int max) {
    V2Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](V1Int& item) {item = GenerateInput(seed, size, max);});
    return nums;
}

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ { 1,0,1 },{ 1,1,1 } }));
    expected.push_back(5);

    nums.push_back(Input({ { 1,0,1,0 },{ 0,1,0,1 } }));
    expected.push_back(5);

    nums.push_back(Input({ { 1,0,1,0 },{ 0,1,0,1 } }));
    expected.push_back(5);

    nums.push_back(Input({ { 1,1,},{ 0,1} }));
    expected.push_back(3);

    nums.push_back(Input({ { 1,1,1,0,0 },{ 0,1,1,0,0 },{0,0,0,1,0},{0,0,1,0,0} }));
    expected.push_back(8);

    nums.push_back(Input({ { 1,1,0,0,0 },{ 0,0,1,0,0 },{0,0,0,1,1} }));
    expected.push_back(5);

    nums.push_back(Input({ { 1,1,0,0,0 },{ 0,1,1,0,0 },{0,0,0,0,1},{0,0,0,0,1} }));
    expected.push_back(6);

    nums.push_back(Input({
        { 1,1,0,0,0 },{ 0,1,1,0,0 },{0,0,0,0,1},{0,0,0,0,1},
        { 1,1,0,0,0 },{ 0,1,1,0,0 },{0,0,0,0,1},{0,0,0,0,1},
        { 1,1,0,0,0 },{ 0,1,1,0,0 },{0,0,0,0,1},{0,0,0,0,1},
        { 1,1,0,0,0 },{ 0,1,1,0,0 },{0,0,0,0,1},{0,0,0,0,1}
        }));
    expected.push_back(50);

    nums.push_back(GenerateInput2(0, 30, 2));
    expected.push_back(660);

    nums.push_back(GenerateInput2(456, 30, 2));
    expected.push_back(780);

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


