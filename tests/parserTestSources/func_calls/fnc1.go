
package main

func main() {

	a:=0
	b:=0
	a,b = addAndSub(5,3)
	nig()
	a = add(4,5)	
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

