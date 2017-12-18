#!/usr/bin/env python
""" This macro reads the inputs of run_periods.txt and tests if all the
inputs exist. A warning message is printed if the sand file is
missing"""

from sys import argv
from CheckFilePython import checkfile
from TextFile import ReadTextFile
from ROOT import gSystem
gSystem.Load('libP0DBANFF')

DATAINDEX = 0
MCMAGNETINDEX = 1
MCSANDINDEX = 2


def readrunperiods(filename='run_periods.txt'):
    """Takes the run_periods file and parses each line
       for the indiv. run periods and returns dictionary
       of file lists (MC and data)
    """

    inputfile = ReadTextFile(filename)
    inputfile.open()
    inputfilelist = inputfile.get_file_as_list()
    # inputfile.close()

    run_periods = {}
    for line in inputfilelist:
        if len(line) <= 1:
            continue
        options = line.split(' ')
        noptions = len(options)
        runkey = options[noptions - 1]

        run_periods[runkey] = ['', '', '']
        if noptions == 3:
            del options[noptions - 1]
            noptions = noptions - 1
        else:
            print "ERROR: input for run period does not match search criteria"
        if noptions != 2:
            print "ERROR: wrong number of files"
        # print runkey
        # print options
        while noptions > 0:
            testfile = options[0]
            if len(testfile) == 0:
                del options[0]
                noptions = noptions - 1
                continue
            # print testfile
            if testfile.find('data') == -1 and testfile.find('mc') == -1:
                print "ERROR: no data or mc descriptor" + testfile
            elif testfile.find('data') != -1:
                datafile = open(testfile).read().split('\n')[0]
                run_periods[runkey][DATAINDEX] = datafile
            elif testfile.find('mc') != -1:
                mcfiles = parsemcfile(testfile)
                sandfile = mcfiles[MCSANDINDEX]
                magnetfile = mcfiles[MCMAGNETINDEX]
                run_periods[runkey][MCMAGNETINDEX] = magnetfile
                run_periods[runkey][MCSANDINDEX] = sandfile
            else:
                print "ERROR: could not determine if mc or data"
            del options[0]
            noptions = noptions - 1
    return run_periods


def parsemcfile(filename):
    """Reads a mc file and returns a list of the sand and magnet files"""
    run_filelist = ['DUMMY', '', '']

    inputfile = ReadTextFile(filename)
    inputfile.open()
    mcfiles = inputfile.get_file_as_list()
    # inputfile.close()

    for line in mcfiles:
        lineoptions = line.split(' ')
        if len(line) == 0:
            continue
        if line.find('magnet') == -1 and line.find('sand') == -1:
            print "ERROR: no magnet or sand descriptor " + line
        elif line.find('magnet') != -1:
            run_filelist[MCMAGNETINDEX] = lineoptions[1]
        elif line.find('sand') != -1:
            run_filelist[MCSANDINDEX] = lineoptions[1]
        else:
            print "ERROR: could not determine if magnet or sand"
    return run_filelist


def main(argv):
    """Runs the main program"""
    run_periods = readrunperiods()
    for run_period, file_list in sorted(run_periods.iteritems()):
        print "Run period: " + str(run_period)
        spaces = '            '
        header = spaces + '--------'
        if len(file_list[DATAINDEX]) == 0:
            print header
            print spaces+"ERROR: %s's data file is NOT listed" % (run_period)
            print header
        filestatus = checkfile(file_list[DATAINDEX])
        if filestatus == 0:
            print header
            print spaces+"ERROR: %s's data file is BAD!" % (run_period)
            print header
        if len(file_list[MCMAGNETINDEX]) == 0:
            print header
            print spaces+"ERROR: %s's magnet file is NOT listed" % (run_period)
            print header
        filestatus = checkfile(file_list[MCMAGNETINDEX])
        if filestatus == 0:
            print header
            print spaces+"ERROR: %s's MC magnet file is BAD!" % (run_period)
            print header

        if len(file_list[MCSANDINDEX]) == 0:
            print spaces+"warning: %s's sand file is NOT listed" % (run_period)
        else:
            filestatus = checkfile(file_list[MCSANDINDEX])
            if filestatus == 0:
                print header
                print spaces+"ERROR: %s's MC sand file is BAD!" % (run_period)
                print header


if __name__ == "__main__":
    main(argv[1:])
