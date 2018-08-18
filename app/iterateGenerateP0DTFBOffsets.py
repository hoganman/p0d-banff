#!/usr/bin/env python2
"""
   This script iterates the process of genP0DTFBOffsets.exe
   Uses multiprocessing to parallelize the steps
   Usage: iterateGenerateP0DTFBOffsets.py [options]

Usage: iterateGenerateP0DTFBOffsets.py [options]

Options:
  -h, --help            show this help message and exit
  -N NITERATIONS, --nIterations=NITERATIONS
                        The number >=0 of genP0DTFBOffsets.exe iterations
                        [default=5]
  -l FILELIST, --fileList=FILELIST
                        The list of P0D sand muon oaEvent files
  -o OUTPUT, --output=OUTPUT
                        The name of the output ROOT file with "_iterN" added
                        after each iteration
  -n THREADS, --threads=THREADS
                        The number of threads [default=5]

AUTHOR: Matthew Hogan
"""

import multiprocessing as mp
import optparse
from os import system
from os.path import isfile
import sys


def add_options():
    """Adds all the options"""
    parser = optparse.OptionParser()
    parser.add_option('-N', '--nIterations', type='int', dest='nIterations',
                      default=5,
                      help='The number >=0 of genP0DTFBOffsets.exe iterations \
[default=5]')
    parser.add_option('-l', '--fileList', type='string', dest='fileList',
                      help='The list of P0D sand muon oaEvent files')
    parser.add_option('-o', '--output', type='string', dest='output',
                      help='The name of the output ROOT file with \"_iterN\" \
added after each iteration')
    parser.add_option('-n', '--threads', type='int', dest='threads',
                      default=5, help='The number of threads [default=5]')
    return parser


def run(command):
    """run the command by printing it as well"""
    print command
    system(command)


def iterate_time_offset_calculations(program, nIterations, output_prefix,
                                     p0dSandMuFile_name):
    """
    For each P0D sand muon file, iterate the process (nIterations+1) times
    """
    for iteration in range(nIterations+1):
        this_iter_output = '%s_%s' % (output_prefix, p0dSandMuFile_name)
        this_iter_output += '_iter%d.root' % (iteration)
        command = '%s -O iter=%d' % (program, iteration)
        command += ' -o %s %s.root' % (this_iter_output, p0dSandMuFile_name)
        run(command)


def main(argv):

    argv  # prevent non-usage errors in flake8/pep/etc
    parser = add_options()
    options = get_options(parser)
    nIterations = options.nIterations
    fileList = options.fileList
    output = options.output

    output_prefix = output[:output.index('.root')]
    program = 'genP0DTFBOffsets.exe'

    inputFile = open(fileList, 'r')
    processes = []

    for p0dSandMuFile in iter(inputFile.readline, ''):
        # Remove the full path and .root extension
        startIndex = p0dSandMuFile.rfind('/')+1
        endIndex = p0dSandMuFile.rfind('.')
        p0dSandMuFile_name = p0dSandMuFile[startIndex:endIndex]
        mp_target = iterate_time_offset_calculations
        mp_args = (program, nIterations, output_prefix, p0dSandMuFile_name)
        mp_process = mp.Process(target=mp_target, args=mp_args)
        processes.append(mp_process)

    inputFile.close()

    num_running_threads = 0
    running_threads = []
    # Run processes
    for proc in processes:
        proc.start()
        running_threads.append(proc)
        num_running_threads += 1
        # Complete the first completed process
        if num_running_threads > options.threads-1:
            rp = running_threads[0]
            rp.join()
            num_running_threads -= 1
            # print num_running_threads
            del running_threads[0]
    # finish any threads not already finished
    for rp in running_threads:
        rp.join()



def get_options(parser):
    """check the options and make sure they are sensible"""
    options, args = parser.parse_args()
    if not options.fileList or not options.output:
        usage(parser, 0)
    if options.output.find('.root') == -1:
        print 'WARNING: output did NOT contain .root extension. Appending...'
        options.output += '.root'

    if options.nIterations < 0:
        print 'ERROR: The number of iterations is less than zero!'
        usage(parser, 0)

    if not isfile(options.fileList):
        print 'ERROR: Input file does not exist!'
        usage(parser, 0)

    testOpen = None
    try:
        testOpen = open(options.fileList, 'r')
        testOpen.close()
    except OSError:
        print 'ERROR: Unable to open file', options.fileList
        usage(parser, 0)

    return options


def usage(parser, exit_status):
    """"""
    parser.print_help()
    sys.exit(exit_status)


if __name__ == "__main__":
    main(sys.argv[1:])
