/*
   A basic error checking package that is mostly redundant except to make code a little more verbose and clear
   AUTHOR: Matthew Hogan
   EMAIL: hoganman@gmail.com
*/
package check

import "os"

func CheckError(err1, err2 error) bool {
	return err1 == err2
}

func Nil(err error) bool {
	return CheckError(err, nil)
}

//func PathError(err error) bool {
//	return CheckError(err, *os.PathError)
//}

func ExistError(err error) bool {
	return os.IsExist(err)
}

func DoesNotExistError(err error) bool {
	return os.IsNotExist(err)
}

func IsNotExist(err error) bool {
	return os.IsNotExist(err)
}

func IsExist(err error) bool {
	return os.IsExist(err)
}
