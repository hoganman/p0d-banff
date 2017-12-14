#!/usr/bin/env python
"""Checks if the POT from header tree (pre- and post-T2KRW file)
    matches the official POT and hardcoded cut-off in ND280AnalysisUtils.cxx
"""

import sys
import os
import math
from os.path import join
from Directory import Directory
import ROOT
ROOT.gSystem.Load('libP0DBANFF')
from ROOT import TotalPOT, Header

P0DBANFF = os.getenv('P0DBANFFROOT')
MCP6B = 'mcp6_Spin_B/neut'


def get_file_header(directory, file_name):
    """Given the directory with the file, return the file header
       with the names "file_name.root" or "file_name_XXX.root" """
    header = Header()
    test_dir = Directory(directory)
    if not test_dir.exists():
        print 'The directory \"%s\" does not exist!' % (directory)
        return header
    file_list = sorted(os.listdir(directory))
    if '%s.root' % (file_name) in file_list:
        header.ReadHeader(join(directory, '%s.root' % (file_name)))
    elif '%s_1.root' % (file_name) in file_list:
        index = 1
        root_file = '%s_%d.root' % (file_name, index)
        while root_file in file_list:
            header.AddHeader(join(directory, root_file))
            index += 1
            root_file = '%s_%d.root' % (file_name, index)
    else:
        print 'No matching files \"%s\" found in %s' % (file_name, directory)
    return header


def check_pot(header, run_name='Run4WaterMC'):
    """for a given run (MC or data), check if pot matches"""
    official_pot = TotalPOT().GetPOT(run_name)
    header_pot = header.GetPOTGoodBeamGoodND280()
    print run_name
    print '   ' + str(official_pot)
    print '   ' + str(header_pot)
    if math.fabs(header_pot - official_pot) / official_pot > 0.01:
        print 'ERROR: %s has a POT mismatch!' % (run_name)


def main(argv):
    """main function"""
    print 'Running %s' % (argv[0])

    mc_directories = [
            '/physics/home/mhogan/flattrees/mcp6_Spin_B/neut/run2-air',
            '/physics/home/mhogan/flattrees/mcp6_Spin_B/neut/run2-water',
            '/physics/home/mhogan/flattrees/mcp6_Spin_B/neut/run3b-air',
            '/physics/home/mhogan/flattrees/mcp6_Spin_B/neut/run3c-air',
            '/physics/home/mhogan/flattrees/mcp6_Spin_B/neut/run4-air',
            '/physics/home/mhogan/flattrees/mcp6_Spin_B/neut/run4-water'
    ]

    flattrees = [
            'Run2_Air',
            'Run2_Water',
            'Run3b_Air',
            'Run3c_Air',
            'Run4_Air',
            'Run4_Water'
    ]
    for index in range(0, len(flattrees)):
        header = get_file_header(mc_directories[index], flattrees[index])
        check_pot(header, '%sMC' % (flattrees[index]))


if __name__ == '__main__':
    main(sys.argv)
