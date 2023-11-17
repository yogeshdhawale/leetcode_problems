/*

25. Reverse Nodes in k-Group
Hard
9.9K
556
Companies
Given the head of a linked list, reverse the nodes of the list k at a time, and return the modified list.

k is a positive integer and is less than or equal to the length of the linked list. If the number of nodes is not a multiple of k then left-out nodes, in the end, should remain as it is.

You may not alter the values in the list's nodes, only nodes themselves may be changed.



Example 1:


Input: head = [1,2,3,4,5], k = 2
Output: [2,1,4,3,5]
Example 2:


Input: head = [1,2,3,4,5], k = 3
Output: [3,2,1,4,5]


Constraints:

The number of nodes in the list is n.
1 <= k <= n <= 5000
0 <= Node.val <= 1000


Follow-up: Can you solve the problem in O(1) extra memory space?

*/

/**
 * Definition for singly-linked list.
 */

package reverseNodes

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

type ListNode struct {
	Val  int
	Next *ListNode
}

func NewNode(data int) *ListNode {
	node := new(ListNode)
	node.Val = data
	node.Next = nil
	return node
}

func insert(node *ListNode, next *ListNode) *ListNode {
	node.Next = next
	return next
}
func makeList(arr []int) *ListNode {

	head := NewNode(-1)
	cur := head
	for _, v := range arr {
		cur = insert(cur, NewNode(v))
	}
	cur = head.Next
	head = nil
	return cur
}

func ListLen(node *ListNode) int {
	cnt := 0
	cur := node

	for cur != nil {
		cur = cur.Next
		cnt += 1
	}
	return cnt
}

func ListPrint(node *ListNode) {
	cur := node
	fmt.Print("List is: ")
	for cur != nil {
		fmt.Printf("%d -> ", cur.Val)
		cur = cur.Next
	}
	fmt.Println()
}

func ListGetItem(node *ListNode, index int) int {
	cur := node
	for cur != nil && index > 0 {
		cur = cur.Next
		index -= 1
	}
	return cur.Val
}

func NextNode(node *ListNode) *ListNode {
	return node.Next
}

func split(node *ListNode, k int, stk []*ListNode) (*ListNode, *ListNode, bool) {

	stk[0] = node
	cur := node.Next

	i := 1
	for ; cur != nil && i < k; i++ {
		stk[i] = cur
		cur = cur.Next

		stk[i].Next = stk[i-1]
	}

	if i != k {
		updata := i
		for i := 0; i < updata-1; i++ {
			stk[i].Next = stk[i+1]
		}

		stk[updata-1].Next = nil
		return nil, nil, false
	}

	stk[0].Next = cur
	return stk[k-1], stk[0], true
}

func reverseKGroup(head *ListNode, k int) *ListNode {

	if k == 0 || k == 1 {
		return head
	}

	tail := head
	stk := make([]*ListNode, k)

	newhead, tail, ok := split(tail, k, stk)
	if !ok {
		return head
	}

	prevtail := tail // last ele of processeed list
	cur := tail.Next

	for cur != nil {
		//fmt.Printf("reminder list is from: %d\n", cur.Val)
		head, tail, ok = split(cur, k, stk)
		if !ok {
			return newhead
		}
		prevtail.Next = head
		prevtail = tail
		cur = tail.Next
	}
	return newhead
}

func Test1(t *testing.T) {

	input := []int{1, 2, 0, 4}
	k := 3
	expected := len(input)
	list := makeList(input)
	ListPrint(list)
	result := reverseKGroup(list, k)
	ListPrint(result)

	assert.Equal(t, expected, ListLen(result), "simple test failed")
	fmt.Printf("%d vs %d", input[k-1], ListGetItem(result, 0))
	assert.Equal(t, input[k-1], ListGetItem(result, 0), "data match failed")
}

func Test2(t *testing.T) {

	input := []int{1, 2, 3, 4, 5}
	k := 3
	expected := len(input)
	result := reverseKGroup(makeList(input), k)

	assert.Equal(t, expected, ListLen(result), "simple test failed")
	assert.Equal(t, input[k-1], ListGetItem(result, 0), "data match failed")
}

func Test3(t *testing.T) {

	input := []int{1, 2, 3, 4, 5}
	k := 2
	expected := len(input)
	result := reverseKGroup(makeList(input), k)

	assert.Equal(t, expected, ListLen(result), "simple test failed")
	assert.Equal(t, input[k-1], ListGetItem(result, 0), "data match failed")
}

func Test4(t *testing.T) {

	input := []int{1}
	k := 4
	expected := len(input)
	list := makeList(input)
	result := reverseKGroup(list, k)

	assert.Equal(t, expected, ListLen(result), "simple test failed")
}
