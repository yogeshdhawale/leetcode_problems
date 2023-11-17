/*
947. Most Stones Removed with Same Row or Column
Medium

On a 2D plane, we place n stones at some integer coordinate points. Each coordinate point may have at most one stone.
A stone can be removed if it shares either the same row or the same column as another stone that has not been removed.

Given an array stones of length n where stones[i] = [xi, yi] represents the location of the ith stone, return the largest possible number of stones that can be removed.



Example 1:

Input: stones = [[0,0],[0,1],[1,0],[1,2],[2,1],[2,2]]
Output: 5
Explanation: One way to remove 5 stones is as follows:
1. Remove stone [2,2] because it shares the same row as [2,1].
2. Remove stone [2,1] because it shares the same column as [0,1].
3. Remove stone [1,2] because it shares the same row as [1,0].
4. Remove stone [1,0] because it shares the same column as [0,0].
5. Remove stone [0,1] because it shares the same row as [0,0].
Stone [0,0] cannot be removed since it does not share a row/column with another stone still on the plane.
Example 2:

Input: stones = [[0,0],[0,2],[1,1],[2,0],[2,2]]
Output: 3
Explanation: One way to make 3 moves is as follows:
1. Remove stone [2,2] because it shares the same row as [2,0].
2. Remove stone [2,0] because it shares the same column as [0,0].
3. Remove stone [0,2] because it shares the same row as [0,0].
Stones [0,0] and [1,1] cannot be removed since they do not share a row/column with another stone still on the plane.
Example 3:

Input: stones = [[0,0]]
Output: 0
Explanation: [0,0] is the only stone on the plane, so you cannot remove it.


Constraints:

1 <= stones.length <= 1000
0 <= xi, yi <= 104
No two stones are at the same coordinate point.
*/

// simple looping solution using 2d matrix

// HOWTO::
// go mod init
// <code>
// go get github.com/stretchr/testify/assert
// go test -v <name>_test.go
// go test -v .\mostStonesRemoved_test.go -run ^Test3

package mostStonesRemoved

import (
	"fmt"
	"github.com/stretchr/testify/assert"
	"testing"
)

type Grid struct {
	row  int
	col  int
	grid [][]int
}

func MakeGrid(row int, col int, stones [][]int) Grid {
	grid := make([][]int, row)
	for i := range grid {
		grid[i] = make([]int, col)
	}
	var i int
	for i = 0; i < len(stones); i++ {
		r := stones[i][0]
		c := stones[i][1]
		grid[r][c] = 1
	}
	return Grid{row, col, grid}
}

func printGrid(data Grid) {
	fmt.Printf("row: %d, col: %d\n", data.row, data.col)
	for _, b := range data.grid {
		for _, j := range b {
			print(j, "\t")
		}
		println()
	}
	println()
}

func processCell(data Grid, x int, y int) int {
	found := 0
	for i := 0; i < data.row; i++ {
		if i == x {
			continue
		}
		if data.grid[i][y] == 1 {
			found = 1
		}
	}
	for j := 0; j < data.col; j++ {
		if j == y {
			continue
		}
		if data.grid[x][j] == 1 {
			found = 1
		}
	}
	return found
}

func updateGrid(data Grid) int {
	removed := 0
	for i := 0; i < data.row; i++ {
		for j := 0; j < data.col; j++ {
			if data.grid[i][j] == 0 {
				continue
			}
			if processCell(data, i, j) == 1 {
				data.grid[i][j] = 0
				removed += 1
			}
		}
	}
	return removed
}
func removeStones(stones [][]int) int {

	//var row, col int
	row := 0
	col := 0
	var i int
	for i = 0; i < len(stones); i++ {
		if stones[i][0] > row {
			row = stones[i][0]
		}
		if stones[i][1] > col {
			col = stones[i][1]
		}
	}

	row += 1
	col += 1

	grid := MakeGrid(row, col, stones)
	printGrid(grid)

	return updateGrid(grid)
}

func main() {}

func Test1(t *testing.T) {
	stones := [][]int{{0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 2}}
	expected := 5
	ret := removeStones(stones)
	//fmt.Printf("Test1 - expected:%d, actual:%d\n", expected, ret)
	assert.Equal(t, expected, ret, "Result not matching")
}

func Test2(t *testing.T) {
	stones := [][]int{{0, 0}, {0, 2}, {1, 1}, {2, 0}, {2, 2}}
	expected := 3
	ret := removeStones(stones)
	assert.Equal(t, expected, ret, "Result not matching")
}

func Test3(t *testing.T) {
	stones := [][]int{{0, 1}, {0, 2}, {4, 3}, {2, 4}, {0, 3}, {1, 1}}
	expected := 4
	ret := removeStones(stones)
	assert.Equal(t, expected, ret, "Result not matching")
}

func Test4(t *testing.T) {
	stones := [][]int{{0, 0}}
	expected := 0
	ret := removeStones(stones)
	assert.Equal(t, expected, ret, "Result not matching")
}
