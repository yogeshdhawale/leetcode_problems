
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
ostream& operator<<(ostream& out, const node* n);


typedef int IDX;
typedef int COUNT;


struct Data {

    void clear() {
    }
    Data() {
        clear();
    }
};

struct seq {
    IDX id;
    char c;
    COUNT seqCount;

    seq(IDX id, char c, COUNT cnt) : id(id), c(c), seqCount(cnt) {
    }
};

ostream& operator<<(ostream& out, const seq* s) {
    out << "{@" << s->id << "," << s->c << "," << s->seqCount << "}";
    return out;
}

class Solution {
public:
    int findMinStep(string board, string hand) {

        int bcnt = board.size();
        int hcnt = hand.size();

        if (bcnt == 0 || hcnt == 0)
            return -1;

        map<char, COUNT> handAvailableCnt;
        for (auto c : hand) {
            handAvailableCnt[c]++;
        }

        map<IDX, seq*> boardMap;
        map<char, COUNT> maxhandNeeded;
        map<char, COUNT> totalBoardCnt;
        map<char, vector<seq*>> char2idx;

        for (int i = 0; i < bcnt; i++) {
            auto c = board[i];
            auto curCnt = 1;
            if (i < bcnt - 1 && board[i] == board[i + 1]) {
                curCnt = 2;
            }
            boardMap.insert({ i, new seq(i, c, curCnt) });
            maxhandNeeded[c] += 3 - curCnt;
            totalBoardCnt[c] += curCnt;
            char2idx[c].push_back(boardMap[i]);
            if (curCnt == 2) i++;
        }


        for (auto& [c, cnt] : totalBoardCnt) {
            if (cnt % 3 == 0) continue;
            auto minhandNeeded = 3 - cnt % 3;
            if (minhandNeeded > handAvailableCnt[c]) {
                LOG("Count match failed for: " << c << " " << cnt << " " << handAvailableCnt[c]);
                return -1;
            }
        }

        auto handCntLeft = hcnt;
        while (boardMap.size() > 0 && handCntLeft > 0) {

            LOG(endl << "handCntLeft: " << handCntLeft);
            LOG(boardMap);
            auto prevhandcntleft = handCntLeft;
            for (auto& [c, cnt] : maxhandNeeded) {

                if (boardMap.size() == 0) break;

                if (cnt <= handAvailableCnt[c]) {
                    LOG("Removing occurrences of " << c << " " << cnt << " " << handAvailableCnt[c]);
                    for (auto& seq : char2idx[c]) {
                        boardMap.erase(seq->id);
                    }
                    handCntLeft -= cnt;
                    handAvailableCnt[c] -= cnt;
                    merge(boardMap);
                    break;
                }
            }
            if (prevhandcntleft == handCntLeft) {
                break;
            }
        }

        if (boardMap.size() > 0) {
            return -1;
        }
        LOG("handCntLeft: " << handCntLeft);
        return hcnt - handCntLeft;
    }

    void merge(map<IDX, seq*>& boardMap) {

        if (boardMap.size() <= 1) return;
        auto prev = boardMap.begin();
        while (prev != boardMap.end()) {

            int cnt = 0;
            auto cur = next(prev);
            for (;cur != boardMap.end() && prev->second->c == cur->second->c; cur = next(cur)) {
                cnt += cur->second->seqCount;
            }

            if (cnt == 0) {
                prev = cur;
                continue;
            }

            cnt += prev->second->seqCount;

            if (cnt < 3) {
                prev->second->seqCount = cnt;
                prev = next(prev);
            }
            while (prev != cur) {
                prev = boardMap.erase(prev);
            }
            prev = cur;
        }
    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return findMinStep(nums.board, nums.hand);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.board << "::" << nums.hand;
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
    expected.push_back(6);

    nums.push_back(Input("RWWRWWRWW", "WWWBBBBRR"));
    expected.push_back(3);

    nums.push_back(Input("WRWW", "WWWRRBBWR"));
    expected.push_back(2);

    nums.push_back(Input("RWRR", "WWWRRBBWR"));
    expected.push_back(2);

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


