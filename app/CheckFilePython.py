#!/usr/bin/env python
""" Check a ROOT file if it has any bad qualities
    checkfile(infilename) will return an int
        return 0 (False) if it is bad
        return 1 (True) if it is good
    checkfileandexit(fileName) will exit with the same
        return status as ChechFile(infilename)

"""

from sys import exit, argv
from ROOTFile import ROOTFile


def Usage():
    print 'Usage:'
    print 'CheckFilePython path/to/input/file.root'


def checkfile(infilename=''):
    """Checks if the file is bad (0) or good (1)"""
    test_file = ROOTFile(infilename)
    return test_file.valid()


def checkfileandexit(infilename):
    """Exits with 0 if bad ROOT file or 1 if it is good"""
    status = checkfile(infilename)
    if status == ROOTFile.IS_GOOD:
        print 'The file "%s" is good!' % (infilename)
    else:
        print 'The file "%s" is bad!' % (infilename)
    exit(status)


def main(argv):
    if len(argv) == 1:
        checkfileandexit(argv[0])
    else:
        Usage()


if __name__ == '__main__':
    main(argv[1:])
