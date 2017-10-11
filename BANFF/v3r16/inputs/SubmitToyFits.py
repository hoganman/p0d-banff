#!/usr/bin/python
import os

#Script meant to be run from the inputs directory.

#Set the number of toys here.
nToys = 400

#A string to make up the base name of the fit.
baseName = "FinalToyFits_20150422"

#Open up the template parameters file.

paramTemplate = open('../parameters/BANFF.parameters.dat','r')

fileContents = paramTemplate.read()

#For each toy, replace the wildcards with the appropriate numbered content, making the parameters file
#and the submission script.
for i in xrange(0,nToys):

	thisFileContents = fileContents

	thisFileContents = thisFileContents.replace("REPLACEFIRSTTOYTOFIT", str(i));
	thisFileContents = thisFileContents.replace("REPLACEOUTPUTFILE", baseName + "_" + str(i) + ".root")

	#Create an output parameters file to use.
	thisParamFile = open(baseName + "_" + str(i) + ".parameters.dat","w")
	thisParamFile.write(thisFileContents)
	thisParamFile.close()

	#Now, write the submission script.
	thisSubmissionScript = open(baseName + "_" + str(i) + ".pbs","w")
	thisSubmissionScript.write("#!/bin/bash\n")
	thisSubmissionScript.write("#PBS -l nodes=1:ppn=8,walltime=24:00:00\n")
	thisSubmissionScript.write("#PBS -N " + baseName + "_" + str(i) + "\n")
	thisSubmissionScript.write("cd $PBS_O_WORKDIR\n")
	thisSubmissionScript.write("source ../../../Run_At_Start_T2K.sh\n")
	thisSubmissionScript.write("source ../cmt/setup.sh\n")
	thisSubmissionScript.write("RunFit2014.exe " + baseName + "_" + str(i) + ".parameters.dat\n")
	thisSubmissionScript.close()

	os.system("qsub " + baseName + "_" + str(i) + ".pbs")
