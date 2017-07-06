package logiclog

import (
	"os"
	"pfinal/logicclock"
	//"fmt"
)

type LogicLog struct {
	fOut      *os.File
	lastClock logicclock.Clock
}

func Create(file *os.File, id string) *LogicLog {
	return &LogicLog{file, logicclock.InitClock(id)}
}

func Check(e error){
	if e != nil{
		panic(e)
	}
}
/* Genera un evento con mensaje de log que recibe como parámetro.
   Actualiza el reloj vectorial e imprime el mensaje en el fichero de logs
*/
func (ll *LogicLog) Log (slog string) {
	ll.lastClock.Increment()
	ll.lastClock.Log = slog
	s, err := logicclock.ToJson(ll.lastClock)
	Check(err)
	ll.fOut.WriteString(s)
	ll.fOut.WriteString("\n")
}
/* Pasa el estado actual del logiclog a formato json
*/
func (ll *LogicLog) Mark() string {
	slog, err := logicclock.ToJson(ll.lastClock)
	Check(err)
	return slog
}
/* Recibe un json con marca lógica y mensaje de log (correspondientes a un evento remoto)
   y actualiza el reloj vectorial.
   Imprime el mensaje en el fichero de logs
*/
func (ll *LogicLog) Event (jsn string) string {
	nlc, err := logicclock.FromJson(jsn)
	Check(err)
	ll.lastClock.Increment()
	ll.lastClock.Merge(nlc)
	s, _ := logicclock.ToJson(nlc)
	ll.fOut.WriteString(s)
	ll.fOut.WriteString("\n")
	return nlc.Log
}
/*Actualiza el reloj vectorial*/
func (ll *LogicLog) Mrg (nl *LogicLog) {
	nl.lastClock.Increment()
	ll.lastClock.Merge(nl.lastClock)
}
