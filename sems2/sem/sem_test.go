package sem
import (
	"fmt"
	"testing"
	"sync"
	"time"
	"math/rand"
)

var wg *sync.WaitGroup
var wg2 *sync.WaitGroup
var _mysem *Sem = NewSem(0)
var _mysm *Sem = NewSem(1)
var count int =0
var count2 int =0;
const ( 
	COUNT = 100
	)
func dosomething(c int, _mysem *Sem) {
	for i := 0; i < COUNT/2; i++ {
		_mysem.Down()
		fmt.Print(c)
		count++
	}
	wg.Done()
}

func dosomething2(c int, _mysm *Sem) {
	for i := 0; i < COUNT; i++ {
		_mysm.Down()
		fmt.Print(c)
		count2++
	}
	wg2.Done()
}

func Test1(t *testing.T) {

	wg = &sync.WaitGroup{}
	wg.Add(2)
	go dosomething(0,_mysem)
	go dosomething(1,_mysem)
	time.Sleep(time.Duration(rand.Intn(50))*time.Millisecond)
	for i:= 0; i < COUNT; i++ {
		_mysem.Up();
	}
	wg.Wait()
	if count != COUNT{
		t.Error()
	}
}

func Test2(t *testing.T) {
	wg2 = &sync.WaitGroup{}
	wg2.Add(1)
	go dosomething2(2,_mysm)
	time.Sleep(time.Duration(rand.Intn(50))*time.Millisecond)
	for i:= 0; i < COUNT; i++ {
		_mysm.Up();
	}
	wg2.Wait()
	if count2 != COUNT{
		t.Error()
	}
}