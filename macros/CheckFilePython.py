#!/usr/bin/env python
""" Check a ROOT file if it has any bad qualities
    checkfile(infilename) will return an int
        return 0 (False) if it is bad
        return 1 (True) if it is good
    checkfileandexit(fileName) will exit with the same
        return status as ChechFile(infilename)

"""

from sys import exit
from os.path import isfile
from ROOT import TFile


def checkfile(infilename=''):
    """Checks if the file is bad (0) or good (1)"""
    status = 0
    if not isfile(infilename):
        status = 0
    else:
        infile = TFile.Open(infilename)
        if not infile or not infile.IsOpen():
            status = 0
        elif infile.IsRaw():
            status = 0
        elif not infile.IsBinary():
            status = 0
        elif not infile.GetListOfKeys().GetSize():
            status = 0
        elif infile.IsZombie():
            status = 0
        elif infile.TestBit(TFile.kRecovered):
            status = 0
        else:
            status = 1
        if infile:
            infile.Close()
    return status


def checkfileandexit(infilename):
    """Exits with 0 if bad ROOT file or 1 if it is good"""
    status = checkfile(infilename)
    exit(status)
