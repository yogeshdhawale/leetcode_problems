/*
1953. Maximum Number of Weeks for Which You Can Work
Medium

There are n projects numbered from 0 to n - 1. You are given an integer array milestones where each milestones[i] denotes the number of milestones the ith project has.

You can work on the projects following these two rules:

Every week, you will finish exactly one milestone of one project. You must work every week.
You cannot work on two milestones from the same project for two consecutive weeks.
Once all the milestones of all the projects are finished, or if the only milestones that you can work on will cause you to violate the above rules, you will stop working. Note that you may not be able to finish every project's milestones due to these constraints.

Return the maximum number of weeks you would be able to work on the projects without violating the rules mentioned above.

Example 1:

Input: milestones = [1,2,3]
Output: 6
Explanation: One possible scenario is:
​​​​- During the 1st week, you will work on a milestone of project 0.
- During the 2nd week, you will work on a milestone of project 2.
- During the 3rd week, you will work on a milestone of project 1.
- During the 4th week, you will work on a milestone of project 2.
- During the 5th week, you will work on a milestone of project 1.
- During the 6th week, you will work on a milestone of project 2.
The total number of weeks is 6.
Example 2:

Input: milestones = [5,2,1]
Output: 7
Explanation: One possible scenario is:
- During the 1st week, you will work on a milestone of project 0.
- During the 2nd week, you will work on a milestone of project 1.
- During the 3rd week, you will work on a milestone of project 0.
- During the 4th week, you will work on a milestone of project 1.
- During the 5th week, you will work on a milestone of project 0.
- During the 6th week, you will work on a milestone of project 2.
- During the 7th week, you will work on a milestone of project 0.
The total number of weeks is 7.
Note that you cannot work on the last milestone of project 0 on 8th week because it would violate the rules.
Thus, one milestone in project 0 will remain unfinished.

Constraints:

n == milestones.length
1 <= n <= 105
1 <= milestones[i] <= 109
*/
package maxweek

import (
	"fmt"
	"sort"
	"testing"

	"github.com/stretchr/testify/assert"
)

func numberOfWeeks2(milestones []int) int64 {

	// using round robit on whole set
	proj_cnt := len(milestones)

	if proj_cnt == 1 {
		return 1
	}
	sort.Sort(sort.Reverse(sort.IntSlice(milestones)))

	result := int64(0)
	for proj_cnt > 1 {
		finished_proj := 0
		for i := 0; i < proj_cnt; i++ {
			if milestones[i] <= 0 {
				continue
			} else if milestones[i] == 1 {
				finished_proj += 1
			}
			milestones[i] -= 1
		}
		result += int64(proj_cnt)
		proj_cnt -= finished_proj
	}

	if proj_cnt == 1 {
		for _, v := range milestones {
			if v <= 0 {
				continue
			} else {
				return result + 1
			}
		}
	}

	return result
}

func numberOfWeeks3(milestones []int) int64 {

	// round robin on 1st two biggest projects
	proj_cnt := len(milestones)
	if proj_cnt == 1 {
		return 1
	}

	result := int64(0)
	for {
		sort.Sort(sort.Reverse(sort.IntSlice(milestones)))

		if milestones[0] <= 0 {
			return result
		} else if milestones[1] <= 0 {
			return result + 1
		} else {
			val := 1
			if milestones[2] < milestones[1] {
				val = milestones[1] - milestones[2]
			}
			milestones[0] -= val
			milestones[1] -= val
			result += int64(val * 2)
		}
	}
}

func max_heapify(heap *[]int, i int, n int) {
	biggest := i
	lChild := 2*i + 1
	rChild := 2*i + 2

	if lChild < n && (*heap)[lChild] > (*heap)[biggest] {
		biggest = lChild
	}
	if rChild < n && (*heap)[rChild] > (*heap)[biggest] {
		biggest = rChild
	}

	if biggest != i {
		(*heap)[i], (*heap)[biggest] = (*heap)[biggest], (*heap)[i]
		max_heapify(heap, biggest, n)
	}
}

func buildHeap(heap *[]int, n int) {
	startIdx := n/2 - 1

	for i := startIdx; i >= 0; i-- {
		max_heapify(heap, i, n)
	}
}

func printArray(arr []int) {
	for _, v := range arr {
		fmt.Print(v, " ")
	}
	fmt.Println()
}
func numberOfWeeks4(milestones []int) int64 {
	// max heap

	n := len(milestones)
	if n == 1 {
		return 1
	} else if n == 2 {
		smaller := milestones[0]
		if smaller > milestones[1] {
			smaller = milestones[1]
		}
		return int64(smaller*2) + 1
	}

	buildHeap(&milestones, n)

	/*
		fmt.Print("After heapify: ")
		printArray(milestones)
	*/
	result := int64(0)
	for {
		//printArray(milestones)
		//fmt.Println(result)
		root := milestones[0]
		maxid := 1
		minid := 2
		if milestones[2] > milestones[1] {
			maxid = 2
			minid = 1
		}

		if root <= 0 {
			return result
		} else if milestones[maxid] <= 0 {
			return result + 1
		} else {
			val := 1
			if milestones[maxid] > milestones[minid] {
				val = milestones[maxid] - milestones[minid]
			}
			result += int64(val * 2)

			milestones[maxid] -= val
			max_heapify(&milestones, maxid, n)

			milestones[0] -= val
			max_heapify(&milestones, 0, n)

		}
	}
}

func numberOfWeeks(milestones []int) int64 {
	// online solution
	max := -1
	n := len(milestones)
	sum := int64(0)
	for i := 0; i < n; i++ {
		if max < milestones[i] {
			max = milestones[i]
		}
		sum += int64(milestones[i])
	}

	remainingsum := sum - int64(max)
	if remainingsum <= int64(max)-1 {
		return 2*remainingsum + 1
	}
	return sum
}

func Test1(t *testing.T) {

	milestones := []int{1, 2, 3}
	expected := int64(6)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test2(t *testing.T) {

	milestones := []int{1, 1, 1, 1, 5}
	expected := int64(9)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}
func Test3(t *testing.T) {

	milestones := []int{3, 3, 3}
	expected := int64(9)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test4(t *testing.T) {

	milestones := []int{5, 2, 1}
	expected := int64(7)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test21(t *testing.T) {

	milestones := []int{1, 5}
	expected := int64(3)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}
func Test31(t *testing.T) {

	milestones := []int{3, 3, 2}
	expected := int64(8)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test5(t *testing.T) {

	milestones := []int{5, 9, 4, 4, 8, 9, 9, 8, 7, 3}

	expected := int64(66)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}

func Test6(t *testing.T) {

	milestones := []int{240, 519, 80, 511, 552, 20, 953, 348, 331, 337, 892, 353, 712, 711, 138, 83, 879, 860, 616, 794, 950, 35, 734, 82, 758, 352, 827, 211, 955, 65, 883, 205, 448, 716, 776, 227, 497, 324, 112, 806, 44, 419, 80, 866, 206, 723, 165, 401, 510, 485, 717, 70, 551, 873, 433, 214, 665, 501, 312, 24, 476, 469, 776, 708, 412, 449, 185, 467, 921, 39, 737, 263, 376, 870, 380, 459, 543, 371, 611, 412, 230, 994, 3, 824, 215, 265, 802, 379, 147, 876, 472, 547, 200, 163, 215, 23, 175, 599, 414, 516, 173, 652, 29, 143, 113, 624, 173, 454, 503, 190, 62, 862, 23, 413, 459, 326, 418, 34, 721, 732, 53, 245, 413, 41, 407, 506, 28, 931, 989, 905, 225, 248, 97, 399, 486, 196, 169, 341, 638, 119, 355, 935, 127, 609, 54, 827, 746, 298, 282, 807, 965, 477, 809, 956, 985, 308, 501, 256, 575, 273, 31, 835, 346, 645, 354, 325, 96, 59, 699, 446, 993, 92, 607, 633, 4, 108, 32, 698, 93, 20, 175, 516, 38, 494, 56, 194, 671, 522, 922, 340, 206, 922, 998, 776, 489, 349, 823, 491, 940, 360, 963, 675, 36, 392, 313, 770, 622, 647, 970, 404, 90, 971, 301, 189, 871, 423, 118, 520, 400, 561, 343, 550, 697, 798, 951, 413, 179, 858, 758, 886, 752, 396, 856, 961, 567, 368, 946, 189, 372, 821, 425, 449, 235, 456, 799, 720, 493, 354, 276, 841, 174, 520, 738, 771, 621, 938, 728, 714, 397, 973, 499, 44, 158, 176, 728, 770, 24, 843, 479, 95, 777, 709, 694, 677, 100, 328, 243, 616, 864, 821, 658, 58, 900, 627, 894, 50, 461, 478, 919, 890, 751, 663, 342, 379, 2, 74, 200, 409, 806, 979, 938, 108, 347, 583, 319, 113, 462, 83, 842, 665, 216, 247, 705, 50, 157, 958, 661, 147, 600, 12, 115, 236, 295, 991, 214, 226, 872, 576, 528, 430, 518, 800, 929, 619, 327, 808, 905, 549, 801, 193, 479, 182, 638, 82, 43, 846, 826, 116, 613, 25, 880, 990, 104, 205, 966, 915, 262, 88, 112, 477, 409, 229, 846, 850, 697, 631, 112, 658, 498, 780, 685, 893, 717, 355, 413, 618, 677, 457, 797, 114, 299, 856, 968, 609, 455, 835, 389, 211, 120, 553, 8, 476, 307, 746, 659, 80, 350, 355, 197, 497, 772, 74, 947, 566, 703, 753, 374, 434, 803, 664, 69, 621, 532, 220, 256, 359, 859, 64, 344, 242, 442, 450, 87, 257, 224, 331, 170, 768, 954, 212, 744, 224, 896, 897, 753, 627, 475, 954, 729, 549, 874, 429, 377, 971, 196, 925, 639, 973, 420, 496, 916, 793, 417, 481, 602, 943, 36, 300, 981, 957, 19, 190, 914, 99, 29, 740, 155, 302, 516, 532, 63, 655, 484, 42, 200, 764, 213, 283, 504, 443, 39, 354, 480, 760, 202, 765, 801, 295, 147, 106, 455, 307, 931, 421, 455, 80, 211, 157, 500, 782, 830, 718, 292, 385, 826, 898, 650, 382, 966, 166, 442, 320, 963, 475, 486, 539, 556, 228, 369, 980, 874, 296, 293, 31, 475, 383, 909, 368, 827, 851, 305, 766, 249, 953, 827, 533, 11, 15, 135, 645, 544, 153, 549, 22, 170, 973, 52, 2, 209, 789, 813, 477, 364, 989, 805, 372, 566, 36, 502, 190, 245, 14, 284, 443, 927, 347, 856, 53, 618, 560, 831, 97, 461, 320, 269, 254, 689, 822, 511, 429, 741, 267, 348, 46, 31, 440, 39, 643, 692, 170, 935, 780, 828, 578, 254, 977, 139, 762, 777, 862, 809, 423, 522, 769, 851, 346, 566, 137, 287, 18, 173, 47, 18, 498, 659, 62, 919, 836, 928, 472, 479, 534, 682, 773, 951, 881, 834, 618, 528, 425, 380, 531, 648, 175, 301, 188, 76, 701, 143, 853, 561, 105, 418, 793, 361, 455, 466, 286, 823, 262, 464, 290, 983, 543, 160, 592, 323, 144, 535, 39, 422, 498, 872, 237, 848, 470, 954, 600, 628, 280, 32, 325, 589, 613, 563, 119, 500, 129, 287, 805, 727, 772, 934}
	expected := int64(330822)
	result := numberOfWeeks(milestones)
	assert.Equal(t, expected, result, "simple test failed")
}
