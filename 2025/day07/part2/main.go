package main

import (
	"fmt"
	"os"
	"strings"
)

const (
	FILE = "input.txt"
)

func isEmpty(r rune) bool {
	return r == '.'
}

func isSplitter(r rune) bool {
	return r == '^'
}

func foo(matrix [][]rune, r, c, size int, cache map[int]int) int {
	if val, exists := cache[(r << 8) + c]; exists {
		return val
	}
	if r >= size-1 {
		return 1
	}
	if isSplitter(matrix[r][c]) {
		return 0
	}
	if isEmpty(matrix[r+1][c]) {
		cache[(r << 8) + c] = foo(matrix, r+1, c, size, cache)
		return cache[(r << 8) + c]
	}
	count := 0
	if isSplitter(matrix[r+1][c]) {
		if !isSplitter(matrix[r+1][c-1]) {
			count += foo(matrix, r+1, c-1, size, cache)
		}
		if !isSplitter(matrix[r+1][c+1]) {
			count += foo(matrix, r+1, c+1, size, cache)
		}
	}
	cache[(r << 8) + c] = count
	return count
}

func main() {
	text, err := os.ReadFile(FILE)
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	matrix := make([][]rune, 0)
	for line := range strings.SplitSeq(string(text), "\n") {
		if strings.TrimSpace(line) != "" {
			matrix = append(matrix, []rune(line))
		}
	}

	fmt.Println(foo(
		matrix, 0,
		strings.Index(string(matrix[0]), "S"),
		len(matrix),
		make(map[int]int),
	))
}
