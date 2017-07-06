package logicclock

import "encoding/json"


type Clock struct {
	Id        string
	ClocksMap map[string]int
	Log       string
}
/* Inicializa el reloj correspondiente a mi proceso a 0*/
func InitClock(id string) Clock { 
	lc := make(map[string]int)
	lc[id] = 0
	return Clock{id, lc, ""}
}
/* Sincroniza mi reloj vectorial con otro recibido */
func (lc Clock) Merge (e Clock) {
	for k, v := range e.ClocksMap {
		val, ok := lc.ClocksMap[k]
		if ok && v < val {
			continue
		}
		lc.ClocksMap[k] = v
	}
}
/* Genera el json con la marca lógica y el mensaje de log*/
func ToJson(e Clock) (string, error) {
	v, err := json.Marshal(e)
	if err != nil {
		return "", err
	}
	return string(v), nil
}
/* Genera un reloj a partir de un mensaje json en el formato correspondiente*/
func FromJson(data string) (Clock, error) {
	var e Clock
	err := json.Unmarshal([]byte(data), &e)
	return e, err
}
/* Incrementa en uno el valor de mi reloj*/
func (lc Clock) Increment () {
	lc.ClocksMap[lc.Id]++
}

