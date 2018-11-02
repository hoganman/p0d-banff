#!/usr/bin/env python
"""submits the mc and data files to queue"""

import os
import sys
from Directory import Directory
from ShellCommand import ShellCommand
import RunName as RN

P0DBANFF = os.getenv('P0DBANFFROOT')
RUNLISTS = P0DBANFF+'/run_lists'
QUEUE = '\"physics.q|short.q\"'
HOSTS = '\"node40|node44|node27|node29\"'
# EXCLUDEHOSTS = [7, 11, 19, 29, 45]
# HOSTS = '\"'
# for x in range(20) + range(27, 31) + range(40, 46):
#     if x in EXCLUDEHOSTS:
#         continue
#     HOSTS += 'node{}|'.format(x)
# HOSTS = HOSTS.rstrip("|")
# HOSTS += '\"'
MEM = '1024'
MCNJOBS = '100'
DATANJOBS = '10'
MCMIN = '300'
DATAMIN = '120'
OUTPUTBASE = '/physics/home/mhogan'
FLATTREEBASE = os.getenv('FLATTREEROOT')
P0DNUMUCCANALYSISBASE = '/physics/home/mhogan/p0dNumuCCAnalysis'
NEUT_6B = 'mcp6_Spin_B/neut'
NEUT_6L = 'mcp6_Spin_L/neut'
SAND = 'mcp6_Spin_B/sand'
DATA_6M = 'rdp6_Spin_M'
DATA_6N = 'rdp6_Spin_N'

QSUBHIGHLAND = 'python qsubmitter_highlandAnalysis.py -a RunP0DNumuCCAnalysis.exe'


def main(argv):
    """submits all the qsub python scripts"""
    submit_hl2_jobs()


def submit_hl2_jobs():
    """submits the MC and data spline jobs"""
    # submit_hl2_data()
    submit_hl2_mc()


def make_qsub_hl2_mc(run_name, production):
    """make spline command for mc files"""
    flattree_dir = Directory('%s/%s/%s' % (FLATTREEBASE,
                                           production, run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flatttree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    hosts = '-N %s' % (HOSTS)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (P0DNUMUCCANALYSISBASE,
                                               production, run_name.low()))
    run_type = '-r MC'
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBHIGHLAND))
    this_run.add(flatttree_dir_list)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def make_qsub_hl2_data(run_name, production):
    """make spline command for data files"""
    flattree_dir = Directory('%s/%s/%s' % (FLATTREEBASE,
                                           production,
                                           run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flatttree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    hosts = '-N %s' % (HOSTS)
    minutes = '-M %s' % (DATAMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (P0DNUMUCCANALYSISBASE,
                                               production,
                                               run_name.low()))
    run_type = '-r data'
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBHIGHLAND))
    this_run.add(flatttree_dir_list)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def submit_hl2_mc():
    """submits MC spline jobs"""
    run2w_hl2_mc = make_qsub_hl2_mc(RN.RUN2W, NEUT_6B)
    run2a_hl2_mc = make_qsub_hl2_mc(RN.RUN2A, NEUT_6B)
    run3b_hl2_mc = make_qsub_hl2_mc(RN.RUN3B, NEUT_6B)
    run3c_hl2_mc = make_qsub_hl2_mc(RN.RUN3C, NEUT_6B)
    run4w_hl2_mc = make_qsub_hl2_mc(RN.RUN4W, NEUT_6B)
    run4a_hl2_mc = make_qsub_hl2_mc(RN.RUN4A, NEUT_6B)
    sand_fhc_hl2_mc = make_qsub_hl2_mc(RN.SANDFHC, SAND)

    # run2w_hl2_mc.run(not ShellCommand.IN_BKG)
    # run2a_hl2_mc.run(not ShellCommand.IN_BKG)
    # run3b_hl2_mc.run(not ShellCommand.IN_BKG)
    run3c_hl2_mc.run(not ShellCommand.IN_BKG)
    # run4w_hl2_mc.run(not ShellCommand.IN_BKG)
    # run4a_hl2_mc.run(not ShellCommand.IN_BKG)
    # sand_fhc_hl2_mc.run(not ShellCommand.IN_BKG)


def submit_hl2_data():
    """submits data spline jobs"""
    run2w_hl2_data = make_qsub_hl2_data(RN.RUN2WDATA, DATA_6M)
    run2a_hl2_data = make_qsub_hl2_data(RN.RUN2ADATA, DATA_6M)
    run3b_hl2_data = make_qsub_hl2_data(RN.RUN3BDATA, DATA_6M)
    run3c_hl2_data = make_qsub_hl2_data(RN.RUN3CDATA, DATA_6M)
    run4w_hl2_data = make_qsub_hl2_data(RN.RUN4WDATA, DATA_6M)
    run4a_hl2_data = make_qsub_hl2_data(RN.RUN4ADATA, DATA_6M)

    run2w_hl2_data.run(not ShellCommand.IN_BKG)
    run2a_hl2_data.run(not ShellCommand.IN_BKG)
    run3b_hl2_data.run(not ShellCommand.IN_BKG)
    run3c_hl2_data.run(not ShellCommand.IN_BKG)
    run4w_hl2_data.run(not ShellCommand.IN_BKG)
    run4a_hl2_data.run(not ShellCommand.IN_BKG)


if __name__ == "__main__":
    main(sys.argv[1:])
