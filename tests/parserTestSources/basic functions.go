
package main

func main() {

	s1 := "Toto je nejaky text"
	nig()

	s2 := s1 + ", ktery jeste trochu obohatime"
	s3:=0
	s3=add(5,6)
}

func nig()(){
	a := 5
}

func add(x int, y int) (int) {
	return x + y
}


func addAndSub(x int, y int) (int,int) {
	return x + y, x - y

}

