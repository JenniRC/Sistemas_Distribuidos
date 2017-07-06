package main

import "fmt"
import "time"
import "math/rand"

const MAXBARBERS = 2
const MAXCHAIRS = 5

func barber(id int, shop <-chan int) {
	for {
		select {
		case c := <-shop:
			fmt.Printf("Barbero %d:  empiezo a cortar el pelo al cliente %d\n", id, c)
			fmt.Printf("Cliente %d:  me empiezo a cortar el pelo\n", c)
			time.Sleep(time.Duration(rand.Intn(350)) * time.Millisecond)
			fmt.Printf("Barbero %d: termino de cortar el pelo al cliente %d\n", id, c)
			fmt.Printf("Cliente %d: termino de cortarme el pelo \n", c)
		default:
			fmt.Printf("Barbero-> Me duermo esperando clientes ZzZ\n")
			time.Sleep(time.Duration(rand.Intn(200)) * time.Millisecond)

		}
	}
}

func customer(shop chan<- int) {
	for i := 1; ; i++ {
		time.Sleep(time.Duration(rand.Intn(250)) * time.Millisecond)
		select {
		case shop <- i:
			fmt.Printf("Cliente %d: me siento en la sala de espera\n", i)
		default:
			fmt.Printf("Cliente %d: me voy de la barbería está llena\n", i)
		}
	}
}

func main() {
	shop := make(chan int, MAXCHAIRS)

	for i := 1; i <= MAXBARBERS; i++ {
		go barber(i, shop)
	}

	fmt.Printf("Barberia abierta con %d barberos y %d sillas, listos para comenzar!\n\n", MAXBARBERS, MAXCHAIRS)
	customer(shop)
}
