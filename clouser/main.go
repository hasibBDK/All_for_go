package main
import "fmt"

const a = 10
var p = 100

func outsidr() func() {
	money := 100
	age := 20
	fmt.Println("age: ", age)
	show := func() {
	money = money +a+ p
    fmt.Println(money)
	}
	return show
}

func call() {
	incr := outsidr()
	incr()
	incr()
	incr2 := outsidr()
	incr2()
	incr2()

}
func main() {
	call()
}
func init() {
	fmt.Println("Today know about clouser")
}