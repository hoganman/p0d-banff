#!/usr/bin/env python
"""A simple python tool to resubmit jobs after qsubmitter.py has been run
    Inputs
        1: submission directory (manadatory)
        2: first job number (assumes 1)
        3: last job number (assumes 1000)
        4: submission script name with %d in it (assumes 'submit_ajob_%d.sh')
        5: sleep time in seconds (assumes 20)

    Simple example: 
        python SubmitJobsWithSleep /path/to/jobs/dir
    Detailed example with 10 sec wait time b/n submits:
        python SubmitJobsWithSleep /path/to/jobs/dir 10 1 1000 submit_ajob_%d.sh 
"""
from os import system, popen
from time import sleep
from sys import argv, exit


def printusage(programname='SubmitJobsWithSleep.py'):
    """if the number of args is < 1, then print this
        or if any invalid input 
    """
    print programname
    print "input 1: submission directory (manadatory)"
    print "input 2: sleep time in seconds (assumes 20)"
    print "input 3: first job number (assumes 1)"
    print "input 4: last job number (assumes 1000)"
    print "input 5: submission script name with %d in it \
(assumes 'submit_ajob_%d.sh')"
    exit(1)


def main(argv):
    """resubmit jobs"""

    if popen('hostname').read().find('ens-hpc') == -1:
        print "ERROR: Please change hosts to ens-hpc"
        printusage()

    nuserargs = len(argv)-1
    if nuserargs > 0:
        try:
            jobscriptsdir = str(argv[1])
        except ValueError:
            print "No submission directory given"
            printusage()
    else:
        printusage()

    if nuserargs > 1:
        try:
            sleeptime = int(argv[2])
        except ValueError:
            print "ERROR: please input an integer for sleep time (sec)"
            printusage()
    else:
        sleeptime = 20

    if nuserargs > 2:
        try:
            firstjob = int(argv[3])
        except ValueError:
            print "ERROR: please input an integer for first job number"
            printusage()
    else:
        firstjob = 1

    if nuserargs > 3:
        try:
            lastjob = int(argv[4])
        except ValueError:
            print "ERROR: please input an integer for last job number"
            printusage()
    else:
        lastjob = 1000


    if nuserargs > 4:
        try:
            submissionscript = str(argv[5])
        except ValueError:
            printusage()
    else:
        submissionscript = 'submit_ajob_%d.sh'

    if nuserargs > 5:
        print 'Too many inputs'
        printusage()
    
    print 'Submiting jobs now'
    for job in range(firstjob,lastjob+1):
        jobscript = jobscriptsdir + '/' + submissionscript % (job)
        system('qsub %s' % (jobscript))
        sleep(sleeptime)
    print 'DONE submitting jobs'


if __name__ == "__main__":
    main(argv)
