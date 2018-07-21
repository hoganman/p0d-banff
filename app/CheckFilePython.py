#!/usr/bin/env python
""" Check a ROOT file if it has any bad qualities
    checkfile(infilename) will return an int
        return 0 (False) if it is bad
        return 1 (True) if it is good
    checkfileandexit(fileName) will exit with the same
        return status as ChechFile(infilename)

"""

from sys import exit
from ROOTFile import ROOTFile


def checkfile(infilename=''):
    """Checks if the file is bad (0) or good (1)"""
    test_file = ROOTFile(infilename)
    return test_file.valid()


def checkfileandexit(infilename):
    """Exits with 0 if bad ROOT file or 1 if it is good"""
    status = checkfile(infilename)
    exit(status)
