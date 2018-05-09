#!/usr/bin/env python3
"""

   This script iterates the process of genP0DTFBOffsets.exe

"""

import optparse
from os.path import isfile
import sys


def add_options(parser):
    """Adds all the options"""
    parser.add_options('-n', '--nIterations', default='5',
                       help='The number >=0 of genP0DTFBOffsets.exe iterations [default=5]')
    parser.add_options('-l', '--fileList',
                       help='The list of P0D sand muon oaEvent files')
    parser.add_options('-o', '--ouput',
                       help='The name of the output ROOT file with \"_iterN\" added after each iteration')


def main(argv):
    print('iterateGenerateP0DTFBOffsets.py [ OPTIONS ]')

    parser = optparse.OptionParser()
    add_options(parser)
    nIterations, fileList, output = get_options(parser)

    program = 'genP0DTFBOffsets.exe'
    for iteration in range(0, nIterations):
        command = '%s -O iter=%d -o' % (program, iteration)
        print(command)


def get_options(parser):
    """check the options and make sure they are sensible"""
    options, args = parser.parse_args()
    fileList = options.fileList
    output = options.output
    try:
        nIterations = int(options.nIterations)
        if nIterations < 0:
            usage(parser, 0)
        if nIterations == 0:
            nIterations = 1
    except ValueError:
        print('Unable to convet', options.nIterations, ' to an integer')
        usage(parser, 1)

    if not isfile(fileList):
        print('ERROR: Input file does not exist!')
        usage(parser, 0)
        return 0

    return nIterations, fileList, output


def usage(parser, exit_status):
    """"""
    parser.print_help()
    sys.exit(exit_status)


if __name__ is '__main__':
    main(sys.argv[1:])
