/*
2092. Find All People With Secret
Hard
Topics
Companies
Hint

You are given an integer n indicating there are n people numbered from 0 to n - 1. You are also given a 0-indexed 2D integer array meetings where meetings[i] = [xi, yi, timei] indicates that person xi and person yi have a meeting at timei. A person may attend multiple meetings at the same time. Finally, you are given an integer firstPerson.

Person 0 has a secret and initially shares the secret with a person firstPerson at time 0. This secret is then shared every time a meeting takes place with a person that has the secret. More formally, for every meeting, if a person xi has the secret at timei, then they will share the secret with person yi, and vice versa.

The secrets are shared instantaneously. That is, a person may receive the secret and share it with people in other meetings within the same time frame.

Return a list of all the people that have the secret after all the meetings have taken place. You may return the answer in any order.



Example 1:

Input: n = 6, meetings = [[1,2,5},{2,3,8},{1,5,10]], firstPerson = 1
Output: [0,1,2,3,5]
Explanation:
At time 0, person 0 shares the secret with person 1.
At time 5, person 1 shares the secret with person 2.
At time 8, person 2 shares the secret with person 3.
At time 10, person 1 shares the secret with person 5.​​​​
Thus, people 0, 1, 2, 3, and 5 know the secret after all the meetings.

Example 2:

Input: n = 4, meetings = [[3,1,3},{1,2,2},{0,3,3]], firstPerson = 3
Output: [0,1,3]
Explanation:
At time 0, person 0 shares the secret with person 3.
At time 2, neither person 1 nor person 2 know the secret.
At time 3, person 3 shares the secret with person 0 and person 1.
Thus, people 0, 1, and 3 know the secret after all the meetings.

Example 3:

Input: n = 5, meetings = [[3,4,2},{1,2,1},{2,3,1]], firstPerson = 1
Output: [0,1,2,3,4]
Explanation:
At time 0, person 0 shares the secret with person 1.
At time 1, person 1 shares the secret with person 2, and person 2 shares the secret with person 3.
Note that person 2 can share the secret at the same time as receiving it.
At time 2, person 3 shares the secret with person 4.
Thus, people 0, 1, 2, 3, and 4 know the secret after all the meetings.



Constraints:

    2 <= n <= 105
    1 <= meetings.length <= 105
    meetings[i].length == 3
    0 <= xi, yi <= n - 1
    xi != yi
    1 <= timei <= 105
    1 <= firstPerson <= n - 1


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

#define __DEBUG_PRINT_ITEMS 10

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& nums)
{
    size_t size = nums.size();
    if (nums.size() > __DEBUG_PRINT_ITEMS) {
        out << "V:" << size << "=";
        size = __DEBUG_PRINT_ITEMS;
    }
    out << "{";

    for (size_t i = 0; i < size; i++)
    {
        out << nums[i] << ",";
    }
    if (nums.size() > __DEBUG_PRINT_ITEMS)
    {
        out << "...";
    }
    out << "}";
    return out;
}

template <typename T, typename U>
ostream& operator<<(ostream& out, const pair<T, U>& nums)
{
    out << "(" << nums.first << "," << nums.second << ")";
    return out;
}

template <typename T, typename U>
ostream& operator<<(ostream& out, const map<T, U>& nums)
{
    out << "{";
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
        out << it->first << ", ";
        //out << it->first << ":" << it->second << ", ";
    }
    out << "}";
    return out;
}

template <typename T>
    requires(std::ranges::input_range<typename T::container_type>)
ostream& operator<<(ostream& out, const T& nums)
{

    struct Printer : T
    {
        ostream& print(ostream& out) const
        {
            out << "[" << this->c << "]";
            return out;
        }
    };

    static_cast<Printer const&>(nums).print(out);
    return out;
}

#define __DEBUG_LOG 4

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

/****************************************************************************/

#include <ranges>

#define INVALID -1
#define RESULT_MOD 1000000007
#define MY_MAX_VAL 100001

#ifndef __DEBUG_LOG 
#define LOG(x)
#define LOGV(x)
#endif

typedef vector<int> V1D;
typedef vector<V1D> V2D;



#define DONTKNOW -1
class Solution {

    typedef vector<int> RECEIVER_MAP;       // personIds(R)
    typedef map <int, RECEIVER_MAP> GIVER_MAP;   // {G, R}

    vector<bool> secretKeepers;             // personIds(G)
    map<int, GIVER_MAP> meetingSchedule;    // {time, {G, R}}

public:

    bool knowsSecret(int person) {
        if (secretKeepers[person]) {
            return true;
        }
        return false;
    }

    void AddToMeetingSchedule(int giver, int receiver, int time) {

        meetingSchedule[time][giver].push_back(receiver);
    }

    int updateSecretKeeper(int giver, int receiver) {
        if(secretKeepers[receiver]) {
            return INVALID;
        }
        secretKeepers[receiver] = true;
        return receiver;
    }

    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {

        secretKeepers.clear();
        meetingSchedule.clear();

        secretKeepers.resize(n, false);
        secretKeepers[0] = true;
        secretKeepers[firstPerson] = true;
        int sKcnt = 2;

        for (size_t i = 0; i < meetings.size(); i++) {
            auto p1 = meetings[i][0];
            auto p2 = meetings[i][1];
            auto time = meetings[i][2];
            AddToMeetingSchedule(p1, p2, time);
            AddToMeetingSchedule(p2, p1, time);
        }

        for (auto [time, givers] : meetingSchedule) {

            LOG("@ " << time << " " << sKcnt);

            if (sKcnt >= n) {
                break;
            }

            vector<int> newGivers;
            for (auto cur : 
            (ranges::views::keys(givers) | ranges::views::filter([this](int p) { return secretKeepers[p]; }) )) {
                for (auto receiver : givers[cur]) {
                    if (updateSecretKeeper(cur, receiver) != INVALID) {
                        newGivers.push_back(receiver);
                        sKcnt++;
                    }
                }
            }
            
            while(newGivers.size()) {
    
                LOG("New givers: " << newGivers);
                vector <int> curGivers = newGivers;
                newGivers.clear();

                for (auto cur : curGivers) {
                    for (auto receiver : givers[cur]) {
                    if (updateSecretKeeper(cur, receiver) != INVALID) {
                        newGivers.push_back(receiver);
                        sKcnt++;
                    }
                    }
                }
            }
        }

        vector<int> res;
        for (int id = 0; id < n; id++) {
            if (secretKeepers[id]) {
                res.push_back(id);
            }
        }
        return res;
    }

};


/***************************************************************************/

struct Input {
    int n;
    V2D meetings;
    int firstPerson;
    Input(int n, V2D meetings, int firstPerson) :n(n), meetings(meetings), firstPerson(firstPerson) {}
};

ostream& operator<<(ostream& out, const Input& nums)
{
    out << "[" << nums.n << "," << nums.firstPerson << " =>" << nums.meetings << "]";
    return out;
}

void test1(int testID)
{
    vector<Input> nums;
    V2D expected;

    nums.push_back({ 6, {{1, 2, 5}, {2, 3, 8}, {1, 5, 10}} , 1 });
    expected.push_back({ 0, 1, 2, 3, 5 });

    nums.push_back({ 4, {{3, 1, 3}, {1, 2, 2}, {0, 3, 3}} , 3 });
    expected.push_back({ 0, 1, 3 });

    nums.push_back({ 5, {{3, 4, 2}, {1, 2, 1}, {2, 3, 1}} , 1 });
    expected.push_back({ 0, 1, 2, 3, 4 });

    nums.push_back(Input(5, { {1, 4, 3},{0, 4, 3} }, 3));
    expected.push_back({ 0, 1, 3, 4 });

    nums.push_back(Input(6, { {0, 2, 1},{1, 3, 1},{4, 5, 1} }, 1));
    expected.push_back({ 0, 1,2, 3 });

    nums.push_back(Input(12, { {10, 8, 6},{9, 5, 11},{0, 5, 18},{4, 5, 13},{11, 6, 17},{0, 11, 10},{10, 11, 7},{5, 8, 3},{7, 6, 16},{3, 6, 10},{3, 11, 1},{8, 3, 2},{5, 0, 7},{3, 8, 20},{11, 0, 20},{8, 3, 4},{1, 9, 4},{10, 7, 11},{8, 10, 18} }, 9));
    expected.push_back({ 0,1,4,5,6,9,11 });

    nums.push_back(Input(5, { {1, 2, 2},{2, 1, 2},{2, 4, 2},{0, 3, 2} }, 4));
    expected.push_back(vector<int>({ 0,1,2,3,4 }));

    cout << "test" << testID << " test cases: " << nums.size() << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.findAllPeople(nums[i].n, nums[i].meetings, nums[i].firstPerson);
        cout << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{

    cout << "test" << testID << endl;
    auto nums = Input(4, { {1,2,1},{0,3,1},{2,0,1} }, 3);
    auto expected = vector<int>({ 0,1,2,3 });

    cout << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.findAllPeople(nums.n, nums.meetings, nums.firstPerson);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us" << endl;
    cout << result << " vs " << expected << endl;
    assert(result == expected);
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
