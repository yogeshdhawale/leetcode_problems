
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

typedef vector<string> V1S;
typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef V1Int OUTPUT_TYPE;


static int inputID = 1;
struct Input
{
    int id;
    V1S ops;
    V2Int nums;

    Input() {
        id = inputID++;
    }
    Input(V1S ops, V2Int nums) {
        this->ops = ops;
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

struct Data {

    void clear() {
    }
    Data() {
        clear();
    }
};

ostream& operator<<(ostream& out, const Data& d);

typedef int IDX1D;
typedef long DTYPE;

class Fancy {
public:

    vector<DTYPE> values;
    vector<DTYPE> adder;
    vector<DTYPE> multiplier;
    vector<DTYPE> prefix;

    int last;
    bool updated;

    Fancy() {
        values.clear();
        prefix.clear();
        adder.clear();
        multiplier.clear();
        last = -1;
        updated = false;
    }

    void append(int val) {
        values.push_back(val);
        adder.push_back(0);
        multiplier.push_back(1);
        last++;
    }

    void addAll(int inc) {
        if (values.empty()) {
            return;
        }
        updated = true;
        adder[last] = (adder.back() + inc) % RESULT_MOD;
    }

    void multAll(int m) {
        if (values.empty()) {
            return;
        }
        updated = true;
        adder[last] = (adder.back() * m) % RESULT_MOD;
        multiplier[last] = (multiplier.back() * m) % RESULT_MOD;
    }

    int getIndex(int idx) {
        if (values.empty() && prefix.empty()) {
            return -1;
        }

        if (idx > (last + prefix.size())) {
            return -1;
        }

        if (updated == true) {
            calculate(idx - prefix.size());
        }

        if (idx < prefix.size()) {
            return prefix[idx];
        }
        return values[idx - prefix.size()];
    }

    void calculate(int idx) {

        DTYPE m = 1;
        DTYPE a = 0;
        for (int i = last; i >= 0; i--) {
            DTYPE cur = values[i];

            a = (a + (adder[i] * m)) % RESULT_MOD;
            m = (m * multiplier[i]) % RESULT_MOD;

            cur = (cur * m) % RESULT_MOD;
            cur = (cur + a) % RESULT_MOD;

            values[i] = cur;
        }

        for (auto& p : prefix) {
            p = (p * m) % RESULT_MOD;
            p = (p + a) % RESULT_MOD;
        }

        copy(values.begin(), values.end(), back_inserter(prefix));
        values.clear();
        adder.clear();
        multiplier.clear();

        last = 0;
        values.push_back(prefix.back());
        adder.push_back(0);
        multiplier.push_back(1);
        prefix.pop_back();

        updated = false;
    }

    vector<int> getArray() {
        getIndex(0);
        LOG("values: " << values);

        vector<int> result;
        for (auto v : prefix) {
            result.push_back(v);
        }
        for (auto v : values) {
            result.push_back(v);
        }

        return result;
    }
};

class Solution {
public:
    OUTPUT_TYPE ProxyFunction(Input& nums) {
        Fancy* f = nullptr;
        for (size_t i = 0; i < nums.ops.size(); i++) {
            if (nums.ops[i] == "Fancy") {
                if (f != nullptr)
                    delete f;
                f = new Fancy();
            }

            if (f == nullptr) {
                continue;
            }

            if (nums.ops[i] == "append") {
                f->append(nums.nums[i][0]);
            }
            else if (nums.ops[i] == "addAll") {
                f->addAll(nums.nums[i][0]);
            }
            else if (nums.ops[i] == "multAll") {
                f->multAll(nums.nums[i][0]);
            }
            else if (nums.ops[i] == "getIndex") {
                int val = f->getIndex(nums.nums[i][0]);
                LOG("@" << i << " with idx: " << nums.nums[i][0] << " val: " << val);
            }
        }
        return f->getArray();
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

    nums.push_back(Input({ "Fancy", "append", "addAll", "append", "multAll", "getIndex", "addAll", "append", "multAll", "getIndex", "getIndex", "getIndex" },
        { {},{2},{3},{7},{2},{0},{3},{10},{2},{0},{1},{2} }));
    expected.push_back({ 26,34,20 });

    nums.push_back(Input({ "Fancy", "append", "addAll", "append", "multAll", "getIndex", "addAll", "append", "multAll", "addAll", "multAll", "getIndex", "getIndex", "getIndex" },
        { {},{2},{3},{7},{2},{0},{3},{10},{2},{1},{2}, {0},{1},{2} }));
    expected.push_back({ 54,70,42 });

    nums.push_back(Input({ "Fancy", "addAll", "getIndex" }, { {},{1},{0} }));
    expected.push_back({});

    nums.push_back(Input({ "Fancy", "append", "getIndex", "append", "multAll", "addAll", "multAll", "multAll", "multAll", "getIndex", "getIndex", "getIndex", "multAll", "multAll", "multAll", "getIndex", "getIndex", "addAll", "append", "append", "getIndex", "getIndex", "append", "getIndex" },
        { {},{2},{0},{5},{7},{9},{4},{3},{9},{1},{0},{0},{9},{6},{7},{1},{0},{8},{6},{3},{0},{3},{2},{3} }));
    expected.push_back({ 938960,1796264,6,3,2 });

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

    nums.push_back(Input(d1, d2));
    expected.push_back({ 1 });

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


