package balltree

import (
	"fmt"
	"math"
	"slices"
)

const (
	BALL_SIZE = 5
)

// Point structure ------------------------------------------------------------

type Point struct {
	x, y, z int
}

func NewPoint(x, y, z int) *Point {
	return &Point{x, y, z}
}

func (p *Point) Distance(q *Point) int {
	return Root(Square(p.x-q.x) + Square(p.y-q.y) + Square(p.z-q.z))
}

func Average(points []*Point) *Point {
	size := len(points)
	if size == 0 {
		return NewPoint(0, 0, 0)
	}
	x, y, z := 0, 0, 0
	for _, p := range points {
		x += p.x
		y += p.y
		z += p.z
	}
	return NewPoint(x/size, y/size, z/size)
}

func Furthest(points []*Point, c *Point) *Point {
	if len(points) == 0 {
		panic("Cannot calculate min and max from an empty list")
	}
	max_i, max_d := 0, points[0].Distance(c)
	for i, p := range points {
		if d := p.Distance(c); d > max_d {
			max_i = i
			max_d = d
		}
	}
	return points[max_i]
}

// Ball structure -------------------------------------------------------------

type Ball struct {
	Points []*Point
	center Point
	radius int
	Left   *Ball
	Right  *Ball
}

func EmptyBall(center Point, radius int) *Ball {
	return &Ball{
		Points: []*Point{},
		center: center,
		radius: radius,
		Left:   nil,
		Right:  nil,
	}
}

func (b *Ball) AddPoint(point *Point) {
	b.Points = append(b.Points, point)
}

func (b *Ball) AddPoints(points ...*Point) {
	for _, p := range points {
		b.Points = append(b.Points, p)
	}
}

func (b *Ball) Nearest(p *Point) *Ball {
	if b.Left != nil && p.Distance(&b.Left.center) <= b.Left.radius {
		return b.Left.Nearest(p)
	} else if b.Right != nil && p.Distance(&b.Right.center) <= b.Right.radius {
		return b.Right.Nearest(p)
	} else if len(b.Points) > 0 {
		return b
	} else {
		return nil
	}
}

func (b *Ball) Shrink() {
	if b.Left == nil && b.Right == nil {
		return
	}
	if len(b.Left.Points) > 1 && len(b.Right.Points) > 1 {
		return
	}
	b.Points = slices.Concat(b.Points, b.Left.Points, b.Right.Points)
	b.Left = nil
	b.Right = nil
}

func (b *Ball) Print(indent string) {
	fmt.Printf("%scenter: %v\n", indent, b.center)
	fmt.Printf("%sradius: %v\n", indent, b.radius)
	fmt.Printf("%spoints: %v\n", indent, Map(b.Points, func(p *Point) Point { return *p }))
	if b.Left != nil {
		fmt.Printf("%sleft:\n", indent)
		b.Left.Print(indent + "  ")
	}
	if b.Right != nil {
		fmt.Printf("%sright:\n", indent)
		b.Right.Print(indent + "  ")
	}
}

// Tree -----------------------------------------------------------------------

func BuildTree(points []*Point) *Ball {
	avg := Average(points)
	max_r := Furthest(points, avg)
	size := len(points)

	ball := EmptyBall(*avg, avg.Distance(max_r))

	if size <= BALL_SIZE {
		ball.AddPoints(points...)
		return ball
	}

	max_l := Furthest(points, max_r)

	leftPoints := make([]*Point, 0, size/2+1)
	rightPoints := make([]*Point, 0, size/2+1)
	for _, p := range points {
		l_d, r_d := p.Distance(max_l), p.Distance(max_r)
		if l_d < r_d {
			leftPoints = append(leftPoints, p)
		} else {
			rightPoints = append(rightPoints, p)
		}
	}

	if len(leftPoints) <= 1 || len(rightPoints) <= 1 {
		ball.AddPoints(points...)
		return ball
	}

	ball.Left = BuildTree(leftPoints)
	ball.Right = BuildTree(rightPoints)
	return ball
}

// Utilities ------------------------------------------------------------------

func Map[A any, B any](l []A, fn func(A) B) []B {
	mapped := make([]B, len(l))
	for i, e := range l {
		mapped[i] = fn(e)
	}
	return mapped
}

func Square(x int) int {
	return x * x
}

func Root(x int) int {
	return int(math.Sqrt(float64(x)))
}
