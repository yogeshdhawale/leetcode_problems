/*
41. First Missing Positive
Hard

Given an unsorted integer array nums, return the smallest missing positive integer.

You must implement an algorithm that runs in O(n) time and uses constant extra space.



Example 1:

Input: nums = [1,2,0]
Output: 3
Explanation: The numbers in the range [1,2] are all in the array.
Example 2:

Input: nums = [3,4,-1,1]
Output: 2
Explanation: 1 is in the array but 2 is missing.
Example 3:

Input: nums = [7,8,9,11,12]
Output: 1
Explanation: The smallest positive integer 1 is missing.


Constraints:

1 <= nums.length <= 105
-2^31 <= nums[i] <= 2^31 - 1
*/

package firstpositive

import (
	"fmt"
	"math"
	"testing"

	"github.com/stretchr/testify/assert"
)

func setbit(flag *int, index int) {
	*flag |= (1 << index)
}
func getbit(flag int, index int) bool {
	return flag&(1<<(index)) != 0
}

func setbit_v(flag int32, index int) int32 {
	return flag | (1 << index)
}
func getbit_v(flag int32, index int) bool {
	return flag&(1<<(index)) != 0
}

func firstMissingPositive_array(nums []int) int {

	size := int(math.Pow(2, 31)) / 32
	//size := 67108864
	result := make([]int, size)

	for i := 0; i < len(result); i++ {
		result[i] = 0
	}

	// skip 0 as its not positive, behave as if its already there
	setbit(&result[0], 0)

	for _, v := range nums {

		if v > 0 {
			id := v / 32
			mod := v % 32
			setbit(&result[id], mod)

			//fmt.Printf("pre flag is: %v [%v, %v] : %v", v, id, mod, result[id])
			//setbit(&result[id], mod)
			//fmt.Printf("\tpost flag is: %v\n", result[id])
		}
	}

	maxv := 0
	for i := 0; i < size; i++ {

		if result[i] != -1 {
			for j := 0; j < 32; j++ {
				if getbit(result[i], j) == false {
					fmt.Printf("returning at: %v, %v\n", i, j)
					return i*32 + j
				}
				if maxv < i {
					maxv = i
				}
			}
		}
	}
	return maxv + 1
}

func firstMissingPositive(nums []int) int {

	chunkSize := 32
	size := int(math.Pow(2, 31)) / chunkSize
	result := make(map[int]int32)

	for i := 0; i < len(result); i++ {
		result[i] = 0
	}

	// skip 0 as its not positive, behave as if its already there
	result[0] = 1

	for _, v := range nums {

		if v > 0 {

			id := v / chunkSize
			mod := v % chunkSize

			//fmt.Printf("pre flag is: %v [%v, %v] : %v", v, id, mod, result[id])
			result[id] = setbit_v(result[id], mod)
			//fmt.Printf("\tpost flag is: %v\n", result[id])
		}
	}

	for i := 0; i < size; i++ {
		flag, ok := result[i]
		if !ok {
			continue
		}
		//fmt.Printf("processing block: %v:%v\n", i, flag)
		if flag == -1 {
			continue
		}
		for j := 0; j < chunkSize; j++ {
			val := i*chunkSize + j
			if getbit_v(flag, j) == false {
				//fmt.Printf("returning at: %v, %v\n", i, j)
				return val
			}
		}
	}
	return len(result) * chunkSize
}
func Test1(t *testing.T) {

	milestones := []int{1, 2, 0}
	expected := 3
	result := firstMissingPositive(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test2(t *testing.T) {

	milestones := []int{3, -1, 2, 0, 32, 33}
	expected := 1
	result := firstMissingPositive(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test3(t *testing.T) {

	milestones := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127}
	expected := 128
	result := firstMissingPositive(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test4(t *testing.T) {

	milestones := []int{98, 93, 95, 10, 91, 4, 90, 88, 56, 84, 65, 62, 83, 80, 78, 60, 73, 77, 76, 29, 63, 12, 57, 17, 69, 68, 50, 11, 31, 33, 8, 42, 38, 7, 0, 37, 48, 26, 20, 44, 46, 43, 52, 51, 47, 18, 49, 58, 2, 39, 30, 81, 22, 55, 36, 40, 15, 27, 21, 32, 64, 41, 53, 19, 28, 24, 9, 25, 3, 59, 66, 82, 61, 70, 23, 34, 71, 54, 74, -1, 1, 45, 14, 79, 5, 35, 13, 72, 75, 85, 87, 6, 16, 86, 67, 89, 94, 92, 96, 97}
	expected := 99
	result := firstMissingPositive(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}
