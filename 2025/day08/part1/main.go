package main

import (
	"fmt"
	"math"
	"os"
	"slices"
	"strconv"
	"strings"
)

const (
	FILE = "input.txt"
)

type Point struct {
        x, y, z int
}

func NewPoint(x, y, z int) *Point {
        return &Point{x, y, z}
}

func (p *Point) Distance(q *Point) int {
        return Square(p.x-q.x) + Square(p.y-q.y) + Square(p.z-q.z)
}

type pair struct {
	p *Point
	q *Point
	dist int
}

func NewPair(p, q *Point) pair {
	return pair{p, q, p.Distance(q)}
}

func (p *pair) String() string {
	return fmt.Sprintf("%v, %v, %d", *p.p, *p.q, p.dist)
}

func print(c []*Point) {
	fmt.Println(Map(c, func (p *Point) Point { return *p }))
}

func AddPair(circuits [][]*Point, p pair) [][]*Point {
	p_c, q_c := -1, -1
	for i, circuit := range circuits {
		for _, x := range circuit {
			if x == p.p {
				p_c = i
			} else if x == p.q {
				q_c = i
			}
		}
	}

	if p_c == -1 && q_c == -1 {
		circuits = append(circuits, []*Point{p.p, p.q})
	} else if p_c != -1 && q_c != -1 && p_c != q_c {
		new_circuit := slices.Concat(circuits[p_c], circuits[q_c])
		circuits[p_c], circuits[q_c] = []*Point{}, []*Point{}
		circuits = slices.DeleteFunc(circuits, func(c []*Point) bool { return len(c) == 0 })
		circuits = append(circuits, new_circuit)
	} else if q_c == -1 {
		circuits[p_c] = append(circuits[p_c], p.q)
	} else if p_c == -1 {
		circuits[q_c] = append(circuits[q_c], p.p)
	}
	return circuits
}

// func printCircuits(circuits [][]*Point) {
// 	slices.SortFunc(circuits, func (a, b []*Point) int { return len(b) - len(a) })
// 	Iter(circuits, print)
// }

func main() {
	points := listToPoints(readLines(FILE))

	pairs := make([]pair, 0, len(points))
	for i, p := range points {
		for _, q := range points[i+1:] {
			pairs = append(pairs, NewPair(p, q))
		}
	}
	slices.SortFunc(pairs, func(a, b pair) int { return a.dist - b.dist })
	pairs = pairs[:1000]

	circuits := Map(points, func(p *Point) []*Point { return []*Point{p} })
	for _, pair := range pairs {
		circuits = AddPair(circuits, pair)
		// fmt.Println(pair.String())
		// printCircuits(circuits)
		// fmt.Println()
	}

	slices.SortFunc(circuits, func (a, b []*Point) int { return len(b) - len(a) })
	fmt.Println(len(circuits[0]) * len(circuits[1]) * len(circuits[2]))
}

// Utilities ------------------------------------------------------------------

func Map[A any, B any](l []A, fn func(A) B) []B {
	mapped := make([]B, len(l))
	for i, e := range l {
		mapped[i] = fn(e)
	}
	return mapped
}

func Iter[A any](l []A, fn func(A)) {
	for _, e := range l {
		fn(e)
	}
}

func Square(x int) int {
	return x * x
}

func Root(x int) int {
	return int(math.Sqrt(float64(x)))
}

func listToPoints(lines []string) []*Point {
	return Map(lines,
		func(s string) *Point {
			nums := Map(strings.Split(s, ","), strToInt)
			return NewPoint(nums[0], nums[1], nums[2])
		},
	)
}

func readLines(filename string) []string {
	if f, err := os.ReadFile(filename); err != nil {
		panic(err)
	} else {
		return strings.Split(string(f), "\n")
	}
}

func strToInt(s string) int {
	if num, err := strconv.Atoi(s); err != nil {
		panic(err)
	} else {
		return num
	}
}
