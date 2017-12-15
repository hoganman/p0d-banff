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
# how much can the hardcoded numbers in TotalPOT can deviate from
#   the header trees
TOLERANCE = 0.02


def get_file_header(directory, file_name):
    """Given the directory with the file, return the file header
       with the names "file_name.root" or "file_name_XXX.root" """
    header = Header()
    test_dir = Directory(directory)
    if not test_dir.exists():
        print 'The directory \"%s\" does not exist!' % (directory)
        return header
    file_list = sorted(os.listdir(directory))
    if '%s_splines.root' % (file_name) in file_list:
        header.ReadHeader(join(directory, '%s_splines.root' % (file_name)))
    elif '%s_data_splines.root' % (file_name) in file_list:
        header.ReadHeader(join(directory, '%s_data_splines.root' % (file_name)))
    # in case we care about flattree output instead
    elif '%s_1.root' % (file_name) in file_list:
        index = 1
        root_file = '%s_%d.root' % (file_name, index)
        while root_file in file_list:
            header.AddHeader(join(directory, root_file))
            index += 1
            root_file = '%s_%d.root' % (file_name, index)
    elif '%s_data_1.root' % (file_name) in file_list:
        index = 1
        root_file = '%s_data_%d.root' % (file_name, index)
        while root_file in file_list:
            header.AddHeader(join(directory, root_file))
            index += 1
            root_file = '%s_data_%d.root' % (file_name, index)

    else:
        print 'No matching files \"%s\" found in %s' % (file_name, directory)
    return header


def check_pot(header, run_name='Run4WaterMC'):
    """for a given run (MC or data), check if pot matches"""
    official_pot = TotalPOT().GetPOT(run_name)
    header_pot = header.GetPOTGoodBeamGoodND280()
    print run_name
    print 'OFFICIAL: %e' % (official_pot)
    print 'COUNTED:  %e' % (header_pot)
    if math.fabs(header_pot - official_pot) / official_pot > TOLERANCE:
        print 'ERROR: %s has a POT mismatch!' % (run_name)


def main(argv):
    """main function"""
    print 'Running %s' % (argv[0])

    mc_directories = [
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run2-air',
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run2-water',
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run3b-air',
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run3c-air',
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run4-air',
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run4-water',
            '/physics/home/mhogan/splines/mcp6_Spin_B/neut/run5-water'
    ]

    data_directories = [
            '/physics/home/mhogan/splines/rdp6_Spin_M/run2-air',
            '/physics/home/mhogan/splines/rdp6_Spin_M/run2-water',
            '/physics/home/mhogan/splines/rdp6_Spin_M/run3b-air',
            '/physics/home/mhogan/splines/rdp6_Spin_M/run3c-air',
            '/physics/home/mhogan/splines/rdp6_Spin_M/run4-air',
            '/physics/home/mhogan/splines/rdp6_Spin_M/run4-water',
            '/physics/home/mhogan/splines/rdp6_Spin_M/run5-water'
    ]

    files = [
            'Run2_Air',
            'Run2_Water',
            'Run3b_Air',
            'Run3c_Air',
            'Run4_Air',
            'Run4_Water',
            'Run5_Water'

    ]
    for index in range(0, len(files)):
        header = get_file_header(mc_directories[index], files[index])
        check_pot(header, '%sMC' % (files[index]))
    for index in range(0, len(files)):
        header = get_file_header(data_directories[index], files[index])
        check_pot(header, '%sData' % (files[index]))

    #compare run3b and run3c MC
    run3b_header = get_file_header(data_directories[2], files[2])
    run3c_header = get_file_header(data_directories[3], files[3])
    run3b_pot = run3b_header.GetPOTGoodBeamGoodND280()
    run3c_pot = run3c_header.GetPOTGoodBeamGoodND280()
    run3_data_ratio = run3b_pot / (run3b_pot + run3c_pot)
    if math.fabs(run3_data_ratio/TotalPOT().GetRun3bAnd3cDataRatio() - 1) > TOLERANCE:
        print 'ERROR: run3b and run3c MC ratio does NOT match what is'
        print '       hard coded in ND280AnalysisUtils.cxx'
        print '       Expected ratio =  %e' % (TotalPOT().GetRun3bAnd3cDataRatio())
        print '       In files ratio =  %e' % (run3_data_ratio)


if __name__ == '__main__':
    main(sys.argv)
