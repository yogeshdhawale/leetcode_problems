/*
1889. Minimum Space Wasted From Packaging
*/

/*

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
    vector<int> packages;
    vector<vector<int>> boxes;
    Input(vector<int> packages_, vector<vector<int>> boxes_) : packages(packages_), boxes(boxes_) {
        id = inputID++;
    }
};

typedef int OUTPUT_TYPE;
typedef int BOX_PROVIDER_ID;

struct Data {
    map<int, int> packages;
    map < BOX_PROVIDER_ID, set<int>> boxSizes;
    Data() {
    }
};


class WasteManager {
public:
    Data& data;
    map<BOX_PROVIDER_ID, int> wasted;
    WasteManager(Data& data) : data(data) {
    }
    int Process() {

        wasted.clear();

        for (auto [p, cnt] : data.packages) {
            LOG("package: " << p << ", cnt: " << cnt << ", Wasted:  " << wasted);
            auto itr = data.boxSizes.begin();

            while (itr != data.boxSizes.end()) {
                auto i = itr->first;
                auto& boxOptions = itr->second;

                auto option = boxOptions.begin();
                for (; *option < p && option != boxOptions.end(); ) {
                    option = boxOptions.erase(option);
                }

                if (option == data.boxSizes[i].end()) {
                    LOG("Box " << i << " invalid");
                    itr = data.boxSizes.erase(itr);
                    wasted.erase(i);
                    if (data.boxSizes.empty())
                        return -1;
                    continue;
                }

                auto boxsize = *option;
                wasted[i] = (wasted[i] + (((boxsize - p) * cnt) % RESULT_MOD)) % RESULT_MOD;
                itr++;
            }
        }

        LOG("wasted:" << wasted);
        if (wasted.empty())
            return -1;
        return min_element(wasted.begin(), wasted.end(),
            [](auto& a, auto& b) { return a.second < b.second; })->second;
    }
};

ostream& operator<<(ostream& out, const Data& data);
ostream& operator<<(ostream& out, const Input& nums);

class Solution {
public:
    int minWastedSpace(vector<int>& packages, vector<vector<int>>& boxes) {

        Data data;
        for (auto p : packages) {
            data.packages[p]++;
        }
        auto maxPsize = data.packages.rbegin()->first;

        for (size_t i = 0; i < boxes.size(); i++) {
            set<int> cur(boxes[i].begin(), boxes[i].end());
            if (*cur.rbegin() >= maxPsize)
                data.boxSizes[i] = cur;
        }

        if (data.boxSizes.empty())
            return -1;
        LOG("data:" << data);

        WasteManager wm(data);
        auto minWasted = wm.Process();

        LOG("minWasted:" << minWasted);
        return minWasted;
    }
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return minWastedSpace(nums.packages, nums.boxes);
    }
};

/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " packages:" << nums.packages << endl;
    out << " boxes:" << nums.boxes << endl;
    return out;
}

ostream& operator<<(ostream& out, const Data& d) {
    out << "packages:" << d.packages << endl;
    out << " boxSizes:" << d.boxSizes << endl;
    return out;
}

/****************************************************************************/

#define TEXT_LOG_PREFIX "___________\t"

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input({ 2, 3, 5 }, { {4, 8}, {2, 8} }));
    expected.push_back(6);

    nums.push_back(Input({ 2, 3, 5 }, { {1, 4}, {2, 3}, {3, 4} }));
    expected.push_back(-1);

    nums.push_back(Input({ 3, 5, 8, 10, 11, 12 }, { {12}, {11, 9}, {10, 5, 14} }));
    expected.push_back(9);

    nums.push_back(Input({ 2,1,5,4,4 }, { {2}, {10 }, {1,2} }));
    expected.push_back(34);

    nums.push_back(Input({ 7,19,11,7,16,13,15,5,9,5 }, { {18},{9,19,14},{13,12,8},{17},{3,2 } }));
    expected.push_back(23);


    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "\ttest for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.ProxyFunction(nums[i]);
        cout << TEXT_LOG_PREFIX << "\t loop: " << i << " ==> " << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_VB

#include "data.h"

void test2(int testID)
{

    cout << TEXT_LOG_PREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input(d1, d2));
    expected.push_back(o1);

    cout << TEXT_LOG_PREFIX << "\ttest for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << TEXT_LOG_PREFIX << "\ttest: " << testID << " ==> " << result << " vs " << expected[0] << endl;
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
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_VB
    test2(2);
#endif

    return 0;
}


