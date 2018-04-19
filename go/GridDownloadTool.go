/*
   For help, type "go run GridDownloadTool.go --help"

   This is a GO implementation of a Grid data download tool
   TODO: An option can be added to change the remote host or storage element
   You need an initialized voms-proxy/dirac-proxy
   Author: Matthew Hogan
   email: hoganman@gmail.com

   Example
       $ go run GridDownloadTool.go -i filelist.list -r production006/X/.../ -o path/to/output/dir --srm=
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
func GridDownloadTool() {
	/* runs nRoutines goroutines to download Grid data */

	//*** Declare the options ***//
	//usage help flag
	helpOption := getopt.BoolLong("help", 'h', "display help")
	//input file flag
	inputFilesOption := getopt.StringLong("inputfiles", 'i', "", "input file list file")
	//remote directory flag
	remoteDirectoryOption := getopt.StringLong("remote", 'r', "", "remote Directory")
	//local output directory flag
	outputDirectoryOption := getopt.StringLong("output", 'o', "", "output Directory")
	//designate the storage element using srm format
	storageElementOption := getopt.StringLong("srm", 's', "srm://t2ksrm.nd280.org/nd280data", "SRM storage element")

	//*** Constants ***//
	var inputFiles []string
	var outputDir, GridDir, srm_se string
	//the result of finishing a Download
	//feel free to change this, but not too high to slow down other routine downloads
	const nRoutines = 5

	// Parse the program arguments
	getopt.Parse()
	args := os.Args[1:]
	//must div by 2 since it counts everything
	nArgs := len(args) / 2

	if *helpOption {
		getopt.Usage()
		os.Exit(1)
	} else if nArgs == 0 {
		inputFiles, outputDir, GridDir, srm_se = GetInputsFromUser()
	} else if nArgs == 4 && len(*inputFilesOption) > 0 &&
		len(*remoteDirectoryOption) > 0 && len(*outputDirectoryOption) > 0 {
		inputFiles = ReadLines(*inputFilesOption)
		GridDir = *remoteDirectoryOption
		outputDir = *outputDirectoryOption
		srm_se = *storageElementOption
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
	fmt.Println("The storage element is", srm_se)
	fmt.Println("The remote direcotry is", GridDir)
	fmt.Println("Sleeping for 5 seconds before executing")

	time.Sleep(constants.KSleep_Confirm)

	//the status of the goroutine
	status := make(chan string)

	//*** Run the download routines ***//
	if len(inputFiles) > nRoutines {

		//Run the first nRoutines routines
		for j := 0; j < nRoutines; j++ {
			go Download(srm_se, inputFiles[j], GridDir, outputDir, status)
		}

		//wait for first routine to finish
		fmt.Println(<-status)

		//when one routine finishes, submit another
		for i := nRoutines; i < len(inputFiles); i++ {
			go Download(srm_se, inputFiles[i], GridDir, outputDir, status)
			fmt.Println(<-status)
		}

		//finish whatever routines are left
		for j := 1; j < nRoutines; j++ {
			fmt.Println(<-status)
		}

	} else {
		//There are too few files, so run the routines in sequence
		for j := 0; j < len(inputFiles); j++ {
			go Download(srm_se, inputFiles[j], GridDir, outputDir, status)
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
func GetInputsFromUser() (inputFiles []string, outputDir, GridDir, srm_se string) {
	/* If no command line inputs are given, get the file list, local outputDir,
	   and remote GridDir from user interactively */

	inputListName := GetFromInput("Enter input list (/path/to/file/list): ")
	inputFiles = ReadLines(inputListName)
	outputDir = GetFromInput("Enter output directory (/path/to/output/directory/): ")
	GridDir = GetFromInput("Enter remote directory (production###/X/... or raw/.../): ")
	srm_se = GetFromInput("Enter SRM directory (default=srm://t2ksrm.nd280.org/nd280data)")
	if len(srm_se) == 0 {
		srm_se = "srm://t2ksrm.nd280.org/nd280data"
		fmt.Println("No SRM storage element set. Setting it to", srm_se)
	}
	return
}

//******************************************************************************
func Download(srm_se, remoteFile, remoteDirectory, outputDirectory string,
	status chan string) {
	/* the download command "lcg-cp" from srm storage element */

	program := "/usr/bin/lcg-cp -vvv"
	VOname := "--vo t2k.org"
	remoteFileFullPath := fmt.Sprintf("%s/%s/%s", srm_se, remoteDirectory, remoteFile)
	localFileFullPath := fmt.Sprintf("%s/%s", outputDirectory, remoteFile)
	//join the command and options with a space between each
	args := strings.Join([]string{VOname, remoteFileFullPath, localFileFullPath}, " ")
	fullcmd := strings.Join([]string{program, args}, " ")
	cmd := exec.Command(program, args)
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
	GridDownloadTool()
	os.Exit(0)
}
