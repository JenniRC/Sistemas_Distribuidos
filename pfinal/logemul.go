package main

import (
	"./logiclog"
	"log"
	"os"
	"strconv"
	"sync"
	"time"
	//"fmt"
	"math/rand"
)

const MAXUSERS = 5

var llog = initLog("chatroom")

type User struct {
	id        int
	log       string
	userschan chan string
}

func initLog(name string) *logiclog.LogicLog {
	os.Mkdir("logs", 0777)
	f, err := os.Create(name + ".txt")
	if err != nil {
		log.Fatal("No se ha podido crear el fichero")
		panic(err)
	}
	return logiclog.Create(f, name)
}
/* Generación de mensajes de chat. Eventos remotos
*/
func chat(room chan User) {
	for {
		user := <-room

		lloguser := initLog("Usuario" + strconv.Itoa(user.id))
		lloguser.Log("Usuario" + strconv.Itoa(user.id) + " me uno al chat ")
		jsonMsg := lloguser.Mark()
		/* Envío de evento al chat*/
		user.userschan <- jsonMsg
		lloguser.Mrg(llog)
		time.Sleep(time.Duration(rand.Intn(350)) * time.Millisecond)
		lloguser.Log("Usuario" + strconv.Itoa(user.id) + ": Hola! Este es mi primer mensaje ")
		lloguser.Mrg(llog)
		lloguser.Log("Usuario" + strconv.Itoa(user.id) + ": Bueno me voy ")
		lloguser.Mrg(llog)
		/* Enviamos el último mensaje*/
		user.userschan <- lloguser.Mark()
	}
}
/* Creación de usuarios */
func users(room chan User) {
	llog := initLog("chatroom")
	c := make(chan string)

	for i := 1; ; i++ {
		time.Sleep(time.Duration(rand.Intn(1000)) * time.Millisecond)
		mu := User{i, "", c}
		select {
		case room <- mu:
			/* Generamos un evento local (no lo enviamos) por cada usuario creado */
			llog.Log("Usuario" + strconv.Itoa(i) + " se une al chat")
		default:
			llog.Log("No hay nuevos usuarios")
			time.Sleep(time.Duration(rand.Intn(1000)) * time.Millisecond)
		}

		select {
		case me := <-c:
			/* Recibimos eventos de los usuarios. Sincronizamos mi reloj */
			msg := llog.Event(me)
			llog.Log(msg)
			time.Sleep(time.Duration(rand.Intn(100)) * time.Millisecond)
		}
	}
}

func main() {
	room := make(chan User)
	wg := &sync.WaitGroup{}
	for i := 1; i <= MAXUSERS; i++ {
		go chat(room)

	}
	users(room)
	wg.Add(1)

	wg.Wait()
}
