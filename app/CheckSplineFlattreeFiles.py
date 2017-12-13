#!/usr/bin/env python
"""Checks if the POT from header tree (pre- and post-T2KRW file)
    matches the official POT and hardcoded cut-off in ND280AnalysisUtils.cxx
"""

import sys
import os
from os.path import join
import ROOT

from Directory import Directory

P0DBANFF = os.getenv('P0DBANFFROOT')
MCP6B = 'mcp6_Spin_B/neut'


def check_directory_for_ROOT_files(directory, file_name):
    """Given the directory with the file, return ROOT.TChain
       with the names "file_name.root" or "file_name_XXX.root" """
    test_dir = Directory(directory)
    if not test_dir.exists():
        print 'The directory \"%s\" does not exist!' % (directory)
        return
    chain = ROOT.TChain('header')
    file_list = sorted(os.listdir(directory))
    if join(directory, '%s.root' % (file_name)) in file_list:
        chain.Add(join(directory, '%s.root' % (file_name)))
    elif join(directory, '%s_1.root' % (file_name)) in file_list:
        index = 1
        root_file = join(directory, '%s_%d.root' % (file_name, index))
        while root_file in file_list:
            chain.Add(root_file)
            index += 1
            root_file = join(directory, '%s_%d.root' % (file_name, index))
    else:
        print 'No matching files \"%s\" found in %s' % (file_name, directory)
    return chain


def main(argv):
    """main function"""
    print 'Running %s' % (argv[0])
    if len(argv) != 2:
        print 'ERROR: The second argument must be the directory'
    if len(argv) != 3:
        print 'ERROR: The third argument must be the file name'
        return
    ROOT.gSystem.Load(os.path.join(P0DBANFF, 'lib/libP0DBANFF.so'))
    from ROOT import TotalPOT, Header
    # base_directory = '/physics/home/mhogan'
    # splines = 'splines'
    # flattrees = 'flattree'
    pot = TotalPOT


if __name__ == '__main__':
    main(sys.argv)
