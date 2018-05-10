/*
   For help, type "go run GridDownloadTool.go --help"

   This is a GO implementation of a Grid data download tool
   You need an initialized voms-proxy/dirac-proxy
   Author: Matthew Hogan
   email: hoganman@gmail.com

   Example
       $ go run GridDownloadTool.go -i filelist.list -o path/to/output/dir
	   Each line in filelist.list is the logical file name (LFN) lfn:/grid/t2k.org/...
*/

package main

import (
	"P0DBANFF/check"
	"P0DBANFF/constants"
	"P0DBANFF/directory"
	"P0DBANFF/file"
	"bufio"
	"bytes"
	"fmt"
	"github.com/pborman/getopt/v2"
	"os"
	"os/exec"
	"strconv"
	"strings"
	"time"
)

//******************************************************************************
func GridDownloadTool() {
	/* runs nRoutines goroutines to download Grid data */

	//*** Declare the manadatory options ***//
	//usage help flag
	helpOption := getopt.BoolLong("help", 'h', "display help")
	//input file flag
	inputFilesOption := getopt.StringLong("inputfiles", 'i', "", "input file list file")
	//local output directory flag
	outputDirectoryOption := getopt.StringLong("output", 'o', "", "output Directory")

	/*** Optional parameters ***/
	//set the number of go routines to run simultaneously
	nRoutinesOption := getopt.StringLong("routines", 'n', "5", "number of go rountines to run simultaneously (default=5)")
	getopt.Lookup("routines").SetOptional()

	var inputFiles []string
	var outputDir string

	// Parse the program arguments
	getopt.Parse()
	args := os.Args[1:]
	//must div by 2 since it counts everything
	nArgs := len(args) / 2

	if *helpOption {
		getopt.Usage()
		os.Exit(1)
	}
	if nArgs == 0 {
		inputFiles, outputDir = GetInputsFromUser()
	}
	if len(inputFiles) == 0 {
		inputFiles = ReadLines(*inputFilesOption)
	}
	if len(outputDir) == 0 {
		outputDir = *outputDirectoryOption
	}

	nRoutines, err := strconv.Atoi(*nRoutinesOption)
	if !check.Nil(err) {
		errMsg := fmt.Sprintf("ERROR: \"%s\" is not a valid input", *nRoutinesOption)
		fmt.Println(errMsg)
		os.Exit(1)
		getopt.Usage()
	}

	if exists, _ := directory.Exists(outputDir); !exists {
		errMsg := fmt.Sprintf("ERROR: \"%s\" directory does NOT exist ", outputDir)
		fmt.Println(errMsg)
		os.Exit(1)
		getopt.Usage()
	}

	if len(inputFiles) < 1 {
		fmt.Println("ERROR: no files to download")
		os.Exit(1)
		getopt.Usage()
	}

	outputDir = strings.TrimRight(outputDir, "/")

	fmt.Println("The number of files to download are", len(inputFiles))
	fmt.Println("The output directory is", outputDir)
	fmt.Println("Sleeping for 5 seconds before executing")

	time.Sleep(constants.KSleep_Confirm)

	//the status of the goroutine
	status := make(chan string)

	//*** Run the download routines ***//
	if len(inputFiles) > nRoutines {

		//Run the first nRoutines routines
		for j := 0; j < nRoutines; j++ {
			go Download(inputFiles[j], outputDir, status)
		}

		//wait for first routine to finish
		fmt.Println(<-status)

		//when one routine finishes, submit another
		for i := nRoutines; i < len(inputFiles); i++ {
			go Download(inputFiles[i], outputDir, status)
			fmt.Println(<-status)
		}

		//finish whatever routines are left
		for j := 1; j < nRoutines; j++ {
			fmt.Println(<-status)
		}

	} else {
		//There are too few files, so run the routines in sequence
		for j := 0; j < len(inputFiles); j++ {
			go Download(inputFiles[j], outputDir, status)
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
func GetInputsFromUser() (inputFiles []string, outputDir string) {
	/* If no command line inputs are given, get the file list, local outputDir,
	   and remote GridDir from user interactively */

	inputListName := GetFromInput("Enter input list (/path/to/file/list): ")
	inputFiles = ReadLines(inputListName)
	outputDir = GetFromInput("Enter output directory (/path/to/output/directory/): ")
	return
}

//******************************************************************************
func GetListOfReplicas(logicalFileName string) (replicas []string) {
	/* the list the replicas locations of a LFN using "lcg-lr" */
	cmd := exec.Command("lcg-lr", "-V t2k.org", logicalFileName)
	stdoutStderr, err := cmd.CombinedOutput()
	if !check.Nil(err) {
		fmt.Println("A problem occured with task", cmd)
		fmt.Printf("%s\n", stdoutStderr)
		panic(err)
	}
	tmpReplicas := bytes.Split(stdoutStderr, []byte("\n"))
	//replicas = make([]string, len(tmpReplicas))
	for _, byteStr := range tmpReplicas {
		str := string(byteStr)
		if !strings.Contains(str, "srm") {
			continue
		}
		replicas = append(replicas, str)
	}
	return
}

//******************************************************************************
func Download(logicalFileName, outputDirectory string, status chan string) {
	/* the download command "lcg-cp" from srm storage element */

	program := "lcg-cp"
	VOname := "-V t2k.org"
	verbose := "-vvv"
	//replicas := GetListOfReplicas(logicalFileName)
	//if len(replicas) == 0 {
	//	fmt.Println("Unable to get list of replicas for file", logicalFileName)
	//	return
	//}
	//remoteFileFullPath := replicas[0]
	remoteFileFullPath := logicalFileName
	remoteFile := remoteFileFullPath[strings.LastIndexAny(remoteFileFullPath, "/")+1:]
	localFileFullPath := fmt.Sprintf("%s/%s", outputDirectory, remoteFile)

	//join the command and options with a space between each
	args := strings.Join([]string{verbose, VOname, remoteFileFullPath, localFileFullPath}, " ")
	fullcmd := strings.Join([]string{program, args}, " ")
	if exists, _ := file.Exists(localFileFullPath); !exists {
		cmd := exec.Command(program, VOname, verbose, remoteFileFullPath, localFileFullPath)
		//cmd := exec.Command("echo", "5")
		stdoutStderr, err := cmd.CombinedOutput()
		if !check.Nil(err) {
			fmt.Println("A problem occured with job", fullcmd)
			fmt.Printf("%s\n", stdoutStderr)
			//panic(err)
			//confirmContinue := GetFromInput("Would you like to continue (y/N)?")
		}
	} else {
		fullcmd = fmt.Sprintf("File %s already downloaded, skipping", localFileFullPath)
	}
	status <- fullcmd
	return
}

//******************************************************************************
func main() {
	GridDownloadTool()
	os.Exit(0)
}
