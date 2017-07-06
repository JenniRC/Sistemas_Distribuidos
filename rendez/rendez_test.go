package rendez
import (
	"fmt"
	"sync"
	"testing"
	"time"
	"os"
)

var wg = &sync.WaitGroup{}
var respuesta =""
var mensaje = "Let's go"

func dosome(segundos time.Duration, tag int) {
	time.Sleep(segundos*time.Second)
	Rendezvous(tag, mensaje)
	wg.Done()
}
func dosomething(segundos time.Duration, tag int,t *testing.T) {
	time.Sleep(segundos*time.Second)
	respuesta:=Rendezvous(tag, mensaje)
	/*respuesta="SD"-> Si queremos que falle, usamos esto*/
	wg.Done()
	if respuesta != mensaje {
		fmt.Fprintln(os.Stderr, "No coindice la pareja")
		t.Error()
	}
}

func TestSimple(t *testing.T) {
	wg.Add(1)
	go dosome(1, 1)
	wg.Add(1)
	go dosome(2, 2)
	wg.Add(1)
	go dosomething(3, 1,t)
	wg.Add(1)
	go dosome(2, 3)
	wg.Add(1)
	go dosomething(1, 2,t)
	wg.Add(1)
	go dosomething(4, 3,t)
	wg.Wait()
}