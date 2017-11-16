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

gSystem.Load('%s/liboaAnalysisReader.so' % (
    os.path.join(P0DBANFF, 'T2KReWeight/libReadoaAnalysis')))

# DIRECTORIES = ['%s/%s' % (RUNLISTS, RDP6M), '%s/%s' % (RUNLISTS, MCP6B)]
DIRECTORIES = ['%s/%s' % (RUNLISTS, MCP6B)]

for directory in DIRECTORIES:
    test_dir = Directory(directory)
    if not test_dir.exists():
        print 'The directory \"%s\" does not exist!' % (directory)
        continue
    for run_list_name in os.listdir(directory):
        run_list_file = ReadTextFile(os.path.join(directory, run_list_name))
        print 'On file %s' % (run_list_file.get_file_name())
        if not run_list_file.exists():
            print 'Cannot open \"%s\", it does not exist' % (
                run_list_file.get_file_name())
            continue
        file_list = run_list_file.get_file_as_list()
        for analysis_file in file_list:
            test_file = ROOTFile(analysis_file)
            if not (test_file.contains(RDP6M) or test_file.contains(MCP6B)):
                print 'ROOT file \"%s\" does not contain right \
file path in %s' % (analysis_file, run_list_name)
            elif test_file.checkfile() is not ROOTFile.IS_GOOD:
                print 'ROOT file \"%s\" is BAD in %s' % \
                        (analysis_file, run_list_name)
            else:
                continue
