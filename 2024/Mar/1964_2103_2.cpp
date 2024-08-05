
/*
1964. Find the Longest Valid Obstacle Course at Each Position
Hard
Topics
Companies
Hint

You want to build some obstacle courses. You are given a 0-indexed integer array obstacles of length n, where obstacles[i] describes the height of the ith obstacle.

For every index i between 0 and n - 1 (inclusive), find the length of the longest obstacle course in obstacles such that:

    You choose any number of obstacles between 0 and i inclusive.
    You must include the ith obstacle in the course.
    You must put the chosen obstacles in the same order as they appear in obstacles.
    Every obstacle (except the first) is taller than or the same height as the obstacle immediately before it.

Return an array ans of length n, where ans[i] is the length of the longest obstacle course for index i as described above.



Example 1:

Input: obstacles = [1,2,3,2]
Output: [1,2,3,3]
Explanation: The longest valid obstacle course at each position is:
- i = 0: [1], [1] has length 1.
- i = 1: [1,2], [1,2] has length 2.
- i = 2: [1,2,3], [1,2,3] has length 3.
- i = 3: [1,2,3,2], [1,2,2] has length 3.

Example 2:

Input: obstacles = [2,2,1]
Output: [1,2,1]
Explanation: The longest valid obstacle course at each position is:
- i = 0: [2], [2] has length 1.
- i = 1: [2,2], [2,2] has length 2.
- i = 2: [2,2,1], [1] has length 1.

Example 3:

Input: obstacles = [3,1,5,6,4,2]
Output: [1,1,2,3,2,2]
Explanation: The longest valid obstacle course at each position is:
- i = 0: [3], [3] has length 1.
- i = 1: [3,1], [1] has length 1.
- i = 2: [3,1,5], [3,5] has length 2. [1,5] is also valid.
- i = 3: [3,1,5,6], [3,5,6] has length 3. [1,5,6] is also valid.
- i = 4: [3,1,5,6,4], [3,4] has length 2. [1,4] is also valid.
- i = 5: [3,1,5,6,4,2], [1,2] has length 2.



Constraints:

    n == obstacles.length
    1 <= n <= 105
    1 <= obstacles[i] <= 107


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
    vector<int> obstacles;

    Input() {
        id = inputID++;
    }

    Input(vector<int> nums) {
        this->obstacles = nums;
        id = inputID++;
    }
};

typedef vector<int> OUTPUT_TYPE;
typedef int LEVEL;
typedef int CHILD_ID;

struct Data {
    //map<int, int> children;
    map<LEVEL, CHILD_ID> children;
    Data() {
        children.clear();
    }

    Data(const Data& data) = delete;
    int AddChild(int val) {

        for (auto lvl = children.rbegin(); lvl != children.rend(); lvl++) {
            if (lvl->second > val) {
                continue;
            }
            int maxval = lvl->first + 1;
            if (children.find(maxval) != children.end()) {
                children[maxval] = min(children[maxval], val);
            }
            else {
                children[maxval] = val;
            }
            return maxval;
        }
        children[1] = val;
        return 1;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    vector<int> longestObstacleCourseAtEachPosition(vector<int>& obstacles) {

        size_t size = obstacles.size();

        if (size == 0) {
            return {};
        }

        vector<int> result(size, 1);
        Data root;

        for (size_t i = 0;i < size;i++) {
            result[i] = root.AddChild(obstacles[i]);
            LOG("@" << i << ":" << root);
        }

        return result;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return longestObstacleCourseAtEachPosition(nums.obstacles);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " obstacles:" << nums.obstacles;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {

    out << "{\"D" << d.children.size() << "\"";

    out << " : [";
    if (d.children.size() == 0) {
        out << "]}";
        return out;
    }
    for (auto itr = d.children.begin(); itr != d.children.end(); itr++) {
        out << "{\"" << itr->first << "\":" << itr->second << "},";
    }
    out << "{}]}";
    return out;
}

/****************************************************************************/

#define TEXT_LOG_PREFIX "___________\t"

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 1,2,3,2 }));
    expected.push_back({ 1,2,3,3 });

    nums.push_back(Input({ 2,2,1 }));
    expected.push_back({ 1,2,1 });

    nums.push_back(Input({ 5,1,5,5,1,3,4,5,1,4 }));
    expected.push_back({ 1,1,2,3,2,3,4,5,3,5 });

    nums.push_back(Input({ 2,2,3,5,1,4,4,1,5,1 }));
    expected.push_back({ 1,2,3,4,1,4,5,2,6,3 });

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.ProxyFunction(nums[i]);
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


