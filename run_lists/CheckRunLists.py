#!/usr/bin/env python
"""test if the file lists have existant files"""

import os
from ROOT import gSystem

from TextFile import ReadTextFile
from Directory import Directory
from ROOTFile import ROOTFile


P0DBANFF = os.getenv('P0DBANFFROOT')
RUNLISTS = P0DBANFF + '/run_lists'
RDP6M = 'rdp6_Spin_M'
MCP6B = 'mcp6_Spin_B/neut'
MCP6BANTI = 'mcp6_Spin_B/anti-neut'

gSystem.Load('%s/liboaAnalysisReader.so' % (
    os.path.join(P0DBANFF, 'T2KReWeight/libReadoaAnalysis')))

DIRECTORIES = ['%s/%s' % (RUNLISTS, RDP6M), '%s/%s' % (RUNLISTS, MCP6B)]
# DIRECTORIES = ['%s/%s' % (RUNLISTS, RDP6M)]
# DIRECTORIES = ['%s/%s' % (RUNLISTS, MCP6B)]


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
    """doc string"""
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
