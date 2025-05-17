package main
import "fmt"

func changeslice(s[]int)[]int{
	s[0]=100
	s=append(s,11)
	return s
}
func main() {
	x:=[]int{1,2,3,4,5}

	x= append(x,6)
	x= append(x,7)

	a:= x[4:]
	z:=changeslice(a)
	fmt.Println(z)
}
//ans hbe[100 6 7 11]