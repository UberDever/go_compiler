# GoCompiler
![Logo](https://github.com/UberDever/go_compiler/blob/master/Logo.png)

![License](https://img.shields.io/github/license/UberDever/go_compiler)
# Description
You look at simple golang compiler, written in C++
# Use
Just launch executable with path to .go program, i.e:
```sh
go_compiler.exe ../hello.go
```
There is one limitation: program.go file must be in root project directory, since at the time, I didn't use std::filesystem.
Thus, /logs folder cannot be created and hence it already created in root directory for you.
# Features
## Logging
Folder /logs contains 3 log files, which created during compilation:
* Lexer file - contains all lexical tokens of compiled program
* AST file - contains AST for compiled program, with lot of additional info about each token
* Runtime file - contains list of imported packages and all declared variables

For example, following golang program
```go
package main

import "fmt"
func main() {
	var a float32 = 4.2 // Simple comment
	fmt.Println("Hello world! I am floating!", a)
}
```
Produces following log files

``` hello_lexer.txt ```
```js
package (Keyword)
main (Identifier)
; (Operator)
import (Keyword)
fmt (Literal)
; (Operator)
func (Keyword)
main (Identifier)
( (Operator)
) (Operator)
{ (Operator)
var (Keyword)
a (Identifier)
float32 (Identifier)
= (Operator)
4.2 (Number)
; (Operator)
fmt (Identifier)
. (Operator)
Println (Identifier)
( (Operator)
Hello world! I am floating! (Literal)
, (Operator)
a (Identifier)
) (Operator)
; (Operator)
} (Operator)
; (Operator)
```

``` hello_ast.txt ```
```js
'src' (Keyword) L R
!--->'head' (Keyword) L R
    !--->'main' (Keyword)
    !--->'import' (Keyword) L
        !--->'fmt' (Literal)
!--->'tld' (Keyword) L
    !--->'func' (Keyword) L R
        !--->'main' (Keyword) L
            !--->'par' (Keyword)
        !--->'{}' (Keyword) L
            !--->'stmt_list' (Keyword) L R
                !--->'var' (Keyword) L R
                    !--->'var_desc' (Keyword) L R
                        !--->'id' (Keyword) L
                            !--->decl: 'a' (Identifier) [1]
                        !--->'float32' (Keyword)
                    !--->'expr_list' (Keyword) L
                        !--->'4.2' (Number)
                !--->'stmt_list' (Keyword) L
                    !--->'()' (Keyword) L R
                        !--->'fmt' (Keyword) L
                            !--->'Println' (Keyword)
                        !--->'expr_list' (Keyword) L R
                            !--->'Hello world! I am floating!' (Literal)
                            !--->'expr_list' (Keyword) L
                                !--->'a' (Identifier)
```

``` hello_runtime.txt ```
```js
Imported pkg fmt

Var a with type float32 on level 1
```

## Syntactic possibilities
GoCompiler can parse basic syntactic set of golang

Thus, compiler supports:
* Operators
```go
+    &     &&    ==    (    )    _
-    |     ||    <     [    ]    &^
*    ^     <-    >     {    }    :
/    <<    ++    =     :=   ,    ;
%    >>    --    !     ...  .	 !=
>    >=    <	 <=	 
```
Although, not all implemented semantically
* Keywords
```go
break        default      func         interface    select
case         defer        go           map          struct
chan         else         goto         package      switch
const        fallthrough  if           range        type
continue     for          import       return       var
```
Also, not everything implemented semantically
* Basic predeclared identifiers
```go
bool float32 int
true false
```
* Different kinds of ```for```s - with and without body
* Differend kinds of ```if```s - with and without ```SimpleStmt```, also unlimited number of ```else if```s supported
* Control flow: ```break```, ```continue```, ```return```
* Package ```fmt``` with ```Println()``` in it
* Package ```math``` with ```Sqrt()``` in it

## Code generation
GoCompiler generates code in MASM32 assembly

# Simple example
Following:
```go
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
```
Will generate assembly program, that outputs:
```
I am a simple program, let me introduce myself:
I can do for:
i is less than 5, continuing...
i is less than 5, continuing...
i is less than 5, continuing...
i is less than 5, continuing...
i is less than 5, continuing...
i is 5
i is 6
i is 7
i is 8
i is 9
I can do if:
Hypotenuse is indeed 5
Hmm, what else can we do? I guess we just must break out
Almost there...
Almost there...
Almost there...
Almost there...
Almost there...
Not now, sorry...
Not now, sorry...
Not now, sorry...
Not now, sorry...
Not now, sorry...
So, we break out!
Thats all folks!
```
