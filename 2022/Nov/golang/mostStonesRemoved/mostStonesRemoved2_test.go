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

// optimized approach - using cells as nodes and connections for neighors
// find all islands, for each island result += len(island) - 1

// HOWTO::
// go mod init
// <code>
// go get github.com/stretchr/testify/assert
// go test -v <name>_test.go
// go test -v .\mostStonesRemoved_test.go -run ^Test3

package mostStonesRemoved2

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

type vertex struct {
	i int
	j int
}

type edges struct {
	id    int
	elist []vertex
}

func MakeEdges(i int, max int) edges {
	return edges{i, make([]vertex, 10)}
}

func replace_id(id int, oldids *map[int]int, data *map[vertex]edges) {

	for v, e := range *data {
		if _, ok := (*oldids)[e.id]; ok {
			e.id = id
			(*data)[v] = e
		}
	}
}
func udpateIds(row int, col int, stones [][]int, data *map[vertex]edges) {

	for i := 0; i < len(stones); i++ {
		// horizontal neighbors
		cv := vertex{stones[i][0], stones[i][1]}
		cid := (*data)[cv].id

		replaceids := make(map[int]int)

		for j := 0; j < col; j++ {
			v := vertex{cv.i, j}
			if e, ok := (*data)[v]; ok {
				if e.id != cid {
					replaceids[e.id] = 1
				}
			}
		}
		for i := 0; i < row; i++ {
			v := vertex{i, cv.j}
			if e, ok := (*data)[v]; ok {
				if e.id != cid {
					replaceids[e.id] = 1
				}
			}
		}
		replace_id(cid, &replaceids, data)
	}
}

func removeStones(stones [][]int) int {

	//var row, col int
	row := 0
	col := 0
	data := make(map[vertex]edges)

	var i int
	for i = 0; i < len(stones); i++ {
		if stones[i][0] > row {
			row = stones[i][0]
		}
		if stones[i][1] > col {
			col = stones[i][1]
		}
		v := vertex{stones[i][0], stones[i][1]}
		data[v] = MakeEdges(i, len(stones))
	}

	row += 1
	col += 1

	udpateIds(row, col, stones, &data)
	if false {
		for k, v := range data {
			fmt.Printf("(%d,%d): %d\n", k.i, k.j, v.id)
		}
	}

	result_m := make(map[int]int)

	for _, v := range data {
		if _, ok := result_m[v.id]; ok {
			result_m[v.id] = result_m[v.id] + 1
		} else {
			result_m[v.id] = 1
		}
	}

	result := 0
	for _, v := range result_m {
		if v > 1 {
			result += (v - 1)
		}
	}
	return result
}

func dummy() {
	fmt.Printf("Dummy\n")
}

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
func Test5(t *testing.T) {
	stones := [][]int{{0, 1}, {1, 2}, {1, 3}, {3, 3}, {2, 3}, {0, 2}}
	expected := 5
	ret := removeStones(stones)
	assert.Equal(t, expected, ret, "Result not matching")
}
