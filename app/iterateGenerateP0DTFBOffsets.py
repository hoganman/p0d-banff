#!/usr/bin/env python2
"""
   This script iterates the process of genP0DTFBOffsets.exe
   Usage: iterateGenerateP0DTFBOffsets.py [options]

   Options:
     -h, --help            show this help message and exit
     -n NITERATIONS, --nIterations=NITERATIONS
                           The number >=0 of genP0DTFBOffsets.exe iterations
                           [default=5]
     -l FILELIST, --fileList=FILELIST
                           The list of P0D sand muon oaEvent files
     -o OUPUT, --ouput=OUPUT
                           The name of the output ROOT file with "_iterN" added
                           after each iteration
"""

import optparse
from os.path import isfile
import sys


def add_options():
    """Adds all the options"""
    parser = optparse.OptionParser()
    parser.add_option('-n', '--nIterations', default='5',
                      help='The number >=0 of genP0DTFBOffsets.exe iterations \
[default=5]')
    parser.add_option('-l', '--fileList',
                      help='The list of P0D sand muon oaEvent files')
    parser.add_option('-o', '--ouput',
                      help='The name of the output ROOT file with \"_iterN\" \
added after each iteration')
    return parser


def run(command):
    """run the command by printing it as well"""
    print command
    # os.system(command)


def main(argv):

    argv  # prevent non-usage errors in flake8/pep/etc
    parser = add_options()
    nIterations, fileList, output = get_options(parser)
    output_prefix = output[0:output.find('.root')]
    program = 'genP0DTFBOffsets.exe'

    inputFile = open(fileList, 'r')

    # for each P0D sand muon file, iterate the process (nIterations+1) times
    for p0dSandMuFile in iter(inputFile.readline, ''):
        for iteration in range(0, nIterations+1):
            this_iter_output = '%s_iter%d.root' % (output_prefix, iteration)
            command = '%s -O iter=%d' % (program, iteration)
            command += '-o %s %s' % (this_iter_output, p0dSandMuFile)
            run(command)


def get_options(parser):
    """check the options and make sure they are sensible"""
    options, args = parser.parse_args()
    if not options.fileList or not options.output:
        usage(parser, 0)
    fileList = options.fileList
    output = options.output
    if output.find('.root') == -1:
        print 'WARNING: output did NOT contain .root extension. Appending...'
        output += '.root'
    try:
        nIterations = int(options.nIterations)
        if nIterations < 0:
            usage(parser, 0)
    except ValueError:
        print 'Unable to convet', options.nIterations, ' to an integer'
        usage(parser, 1)

    if not isfile(fileList):
        print 'ERROR: Input file does not exist!'
        usage(parser, 0)

    testOpen = None
    try:
        testOpen = open(fileList, 'r')
        testOpen.close()
    except OSError:
        print 'ERROR: Unable to open file', fileList
        usage(parser, 0)

    return nIterations, fileList, output


def usage(parser, exit_status):
    """"""
    parser.print_help()
    sys.exit(exit_status)


if __name__ == "__main__":
    main(sys.argv[1:])
