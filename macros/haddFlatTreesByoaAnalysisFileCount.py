#!/bin/python
import os,sys,math

def RunCommand(command):
    print command
    os.system(command)

def GetNumberOfFiles(inputFileListName):
    inputFile = open(inputFileListName,'r')
    inputFileList = inputFile.read().split('\n')
    return len(inputFileList)


def haddFlatTrees(inputFlatTreeFileListName,inputOAAnalysisFileListName,inputPath,inputNamePrefix,nOAAnalysisFilesPerHadd=100):
    nFlatTreeFiles = GetNumberOfFiles(inputFlatTreeFileListName)
    #print nFlatTreeFiles 
    nOAAnalysisFiles = GetNumberOfFiles(inputOAAnalysisFileListName)
    #print nOAAnalysisFiles 
    nHadds = int(math.ceil(float(nOAAnalysisFiles) / nOAAnalysisFilesPerHadd))
    nFlatTreesPerHadd = int(math.ceil(float(nFlatTreeFiles) / nHadds))
    #print nFlatTreesPerHadd 
    startCounter = 1
    outputFiles = []
    haddCommands = []
    for haddIndex in range(1,nHadds+1):
        outputFile = '%s/%s_hadd_%d.root'%(inputPath,inputNamePrefix,haddIndex)
        outputFiles.append(outputFile)
        haddCommand = 'hadd %s '%(outputFile)
        nFilesAdded = 0
        for counter in range(startCounter,startCounter+nFlatTreesPerHadd):
            fileName = '%s/%s_%d.root'%(inputPath,inputNamePrefix,counter)
            if(os.path.isfile(fileName)):
                #print fileName
                haddCommand = haddCommand+' %s '%(fileName)
                nFilesAdded += 1
        if(nFilesAdded > 0.5 * nFlatTreesPerHadd):
            haddCommands.append(haddCommand)
        else:
            for counter in range(startCounter,startCounter+nFlatTreesPerHadd):
                fileName = '%s/%s_%d.root'%(inputPath,inputNamePrefix,counter)
                if(os.path.isfile(fileName)):
                    #print fileName
                    haddCommands[len(haddCommands)-1] = haddCommands[len(haddCommands)-1] + ' %s '%(fileName)
        startCounter += nFlatTreesPerHadd 
    haddFileList = open('%s/%s_hadd.list'%(inputPath,inputNamePrefix),'w')
    for index in range(0,len(haddCommands)):
        RunCommand(haddCommands[index])
        haddFileList.write('%s\n'%(outputFiles[index]))
    haddFileList.close()
