package logicclock

import (
	"testing"
)

func TestClock(t *testing.T) {
	var c1, c2 = "distribuidos", "sistemas"
	c:=InitClock(c1)
	cc:=InitClock(c2)

	c.Increment()
	json,_ := ToJson(c)
	
	str:="{\"Id\":\"distribuidos\",\"ClocksMap\":{\"distribuidos\":1},\"Log\":\"\"}"
	if(json != str){
		t.Error("Test toJson")
	}

	js,_ := ToJson(cc)
	str2:="{\"Id\":\"sistemas\",\"ClocksMap\":{\"sistemas\":0},\"Log\":\"\"}"
	if(js != str2){
		t.Error("Test toJson")
	}

	c.Merge(cc)
	nc,_:=ToJson(c)
	str3:="{\"Id\":\"distribuidos\",\"ClocksMap\":{\"distribuidos\":1,\"sistemas\":0},\"Log\":\"\"}"

	if(nc!=str3){
		t.Error("Merge fails")
	}
}
