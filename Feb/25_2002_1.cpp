
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

using namespace std;

#define __DEBUG_PRINT_ITEMS 10

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

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/***************************************************************************/

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

struct ResultTracker
{
    ListNode *newcur;
    ListNode *newhead;
    ListNode *nextgroup;

    ResultTracker() : newcur(nullptr), newhead(nullptr), nextgroup(nullptr)
    {
    }
    ResultTracker(ListNode *cur) : newcur(nullptr), newhead(nullptr), nextgroup(cur)
    {
    }
    ResultTracker(ListNode *cur, ListNode *tail, ListNode *nextgroup) : newcur(cur), newhead(tail), nextgroup(nextgroup)
    {
    }
};
class Solution
{
public:
    ListNode *reverseKGroup(ListNode *head, int k)
    {
        if (k == 1)
            return head;

        ListNode *groupTail = new ListNode();
        ListNode *result = nullptr;

        ListNode *cur = head;
        while (cur != nullptr)
        {
            LOG("Processing " << cur->val);
            auto res = reverseList(cur, k);
            if (res.newhead == nullptr)
            {
                groupTail->next = cur;
                break;
            }

            LOG("Reversed :  " << res.newhead->val << " -> " << res.newcur->val << " nextGroup: " << (res.nextgroup ? res.nextgroup->val : 0));

            if (result == nullptr)
            {
                LOG("Setting up result " << res.newhead->val);
                result = res.newhead;
            }
            LOG("Setting past group tail " << groupTail->val << " to " << res.newcur->val);

            groupTail->next = res.newhead;
            groupTail = res.newcur;
            groupTail->next = nullptr;
            cur = res.nextgroup;
        }

        return result;
    }

    ResultTracker reverseList(ListNode *cur, int k)
    {
        if (cur == nullptr)
            return ResultTracker();
        if (k == 1)
            return ResultTracker(cur, cur, cur->next);

        auto res = reverseList(cur->next, k - 1);
        if (res.newcur == nullptr)
        {
            return res;
        }

        res.newcur->next = cur;
        LOG("\tLinking " << res.newcur->val << " -> " << cur->val);
        return ResultTracker(cur, res.newhead, res.nextgroup);
    }
};

ListNode *createList(vector<int> &nums)
{
    ListNode *head = new ListNode(nums[0]);
    ListNode *p = head;
    for (size_t i = 1; i < nums.size(); i++)
    {
        p->next = new ListNode(nums[i]);
        p = p->next;
    }
    return head;
}

void match_helper(ListNode *result, vector<int> &expected)
{
    for (size_t i = 0; i < expected.size(); i++)
    {
        if (result->val != expected[i])
        {
            assert(result->val == expected[i]);
        }
        result = result->next;
    }
    assert(result == nullptr);
}
void test1(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<pair<vector<int>, int>>({
        make_pair<vector<int>, int>({1, 2, 3, 4, 5}, 2),
        make_pair<vector<int>, int>({1, 2, 3, 4, 5}, 3),
        make_pair<vector<int>, int>({1, 2}, 2),
    });

    auto expected = vector<vector<int>>({
        {2, 1, 4, 3, 5},
        {3, 2, 1, 4, 5},
        {2, 1},
    });

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;
        auto result = s.reverseKGroup(createList(nums[i].first), nums[i].second);
        match_helper(result, expected[i]);
    }
}

void test2(int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<int>({1, 2});
    auto k = 2;
    auto expected = vector<int>({2, 1});
    cout << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.reverseKGroup(createList(nums), k);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us" << endl;
    cout << result << " vs " << expected << endl;
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
