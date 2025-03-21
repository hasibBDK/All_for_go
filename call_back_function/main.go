package main
import "fmt"

type User struct{
	Name string
	Age int
}

func printUserDetails(usr User){
	fmt.Println("User Name: ",usr.Name)
	fmt.Println("User Age: ",usr.Age)
}

func main(){
	var usr1 User
	usr1 = User{
		Name : "Rahul",
		Age : 20,
	}
	printUserDetails(usr1)
	usr2 := User{
		Name : "Rohit",
		Age : 21,
	}
	printUserDetails(usr2)
}