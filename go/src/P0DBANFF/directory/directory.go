/*
   A directory manipulation package that reproduces most already implemented functionality in Go, but fits my needs
   AUTHOR: Matthew Hogan
   EMAIL: hoganman@gmail.com
*/
package directory

import (
	"P0DBANFF/file"
)

func Exists(directory string) (bool, error) {
	return file.Exists(directory)
}
