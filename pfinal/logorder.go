package main

import (
	"./logicclock"
	"bufio"
	"fmt"
	"math"
	"os"
	"sort"
)
/*go run logorder.go chatroom.txt Usuario1.txt Usuario2.txt Usuario3.txt*/
type ClockLists struct {
	c []logicclock.Clock
}

func Check(e error) {
	if e != nil {
		panic(e)
	}
}
func Mod(c logicclock.Clock) float64 {
	var mod1 float64 = 0
	for _, v := range c.ClocksMap {
		mod1 += math.Pow(float64(v), 2)
	}
	return math.Sqrt(mod1)
}

func (c *ClockLists) Less(i, j int) bool {
	var mod1 float64 = 0
	mod2 := mod1
	mod1 = Mod(c.c[i])
	mod2 = Mod(c.c[j])
	return mod1 < mod2
}

func (c *ClockLists) Swap(i, j int) {
	c.c[i], c.c[j] = c.c[j], c.c[i]
}
func (c *ClockLists) Len() int {
	return len(c.c)
}

func main() {
	clocks := new(ClockLists)
	args := os.Args[1:]
	numfiles := len(args)

	if numfiles < 1 {
		fmt.Println("Please use more files")
	} else {
		for _, filename := range args {
			file, err := os.Open(filename)
			Check(err)
			scanner := bufio.NewScanner(file)
			for scanner.Scan() {
				e, err := logicclock.FromJson(scanner.Text())
				Check(err)
				clocks.c = append(clocks.c, e)
			}
		}
		sort.Sort(clocks)
		for _, ck := range clocks.c {
			fmt.Println(ck.Id + " : " + ck.Log)
		}
	}

}
/*package sort

type Interface interface {

	// Len is the number of elements in the collection.

	Len() int

	// Less reports whether the element with

	// index i should sort before the element with index j.

	Less(i, j int) bool

	// Swap swaps the elements with indexes i and j.

	Swap(i, j int)

}
*/