/*
   This is a GO implementation of the gridDownloadTool.py for TRIUMF only
   You need an initialized voms-proxy (voms-proxy-init)
   Author: Matthew Hogan
   email: hoganman@gmail.com

   for help, type "go run TRIUMFDownloadTool.go --help"
*/

package main

import (
	"bufio"
	"fmt"
	"os"
	//"os/exec"
	"github.com/pborman/getopt/v2"
	"strings"
	"time"
)

/* if anything bad happens, be sure to let the user know */
func Check(e error) {
	if e != nil {
		panic(e)
		os.Exit(1)
	}
}

/* replace all instances of "\n" from stdio with "" */
func GetFromInput(request string) (retString string) {
	fmt.Print(request)
	//create stdin instance
	reader := bufio.NewReader(os.Stdin)
	retString, err := reader.ReadString('\n')
	Check(err)
	retString = strings.Replace(retString, "\n", "", -1)
	return
}

/* get each line of "path" as entry in "lines" */
func ReadLines(path string) (lines []string) {
	inFile, _ := os.Open(path)
	//only close file until function has returned
	defer inFile.Close()
	if len(path) < 1 {
		inFile.Close()
		os.Exit(1)
	}
	//declare large array to store all possible files
	var tmplines [10000]string
	scanner := bufio.NewScanner(inFile)
	scanner.Split(bufio.ScanLines)

	var lineNum uint = 0
	for scanner.Scan() {
		text := scanner.Text()
		if len(text) <= 1 {
			continue
		}
		tmplines[lineNum] = text
		lineNum++
	}
	//return the slice of all lines in the file
	lines = tmplines[:lineNum]
	return
}

/* If no command line inputs are given, get the file list, local outputDir,
and remote TRIUMFDir from user interactively */
func GetInputsFromUser() (inputFiles []string, outputDir, TRIUMFDir string) {
	inputListName := GetFromInput("Enter input list (/path/to/file/list): ")
	inputFiles = ReadLines(inputListName)
	outputDir = GetFromInput("Enter output directory (/path/to/output/directory/): ")
	TRIUMFDir = GetFromInput("Enter remote directory (production###/X/... or raw/.../): ")
	return
}

/* the download command "lcg-cp" from TRIUMF */
func Download(remoteFile, remoteDirectory, outputDirectory string, status chan string) {
	program := "/usr/bin/lcg-cp"
	VOname := "--vo t2k.org"
	remoteFileFullPath := fmt.Sprintf("-v srm://t2ksrm.nd280.org/nd280data/%s/%s", remoteDirectory, remoteFile)
	localFileFullPath := fmt.Sprintf("%s/%s", outputDirectory, remoteFile)
	//join the command and options with a space between each
	fullcmd := strings.Join([]string{program, VOname, remoteFileFullPath, localFileFullPath}, " ")
	//cmd := exec.Command(program, VOname, remoteFile, localFile)
	//err := cmd.Run()
	status <- fullcmd
}

/* runs nRoutines goroutines to Download TRIUMF data */
func TRIUMFDownloadTool() {
	var inputFiles []string
	var outputDir, TRIUMFDir string
	//the result of finishing a Download
	//feel free to change this, but not too high to slow down other routine downloads
	const nRoutines = 3

	// Declare the flags to be used
	//usage help flag
	helpFlag := getopt.BoolLong("help", 'h', "display help")
	//input file flag
	inputFilesFlag := getopt.StringLong("inputfiles", 'i', "", "input file list file")
	//remote directory flag
	remoteDirectoryFlag := getopt.StringLong("remote", 'r', "", "remote Directory")
	//local output directory flag
	outputDirectoryFlag := getopt.StringLong("output", 'o', "", "output Directory")

	// Parse the program arguments
	getopt.Parse()
	args := os.Args[1:]
	//must div by 2 since it counts everything
	nArgs := len(args) / 2

	if *helpFlag {
		getopt.Usage()
		os.Exit(1)
	} else if nArgs == 0 {
		inputFiles, outputDir, TRIUMFDir = GetInputsFromUser()
	} else if nArgs == 3 && len(*inputFilesFlag) > 0 &&
		len(*remoteDirectoryFlag) > 0 && len(*outputDirectoryFlag) > 0 {
		inputFiles = ReadLines(*inputFilesFlag)
		TRIUMFDir = *remoteDirectoryFlag
		outputDir = *outputDirectoryFlag
	} else {
		getopt.Usage()
		os.Exit(1)
	}

	fmt.Println("The number of files to download are", len(inputFiles))
	fmt.Println("The output directory is", outputDir)
	fmt.Println("The remote direcotry is", TRIUMFDir)
	fmt.Println("Sleeping for 5 seconds before executing")

	time.Sleep(5 * time.Second)

	status := make(chan string)
	//Run the first nRoutines routines
	for j := 0; j < nRoutines; j++ {
		go Download(inputFiles[j], TRIUMFDir, outputDir, status)
	}

	//wait for first routine to finish
	fmt.Println(<-status)

	//when one routine finishes, submit another
	for i := nRoutines; i < len(inputFiles); i++ {
		go Download(inputFiles[i], TRIUMFDir, outputDir, status)
		fmt.Println(<-status)
	}

	//finish whatever routines are left
	for j := 1; j < nRoutines; j++ {
		fmt.Println(<-status)
	}
}

func main() {
	TRIUMFDownloadTool()
	os.Exit(0)
}
