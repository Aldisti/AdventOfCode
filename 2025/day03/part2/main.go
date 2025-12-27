package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

const (
	FILE = "input.txt"
	SIZE = 12
)

// https://www.geeksforgeeks.org/dsa/introduction-to-monotonic-stack-2/
func main() {
	nums, err := parseInput(FILE)
	if err != nil {
		fmt.Println(err)
		return
	}

	sum := 0
	for _, num := range nums {
		s := Stack{}
		digits := len(num)
		for i, n := range num {
			top, ok := s.Peek()
			if !ok { // if the stack is empty add anything
				s.Push(n)
				continue
			}
			for ok && (digits - i) + s.Size() > SIZE && n > top {
				s.Remove()
				top, ok = s.Peek()
			}
			if s.Size() < SIZE {
				s.Push(n)
			}
		}
		sum += toInt(s.ToArray())
	}
	fmt.Println(sum)
}

// Stack structure ------------------------------------------------------------

type Stack struct {
	items []int
}

func (s *Stack) IsEmpty() bool {
	return len(s.items) == 0
}

func (s *Stack) Size() int {
	return len(s.items)
}

func (s *Stack) Push(n int) {
	s.items = append(s.items, n)
}

func (s *Stack) ToArray() []int {
	return s.items
}

func (s *Stack) Peek() (int, bool) {
	if s.IsEmpty() {
		return 0, false
	}
	return s.items[len(s.items)-1], true
}

func (s *Stack) Pop() (int, bool) {
	if s.IsEmpty() {
		return 0, false
	}
	n := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	return n, true
}

func (s *Stack) Remove() {
	if s.IsEmpty() {
		return
	}
	s.items = s.items[:len(s.items)-1]
}

// Utilities ------------------------------------------------------------------

func toInt(nums []int) int {
	var b strings.Builder
	for _, num := range nums {
		b.WriteRune(rune(num) + '0')
	}
	n, _ := strconv.Atoi(b.String())
	return n
}

func parseInput(filename string) ([][]int, error) {
	text, err := os.ReadFile(filename)
	if err != nil {
		return nil, err
	}

	return Map(
		strings.Split(string(text), "\n"),
		func(s string) []int {
			nums := make([]int, len(s))
			for i, c := range s {
				nums[i] = int(c - '0')
			}
			return nums
		},
	), nil
	
}

func Map[A any, B any](l []A, fn func(A) B) []B {
	mapped := make([]B, len(l))
	for i, e := range l {
		mapped[i] = fn(e)
	}
	return mapped
}
