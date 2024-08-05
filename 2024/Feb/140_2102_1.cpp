
/*
140. Word Break II
Solved
Hard
Topics
Companies

Given a string s and a dictionary of strings wordDict, add spaces in s to construct a sentence where each word is a valid dictionary word. Return all such possible sentences in any order.

Note that the same word in the dictionary may be reused multiple times in the segmentation.



Example 1:

Input: s = "catsanddog", wordDict = ["cat","cats","and","sand","dog"]
Output: ["cats and dog","cat sand dog"]

Example 2:

Input: s = "pineapplepenapple", wordDict = ["apple","pen","applepen","pine","pineapple"]
Output: ["pine apple pen apple","pineapple pen apple","pine applepen apple"]
Explanation: Note that you are allowed to reuse a dictionary word.

Example 3:

Input: s = "catsandog", wordDict = ["cats","dog","sand","and","cat"]
Output: []



Constraints:

    1 <= s.length <= 20
    1 <= wordDict.length <= 1000
    1 <= wordDict[i].length <= 10
    s and wordDict[i] consist of only lowercase English letters.
    All the strings of wordDict are unique.
    Input is generated in a way that the length of the answer doesn't exceed 105.


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

using namespace std;

#define __DEBUG_PRINT_ITEMS 26

template <typename T>
ostream &operator<<(ostream &out, const vector<T> &nums)
{
    size_t size = nums.size();
    if (nums.size() > __DEBUG_PRINT_ITEMS)
        size = __DEBUG_PRINT_ITEMS;
    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << ",";
    }
    if (nums.size() > __DEBUG_PRINT_ITEMS)
    {
        out << "...";
    }
    return out;
}

template <typename T>
ostream &operator<<(ostream &out, vector<vector<T>> &nums)
{
    size_t size = nums.size();
    if (nums.size() > __DEBUG_PRINT_ITEMS)
        size = __DEBUG_PRINT_ITEMS;
    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << "::";
    }
    if (nums.size() > __DEBUG_PRINT_ITEMS)
    {
        out << "...";
    }
    return out;
}

template <typename T, typename U>
ostream &operator<<(ostream &out, const pair<T, U> &nums)
{
    out << "(" << nums.first << "," << nums.second << ")";
    return out;
}

template <typename T>
    requires(std::ranges::input_range<typename T::container_type>)
ostream &operator<<(ostream &out, const T &nums)
{

    struct Printer : T
    {
        ostream &print(ostream &out) const
        {
            out << "[" << this->c << "]";
            return out;
        }
    };

    static_cast<Printer const &>(nums).print(out);
    return out;
}

/****************************************************************************/

#define __DEBUG_LOG 3

#define LOGDATE __TIMESTAMP__
#define LOG_MSG(msg) \
    clog << "[" << LOGDATE << "]\t" << msg << endl

#define __DEBUG_LOG_NO 0
#define __DEBUG_LOG_NR 1
#define __DEBUG_LOG_VB 2
#define __DEBUG_LOG_O 4

#ifdef __DEBUG_LOG
#define LOG(x)                                                         \
    if (__DEBUG_LOG != __DEBUG_LOG_NO && __DEBUG_LOG != __DEBUG_LOG_O) \
    LOG_MSG(x)
#define LOGV(x)                        \
    if (__DEBUG_LOG == __DEBUG_LOG_VB) \
    LOG_MSG(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

#define RESULT_MOD 1000000007
#define MY_MAX_VAL 100001

#define ALFA_SIZE 26

inline int ALFA_INDEX(char c) { return c - 'a'; }
inline char ALFA_CHAR(int i) { return 'a' + i; }

struct TriDict
{
    int key;
    bool isWord;
    vector<TriDict *> children;

    TriDict(int k = -1, bool w = false) : key(k), isWord(w), children(ALFA_SIZE, nullptr) {}
    TriDict(char c, bool isWord = false) : TriDict(ALFA_INDEX(c), isWord) {}
    TriDict(const TriDict &t) : key(t.key), isWord(t.isWord), children(t.children) {}

    TriDict *Insert(char c)
    {
        int k = ALFA_INDEX(c);
        if (children[k] == nullptr)
            children[k] = new TriDict(k);
        return children[k];
    }

    void Insert(string s)
    {
        if (key != -1)
        {
            return;
        }
        TriDict *cur = this;
        for (char c : s)
        {
            cur = cur->Insert(c);
        }
        cur->isWord = true;
    }

    inline bool isValid(int ck)
    {
        return children[ck] != nullptr ? true : false;
    }
    inline bool isValid(char c) { return isValid(ALFA_INDEX(c)); }
    inline bool wordEndingAt(int k)
    {
        return (isValid(k) && children[k]->isWord == true) ? true : false;
    }

    vector<size_t> Match(string s)
    {
        if (key != -1)
        {
            return vector<size_t>();
        }

        vector<size_t> result;
        TriDict *cur = this;
        for (size_t i = 0; i < s.size() && cur != nullptr; i++)
        {
            // LOG("\t\tprocessing char:" << s[i] << " key:" << ALFA_CHAR(cur->key) << " children: " << cur->children);
            if (cur->isWord)
            {
                result.push_back(i);
            }
            cur = cur->children[ALFA_INDEX(s[i])];
        }
        if (cur != nullptr && cur->isWord)
        {
            result.push_back(s.size());
        }
        return result;
    }
    friend ostream &operator<<(ostream &out, const TriDict *t)
    {
        if (t == nullptr)
            return out;
        out << ALFA_CHAR(t->key);
        if (t->isWord)
            out << "*";
        return out;
    }
};

class Solution
{
public:
    vector<string> wordBreak(string s, vector<string> &wordDict)
    {
        TriDict root;
        size_t maxDictSize = 0;
        for (string word : wordDict)
        {
            root.Insert(word);
            maxDictSize = max(maxDictSize, word.size());
        }
        maxDictSize += 1;

        map<int, vector<string>> partials;

        int size = s.size();

        auto curMatches = root.Match(s);
        for (auto match : curMatches)
            partials[match].push_back(s.substr(0, match));

        for (int idx = 1; idx < size; idx++)
        {
            if (idx - maxDictSize >= 0)
            {
                partials.erase(idx - maxDictSize);
            }

            if (partials.find(idx) != partials.end() && partials[idx].size() == 0)
                continue;

            LOG("processing idx: " << idx << " with:" << s.substr(idx));
            auto curMatches = root.Match(s.substr(idx));
            LOG("\tidx: " << idx << " matches: " << curMatches);

            for (auto match : curMatches)
            {
                int nextIdx = idx + match;
                for (auto p : partials[idx])
                {
                    partials[nextIdx].push_back(p + " " + s.substr(idx, match));
                    LOG("\t\t\tPartials At " << nextIdx << ":" << partials[nextIdx]);
                }
            }
        }

        return partials[size];
    }
};

template <typename T>
// requires(std::ranges::input_range<typename T::container_type>)
void match_helper(const T &result, const T &expected)
{
    cout << "size: " << result.size() << " vs " << expected.size() << endl;
    for (size_t i = 0; i < result.size(); i++)
    {
        auto itr = find(expected.begin(), expected.end(), result[i]);
        if (itr == expected.end())
        {
            cout << "not found:" << result[i] << endl;
            assert(false);
        }
    }
    assert(result.size() == expected.size());
}
void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<pair<string, vector<string>>>({
        make_pair<string, vector<string>>("catsanddog", {"cat", "cats", "and", "sand", "dog"}),
        make_pair<string, vector<string>>("pineapplepenapple", {"apple", "pen", "applepen", "pine", "pineapple"}),
        make_pair<string, vector<string>>("catsandog", {"cats", "dog", "sand", "and", "cat"}),
    });

    auto expected = vector<vector<string>>({
        {"cats and dog", "cat sand dog"},
        {"pine apple pen apple", "pine applepen apple", "pineapple pen apple"},
        {},
    });

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.wordBreak(nums[i].first, nums[i].second);
        cout << result << " vs " << expected[i] << endl;
        match_helper(result, expected[i]);
    }
}

void test2(int testID)
{
    cout << "test" << testID << endl;
    auto nums = "aaaaaaa";
    auto dict = vector<string>({"aaaa", "aa", "a"});
    auto expected = vector<string>({"a a a a a a a", "aa a a a a a", "a aa a a a a", "a a aa a a a", "aa aa a a a", "aaaa a a a", "a a a aa a a", "aa a aa a a", "a aa aa a a", "a aaaa a a", "a a a a aa a", "aa a a aa a", "a aa a aa a", "a a aa aa a", "aa aa aa a", "aaaa aa a", "a a aaaa a", "aa aaaa a", "a a a a a aa", "aa a a a aa", "a aa a a aa", "a a aa a aa", "aa aa a aa", "aaaa a aa", "a a a aa aa", "aa a aa aa", "a aa aa aa", "a aaaa aa", "a a a aaaa", "aa a aaaa", "a aa aaaa"});
    Solution s;
    auto result = s.wordBreak(nums, dict);
    match_helper(result, expected);
}

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
