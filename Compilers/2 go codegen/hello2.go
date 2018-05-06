package main

import "fmt"

func generate(start, finish int, out chan int) {
	for i := start; i <= finish; i++ {
		out <- i
	}
	close(out)
}

func main() {
	fmt.Printf("Hello, World!\n")
	if 5 > 4 {
		a := make(map[string]int, 28)
		a["alpha"] = 10
		a["alpha"] = 20
		fmt.Printf("%d %d \n", a["alpha"], a["beta"])
	} else {
		a := make(map[string]int, 16) //тут будет подставлено 16
		a["alpha"] = 10
		a["alpha"] = 20
		fmt.Printf("%d %d \n", a["alpha"], a["beta"])
	}

	ch := make(chan int) //Тут не будет изменений
	go generate(1, 10, ch)
	for x := range ch {
		fmt.Printf("%d ", x)
	}
}
