package sem
import "sync"
/*Interface Puedo dejara en el mismo o separado*/
type Sem struct {
	_m *sync.Mutex
	_c *sync.Cond
	ntok int
}
type UpDowner interface {
	NewSem(ntok int) *Sem
	Up()
	Down()
}

