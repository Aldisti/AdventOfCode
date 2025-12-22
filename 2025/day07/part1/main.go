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

func extendBeam(matrix [][]rune, r, c int) int {
	next := matrix[r + 1][c]
	if isEmpty(next) {
		matrix[r + 1][c] = '|'
		return 0
	}
	if !isSplitter(next) {
		return 0
	}
	count := 0
	if c > 0 && isEmpty(matrix[r + 1][c - 1]) {
		matrix[r + 1][c - 1] = '|'
		count = 1
	}
	if c < len(matrix[r]) && isEmpty(matrix[r + 1][c + 1]) {
		matrix[r + 1][c + 1] = '|'
		count = 1
	}
	return count
}

func main() {
	text, err := os.ReadFile(FILE)
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	matrix := make([][]rune, 0)
	for _, line := range strings.Split(string(text), "\n") {
		if strings.TrimSpace(line) != "" {
			matrix = append(matrix, []rune(line))
		}
	}

	res := 0
	length := len(matrix)
	for r, row := range matrix {
		if r >= length - 1 {
			break
		}
		for c, col := range row {
			if col == '|' || col == 'S' {
				res += extendBeam(matrix, r, c)
			}
		}
	}

	fmt.Println(res)
}
