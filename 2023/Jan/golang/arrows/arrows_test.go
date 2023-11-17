/*
--- one test case failing

452. Minimum Number of Arrows to Burst Balloons
Medium

There are some spherical balloons taped onto a flat wall that represents the XY-plane. The balloons are represented as a 2D integer array points where points[i] = [xstart, xend] denotes a balloon whose horizontal diameter stretches between xstart and xend. You do not know the exact y-coordinates of the balloons.

Arrows can be shot up directly vertically (in the positive y-direction) from different points along the x-axis. A balloon with xstart and xend is burst by an arrow shot at x if xstart <= x <= xend. There is no limit to the number of arrows that can be shot. A shot arrow keeps traveling up infinitely, bursting any balloons in its path.

Given the array points, return the minimum number of arrows that must be shot to burst all balloons.



Example 1:

Input: points = [[10,16],[2,8],[1,6],[7,12]]
Output: 2
Explanation: The balloons can be burst by 2 arrows:
- Shoot an arrow at x = 6, bursting the balloons [2,8] and [1,6].
- Shoot an arrow at x = 11, bursting the balloons [10,16] and [7,12].

    1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
B                     . . . . . . .
B     . . . . . . .
B   . . . . . .
B               . . . . . .

Example 2:

Input: points = [[1,2],[3,4],[5,6],[7,8]]
Output: 4
Explanation: One arrow needs to be shot for each balloon for a total of 4 arrows.
Example 3:

Input: points = [[1,2],[2,3],[3,4],[4,5]]
Output: 2
Explanation: The balloons can be burst by 2 arrows:
- Shoot an arrow at x = 2, bursting the balloons [1,2] and [2,3].
- Shoot an arrow at x = 4, bursting the balloons [3,4] and [4,5].


Constraints:

1 <= points.length <= 10^5
points[i].length == 2
-2^31 <= x-start < x-end <= 2^31 - 1

*/

package arrows

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

type arrowPos struct {
	x int
	y int
}

func NewArrowPos(point []int) arrowPos {
	return arrowPos{x: point[0], y: point[1]}
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func printArray(arrows []arrowPos) {
	fmt.Print("\t\tRange is: ")
	for _, arr := range arrows {
		fmt.Printf("%d-%d\t", arr.x, arr.y)
	}
	fmt.Println()
}

func findMinArrowShots(points [][]int) int {
	var arrows []arrowPos

	for _, pt := range points {

		fmt.Printf("Processing : %d %d\n", pt[0], pt[1])
		rangeMatched := false
		idx := 0
		for i, arr := range arrows {

			if rangeMatched {
				break
			}

			if pt[1] < arr.x {
				//      '   '
				//  ''
				idx = i
				break
			} else if pt[0] < arr.x && pt[1] >= arr.x {
				//      '   '
				//   '...
				rangeMatched = true
				arrows[i].y = min(pt[1], arr.y)
				break
			} else if pt[0] == arr.x && pt[1] == arr.y {
				//  '   '
				//  '   '
				rangeMatched = true
				break
			} else if pt[0] >= arr.x && pt[0] <= arr.y {
				//  '       '
				//      '...
				rangeMatched = true
				arrows[i].x = pt[0]
				arrows[i].y = min(pt[1], arr.y)
			} else if pt[0] > arr.y {
				//  '   '
				//          '...
				idx = i + 1
			}
		}

		if !rangeMatched {

			if idx == 0 {
				arrows = append([]arrowPos{NewArrowPos(pt)}, arrows...)
			} else if idx >= len(arrows) {
				arrows = append(arrows, NewArrowPos(pt))
			} else {
				arrows = append(append(arrows[:idx+1], NewArrowPos(pt)), arrows[idx+1:]...)
			}
		}

		printArray(arrows)
	}
	return len(arrows)
}

func Test1(t *testing.T) {

	fmt.Println("Running test 1")
	input := [][]int{{10, 16}, {2, 8}, {1, 6}, {7, 12}}
	expected := 2
	result := findMinArrowShots(input)
	assert.Equal(t, expected, result, "test failed")
}

func Test2(t *testing.T) {

	fmt.Println("Running test 2")
	input := [][]int{{1, 2}, {2, 3}, {3, 4}, {4, 5}}
	expected := 2
	result := findMinArrowShots(input)
	assert.Equal(t, expected, result, "test failed")
}

func Test3(t *testing.T) {

	fmt.Println("Running test 3")
	input := [][]int{{1, 2}, {3, 4}, {5, 6}, {7, 8}}
	expected := 4
	result := findMinArrowShots(input)
	assert.Equal(t, expected, result, "test failed")
}

func Test41(t *testing.T) {

	fmt.Println("Running test 4-1")
	input := [][]int{{3, 9}, {7, 12}, {3, 8}, {6, 8}, {9, 10}, {2, 9}, {0, 9}, {3, 9}, {0, 6}, {2, 8}}
	expected := 2
	result := findMinArrowShots(input)
	assert.Equal(t, expected, result, "test failed")
}
func Test42(t *testing.T) {

	fmt.Println("Running test 4-2")
	input := [][]int{{3, 9}, {3, 8}, {6, 8}, {9, 10}, {2, 9}, {0, 9}, {3, 9}, {0, 6}, {2, 8}, {7, 12}}
	expected := 2
	result := findMinArrowShots(input)
	assert.Equal(t, expected, result, "test failed")
}
