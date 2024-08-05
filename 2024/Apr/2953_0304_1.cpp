
/*
2953. Count Complete Substrings
Hard
Topics
Companies
Hint

You are given a string word and an integer k.

A substring s of word is complete if:

    Each character in s occurs exactly k times.
    The difference between two adjacent characters is at most 2. That is, for any two adjacent characters c1 and c2 in s, the absolute difference in their positions in the alphabet is at most 2.

Return the number of complete substrings of word.

A substring is a non-empty contiguous sequence of characters in a string.



Example 1:

Input: word = "igigee", k = 2
Output: 3
Explanation: The complete substrings where each character appears exactly twice and the difference between adjacent characters is at most 2 are: igigee, igigee, igigee.

Example 2:

Input: word = "aaabbbccc", k = 3
Output: 6
Explanation: The complete substrings where each character appears exactly three times and the difference between adjacent characters is at most 2 are: aaabbbccc, aaabbbccc, aaabbbccc, aaabbbccc, aaabbbccc, aaabbbccc.



Constraints:

    1 <= word.length <= 105
    word consists only of lowercase English letters.
    1 <= k <= word.length


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

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef size_t POSITION;
typedef int VALUE;
typedef size_t COUNT;
typedef pair < int, int> RANGE;

typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    string nums;
    size_t k;

    Input() {
        id = inputID++;
    }
    Input(string nums, int k) {
        this->nums = nums;
        this->k = k;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct ChainNode {
    vector<RANGE> list;
    ChainNode() = delete;

    ChainNode(RANGE r) {
        list.push_back(r);
    }
    bool Add(RANGE range) {
        for (auto n : list) {
            if (n.second + 1 == range.first) {
                list.push_back(range);
                return true;
            }
        }
        return false;
    }
    bool operator<(const ChainNode& n) const {
        return list.size() < n.list.size();
    }
};

ostream& operator << (ostream& out, const ChainNode& n) {
    out << n.list;
    return out;
}

struct Data {
    size_t k;
    vector<map<POSITION, bool>> counts;

    map<POSITION, VALUE> unmatchedPositions;

    RANGE range;
    pair<POSITION, VALUE> prev;
    vector<ChainNode> chains;
    map<RANGE, bool> cache;

    void clear(int i = 0) {
        counts.clear();
        counts.resize(26, {});

        unmatchedPositions.clear();

        prev = { INVALID, INVALID };
        chains.clear();
        cache.clear();
    }

    Data(int size, int k) : k(k) {
        range = { 0, 0 };
        clear();
    }

    void newGroup(RANGE range) {

        if (cache.find(range) != cache.end()) return;

        cache[range] = true;
        if (chains.size() == 0) {
            chains.push_back(ChainNode(range));
            return;
        }

        for (auto& cn : chains) {
            if (cn.Add(range)) {
                return;
            }
        }
        chains.push_back(ChainNode(range));
    }

    void AddGroup() {
        if (range.second + 1 - range.first < k)
            return;

        if (unmatchedPositions.size() == 0) {
            newGroup(range);
            clearWindow(range.first);
            return;
        }

        RANGE curRange = { INT32_MAX, INT32_MIN };
        bool updated = false;
        for (size_t i = 0; i < 26; i++) {

            auto& vp = counts[i];
            if (vp.size() < k) continue;

            if (vp.begin()->first >= range.first) {
                updated = true;
                curRange.first = min(curRange.first, (int)vp.begin()->first);
                curRange.second = max(curRange.second, (int)vp.rbegin()->first);
            }
        }
        if (!updated)
            return;

        auto hole = unmatchedPositions.lower_bound(curRange.first);
        if (hole != unmatchedPositions.end() && hole->first <= curRange.second) {
            return;
        }

        newGroup(curRange);
        if (curRange.first == range.first)
            range = { curRange.first + 1, curRange.second + 1 };
    }

    int ExtendWindow(VALUE v, POSITION p) {

        AddGroup();

        int result = 0;
        if (prev.second != INVALID) {
            if (abs(v - prev.second) > 2) {
                result = EndWindow(range.first, range.second);
            }
        }

        counts[v][p] = true;
        prev = { p, v };
        range.second = p;

        if (counts[v].size() < k) {
            unmatchedPositions[p] = v;
            return result;
        }
        if (counts[v].size() == k) {
            for (auto [pos, flag] : counts[v])
                unmatchedPositions.erase(pos);
            return result;
        }

        auto newStart = counts[v].begin()->first;
        counts[v].erase(counts[v].begin());
        shrinkWindow(newStart);
        return result;
    }

    void shrinkWindow(POSITION pos) {
        for (auto i = range.first;i <= pos;i++) {
            if (unmatchedPositions.find(i) == unmatchedPositions.end()) continue;

            VALUE v = unmatchedPositions[i];
            unmatchedPositions.erase(i);
            counts[v].erase(counts[v].find(i));
            range.first = i + 1;
            AddGroup();
        }
        return;
    }

    void clearWindow(POSITION pos) {

        while (unmatchedPositions.size() > 0 && unmatchedPositions.begin()->first <= pos) {
            int i = unmatchedPositions.begin()->first;
            VALUE v = unmatchedPositions.begin()->second;
            counts[v].erase(i);
            unmatchedPositions.erase(unmatchedPositions.begin());
        }

        range.first = pos + 1;
    }

    int EndWindow() {
        AddGroup();
        return EndWindow(range.first, range.second);
    }

    int EndWindow(POSITION start, POSITION end) {

        int result = 0;
        for (auto i = start; i <= end; i++) {
            shrinkWindow(start);
        }

        sort(chains.begin(), chains.end());
        LOG("sorted groups:" << chains);

        for (auto& g : chains) {
            int size = g.list.size();
            result += (size * (size + 1)) / 2;
        }

        clear();

        return result;
    }
};

ostream& operator<<(ostream& out, const Data& d);

class Solution {
public:

    int countCompleteSubstrings(string word, int k) {

        if (k == 1) {
            // TODO
            return word.size();
        }

        int result = 0;
        Data d(word.size(), k);

        for (int i = 0; i < k; i++)
            d.ExtendWindow(word[i] - 'a', i);

        for (size_t i = k; i < word.size(); i++) {

            auto v = word[i] - 'a';
            LOG("processing @:" << i << " " << word[i] << "(" << v << ") result=" << result);
            int res = d.ExtendWindow(v, i);
            result += res;

            LOG("processed @:" << i << " " << word[i] << "(" << v << ") res=" << res << " updated result=" << result);
            LOG("\tafter: " << d);
        }

        int res = d.EndWindow();
        LOG("At end, result:" << result << " res:" << res);
        return result + res;
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return countCompleteSubstrings(nums.nums, nums.k);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    out << " k:" << nums.k;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {

    string sep;

    sep = " ";
    out << "counts:" << sep << "{";
    for (size_t i = 0; i < 26; i++) {
        if (d.counts[i].size() == 0) continue;
        out << sep << char('a' + i) << ":" << d.counts[i];
        sep = ", ";
    }
    out << "} " << endl;

    sep = " ";
    out << "unmatchedInfo:" << sep << "{";
    for (auto [p, v] : d.unmatchedPositions) {
        out << sep << char('a' + v) << ":" << p;
        sep = ", ";
    }
    out << "} ";
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
void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input("aaabbbccc", 3));
    expected.push_back(6);

    nums.push_back(Input("igigee", 2));
    expected.push_back(3);

    nums.push_back(Input("igigpee", 2));
    expected.push_back(2);

    nums.push_back(Input("igigfeef", 2));
    expected.push_back(4);

    nums.push_back(Input("igigfee", 2));
    expected.push_back(2);

    nums.push_back(Input("higighee", 2));
    expected.push_back(3);

    nums.push_back(Input("higigh", 2));
    expected.push_back(2);

    nums.push_back(Input("zzzzz", 3));
    expected.push_back(3);

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


