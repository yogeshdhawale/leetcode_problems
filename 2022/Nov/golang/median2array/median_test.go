/*

4. Median of Two Sorted Arrays
Hard

20937

2367

Add to List

Share
Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays.

The overall run time complexity should be O(log (m+n)).



Example 1:

Input: nums1 = [1,3], nums2 = [2]
Output: 2.00000
Explanation: merged array = [1,2,3] and median is 2.
Example 2:

Input: nums1 = [1,2], nums2 = [3,4]
Output: 2.50000
Explanation: merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.


Constraints:

nums1.length == m
nums2.length == n
0 <= m <= 1000
0 <= n <= 1000
1 <= m + n <= 2000
-106 <= nums1[i], nums2[i] <= 106

*/

package median

import (
	"fmt"
	"math"
	"testing"

	"github.com/stretchr/testify/assert"
)

const LOG = 1

func findMedian(nums []int, startidx int, endidx int) float64 {

	len := endidx - startidx + 1
	mid := startidx + ((endidx - startidx) / 2)
	isodd := (len % 2) != 0
	if LOG == 1 {
		fmt.Printf("Single Array(%v):[ %v, %v, %v]\n", len, startidx, mid, endidx)
	}

	if len == 0 {
		return 0
	} else if len == 1 {
		return float64(nums[startidx])
	}

	if isodd {
		return float64(nums[mid])
	} else {
		return float64(nums[mid]+nums[mid+1]) / 2
	}
}
func findMedianSortedArrays(nums1 []int, nums2 []int) float64 {
	//return findMedianSortedArrays_online(nums1, nums2)
	return findMedianSortedArrays_my(nums1, nums2)
}
func findMedianSortedArrays_online(nums1 []int, nums2 []int) float64 {

	if len(nums1) > len(nums2) {
		return findMedianSortedArrays_online(nums2, nums1)
	}

	len1 := len(nums1)
	len2 := len(nums2)

	if len1 == 0 {
		return findMedian(nums2, 0, len2-1)
	}
	if len2 == 0 {
		return findMedian(nums1, 0, len1-1)
	}

	partEles := ((len1 + len2 + 1) / 2) // we want major partition as 1st part, hence adding 1
	isodd := ((len1 + len2) % 2) != 0

	if LOG == 1 {
		fmt.Printf("Total Elements: %v, xy(%v, %v)\n", len1+len2, len1, len2)
	}

	xlow := 0
	xhigh := len1
	for xlow <= xhigh {
		xmid := (xlow + xhigh) / 2
		ymid := partEles - xmid

		if LOG == 1 {

			fmt.Printf("Range: (%v, %v), Mid is: (%v, %v)\n", xlow, xhigh, xmid, ymid)
		}

		if xlow > xhigh {
			break
		}

		var ele11, ele12 float64
		var ele21, ele22 float64

		if xmid-1 < 0 {
			ele11 = math.MinInt
		} else {
			ele11 = float64(nums1[xmid-1])
		}
		if xmid >= len1 {
			ele12 = math.MaxInt
		} else {
			ele12 = float64(nums1[xmid])
		}
		if ymid-1 < 0 {
			ele21 = math.MinInt
		} else {
			ele21 = float64(nums2[ymid-1])
		}
		if ymid >= len2 {
			ele22 = math.MaxInt
		} else {
			ele22 = float64(nums2[ymid])
		}

		check1 := (ele11 <= ele22)
		check2 := (ele21 <= ele12)

		if LOG == 1 {
			fmt.Printf("Numbers are: x(%v, %v), y(%v, %v)\n", ele11, ele12, ele21, ele22)
		}
		if check1 && check2 {
			m1 := math.Max(float64(ele11), float64(ele21))
			if isodd {
				return m1
			} else {
				m2 := math.Min(float64(ele12), float64(ele22))
				return (m1 + m2) / 2
			}
		} else if !check2 {
			xlow = xmid + 1
		} else {
			xhigh = xmid - 1
		}

	}
	return -1
}

func findMedianSortedArrays_my(nums1 []int, nums2 []int) float64 {

	if len(nums1) > len(nums2) {
		return findMedianSortedArrays_my(nums2, nums1)
	}

	len1 := len(nums1)
	len2 := len(nums2)

	if len1 == 0 {
		return findMedian(nums2, 0, len2-1)
	}
	if len2 == 0 {
		return findMedian(nums1, 0, len1-1)
	}

	if len1 == len2 && len1 == 1 {
		return (float64(nums1[0]) + float64(nums2[0])) * 0.5
	}

	partEles := ((len1 + len2 + 1) / 2) // we want major partition as 1st part, hence adding 1
	partId := partEles - 2              // compensating for index start from 0
	isodd := ((len1 + len2) % 2) != 0

	xlow := 0
	xhigh := len1 - 1
	//xmid := (len1 - 1) / 2 //  mid2 is partEles - (mid + 1) - 1

	if LOG == 1 {
		fmt.Printf("Total Elements: %v, xy(%v, %v)\n", len1+len2, len1, len2)
	}

	for {
		xmid := (xlow + xhigh) / 2
		ymid := partId - xmid

		if LOG == 1 {
			fmt.Printf("Range: (%v, %v), Mid is: (%v, %v)\n", xlow, xhigh, xmid, ymid)
		}

		if xlow > xhigh {
			break
		}

		var ele11, ele21 float64
		if xmid < 0 {
			ele11 = math.MinInt
		} else {
			ele11 = float64(nums1[xmid])
		}
		if ymid < 0 {
			ele21 = math.MinInt
		} else {
			ele21 = float64(nums2[ymid])
		}

		var ele12, ele22 float64
		if xmid+1 >= len1 {
			ele12 = math.MaxInt
		} else {
			ele12 = float64(nums1[xmid+1])
		}
		if ymid+1 >= len2 {
			ele22 = math.MaxInt
		} else {
			ele22 = float64(nums2[ymid+1])
		}

		check1 := (ele11 <= ele22)
		check2 := (ele21 <= ele12)

		if LOG == 1 {
			fmt.Printf("Numbers are: x(%v, %v), y(%v, %v)\n", ele11, ele12, ele21, ele22)
		}
		if check1 && check2 {
			m1 := math.Max(float64(ele11), float64(ele21))
			if isodd {
				return m1
			} else {
				m2 := math.Min(float64(ele12), float64(ele22))
				return (m1 + m2) / 2
			}
		} else {
			if check1 {
				xlow = xmid + 1
			} else {
				xhigh = xmid - 1
			}
		}
	}
	return -1
}
func Test11(t *testing.T) {

	num1 := []int{1, 3}
	num2 := []int{2}
	expected := 2.0
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test12(t *testing.T) {

	num1 := []int{3}
	num2 := []int{1, 2}
	expected := 2.0
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test21(t *testing.T) {

	num1 := []int{1, 3}
	num2 := []int{2, 4}
	expected := 2.5
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}
func Test22(t *testing.T) {

	num1 := []int{2, 4}
	num2 := []int{1, 3}
	expected := 2.5
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test23(t *testing.T) {

	num1 := []int{1, 2}
	num2 := []int{3, 4}
	expected := 2.5
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}
func Test3(t *testing.T) {

	num1 := []int{1, 3}
	num2 := []int{2, 4, 5}
	expected := 3.0
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test4(t *testing.T) {

	num1 := []int{10, 11, 12, 13, 14, 15, 16, 17}
	num2 := []int{2}
	expected := 13.0
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}
func Test5(t *testing.T) {

	num1 := []int{10, 11, 12, 13, 14, 15, 16, 17}
	num2 := []int{2, 20}
	expected := 13.5
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test6(t *testing.T) {

	num1 := []int{11, 12, 13, 14, 15, 16, 17}
	num2 := []int{}
	expected := 14.0
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test71(t *testing.T) {

	num1 := []int{1, 2, 3}
	num2 := []int{4, 5, 6, 7, 8, 9, 10}
	expected := 5.5
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test81(t *testing.T) {

	num1 := []int{111}
	num2 := []int{110}
	expected := 110.5
	result := findMedianSortedArrays(num1, num2)
	assert.Equal(t, expected, result, "simple test failed")
}
