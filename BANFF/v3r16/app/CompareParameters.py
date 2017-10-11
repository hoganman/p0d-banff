#!/usr/bin/python
from ROOT import *
import sys
import math

#First get the number of files to tabulate.
#Command line format is: textFileWithParamNamesToPutInTable file1 file2
nFiles = len(sys.argv) - 2

#Now, read in the parameters to include in the table from the text file.
textFile = open(sys.argv[1],'r')

#This is a map of the name to either 0 or the nominal value that the parameter
#is a fraction of
paramsToInclude = {}

#File format is 1 name for each line.  If the parameter as fitted is actually a
#fraction of nominal, the nominal value is included after a space.
#So, we rstrip off the "\n", look for " ", and if found, split(" ")
for line in textFile:

    line = line.rstrip()

    if(" " not in line):
        paramsToInclude[line] = 0

    else:

        lineList = line.split(" ")
        paramsToInclude[lineList[0]] = float(lineList[1])

#Load in the input files.
inputFiles = {}
for i in xrange(2,len(sys.argv)):
    
    inputFiles[sys.argv[i]] = TFile(sys.argv[i])


#From each file, load in the FittedParameters0 and FittedCovariance0.
FittedParameters = {}
FittedCovariance = {}
parameterNames = {}
for key in inputFiles:

    paramNamesArray = []
    FittedParameters[key] = inputFiles[key].Get("FittedParameters0")
    FittedCovariance[key] = inputFiles[key].Get("FittedCovariance0")

    for i in xrange(0,FittedParameters[key].GetNrows()):

        paramNamesArray.append(inputFiles[key].Get(("ParamName" + str(i))).GetString())

    #Add the completed paramNamesArray to the parameterNames dict.
    parameterNames[key] = paramNamesArray

#With everything loaded, check that the parameter name maps match for all the
#files.  If not, exit with an error.
nParams = 0
for key1 in inputFiles:

    #Store the number of parameters for later.
    nParams = len(parameterNames[key1])

    for key2 in inputFiles:

        if len(parameterNames[key1]) != len(parameterNames[key2]):

            print "Parameter number mismatch!  Exiting."
            sys.exit(1)

        #If we at least have the same number of parameters, see whether the
        #names match.
        for i in xrange(0,len(parameterNames[key1])):

            if parameterNames[key1][i] != parameterNames[key2][i]:

                print "Parameter name mismatch: " + parameterNames[key1] + " != "
                + parameterNames[key2]
                sys.exit(1)

#Create the output file that the Latex code for the table will go into.
outFile = open("paramTable.txt","w")
tableString = "\\begin{tabular}{ | c |"
for file in inputFiles:
    tableString += " c |"

tableString += " }\n"
outFile.write(tableString)

#Now, if we've gotten to here, the parameters match up just fine.  So, loop
#through, check the paramsToInclude list, and if it needs to be included, do so.

for i in xrange(0, nParams):
    paramString = "\\hline\n"

    for key in parameterNames:

        if str(parameterNames[key][i]) in paramsToInclude:

            if paramString == "\\hline\n":

                paramName = str(parameterNames[key][i])
                paramName = paramName.replace("_","\\char`_")
                paramString += "\\texttt{"+ paramName + "} "

            value = FittedParameters[key](i)
            error = math.sqrt(FittedCovariance[key](i,i))
            nbnr = paramsToInclude[str(parameterNames[key][i])] 
            
            #For not doing this if using a postfit derived result, where it has
            #already been converted.
            if nbnr != 0 and ("post" not in key):
               
                value = nbnr + (nbnr)*value
                error = nbnr*error

            elif nbnr != 0 and ("post" in key):
                value = nbnr*value
                error = nbnr*error

            #Make it 5 signficant digits each, mirroring the tech note.
            stringVal = '%s' % float('%.5g' % value)
            stringErr = '%s' % float('%.5g' % error)
            paramString += "& " + stringVal + " $\pm$ " + stringErr + " "

   
    if(paramString != "\\hline\n"):
            
        #Once have looped through all files, add the line break and the hline.
        paramString += "\\\\ " 
 
        outFile.write(paramString)

#So, after the very last parameter, add a newline, then the hline, then close
#off the table.
tableString = "\n\\hline\n\\end{tabular}"
outFile.write(tableString)

#Write out the file order so know which column is which.
#TODO: Do a better job of this.
outFile.write("\n")
for key in parameterNames:
    outFile.write("%" + key + "\n")


