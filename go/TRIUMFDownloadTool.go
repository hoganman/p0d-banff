/*
   For help, type "go run TRIUMFDownloadTool.go --help"

   This is a GO implementation of a T2K data download tool for TRIUMF only
   TODO: An option can be added to change the remote host or storage element
   You need an initialized voms-proxy/dirac-proxy
   Author: Matthew Hogan
   email: hoganman@gmail.com

   Example
       $ go run TRIUMFDownloadTool.go -i filelist.list -r production006/X/.../ -o path/to/output/dir
	   Each line in filelist.list is the file located in production006/X/.../
*/

package main

import (
	"P0DBANFF/check"
	"P0DBANFF/constants"
	"P0DBANFF/directory"
	"P0DBANFF/file"
	"bufio"
	"fmt"
	"github.com/pborman/getopt/v2"
	"os"
	"os/exec"
	"strings"
	"time"
)

//******************************************************************************
func TRIUMFDownloadTool() {
	/* runs nRoutines goroutines to download TRIUMF data */

	//*** Declare the options ***//
	//usage help flag
	helpFlag := getopt.BoolLong("help", 'h', "display help")
	//input file flag
	inputFilesFlag := getopt.StringLong("inputfiles", 'i', "", "input file list file")
	//remote directory flag
	remoteDirectoryFlag := getopt.StringLong("remote", 'r', "", "remote Directory")
	//local output directory flag
	outputDirectoryFlag := getopt.StringLong("output", 'o', "", "output Directory")

	//*** Constants ***//
	var inputFiles []string
	var outputDir, TRIUMFDir string
	//the result of finishing a Download
	//feel free to change this, but not too high to slow down other routine downloads
	const nRoutines = 5

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

	if exists, _ := directory.Exists(outputDir); !exists {
		errMsg := fmt.Sprintf("ERROR: \"%s\" does NOT exist ", outputDir)
		fmt.Println(errMsg)
		os.Exit(1)
		getopt.Usage()
	}
	if len(inputFiles) < 1 {
		fmt.Println("ERROR: no files to download")
		os.Exit(1)
		getopt.Usage()
	}

	fmt.Println("The number of files to download are", len(inputFiles))
	fmt.Println("The output directory is", outputDir)
	fmt.Println("The remote direcotry is", TRIUMFDir)
	fmt.Println("Sleeping for 5 seconds before executing")

	time.Sleep(constants.KSleep_Confirm)

	//the status of the goroutine
	status := make(chan string)

	//*** Run the download routines ***//
	if len(inputFiles) > nRoutines {

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

	} else {
		//There are too few files, so run the routines in sequence
		for j := 0; j < len(inputFiles); j++ {
			go Download(inputFiles[j], TRIUMFDir, outputDir, status)
			fmt.Println(<-status)
		}
	}
}

//******************************************************************************
func GetFromInput(request string) (retString string) {
	/* replace all instances of "\n" from stdio with "" */

	fmt.Print(request)
	//create stdin instance
	reader := bufio.NewReader(os.Stdin)
	retString, err := reader.ReadString('\n')
	if !check.Nil(err) {
		fmt.Println("ERROR: Could not get input")
		panic(err)
		os.Exit(1)
	}
	retString = strings.Replace(retString, "\n", "", -1)
	return
}

//******************************************************************************
func ReadLines(path string) (lines []string) {
	/* get each line of "path" as entry in "lines" */

	if exists, _ := file.Exists(path); !exists {
		fmt.Println("ERROR: File does not exist!")
		os.Exit(1)
	}
	inFile, _ := os.Open(path)
	//only close file until function has returned
	defer inFile.Close()
	if len(path) < 1 {
		inFile.Close()
		os.Exit(1)
	}
	//declare large array to store all possible files
	var tmplines [100000]string
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

//******************************************************************************
func GetInputsFromUser() (inputFiles []string, outputDir, TRIUMFDir string) {
	/* If no command line inputs are given, get the file list, local outputDir,
	   and remote TRIUMFDir from user interactively */

	inputListName := GetFromInput("Enter input list (/path/to/file/list): ")
	inputFiles = ReadLines(inputListName)
	outputDir = GetFromInput("Enter output directory (/path/to/output/directory/): ")
	TRIUMFDir = GetFromInput("Enter remote directory (production###/X/... or raw/.../): ")
	return
}

//******************************************************************************
func Download(remoteFile, remoteDirectory, outputDirectory string,
	/* the download command "lcg-cp" from TRIUMF */

	status chan string) {
	program := "lcg-cp"
	VOname := "--vo t2k.org"
	remoteFileFullPath := fmt.Sprintf("-v srm://t2ksrm.nd280.org/nd280data/%s/%s", remoteDirectory, remoteFile)
	localFileFullPath := fmt.Sprintf("%s/%s", outputDirectory, remoteFile)
	//join the command and options with a space between each
	fullcmd := strings.Join([]string{program, VOname, remoteFileFullPath, localFileFullPath}, " ")
	cmd := exec.Command(program, "--vo", "t2k.org", "-v", fmt.Sprintf("srm://t2ksrm.nd280.org/nd280data/%s/%s", remoteDirectory, remoteFile), localFileFullPath)
	//cmd := exec.Command("/bin/sleep", "5")
	err := cmd.Run()
	if !check.Nil(err) {
		fmt.Println("A problem occured with job", fullcmd)
		fmt.Println(err)
		panic(err)
		//confirmContinue := GetFromInput("Would you like to continue (y/N)?")
	}
	status <- fullcmd
}

//******************************************************************************
func main() {
	TRIUMFDownloadTool()
	os.Exit(0)
}
