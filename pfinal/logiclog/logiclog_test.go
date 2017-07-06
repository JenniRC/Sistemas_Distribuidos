package logiclog

import (
	"log"
	"os"
	"testing"
)


func TestLogs(t *testing.T) {
	f, err := os.Create("DY" + ".txt")
	if err != nil {
		log.Fatal("No se ha podido crear el fichero")
		panic(err)
	}
	ll := Create(f,"DY")

	ll.Log("log 1")
	ll.Log("log 2")
	ll.Log("log 3")
	s:= ll.Mark()
	z:="{\"Id\":\"DY\",\"ClocksMap\":{\"DY\":3},\"Log\":\"log 3\"}"
	if(s!=z){
		t.Error("Mark fails")
	}
	k:="{\"Id\":\"DY\",\"ClocksMap\":{\"DY\":3},\"Log\":\"log 4\"}"

	ll.Msg(k)
	x:= ll.Mark()

	if(x==k){
		t.Error("Msg fails")
	}
}

