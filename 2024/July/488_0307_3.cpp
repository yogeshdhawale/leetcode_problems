
/*
488. Zuma Game
Hard
Topics
Companies

You are playing a variation of the game Zuma.

In this variation of Zuma, there is a single row of colored balls on a board, where each ball can be colored red 'R', yellow 'Y', blue 'B', green 'G', or white 'W'. You also have several colored balls in your hand.

Your goal is to clear all of the balls from the board. On each turn:

    Pick any ball from your hand and insert it in between two balls in the row or on either end of the row.
    If there is a group of three or more consecutive balls of the same color, remove the group of balls from the board.
        If this removal causes more groups of three or more of the same color to form, then continue removing each group until there are none left.
    If there are no more balls on the board, then you win the game.
    Repeat this process until you either win or do not have any more balls in your hand.

Given a string board, representing the row of balls on the board, and a string hand, representing the balls in your hand, return the minimum number of balls you have to insert to clear all the balls from the board. If you cannot clear all the balls from the board using the balls in your hand, return -1.



Example 1:

Input: board = "WRRBBW", hand = "RB"
Output: -1
Explanation: It is impossible to clear all the balls. The best you can do is:
- Insert 'R' so the board becomes WRRRBBW. WRRRBBW -> WBBW.
- Insert 'B' so the board becomes WBBBW. WBBBW -> WW.
There are still balls remaining on the board, and you are out of balls to insert.

Example 2:

Input: board = "WWRRBBWW", hand = "WRBRW"
Output: 2
Explanation: To make the board empty:
- Insert 'R' so the board becomes WWRRRBBWW. WWRRRBBWW -> WWBBWW.
- Insert 'B' so the board becomes WWBBBWW. WWBBBWW -> WWWW -> empty.
2 balls from your hand were needed to clear the board.

Example 3:

Input: board = "G", hand = "GGGGG"
Output: 2
Explanation: To make the board empty:
- Insert 'G' so the board becomes GG.
- Insert 'G' so the board becomes GGG. GGG -> empty.
2 balls from your hand were needed to clear the board.



Constraints:

    1 <= board.length <= 16
    1 <= hand.length <= 5
    board and hand consist of the characters 'R', 'Y', 'B', 'G', and 'W'.
    The initial row of balls on the board will not have any groups of three or more consecutive balls of the same color.


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

#include "log.h"

using namespace std;

/****************************************************************************/
#include <list>
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

typedef string INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    INPUT_TYPE board;
    INPUT_TYPE hand;

    Input() {
        id = inputID++;
    }
    Input(INPUT_TYPE board, INPUT_TYPE hand) {
        this->board = board;
        this->hand = hand;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};

struct Data;
struct node;
ostream& operator<<(ostream& out, const Data& d);


typedef int IDX;
typedef int COUNT;


struct Data {
    void clear() {
    }
};


struct UniqCharSeq {
    IDX id;
    char c;
    COUNT seqCount;

    UniqCharSeq(IDX id, char c, COUNT cnt) : id(id), c(c), seqCount(cnt) {
    }
    UniqCharSeq(const UniqCharSeq& s) : id(s.id), c(s.c), seqCount(s.seqCount) {
    }

    UniqCharSeq& operator=(const UniqCharSeq& s) {
        id = s.id;
        c = s.c;
        seqCount = s.seqCount;
        return *this;
    }
    string toString() const {
        return "@" + to_string(c) + "#" + to_string(seqCount);
    }
};

ostream& operator<<(ostream& out, const UniqCharSeq* s) {
    out << "{@" << s->id << "," << s->c << "," << s->seqCount << "}";
    return out;
}
ostream& operator<<(ostream& out, const UniqCharSeq& s) {
    out << "{@" << s.id << "," << s.c << "," << s.seqCount << "}";
    return out;
}

struct CharCount {
    map<char, COUNT> cnt;
    CharCount() {
        cnt.clear();
    }
    void add(char c, int val) {
        cnt[c] += val;
    }
    int get(char c) const {
        if (cnt.find(c) == cnt.end()) return 0;
        return cnt.at(c);
    }
    void sub(char c, int val) {
        cnt[c] -= val;
    }
    bool isSubset(const CharCount& other) const {
        for (auto [c, v] : other.cnt) {
            if (v == 0) continue;
            if (cnt.find(c) == cnt.end()) return false;
            if (cnt.at(c) < v)  return false;
        }
        return true;
    }
};

class Solution {
public:

    // TODO how cache should work
    unordered_map<string, int> cache;
    Solution() {
        cache.clear();
    }
    int findMinStep(string board, string hand) {

        size_t bsize = board.size();
        size_t hsize = hand.size();

        map<IDX, UniqCharSeq> boardSeq;
        CharCount bCharOccuranceCnt;
        CharCount hCharCntMap;
        map<char, vector<IDX>> char2IdxMap;

        for (auto c : hand) {
            hCharCntMap.add(c, 1);
        }

        for (size_t i = 0; i < bsize; i++) {
            auto c = board[i];
            auto curCnt = 1;
            if (i < bsize - 1 && board[i] == board[i + 1]) {
                curCnt = 2;
            }
            boardSeq.insert({ i, UniqCharSeq(i, c, curCnt) });
            char2IdxMap[c].push_back(i);
            bCharOccuranceCnt.add(c, curCnt);
            if (curCnt == 2) i++;
        }

        LOG("boardMap:" << boardSeq);
        LOG("boardCntMap:" << bCharOccuranceCnt.cnt);
        LOG("handCntMap:" << hCharCntMap.cnt);
        LOG(endl);

        int prefixResult = 0;
        for (auto [c, bc] : bCharOccuranceCnt.cnt) {
            if (bc > 2) continue;
            if (bc + hCharCntMap.get(c) < 3) {
                LOG("Not enough hands for valid solution for c:"
                    << c << " bc:" << bc << " handCntMap[c]:" << hCharCntMap.get(c));
                return -1;
            }

            if (char2IdxMap[c].size() == 1) {
                auto itr = boardSeq.find(char2IdxMap[c].back());
                char2IdxMap[c].pop_back();

                if (itr == boardSeq.end()) continue;

                auto neededHands = 3 - (itr->second.seqCount % 3);
                hCharCntMap.sub(c, neededHands);
                prefixResult += neededHands;
                auto next_itr = boardSeq.erase(itr);
                LOG("erasing " << itr->second << "prefixResult:" << prefixResult);
                merge(boardSeq, next_itr);
            }
        }

        if (boardSeq.size() == 0) {
            return prefixResult;
        }

        LOG("boardMap:" << boardSeq);
        LOG("handCntMap:" << hCharCntMap.cnt);
        LOG(endl);

        int pendingHandCnt = hsize - prefixResult;
        int result = boardSeq.size() * 3;

        for (auto itr = boardSeq.begin(); itr != boardSeq.end(); itr++) {

            LOG("Starting from: " << itr->second << " pendingHandCnt:" << pendingHandCnt);
            int ret = process(itr->second, boardSeq, hCharCntMap, pendingHandCnt);

            if (ret != -1)
                result = min(ret, result);
            LOG("ret:" << ret << " result:" << result);
        }
        if (result == boardSeq.size() * 3) return -1;
        return result + prefixResult;
    }

    string toString(map<IDX, UniqCharSeq> boardMap) {
        string ret = "";
        for (auto itr = boardMap.begin(); itr != boardMap.end(); itr++) {
            ret += itr->second.toString() + ",";
        }
        return ret;
    }

    int process(UniqCharSeq curSeq, map<IDX, UniqCharSeq> boardMap, CharCount handCntMap, int pendingHandCnt) {

        auto neededCnt = 3 - (curSeq.seqCount % 3);
        auto bmstring = toString(boardMap);

        if (pendingHandCnt < neededCnt) return -1;
        if (handCntMap.get(curSeq.c) < neededCnt) return -1;

        pendingHandCnt -= neededCnt;
        handCntMap.sub(curSeq.c, neededCnt);

        LOG("\tIn " << boardMap << " erasing:" << curSeq << " handsUsed:" << 3 - curSeq.seqCount);
        auto next_itr = boardMap.erase(boardMap.find(curSeq.id));
        merge(boardMap, next_itr);

        int result = boardMap.size() * 3;
        if (boardMap.size() == 0) {
            return neededCnt;
        }

        for (auto itr = boardMap.begin(); itr != boardMap.end(); itr++) {
            auto& curSeq = itr->second;
            int ret = process(curSeq, boardMap, handCntMap, pendingHandCnt);
            if (ret != -1)
                result = min(result, ret);
        }
        LOGV("\tReturing neededCnt:" << neededCnt << " result:" << result << " for " << boardMap);
        int loopResult = -1;
        if (result != boardMap.size() * 3)
            loopResult = neededCnt + result;

        cache[bmstring] = loopResult;
        return loopResult;
    }

    void merge(map<IDX, UniqCharSeq>& boardMap, auto next_itr) {

        if (next_itr == boardMap.begin() || next_itr == boardMap.end()) return;

        auto prev_itr = prev(next_itr);
        if (prev_itr->second.c != next_itr->second.c) return;

        prev_itr->second.seqCount += next_itr->second.seqCount;

        next_itr = boardMap.erase(next_itr);
        if (prev_itr->second.seqCount > 2) boardMap.erase(prev_itr);

        merge(boardMap, next_itr);
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return findMinStep(nums.board, nums.hand);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums: board " << nums.board << " hand " << nums.hand;
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

void test1(int testID)
{
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input("WRRBBW", "RB"));
    expected.push_back(-1);

    nums.push_back(Input("WWRRBBW", "WRBRW"));
    expected.push_back(2);

    nums.push_back(Input("RRWWBBR", "WBRW"));
    expected.push_back(2);

    nums.push_back(Input("RWWRWWRWW", "RWBRRRRRR"));
    expected.push_back(4);
    // RWW*RWWRWW -> (1W) -> RRWWR*WW -> (2R) -> RRWWWW -> RR* -> (1R) -> RRR -> "" -> Done

    nums.push_back(Input("RWWRWWRWW", "WWWBBBBRR"));
    expected.push_back(3);

    nums.push_back(Input("WRWW", "WWWRRBBWR"));
    expected.push_back(2);

    nums.push_back(Input("WWRWW", "RR"));
    expected.push_back(2);

    nums.push_back(Input("RWRR", "WWWRRBBWR"));
    expected.push_back(2);

    nums.push_back(Input("RWRWW", "WWWRRBBWR"));
    expected.push_back(4);

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


