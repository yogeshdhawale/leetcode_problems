/*
2. Add Two Numbers
Medium

19711

3952

Add to List

Share
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order, and each of their nodes contains a single digit. Add the two numbers and return the sum as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.



Example 1:


Input: l1 = [2,4,3], l2 = [5,6,4]
Output: [7,0,8]
Explanation: 342 + 465 = 807.
Example 2:

Input: l1 = [0], l2 = [0]
Output: [0]
Example 3:

Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
Output: [8,9,9,9,0,0,0,1]


Constraints:

The number of nodes in each linked list is in the range [1, 100].
0 <= Node.val <= 9
It is guaranteed that the list represents a number that does not have leading zeros.
Accepted
2,920,583
Submissions
7,473,283
*/

#include <iostream>

using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}

    ListNode(int arr[], int size)
    {
        ListNode *cur = this;
        for (int i = 1; i < size; i++)
        {
            cur->val = arr[i - 1];
            cur->next = new ListNode(arr[i]);
            cur = cur->next;
        }
    }

    void PrintList()
    {
        ListNode *cur = this;
        while (cur != nullptr)
        {
            cout << cur->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

class Solution
{
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {

        // we are putting dummy node at the start, return the result from 2nd node
        ListNode *result = new ListNode(0);
        ListNode *cur = result;

        int carry = 0;
        while (l1 != nullptr && l2 != nullptr)
        {
            int value = l1->val + l2->val + carry;
            carry = value / 10;
            value = value % 10;
            cur->next = new ListNode(value);
            cur = cur->next;
            l1 = l1->next;
            l2 = l2->next;
        }

        ListNode *l3 = (l1 == nullptr) ? l2 : l1;
        while (l3 != nullptr)
        {
            int value = l3->val + carry;
            carry = value / 10;
            value = value % 10;
            cur->next = new ListNode(value);
            cur = cur->next;
            l3 = l3->next;
        }

        if (carry)
            cur->next = new ListNode(carry);

        cur = result->next;
        delete result;
        return cur;
    }
};

int main()
{
   // if (false)
    {
        Solution s;

        cout << "Test 1:" << endl;
        int arry[7] = {9, 9, 9, 9, 9, 9, 9};
        ListNode l1(arry, 7);
        ListNode l2(arry, 4);
        
        cout << "Input:" << endl;
        l1.PrintList();
        l2.PrintList();

        ListNode *result = s.addTwoNumbers(&l1, &l2);

        cout << "Result is: " << endl;
        result->PrintList();
    }
}
