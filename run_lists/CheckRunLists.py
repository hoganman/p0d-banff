#!/usr/bin/env python
"""Check both P6 MCP and RDP run list contents"""

import os
import sys
from ROOT import gSystem

from TextFile import ReadTextFile
from Directory import Directory
from ROOTFile import ROOTFile

P0DBANFF = os.getenv('P0DBANFFROOT')
RUNLISTS = P0DBANFF + '/run_lists'
RDP6M = 'rdp6_Spin_M'
MCP6B = 'mcp6_Spin_B/neut'
MCP6BANTI = 'mcp6_Spin_B/anti-neut'

# this library helps ROOT not complain about missing dictionaries
gSystem.Load('%s/liboaAnalysisReader.so' % (
    os.path.join(P0DBANFF, 'T2KReWeight/libReadoaAnalysis')))


def check_oaanalysis_files(file_list, file_list_name):
    """loop over run list file and check contents"""
    if not isinstance(file_list, list):
        file_list = ReadTextFile(file_list).get_file_as_list()
    for analysis_file in file_list:
        test_file = ROOTFile(analysis_file)
        if not (test_file.contains(RDP6M) or test_file.contains(MCP6B) or
                test_file.contains(MCP6BANTI)):
            print 'ROOT file \"%s\" does not contain right \
file path in %s' % (analysis_file, file_list_name)
        elif test_file.checkfile() is not ROOTFile.IS_GOOD:
            print 'ROOT file \"%s\" is BAD in %s' % \
                    (analysis_file, file_list_name)
        else:
            continue


def check_directory_run_lists(directory):
    """Given the directory with the run lists, check oaanalysis contents"""
    test_dir = Directory(directory)
    if not test_dir.exists():
        print 'The directory \"%s\" does not exist!' % (directory)
        return
    for run_list_name in sorted(os.listdir(directory)):
        run_list_file = ReadTextFile(os.path.join(directory, run_list_name))
        print 'On file %s' % (run_list_file.get_file_name())
        if not run_list_file.exists():
            print 'Cannot open \"%s\", it does not exist' % (
                run_list_file.get_file_name())
            continue
        file_list = run_list_file.get_file_as_list()
        check_oaanalysis_files(file_list, run_list_name)


def main(argv):
    """Check both P6 MCP and RDP"""
    check_directory_run_lists(os.path.join(RUNLISTS, MCP6B))
    check_directory_run_lists(os.path.join(RUNLISTS, RDP6M))


if __name__ == '__main__':
    main(sys.argv)
