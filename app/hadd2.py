#!/usr/bin/env python2
"""
Usage: iterateGenerateP0DTFBOffsets.py [options] output input1 input2 input3 ...

Options:
  -h, --help            show this help message and exit
  -f , --force          Force rewrite of output if it exists and GOOD

AUTHOR: Matthew Hogan
"""

import optparse
from os import system
from ROOTFile import ROOTFile
from ROOT import TFileMerger
import sys


def add_options():
    """Adds all the options"""
    usage = "hadd2 [-f] targetfile source1 [source2 source3 ...]"
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-f', '--force', action='store_true', dest='force',
                      default=False,
                      help='Force rewrite of output if it exists and GOOD')
    return parser


def run(command):
    """run the command by printing it as well"""
    print command
    system(command)


def main(argv):

    parser = add_options()
    if len(argv) < 2:
        usage(parser, 1)
    options = get_options(parser)
    outputFileName = argv[0]
    del argv[0]
    inputFilesList = list(argv)
    force = options.force
    print outputFileName
    print 'force = ', force
    print inputFilesList

    merger = TFileMerger(False)
    outputFile = ROOTFile(outputFileName)
    if outputFile.valid and not force:
        print 'Output file \"%s\" is a valid file, must set force override to proceed' % (outputFileName)
        sys.exit(1)
    run('rm -f %s' % (outputFileName))
    del outputFile

    proceedWithMerge = True
    for aFileName in inputFilesList:
        testFile = ROOTFile(aFileName)
        if not testFile.valid:
            proceedWithMerge = False
            print 'File \"%s\" is NOT valid, please check your inputs!'

    if not proceedWithMerge:
        sys.exit(1)
    createFileSuccess = bool(merger.OutputFile(outputFileName))
    if not createFileSuccess:
        print 'Unable to create output file \"%s\"' % (outputFileName)
        sys.exit(1)

    for aFileName in inputFilesList:
        print 'Adding file \"%s\"' % (aFileName)
        merger.AddFile(aFileName)

    mergeFileSuccess = bool(merger.Merge())
    if not mergeFileSuccess:
        print 'The program TFileMerger::Merge was unsuccessful. Unable to successfully write \"%s\"' % (outputFileName)
        sys.exit(1)
    sys.exit(0)


def get_options(parser):
    """check the options and make sure they are sensible"""
    options, args = parser.parse_args()
    return options


def usage(parser, exit_status):
    """"""
    parser.print_help()
    sys.exit(exit_status)


if __name__ == "__main__":
    main(sys.argv[1:])
