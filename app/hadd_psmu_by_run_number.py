#!/usr/bin/env python2

"""
This program runs the hadd macro on P0D sand muon files
and merges all the sub runs in the same run number.
It uses multiprocessing so the hadd process is completed
faster. If you are NOT using a RAID setup, set the number
of threads to 1.

In principle, this problem can be used to merge
sub-runs in the same run into one file

Usage: ./hadd_psumu_by_run_number.py [OPTIONS]

Options:
  -h, --help            show this help message and exit
  -f FIRST, --first=FIRST
                        The first run number in the series
  -l LAST, --last=LAST  The last run number in series. If not given, assumes
                        first+999
  -d DIRECTORY, --directory=DIRECTORY
                        The directory of the input ROOT files
  -o OUTPUT, --output=OUTPUT
                        The name of the output ROOT files
  -n THREADS, --threads=THREADS
                        The number of threads (default=5)

Author: Matthew Hogan
"""

from glob import glob
import multiprocessing as mp
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
    parser.add_option('-n', '--threads', type='int', dest='threads',
                      default=5, help='The number of threads (default=5)')
    return parser


def run(command):
    """run the command by printing it as well"""
    print command
    os.system(command)


def create_file(program, input_files, output_file):
    """Runs the hadd program to create a file"""
    command = '%s %s %s' % (program, output_file, input_files)
    run(command)


def main(argv):

    argv  # prevent non-usage errors in flake8/pep/etc
    parser = add_options()
    options = get_options(parser)
    program = 'hadd'
    processes = []

    for run_num in range(options.first, options.last+1):
        input_files = '%s/*%d*root' % (options.directory, run_num)
        if len(glob(input_files)) == 0:
            continue
        hadd_filename = '%s_%d_hadd.root' % (options.output, run_num)
        hadd_filename = os.path.join(options.directory, hadd_filename)
        processes.append(mp.Process(target=create_file,
                                    args=(program, input_files, hadd_filename)))
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
    if not options.first or not options.output:
        usage(parser)
    if options.first < 0:
        print 'ERROR: There first number must be > 0'
        usage(parser)
    if not options.last or options.last < 0:
        options.last = options.first + 999
    if options.threads < 0:
        print 'ERROR: Number of threads must be > 0'
        usage(parser)
    return options


if __name__ == "__main__":
    main(sys.argv[1:])
