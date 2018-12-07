#!/usr/bin/env python

import sys, getopt,os,datetime,math,glob,time,subprocess
import random

#######################
inOptions = {
        'c:':['cluster=','which computing cluster currently supported are either "csuhpc" OR "csuhep"'],
        'e:':['email=','email address to notify'],
        'h:':['help=','this help message'],
        'H:':['hours=','max time limit in hours'],
        'j:':['other=','other options separated by the # (pound) character'],
        'L:':['list=','input file list'],
        'm:':['memory=','per-job maximum memory limit in MEGAbytes'],
        'M:':['minutes=','max time in minutes'],
        'n:':['num-jobs=','number of jobs at max'],
        'N:':['hostname=','sets the node/hostname (no defaults set)'],
        'o:':['output-name-prefix=','output file prefix (example -o naME produces naME_1.root)'],
        'O:':['opportunistic-nodes=','Use opportunistic nodes (default=0)'],
        'p:':['output-path=','path for output files'],
        'P:':['priority=','job priority'],
        'q:':['qname=','sets which queue (default=\"physics.q|short.q\")'],
        'Q:':['num-files-per-job=','sets the maximum number of input files per job'],
        'r:':['run=','run number'],
        's:':['soft=','soft resource requirments for each job'],
        'S:':['hard=','hard resource requirments for each job']
}
#       s_cpu     The per-process CPU time limit in seconds.
#       s_data    The per-process maximum memory limit in bytes.
#       s_vmem    The same as s_data (if both are set the minimum is used).
#       h_cpu     The per-job CPU time limit in seconds.
#       h_data    The per-job maximum memory limit in bytes.
#       h_vmem    The same as h_data (if both are set the minimum is used).

SECONDS_BTN_RUN = 10
SECONDS_BTN_QSUB = 16
csuhpc = -1
queueTag = '$'
HIGHLANDIOROOT = os.getenv('HIGHLANDIOROOT')
BIN = '/physics/INSTALLATION/bin'
BASE = os.getenv('P0DBANFFROOT')
MACROS = '%s/macros' % (BASE)
RUNCREATEFLATTREE = subprocess.Popen(['which','RunCreateFlatTree.exe'],stdout=subprocess.PIPE).communicate()[0].split('\n')[0]
ROOT = subprocess.Popen(['which','root'],stdout=subprocess.PIPE).communicate()[0].split('\n')[0]+' -l -q -b'
CMTPATH = os.getenv('CMTPATH')
PYTHONPATH = '%s:%s/macros'%(os.getenv('PYTHONPATH'),BASE)
CMTPROJECTPATH = os.getenv('CMTPROJECTPATH')
PATH=os.getenv('PATH')
LD_LIBRARY_PATH=os.getenv('LD_LIBRARY_PATH')
nNodes = 43
nFreeUseNodes = 19
physicsNodes = [40,41,42,43]
FlatTrees = []
fileList = {} # a dictionary with a oaAnalysis file as the key and 0(unused) or 1(used) as the value
totalTimeInHours = 0

class computeNode:
    def __init__(self,name,opportunistic):
        self.name = name
        self.opportunistic = opportunistic
        self.cores         = int(12)
        self.jobs          = int(0)
	self.state         = ''
    def IsOpportunistic(self):
        return self.opportunistic
    def GetNCores(self):
        return self.cores
    def GetName(self):
        return self.name
    def GetNodeName(self):
        return self.name
    def GetNJobs(self):
        return self.jobs
    def GetState(self):
	return self.state
    def SubmitJob(self):
        self.jobs = int(self.jobs) + 1
    def JobRunningAlready(self):
        self.jobs = int(self.jobs) + 1
    def SetState(self,state):
	self.state = str(state)
    def SetNCores(self,cores):
	self.cores = int(cores)
    def SetNJobs(self,jobs):
	self.jobs = int(jobs)

class gpuNode(computeNode):
    def __init__(self,name,opportunistic):
        self.name = name
        self.opportunistic = opportunistic
        self.cores         = int(12)
        self.jobs          = int(0)
	self.state         = ''

class queueOptions:
    def __init__(self):
        self.queueTag               =  queueTag
    def __init__(self,walltimeHours,walltimeMinutes,maxMemory,softRequirements,hardRequirements,useQueueName,useHostName,otherRequirementsList,emailAddress):
        self.queueTag               =  queueTag
        self.walltimeHours          =  walltimeHours
        self.walltimeMinutes        =  walltimeMinutes
        self.maxMemory              =  maxMemory
        self.softRequirements       =  softRequirements
        self.hardRequirements       =  hardRequirements
        self.useQueueName           =  useQueueName
        self.useHostName            =  useHostName
        self.otherRequirementsList  =  otherRequirementsList
        self.emailAddress           =  emailAddress
    def GetQueueTag(self):
        return self.queueTag

nodes = {} # a dictionary with nodes in the range node1 - node43

queues = [
        'ens.q'            ,
        'gpu.q'            ,
        'defaultstudent.q' ,
        'defaultfaculty.q' ,
        'musky.q'          ,
        'snow.q'           ,
        'physics.q'
]

def GetOutputFromCommand(command,splitBy = ''):
        out = os.popen(command).read()
        if out.find('error') != -1:
                if len(splitBy) > 0:
                        return []
                else:
                        return ''
        if len(splitBy) > 0:
                out = out.split(splitBy)
        return out



def InitNodes():

    for i in range(1,nNodes+1):
        opportunistic = 0
        if i > nFreeUseNodes:
            opportunistic = 1
        nodeName = 'node'+str(i)
        node = computeNode(nodeName,opportunistic)
        nodes[nodeName] = node
    currentJobs = GetOutputFromCommand('qstat -f','\n')
    if len(currentJobs) == 0:
    	return
    for line in currentJobs:
    	hostName = ''
    	queueName = ''
    	if line.find('.q@') == -1:
    		continue
        if '-NA-' in line:
            continue
    	index = line.find('@')-1
    	queueName = line[0:line.find('@')]
    	index = line.find('@')
    	while line[index] != ' ':
    		index += 1
    	index -= 1
    	hostName = line[line.find('@')+1:index+2].strip()
    	if hostName not in nodes.keys():
    		continue
    	jobsAndSlots = line.split('BIP   ')[1].split('.')[0]
    	jobsAndSlots = jobsAndSlots[0:len(jobsAndSlots)-2].strip().split('/')
    	state = line.split('lx-amd64')[1].strip()
    	#reserved = jobsAndSlots[0]
    	#jobs = jobsAndSlots[1]
    	#slots = jobsAndSlots[2]
        #if hostName not in nodes:
        #    continue
        #node = nodes[hostName]
	#if node.GetNCores() < slots:
        #    nodes[hostName].SetNCores(slots)
	if len(state) > 0:
            if len(node.GetState()) == 0:
                node.SetState(state)
            if state not in node.GetState():
                node.SetState(node.GetState()+','+state)


def GetLongOption(shortOption):
    if shortOption.find(':') == -1:
        shortOption = shortOption+':'
    if shortOption[0] == '-':
        shortOption = shortOption[1:len(shortOption)]
    longOption = inOptions[shortOption][0]
    if longOption.find('=') != -1:
        longOption = longOption[0:len(longOption)-1]
    return '--'+longOption

def SubmitJob(inputCommand):
    qsubCommand = 'qsub %s'%(inputCommand)
    os.system(qsubCommand)


########################

def CreateExportedPathsScript(submissionFileName):
    submission = open(submissionFileName,'w')
    submission.write('#!/bin/sh\n')
    submission.write('\n')
    submission.write('export PATH=%s \n'%(PATH))
    submission.write('export LD_LIBRARY_PATH=%s \n'%(LD_LIBRARY_PATH))
    submission.write('export PYTHONPATH=%s \n'%(PYTHONPATH))
    submission.write('export CMTPATH=%s \n'%(CMTPATH))
    submission.write('export CMTPROJECTPATH=%s \n'%(CMTPROJECTPATH))
    submission.close()



def CreateFlatTreeSubmissionScript(jobNum,numJobs,priority,walltimeHours,walltimeMinutes,maxMemory,softRequirements,hardRequirements,useQueueName,useHostName,otherRequirementsList,emailAddress):

    CWD = os.getcwd()
    #create the submission script
    submissionFileName = 'submit_ajob_'+str(jobNum)+'.sh'
    submission = open(submissionFileName,'w')
    submission.write('#!/bin/sh\n')
    submission.write('\n')
    if csuhpc == 1:
      submission.write('#set the job priority\n')
      submission.write('#%s -p %d \n'%(queueTag,priority))
    submission.write('#merge stdout and stderr into one file\n')
    submission.write('#%s -j yes \n'%(queueTag))
    submission.write('#%s -wd %s\n'%(queueTag,BASE))

    submission.write('#%s -o %s \n'%(queueTag,CWD))
    #soft requirements
    if len(softRequirements) > 0:
        submission.write('#%s -soft %s \n'%(queueTag,softRequirements))
        submission.write('\n')
    #hard requirements
    if len(hardRequirements) > 0:
        submission.write('#%s -hard %s \n'%(queueTag,hardRequirements))
        submission.write('\n')
    if len(emailAddress) > 0:
        submission.write('# email user\n')
        submission.write('#%s -M %s \n'%(queueTag,emailAddress))
        submission.write('#%s -m a \n'%(queueTag))
        submission.write('\n')
    #hostname and qname
    if len(useHostName) > 0 or len(useQueueName) > 0:
        if len(useHostName) > 0:
            if csuhpc == 1:
              submission.write('#hostname\n')
              submission.write('#%s -l hostname="%s"\n'%(queueTag,useHostName))
        if len(useQueueName) > 0:
            if csuhpc == 1:
              submission.write('#qname\n')
              submission.write('#%s -l qname="%s"\n'%(queueTag,useQueueName))
    if walltimeHours > -1 or walltimeMinutes > -1:
        if walltimeHours != -1:
            walltimeHours = str(walltimeHours).zfill(2)
        else:
            walltimeHours = '00'
        if walltimeMinutes != -1:
            walltimeMinutes = str(walltimeMinutes).zfill(2)
        else:
            walltimeMinutes = '00'

        if csuhpc == 1:
            walltimeSeconds = str((float(walltimeHours)*60+float(walltimeMinutes))*60)
            submission.write('#%s -l h_cpu=%s \n'%(queueTag,walltimeSeconds))
        else:
            submission.write('#%s -l walltime=%s:%s:00 \n'%(queueTag,walltimeHours,walltimeMinutes))
    if maxMemory > -1:
        if csuhpc == 1:
            submission.write('#%s -l h_data=%sm \n'%(queueTag,maxMemory))
        else:
            submission.write('#%s -l pmem=%smb \n'%(queueTag,maxMemory))
    #other options
    if len(otherRequirementsList) > 0:
        for otherOption in otherRequirementsList:
            submission.write('#$ '+otherOption+' \n')
            submission.write('\n')
    submission.write('source %s/ExportedPaths.sh \n'%(CWD))
    submission.write('source %s/Setup-P0DBANFF.sh\n'%(BASE))
    #submission.write('source %s/nd280Highland2/%s/cmt/setup.sh\n'%(BASE, os.getenv('HIGHLAND2VERSION')))
    # ROOTrandomSeed = int(math.ceil((datetime.datetime.now() -
    #                                 datetime.datetime(1970, random.randint(1, 12), random.randint(1, 28))
    #                                ).total_seconds()))
    submission.write('%s \'${P0DBANFFROOT}/macros/ROOTRandomSleep.C(%d)\'\n' % (ROOT, SECONDS_BTN_RUN*numJobs))
    submission.write('$P0DBANFFROOT/app/gethpcstorage_usage.py\n')
    submission.write('\n')
    submission.write('sh %s/ajob_%d.sh\n'%(CWD,jobNum))
    submission.write('\n')
    submission.close()
    os.system('chmod +x %s'%(submissionFileName))

def CreateFlatTreeJobScript(jobNum,subFileList,runNumber,outputPath,outputName):
    #create filelist
    CWD = os.getcwd()
    subFileListName = '%s/filelists/filelist_%d.txt'%(CWD,jobNum)
    subFile = open(subFileListName,'w')
    for fileEntry in subFileList:
        subFile.write(fileEntry+'\n')
    subFile.close()
    #create job
    jobFileName = 'ajob_'+str(jobNum)+'.sh'
    job = open(jobFileName,'w')
    job.write('#!/bin/sh\n')
    outputFile = '%s_%d.root'%(outputName,jobNum)
    outputFileAndPath = '%s/%s'%(outputPath,outputFile)
    FTOUTPUT = '%s/%s'%(outputPath,outputFile)
    job.write('%s \'%s/CheckFileShell.C("%s")\'\n'%(ROOT,MACROS,FTOUTPUT))
    job.write('FILEGOOD=$?\n')
    job.write('if [ $FILEGOOD -eq 0 ]; then\n')
    command = '  %s -v -o %s %s \n'%(RUNCREATEFLATTREE,outputFileAndPath,subFileListName)
    job.write(command)
    job.write('fi\n')
    job.write('%s \'%s/CheckFileShell.C("%s")\'\n'%(ROOT,MACROS,FTOUTPUT))
    job.write('FILEGOOD=$?\n')
    job.write('if [ $FILEGOOD -eq 0 ]; then\n')
    job.write('  echo "The FlatTree failed! There was some kind of crash!"\n')
    job.write('  exit 1\n')
    job.write('fi\n')
    job.write('\n')
    job.close()
    os.system('chmod +x %s'%(jobFileName))


def MakeJobs(runNumber,outputPath,outputName,numJobs,numFilesPerJob,priority,walltimeHours,walltimeMinutes,maxMemory,softRequirements,hardRequirements,useOpportunisticNodes,useQueueName,useHostName,otherRequirementsList,emailAddress,ExportedPathsName):

    subFileList = []
    os.mkdir('filelists')
    os.system('chmod +w filelists')
    if len(useQueueName) == 0 and len(useHostName) == 0:
        if useOpportunisticNodes == 0 and len(useHostName) == 0:
            useHostName = ''
            for i in range(1,nFreeUseNodes+1):
                nodeName = 'node'+str(i)
                if nodeName not in nodes:
                    continue
                if len(nodes[nodeName].GetState()) > 0:
                    print 'WARNING: Will not being using '+nodeName+' due to state '+nodes[nodeName].GetState()+' (check qstat -f)'
                    continue
                useHostName = useHostName+nodeName+'|'
        else:
            useHostName = ''
            for i in range(1,nNodes+1):
                nodeName = 'node'+str(i)
                if nodeName not in nodes:
                    continue
                if len(nodes[nodeName].GetState()) > 0:
                    print 'WARNING: Will not being using  '+nodeName+' due to state '+nodes[nodeName].GetState()+' (check qstat -f)'
                    continue
                useHostName = useHostName+nodeName+'|'
        while useHostName[len(useHostName)-1] == '|':
            useHostName = useHostName[0:len(useHostName)-1]
    globalFileListIndex = 0
    global fileList
    oaAnalysisFiles = fileList.keys()
    for jobNum in range(0,numJobs):
        outputFile = '%s/%s_%d.root'%(outputPath,outputName,jobNum+1)
        fileCounter = 0
        while fileCounter < numFilesPerJob:
            if globalFileListIndex == len(fileList):
                break
            aoaAnalysisFile = oaAnalysisFiles[globalFileListIndex]
            if fileList[aoaAnalysisFile] == 1:
                print "You should not be calling an already used oaAnalysis file"
                print "   going to the next one "
            else:
                subFileList.append(aoaAnalysisFile)
                fileList[aoaAnalysisFile] = 1
                fileCounter = fileCounter + 1
            globalFileListIndex = globalFileListIndex + 1

        if len(subFileList) == 0:
            break
        #create submit script
        CreateFlatTreeSubmissionScript(jobNum+1,numJobs,priority,walltimeHours,walltimeMinutes,maxMemory,softRequirements,hardRequirements,useQueueName,useHostName,otherRequirementsList,emailAddress)

        #create job script
        FlatTrees.append(outputFile)
        CreateFlatTreeJobScript(jobNum+1,subFileList,runNumber,outputPath,outputName)

        #submit job
        SubmitJob('submit_ajob_%d.sh'%(jobNum+1))

        if jobNum+1 != numJobs:
            print "sleeping for", SECONDS_BTN_QSUB, "seconds till next job sub"
            time.sleep(SECONDS_BTN_QSUB)  # seconds

        #restart list
        del subFileList[0:]
    #end of loop over jobs

def MakeHaddScript(outputPath,outputName,ExportedPathsName,lastJobNumber,jobSubmissionFileName="submit_ajob",nHadds=9):

    CWD = os.getcwd()
    #create a script to check if all submitted jobs are done and hadd them.
    filePrefixes = ['']
    haddJobFileName = 'haddFiles.sh'
    haddJob = open(haddJobFileName,'w')
    haddJob.write('#!/bin/sh\n')
    haddJob.write('\n')
    haddJob.write('source %s/%s\n'%(CWD,ExportedPathsName))
    haddJob.write('source %s/Setup-P0DBANFF.sh\n'%(BASE))
    for prefixes in filePrefixes:
        outputNameWithPrefix = prefixes+outputName
	haddJob.write('#%s\n'%(prefixes))
        haddJob.write('python -c \'import hadd; hadd.haddProcessedFiles("%s","%s","%s",1,%d,"%s",%d)\'\n'%(outputPath,outputPath,outputNameWithPrefix,lastJobNumber,jobSubmissionFileName,nHadds))
        haddJob.write('#This job will fail if the top one fails\n')
        haddJob.write('python -c \'import hadd; hadd.haddThirds("%s","%s","%s")\'\n'%(outputPath,outputPath,outputNameWithPrefix))
	haddJob.write('\n')

    haddJob.close()
    os.system('chmod +x %s'%(haddJobFileName))



def GetListofFiles(inputlistfile):
    filelist = []
    if not os.path.isfile(inputlistfile):
        return filelist
    infile = open(inputlistfile)
    line = infile.readline()
    while len(line) > 0:
        #skip comment lines
        if line.find('#') == -1:
            filelist.append(line.split('\n')[0])
        line = infile.readline()
    infile.close()
    return filelist

def main(argv):
    if HIGHLANDIOROOT.find('None') == 0:
        print 'You must have HIGHLANDIOROOT exported for this script to work'
        print helpstatement
        sys.exit(2)
    shortArgsList = []
    longArgsList = []
    argDescriptionList = []
    for key in inOptions.keys():
        sublist = inOptions[key]
        shortArgsList.insert(0,key)
        longArgsList.insert(0,sublist[0])
        argDescriptionList.insert(0,sublist[1])
    shortArgs = ''.join(shortArgsList)
    inputList = ''
    runNumber = -1
    outputPath = ''
    outputName = ''
    numJobs = -1
    priority = -10
    numFilesPerJob = -1
    walltimeHours = -1
    walltimeMinutes = -1
    maxMemory = -1 #in MEGAbytes
    softRequirements = ''
    hardRequirements = ''
    otherRequirements = ''
    emailAddress = ''
    clusterName = ''
    global csuhpc
    global queueTag
    useQueueName = ''
    useHostName = ''
    useOpportunisticNodes = 0
    helpstatement = 'qsubmitter.py [OPTIONS]\n'
    for ele in range(0,len(shortArgsList)):
        helpstatement=helpstatement+'-%s/--%s == %s \n'%(shortArgsList[ele][0:1],longArgsList[ele][0:len(longArgsList[ele])-1],argDescriptionList[ele])
    try:
        opts, args = getopt.getopt(argv,shortArgs,longArgsList)
    except getopt.GetoptError:
	print 'Option ERROR'
	print 'Error message = '+str(getopt.GetoptError.msg)
	print 'Option = '+str(getopt.GetoptError.opt)
        print 'Printing help statement'
        print helpstatement
        sys.exit(2)
    if len(opts) == 0:
	print 'No options input'
        print 'Printing help statement'
        print helpstatement
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-h',GetLongOption('-h')):
            print helpstatement
            sys.exit()
        elif opt in ('-L',GetLongOption('-L')):
            inputList = arg
        elif opt in ('-r',GetLongOption('-r')):
            runNumber = int(arg)
        elif opt in ('-p',GetLongOption('-p')):
            outputPath = arg
        elif opt in ('-P',GetLongOption('-P')):
	    priority = int(arg)
        elif opt in ('-o',GetLongOption('-o')):
            outputName = arg
        elif opt in ('-n',GetLongOption('-n')):
            numJobs = int(arg)
        elif opt in ('-Q',GetLongOption('-Q')):
            numFilesPerJob = int(arg)
        elif opt in ('-H',GetLongOption('-H')):
            walltimeHours = int(arg)
        elif opt in ('-M',GetLongOption('-M')):
            walltimeMinutes = int(arg)
        elif opt in ('-s',GetLongOption('-s')):
            softRequirements = arg
        elif opt in ('-S',GetLongOption('-S')):
            hardRequirements = arg
        elif opt in ('-j',GetLongOption('-j')):
            otherRequirements = arg
        elif opt in ('-e',GetLongOption('-e')):
            emailAddress = arg
        elif opt in ('-m',GetLongOption('-m')):
            maxMemory = arg
        elif opt in ('-c',GetLongOption('-c')):
            clusterName = arg
        elif opt in ('-O',GetLongOption('-O')):
            useOpportunisticNodes = int(arg)
        elif opt in ('-q',GetLongOption('-q')):
            useQueueName = arg
        elif opt in ('-N',GetLongOption('-N')):
            useHostName = arg
        else:
            print 'ERROR: Parameter not known: '+opt
            print helpstatement
            sys.exit()
    #end loop over options

    localFileList = GetListofFiles(inputList)
    global fileList
    for aFile in localFileList:
        fileList[aFile] = 0
    otherRequirementsList = []

    if len(inputList) == 0 or len(fileList) == 0:
        print 'ERROR: Invalid file list! Check the file name or contents.'
        print helpstatement
        sys.exit()
    if len(outputName) == 0:
        print 'ERROR: No input name is present!'
        print helpstatement
        sys.exit()
    if walltimeMinutes == -1 and walltimeHours == -1:
        print "Please insert the time length of each job"
        print helpstatement
        sys.exit()
    if numJobs == -1 and numFilesPerJob == -1:
        print 'ERROR: The number of jobs or files per job must be stated'
        print helpstatement
        sys.exit()
    if len(outputPath) == 0:
        print 'WARNING: Output path is the CWD.'
        outputPath = '.'
    if len(clusterName) == 0:
        print 'WARNING: No cluster name specified! Using csuhpc as default'
        clusterName = 'csuhpc'
    if clusterName not in ('csuhpc','csuhep'):
        print 'ERROR: Cluster name not supported!'
        print '    you put in '+clusterName
        print helpstatement
        sys.exit()
    if priority not in (-11,-10,-9):
	print 'ERROR: Priority only accepts -11, -10, and -9'
        sys.exit()
    else:
      if clusterName in ('csuhep'):
        print 'Using CSU HEP torque PBS as batch queue'
        csuhpc = 0
        #csuhep = 1
      elif clusterName in ('csuhpc'):
        print 'Using CSU HPC Grid Engine as batch queue'
        csuhpc = 1
        #csuhep = 0
      elif int(os.popen('man qstat | grep "UGE" | wc -l').read()) > 0:
        print 'Using CSU HPC Grid Engine as batch queue'
        csuhpc = 1
        #csuhep = 0
        queueTag = '$'
      elif int(os.popen('man qstat | grep "PBS" | wc -l').read()) > 0:
        print 'Using CSU HEP torque PBS as batch queue'
        #csuhep = 1
        csuhpc = 0
        queueTag = 'PBS'
      else:
        print 'ERROR: Cluster not supported!'
        print '    you put in '+clusterName
        print helpstatement
        sys.exit()
    if len(useQueueName) == 0 and len(useHostName) == 0:
	print 'Assuming the parameter qname=\"physics.q|short.q\"'
	useQueueName = 'physics.q|short.q'
    if len(useHostName) > 1:
        if len(useQueueName) > 1:
            print 'WARNING: Be careful about specifying the hostname and qname together'
	useOpportunisticNodes = 0
    if len(otherRequirements) > 0:
        otherRequirementsList = otherRequirements.split('#')
    nFiles = len(fileList)
    if numJobs > 0 and numFilesPerJob == -1:
        numFilesPerJob = math.ceil(float(nFiles) / numJobs)
        numJobs = int(math.ceil(float(nFiles) / numFilesPerJob))
    if numFilesPerJob > 0 and numJobs == -1:
        numJobs = int(math.ceil(float(nFiles) / numFilesPerJob))
    InitNodes()
    #make directory for new jobs
    currentTime = datetime.datetime.now()
    submissionDirName = 'qsubmitter_%s_%d%s%s_%s%s'%(outputName,currentTime.year,str(currentTime.month).zfill(2),str(currentTime.day).zfill(2),str(currentTime.hour).zfill(2),str(currentTime.minute).zfill(2))
    ExportedPathsName = 'ExportedPaths.sh'
    os.mkdir(submissionDirName)
    os.system('chmod +rw %s'%(submissionDirName))
    os.chdir(submissionDirName)
    CreateExportedPathsScript(ExportedPathsName)
    os.system('chmod +x %s'%(ExportedPathsName))
    print useHostName
    global totalTimeInHours
    walltimeMinutes = walltimeMinutes if walltimeMinutes >= 0 else 0
    walltimeHours = walltimeHours if walltimeHours >= 0 else 0
    totalTimeInHours = float(walltimeHours + 1./60. * walltimeMinutes)
    MakeJobs(runNumber,outputPath,outputName,numJobs,numFilesPerJob,priority,walltimeHours,walltimeMinutes,maxMemory,softRequirements,hardRequirements,useOpportunisticNodes,useQueueName,useHostName,otherRequirementsList,emailAddress,ExportedPathsName)
    MakeHaddScript(outputPath,outputName,ExportedPathsName,numJobs,"submit_ajob")
    os.chdir('../')

if __name__ == "__main__":
    main(sys.argv[1:])
