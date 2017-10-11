#!/usr/bin/python
import os

#The root of the fit output file name.
#fileRoot = "ToyFit_20150202/ToyFit_20150202_"
fileRoot = "~/data/FinalToyFits_20150422/FinalToyFits_20150422_"

#The beginning of the string whose command will be invoked.
#commandString = "PerformPullValidation.exe fxo full_300toys_20150202.root 100 "
#PerformPullValidation.exe just needs a toy file with the same number of
#parameters of each type as was used in the fit.  Ideally this should be
#changed (since toys use Detector throws and the fit uses ObsNorm) but using a
#different file allows this to work for the time being.
commandString = "PerformPullValidation.exe fxo fixedCCQE_400toys_NIWGTune_20150224.root 400 "

#A list of bad file numbers
#badFiles = [0,13,16,17,18,19,21,24,26,27,31,35,4,42,45,49,55,56,57,64,67,70,8,84,9]

#Add in the file names.
for i in xrange(0,400):

    #TODO: Actually, cannot skip failed fits here.  Needs to be coded into
    #PerformPullValidation.cxx as this is unfortunately order dependent.
    #Skip the failed fits.
    #if i in [226,24,288]:
    #    continue

	commandString += fileRoot + str(i) + ".root "

#With all the files looped over, we have the full command.  Print it to stdout.
#print commandString

os.system(commandString)
