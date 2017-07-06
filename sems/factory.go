package main

import (
	"./sem"
	"fmt"
	"math/rand"
	"time"
)

const mMobiles = 30
const nRobots = 3

var round int = 0
var s = sem.NewSem(0)

type mobile struct {
	sc   int
	wr   [5]int
	bx   int
	bd   int
	done int /*1 todo lo que necesitamos ,0 falta algo*/
}
type piece struct {
	psem   *sem.Sem
	ptotal *sem.Sem
	id     []int
	name   string
	max    int
	cid    int
}

var mscreen piece
var mbox piece
var mboard piece
var mwire piece

func (m *piece) start(max int, name string) *piece {
	m.ptotal = sem.NewSem(max)
	m.psem = sem.NewSem(0)
	m.name = name
	m.id = make([]int, max)
	m.max = max
	m.cid = 0
	return m
}
func (m *piece) producer() {
	for i := 0; i < m.max; i++ {
		m.ptotal.Down()
		m.id[i] = i
		m.psem.Up()
	}
}
func (m *piece) consumer() int {
	m.psem.Down()
	idnt := m.id[m.cid]
	m.cid = m.cid + 1
	m.ptotal.Up()
	return idnt
}
func (m *piece) consumerwire() [5]int {
	sum := 0
	var to [5]int
	for sum < 5 {
		m.psem.Down()
		to[sum] = m.id[round*5+sum]
		m.ptotal.Up()
		sum++
	}
	return to
}
func ensemble(id int) { /*me creo el array de cables pantallas y etc. asique seran 5 cables +3 osea, 8*/
	var newmobile mobile
	newmobile.done = 0
	for round < mMobiles {
		for newmobile.done != 1 {
			newmobile.wr = mwire.consumerwire()
			newmobile.sc = mscreen.consumer()
			newmobile.bx = mbox.consumer()
			newmobile.bd = mboard.consumer()
			newmobile.done = 1
			round++
		}

		makeMobile(id, newmobile)
		newmobile.done = 0

		if round == 30 {
			s.Up()
			break
		}
	}
}

func makeMobile(id int, mymobile mobile) {

	printMobile(id, mymobile)
	fmt.Println(" Comenzando")
	time.Sleep(time.Duration(rand.Intn(250)) * time.Millisecond)
	printMobile(id, mymobile)
	fmt.Println(" Terminando")
}

func printMobile(id int, newmobile mobile) {
	fmt.Print("Robot ", id)
	fmt.Print(" ", mwire.name, newmobile.wr)
	fmt.Print(" ", mscreen.name, newmobile.sc)
	fmt.Print(" ", mbox.name, newmobile.bx)
	fmt.Print(" ", mboard.name, newmobile.bd)
}

func main() {

	mscreen.start(mMobiles, "pantalla")
	mbox.start(mMobiles, "carcasa")
	mboard.start(mMobiles, "placa")
	mwire.start(5*mMobiles, "cables")
	go mscreen.producer()
	go mwire.producer()
	go mboard.producer()
	go mbox.producer()
	go ensemble(0)
	go ensemble(1)
	go ensemble(2)

	//s := sem.NewSem(0)
	s.Down()
}
