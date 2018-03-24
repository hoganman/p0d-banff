package main

import (
	"P0DBANFF/check"
	"fmt"
)

func main() {
	fmt.Println("P0DBANFF")
	check.DoesNotExistError(nil)
}
