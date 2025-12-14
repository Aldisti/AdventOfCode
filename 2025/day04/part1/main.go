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

func is_accessible(mat []string, row, col int) bool {
	height, width := get_dimensions(mat)
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

	// for _, line := range lines {
	// 	fmt.Println(line)
	// }

	// fmt.Println()
	height, width := get_dimensions(lines)

	count := 0
	for row := 0; row < height; row++ {
		for col := 0; col < width; col++ {
			if lines[row][col] == CHAR && is_accessible(lines, row, col) {
				count++
				// fmt.Print("x")
			} else {
				// fmt.Printf("%c", lines[row][col])
			}
		}
		// fmt.Print("\n")
	}
	fmt.Println(count)
}
