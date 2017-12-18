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
MCP6LANTI = 'mcp6_Spin_L/anti-neut'

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
                test_file.contains(MCP6BANTI) or test_file.contains(MCP6LANTI)):
            print 'ROOT file \"%s\" does not contain right \
file path in %s' % (analysis_file, file_list_name)
        elif test_file.checkfile() is not ROOTFile.IS_GOOD:
            print 'ROOT file \"%s\" is BAD in %s' % \
                    (analysis_file, file_list_name)
        else:
            continue


def check_directory_run_lists(directory):
    """Given the directory with the run lists, loop over run lists"""
    test_dir = Directory(directory)
    if not test_dir.exists():
        print 'The directory \"%s\" does not exist!' % (directory)
        return
    for run_list_name in sorted(os.listdir(directory)):
        check_run_list_file(os.path.join(directory, run_list_name))
        # run_list_file = ReadTextFile(os.path.join(directory, run_list_name))
        # print 'On file %s' % (run_list_file.get_file_name())
        # if not run_list_file.exists():
        #     print 'Cannot open \"%s\", it does not exist' % (
        #         run_list_file.get_file_name())
        #     continue
        # file_list = run_list_file.get_file_as_list()
        # check_oaanalysis_files(file_list, run_list_name)


def check_run_list_file(run_list_filename):
    """given a run list file, check oaanalysis contents"""
    run_list_file = ReadTextFile(run_list_filename)
    print 'On file %s' % (run_list_file.get_file_name())
    if not run_list_file.exists():
        print 'Cannot open \"%s\", it does not exist' % (
            run_list_file.get_file_name())
        return
    file_list = run_list_file.get_file_as_list()
    check_oaanalysis_files(file_list, run_list_filename)


def main(argv):
    """Check both P6 MCP and RDP unless a file/directory is given"""
    if len(argv) == 1:
        check_directory_run_lists(os.path.join(RUNLISTS, MCP6B)) # runs 2-4
        check_directory_run_lists(os.path.join(RUNLISTS, MCP6BANTI)) # runs 5-6
        check_directory_run_lists(os.path.join(RUNLISTS, MCP6LANTI)) # run 7
        check_directory_run_lists(os.path.join(RUNLISTS, RDP6M)) # runs 2-7
    if len(argv) == 2:
        if ReadTextFile(argv[1]).exists():
            check_run_list_file(argv[1])
        elif Directory(argv[1]).exists():
            check_directory_run_lists(argv[1])
        else:
            print 'ERROR: file or directory %s does NOT exist' % (argv[1])


if __name__ == '__main__':
    main(sys.argv)
