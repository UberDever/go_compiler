package main

import "fmt"
import "math"

func main() {
    fmt.Println("I am a simple program, let me introduce myself:")
    fmt.Println("I can do for:")
    for i := 0; i < 10; i++ {
		if (i < 5) {
			fmt.Println("i is less than 5, continuing...")
			continue
		}
        fmt.Println("i is", i)
    }
    fmt.Println("I can do if:")
    if hyp := 5.0; hyp == math.Sqrt(3 * 3 + 4 * 4) {
        fmt.Println("Hypotenuse is indeed 5")
    } else if hyp != 5 {
        fmt.Println("You just broke Euclidean math, good job!")
    }
	fmt.Println("Hmm, what else can we do? I guess we just must break out")
	i := 0
	for {
		if i == 10 {
			fmt.Println("So, we break out!")
			break
		}
		i++
		if i <= 5 {
			fmt.Println("Almost there...")
		} else {
			fmt.Println("Not now, sorry...")
		}
	}
	fmt.Println("Thats all folks!")
	return
}
