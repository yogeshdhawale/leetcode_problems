package main

import (
	"fmt"
	"runtime"
	"testing"

	"github.com/stretchr/testify/assert"
)

func isMatch(s string, p string) bool {
	rows := len(s)
	columns := len(p)
	var i, j int

	if rows == 0 && columns == 0 {
		return true
	}
	if columns == 0 {
		return false
	}

	dp := make([][]bool, rows+1)
	for i := range dp {
		dp[i] = make([]bool, columns+1)
		for j := range dp[i] {
			dp[i][j] = false
		}
	}

	// Empty string and empty pattern are a match
	dp[0][0] = true

	// Deals with patterns with *
	for i = 1; i < columns; i++ {
		if p[i] == '*' {
			dp[0][i+1] = dp[0][i-1]
		}
	}
	// For remaining characters
	for i = 0; i < rows; i++ {
		for j = 0; j < columns; j++ {
			if s[i] == p[j] || p[j] == '.' {
				dp[i+1][j+1] = dp[i][j]
			} else if p[j] == '*' {
				dp[i+1][j+1] = dp[i+1][j-1]
				if p[j-1] == '.' || p[j-1] == s[i] {
					dp[i+1][j+1] = dp[i+1][j+1] || dp[i][j+1]
				}
			}
		}
	}
	return dp[rows][columns]
}

func currentFunction() string {
	counter, _, _, success := runtime.Caller(1)
	if !success {
		return "<CANT EXTRACT FUNC NAME>"
	}
	return runtime.FuncForPC(counter).Name()
}

func Test1(t *testing.T) {
	fmt.Println("Testing: " + currentFunction())
	assert.Equal(t, true, isMatch("abc", ".*"))
}
func Test2(t *testing.T) {
	fmt.Println("Testing: " + currentFunction())
	assert.Equal(t, true, isMatch("abc", "abc"))
}
func Test3(t *testing.T) {
	fmt.Println("Testing: " + currentFunction())
	assert.Equal(t, true, isMatch("abc", ".*c"))
}
func Test4(t *testing.T) {
	fmt.Println("Testing: " + currentFunction())
	assert.Equal(t, false, isMatch("abc", "a.b"))
}
