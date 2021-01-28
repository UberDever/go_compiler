package main

import "fmt"
import "math"

func main() {
	var a, t, j = 10, -2 * 4 + 9, 5
	alpha, beta, gamma := (-400 + 1000) / 32, 8400, -7 / 3
	var b = math.sqrt(25)
	fmt.Println(alpha, beta, gamma)
	fmt.Println("I can print like anything!", 1, 2, 3 + (8 / 2) * 4 - 4, gamma)
	for i := 0; i < a; i++ {
		var alpha = 10
		if c := math.sqrt(3 * 3 + 4 * 4); b == 5 {
			fmt.Println("It is b:", b)
			fmt.Println("And c:", c)
			b = 8
		} else if c = 3; a < 5 {
			fmt.Println("It will not be printed")
		} else if gamma < 0 {
			fmt.Println("It will be printed eventually, gamma:", gamma)
			fmt.Println("And c:", c)
			break
		}
	}
	for { k1 := 0 
	break
		for { k2 := 0 
		break
			for { k3 := 0 
			break
				for { k4 := 0 
				break
					for { k5 := 0 
					break
						}
					}
				}
			}
		}
}