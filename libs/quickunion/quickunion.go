package quickunion

import "fmt"

// Weighted quick-union with path compression
type QuickUnion struct {
	items []int
}

func NewQuickUnion(n int) QuickUnion {
	if n <= 0 {
		panic("Invalid Quick Union size: " + string(n))
	}
	qu := QuickUnion{make([]int, n)}
	for i := range n {
		qu.items[i] = i
	}
	return qu
}

func (qu *QuickUnion) Count() int {
	return len(qu.items)
}

/*
	Returns the root of p, during the search it applies
	path compression to the tree.
*/
func (qu *QuickUnion) Find(p int) int {
	if p < 0 || p >= qu.Count() {
		panic("Invalid Quick Union index: " + string(p))
	}
	if qu.items[p] == p {
		return p
	} else {
		root := qu.Find(qu.items[p])
		qu.items[p] = root // Adds path compression
		return root
	}
}

/*
	Returns the root of p and its distance from the root.

	This is used in the Union implementation, this way we
	make sure to always add the smaller tree under the
	bigger one.
*/
func (qu *QuickUnion) FindWithWeight(p int) (int, int) {
	if p < 0 || p >= qu.Count() {
		panic("Invalid Quick Union index: " + string(p))
	}
	if qu.items[p] == p {
		return p, 0
	} else {
		root, weight := qu.FindWithWeight(qu.items[p])
		qu.items[p] = root // Adds path compression
		return root, weight + 1
	}
}

/*
	Connects p with q, placing the smallest tree under
	the bigger one.
*/
func (qu *QuickUnion) Union(p, q int) {
	if p < 0 || p >= qu.Count() {
		panic("Invalid Quick Union index: " + string(p))
	} else if q < 0 || q >= qu.Count() {
		panic("Invalid Quick Union index: " + string(q))
	}
	p_r, p_w := qu.FindWithWeight(p)
	q_r, q_w := qu.FindWithWeight(q)
	if p_w < q_w {
		qu.items[p_r] = q_r
	} else {
		qu.items[q_r] = p_r
	}
}

func (qu *QuickUnion) Connected(p, q int) bool {
	return p == q || qu.Find(p) == qu.Find(q)
}

func main() {
	qu := NewQuickUnion(10)
	fmt.Println(qu.items)
	qu.Union(2, 5)
	qu.Union(5, 9)
	qu.Union(8, 1)
	qu.Union(1, 5)
	fmt.Println(qu.items)
	fmt.Println(qu.Find(3))
	fmt.Println(qu.Find(2))
	fmt.Println(qu.items)
	fmt.Println(2, 3, qu.Connected(2, 3))
	fmt.Println(2, 5, qu.Connected(2, 5))
}
