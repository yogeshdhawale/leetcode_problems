/*

LeetCode 269 — Alien Dictionary

Problem
There is a new alien language that uses the English alphabet. However, the order among the letters is unknown to you.

You are given a list of strings words from the alien language’s dictionary, where the strings in words are sorted lexicographically by the rules of this new language.

Return a string of the unique letters in the new alien language, sorted in lexicographically increasing order by the new language’s rules. If there is no solution, return “”. If there are multiple solutions, return any of them.

A string s is lexicographically smaller than a string t if at the first letter where they differ, the letter in s comes before the letter in t in the alien language. If the first min(s.length, t.length) letters are the same, then s is smaller if and only if s.length < t.length.

Constraints:
1 ≤ words.length ≤ 100
1 ≤ words[i].length ≤ 100
words[i] consists of only lowercase English letters.
Algorithm

Example 1:

Input:
[
  "wrt",
  "wrf",
  "er",
  "ett",
  "rftt"
]

Output: "wertf"
Example 2:

Input:
[
  "z",
  "x"
]

Output: "zx"
Example 3:

Input:
[
  "z",
  "x",
  "z"
]

Output: ""

Explanation: The order is invalid, so return "".
Note:

You may assume all letters are in lowercase.
You may assume that if a is a prefix of b, then a must appear before b in the given dictionary.
If the order is invalid, return an empty string.
There may be multiple valid order of letters, return any one of them is fine.


*/

package main

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func findDiffLetter(prevWord string, currentWord string) (byte, byte, bool) {

	len1 := len(prevWord)
	len2 := len(currentWord)

	for i := 0; i < len1 || i < len2; i++ {
		if prevWord[i] != currentWord[i] {
			return prevWord[i], currentWord[i], true
		}
	}
	return 0, 0, false
}

func alienOrder(words []string) string {

	orderInfo := make(map[byte]byte)

	slen := len(words)

	currentWord := words[0]
	firstchar := currentWord[0]

	/*
		charInfo := make(map[byte]bool)
		for i := range currentWord {
			charInfo[currentWord[i]] = true
		}
	*/

	for i := 1; i < slen; i++ {
		prevWord := words[i-1]
		currentWord := words[i]

		/*
			for i := range currentWord {
				charInfo[currentWord[i]] = true
			}
		*/

		first, second, ok := findDiffLetter(prevWord, currentWord)

		if ok {
			//fmt.Printf("Adding mapping - %c:%c\n", first, second)
			orderInfo[first] = second
		}
	}

	resultInfo := make(map[byte]bool)
	result := string("")
	result += string(firstchar)
	resultInfo[firstchar] = true

	nextch := firstchar

	for {
		//fmt.Printf("Adding char:%c\n", nextch)
		val, ok := orderInfo[nextch]
		if !ok {
			break
		}

		if _, ok = resultInfo[val]; ok {
			return ""
		}

		result += string(val)
		nextch = val
	}

	/*if len(charInfo) != len(result) {
		return ""
	}*/

	return result
}

func Test1(t *testing.T) {
	words := []string{"wrt", "wrf", "ea", "er", "ett", "rftt"}
	result := alienOrder(words)
	fmt.Printf("Result is: %v", result)
	assert.Equal(t, "wertf", result)
}

func Test2(t *testing.T) {
	words := []string{"z", "x"}
	result := alienOrder(words)
	fmt.Printf("Result is: %v", result)
	assert.Equal(t, "zx", result)
}

func Test3(t *testing.T) {
	words := []string{"z", "x", "z"}
	result := alienOrder(words)
	fmt.Printf("Result is: %v", result)
	assert.Equal(t, "", result)
}
