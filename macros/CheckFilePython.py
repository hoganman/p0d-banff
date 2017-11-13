#!/usr/bin/env python
""" Check a ROOT file if it has any bad qualities 
    CheckFile(inFileName) will return an int
        return 0 (False) if it is bad
        return 1 (True) if it is good
    CheckFileAndExit(fileName) will exit with the same
        return status as ChechFile(inFileName)

"""

from sys import exit
from os.path import isfile
from ROOT import TFile


def CheckFile(inFileName = ''):
    """Checks if the file is bad (0) or good (1)"""
    if not isfile(inFileName):
        return 0
    inFile = TFile.Open(inFileName)
    if not inFile or not inFile.IsOpen():
        return 0
    elif inFile.IsRaw():
        inFile.Close()
        return 0
    elif not inFile.IsBinary():
        inFile.Close()
        return 0
    elif not inFile.GetListOfKeys().GetSize():
        inFile.Close()
        return 0
    elif inFile.IsZombie():
        inFile.Close()
        return 0
    elif inFile.TestBit(TFile.kRecovered):
        inFile.Close()
        return 0
    else:
        inFile.Close()
        return 1


def CheckFileAndExit(inFileName):
    """Exits with 0 if bad ROOT file or 1 if it is good"""
    status = CheckFile(inFileName)
    exit(status)
