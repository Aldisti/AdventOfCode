package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

const (
	FILE = "input.txt"
)

func getNeutral(sign rune) int {
	switch sign {
	case '+':
		return 0
	case '*':
		return 1
	default:
		panic("Invalid operator: '" + string(sign) + "'")
	}
}

func getOperation(sign rune) func(int, int) int {
	switch sign {
	case '+':
		return func(a, b int) int { return a + b }
	case '*':
		return func(a, b int) int { return a * b }
	default:
		panic("Invalid operator: '" + string(sign) + "'")
	}
}

func main() {
	text, err := os.ReadFile(FILE)
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	lines := strings.Split(string(text), "\n")
	matrix := make([][]rune, len(lines)-1)
	for i := range len(lines) - 1 {
		matrix[i] = []rune(lines[i])
	}

	var row []string
	var nums [][]string
	for c := range len(matrix[0]) {
		var num strings.Builder
		for r := range len(matrix) {
			num.WriteRune(matrix[r][c])
		}
		n := strings.TrimSpace(num.String())
		if n != "" {
			row = append(row, n)
		}
		if n == "" || c == len(matrix[0])-1 {
			nums = append(nums, row)
			row = []string{}
		}
	}

	result := 0
	i := 0
	for _, sign := range lines[len(lines)-1] {
		if sign == ' ' {
			continue
		}
		op := getOperation(sign)
		acc := getNeutral(sign)
		for _, n := range nums[i] {
			num, _ := strconv.Atoi(n)
			acc = op(acc, num)
		}
		result += acc
		i++
	}
	fmt.Println(result)
}
