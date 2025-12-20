package main

import (
	"fmt"
	"os"
	"strings"
)

const (
	FILE = "input.txt"
	CHAR = '@'
)

var ADDINGS = [][]int{
	{-1, -1}, {-1, 0}, {-1, +1},
	{0, -1}, {0, +1},
	{+1, -1}, {+1, 0}, {+1, +1},
}

func get_dimensions(matrix []string) (int, int) {
	height := len(matrix)
	if height == 0 {
		return height, 0
	}
	return height, len(matrix[0])
}

func is_accessible(mat [][]rune, height, width, row, col int) bool {
	count := 0
	for _, pair := range ADDINGS {
		x, y := pair[0], pair[1]
		if row + x < 0 || row + x >= height {
			continue
		} else if col + y < 0 || col + y >= width {
			continue
		} else if mat[row + x][col + y] == CHAR {
			count++
		}
	}
	return count < 4
}

func main() {
	text, err := os.ReadFile(FILE)
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	lines := strings.Split(string(text), "\n")
	matrix := make([][]rune, len(lines))
	for i, line := range lines {
		matrix[i] = []rune(line)
	}

	height, width := get_dimensions(lines)

	var count, total int
	total = 0
	for ;; {
		count = 0
		for row := range height {
			for col := range width {
				if matrix[row][col] == CHAR && is_accessible(matrix, height, width, row, col) {
					matrix[row][col] = 'x'
					count++
				}
			}
		}
		total += count
		if count == 0 {
			break
		}
	}

	fmt.Println(total)
}
