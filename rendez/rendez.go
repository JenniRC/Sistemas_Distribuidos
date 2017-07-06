package rendez

import (
	//"fmt"
	"sync"
)
type meeting struct {
	i interface{}
	waitg sync.WaitGroup
}
var Rendezmap = make(map[int]*meeting)
var rmap = &sync.Mutex{}

func Rendezvous(tag int, val interface{}) interface{} {
	rmap.Lock()
	a, ok := Rendezmap[tag]
	if ok{
		Rendezmap[tag].waitg.Done()
		Rendezmap[tag].i=val
		delete(Rendezmap,tag)
		rmap.Unlock()
	}else{
		Rendezmap[tag]= new(meeting)
		Rendezmap[tag].waitg.Add(1)
		Rendezmap[tag].i=val
		a = Rendezmap[tag]
		rmap.Unlock()
		Rendezmap[tag].waitg.Wait()
	}
	return a.i
}
