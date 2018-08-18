#!/usr/bin/env python2

""""""

import optparse
import os
import sys


def usage(parser, exit_status=0):
    """Prints the help message"""
    parser.print_help()
    sys.exit(exit_status)


def add_options():
    """Adds all the options"""
    parser = optparse.OptionParser()
    parser.add_option('-f', '--first', dest='first', type='int',
                      help='The first run number in the series')
    parser.add_option('-l', '--last', dest='last', type='int',
                      help='The last run number in series. \
If not given, assumes first+999')
    parser.add_option('-d', '--directory', type='string', dest='directory',
                      help='The directory of the input ROOT files')
    parser.add_option('-o', '--output', type='string', dest='output',
                      help='The name of the output ROOT files')
    return parser


def run(command):
    """run the command by printing it as well"""
    print command
    # os.system(command)


def main(argv):

    argv  # prevent non-usage errors in flake8/pep/etc
    parser = add_options()
    options = get_options(parser)
    program = 'hadd'

    for run_num in range(options.first, options.second+1):
        hadd_filename = '%s_%d_hadd.root' % (options.output, run_num)
        hadd_filename = os.path.join(options.directory, hadd_filename)
        command = '%s %s %s/*%d*root' % (program, hadd_filename,
                                         options.directory, run_num)
        run(command)


def get_options(parser):
    """check the options and make sure they are sensible"""
    options, args = parser.parse_args()
    if not options.first or not options.output:
        usage(parser)
    if options.first < 0:
        usage(parser)
    if not options.last or options.last < 0:
        options.last = options.first + 999
    return options


if __name__ == "__main__":
    main(sys.argv[1:])
