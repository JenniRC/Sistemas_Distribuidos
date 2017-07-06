package sem
import "sync"

/*Semaphores Function*/
func NewSem(ntok int) *Sem {
	var semmut sync.Mutex
	c := sync.NewCond(&semmut)
	return &Sem{&semmut, c, ntok}
}

func (s *Sem) Up() {
	s._m.Lock()
	s.ntok++
	s._c.Signal()
	s._m.Unlock()
}

func (s *Sem) Down() {
	s._m.Lock()
	if s.ntok == 0{
		s._c.Wait()
	}
	s.ntok--
	s._m.Unlock()
}
