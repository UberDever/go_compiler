package main

import "fmt"
import "math"

func main() {
    fmt.Println("I am a simple program, let me introduce myself:")
    fmt.Println("I can do for:")
    for i := 0; i < 10; i++ {
        fmt.Println("i is ", i)
    }
    fmt.Println("I can do if:")
    if hyp := 5.; hyp == math.Sqrt(3 * 3 + 4 * 4) {
        fmt.Println("Hypotenuse is indeed 5")
    } else {
        fmt.Println("You just broke Euclidean math, good job!")
    }

}
