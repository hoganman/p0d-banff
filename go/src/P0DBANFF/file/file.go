/*
   A file manipulation package that reproduces most already implemented functionality in Go, but fits my needs
   AUTHOR: Matthew Hogan
   EMAIL: hoganman@gmail.com
*/

package file

import (
	"P0DBANFF/check"
	"os"
)

func Exists(filename string) (bool, error) {
	_, err := os.Stat(filename)
	if check.Nil(err) {
		return true, nil
	}
	if check.DoesNotExistError(err) {
		return false, nil
	}
	return true, err
}
