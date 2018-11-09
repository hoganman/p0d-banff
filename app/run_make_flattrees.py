#!/usr/bin/env python
"""submits the mc and data files to queue"""

import os
import sys
from TextFile import TextFile
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
FLATTREEBASE = os.getenv('FLATTREEROOT')
NEUT_6B = 'mcp6_Spin_B/neut'
NEUT_6L = 'mcp6_Spin_L/neut'
SAND = 'mcp6_Spin_B/sand'
DATA_6M = 'rdp6_Spin_M'
DATA_6N = 'rdp6_Spin_N'

QSUBFLAT = 'python qsubmitter.py'


def main(argv):
    """submits all the qsub python scripts"""
    submit_flattree_jobs()


def submit_flattree_jobs():
    """submits the MC and data flat tree jobs"""
    submit_ft_data()
    submit_ft_mc()


def make_qsub_flattree_mc(run_name, production):
    """makes a MC qsubmitter.py command for a particular RN class input"""
    run_list_file = TextFile('%s/%s/%s.list' % (RUNLISTS,
                                                production,
                                                run_name.upper_case()))
    if not run_list_file.exists():
        print 'ERROR: run list file'
        print '\"%s\"' % (run_list_file.get_file_name())
        print 'does not exist!'
        return None

    run_list = '-L %s' % (run_list_file.get_file_name())
    queue = '-q %s' % (QUEUE)
    hosts = '-N %s' % (HOSTS)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (FLATTREEBASE,
                                               production, run_name.low()))
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()

    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s' % (run_name.upper_case())
    n_jobs = '-n %s' % (MCNJOBS)

    this_run = ShellCommand('nohup %s' % (QSUBFLAT))
    this_run.add(run_list)
    this_run.add(memory)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(n_jobs)
    this_run.add(output_name)
    return this_run


def make_qsub_flattree_data(run_name, production):
    """makes a data qsubmitter.py command class for a
    particular RN class input"""
    run_list_file = TextFile('%s/%s/%s.list' % (RUNLISTS,
                                                production,
                                                run_name.upper_case()))
    if not run_list_file.exists():
        print 'ERROR: run list file'
        print '\"%s\"' % (run_list_file.get_file_name())
        print 'does not exist!'
        return None

    run_list = '-L %s' % (run_list_file.get_file_name())
    queue = '-q %s' % (QUEUE)
    hosts = '-N %s' % (HOSTS)
    minutes = '-M %s' % (DATAMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (FLATTREEBASE,
                                               production,
                                               run_name.low()))
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s' % (run_name.upper_case())
    n_jobs = '-n %s' % (DATANJOBS)

    this_run = ShellCommand('nohup %s' % (QSUBFLAT))
    this_run.add(run_list)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(n_jobs)
    this_run.add(output_name)
    return this_run


def submit_ft_mc():
    """submits MC flattree jobs"""
    run2w_ft_mc = make_qsub_flattree_mc(RN.RUN2W, NEUT_6B)
    run2a_ft_mc = make_qsub_flattree_mc(RN.RUN2A, NEUT_6B)
    run3b_ft_mc = make_qsub_flattree_mc(RN.RUN3B, NEUT_6B)
    run3c_ft_mc = make_qsub_flattree_mc(RN.RUN3C, NEUT_6B)
    run4w_ft_mc = make_qsub_flattree_mc(RN.RUN4W, NEUT_6B)
    run4a_ft_mc = make_qsub_flattree_mc(RN.RUN4A, NEUT_6B)
    run5c_ft_mc = make_qsub_flattree_mc(RN.RUN5C, NEUT_6B)
    run6b_ft_mc = make_qsub_flattree_mc(RN.RUN6B, NEUT_6B)
    run6c_ft_mc = make_qsub_flattree_mc(RN.RUN6C, NEUT_6B)
    run6d_ft_mc = make_qsub_flattree_mc(RN.RUN6D, NEUT_6B)
    run6e_ft_mc = make_qsub_flattree_mc(RN.RUN6E, NEUT_6B)
    run7b_ft_mc = make_qsub_flattree_mc(RN.RUN7B, NEUT_6L)
    sand_fhc_ft_mc = make_qsub_flattree_mc(RN.SANDFHC, SAND)
    sand_rhc_ft_mc = make_qsub_flattree_mc(RN.SANDRHC, SAND)

    run2w_ft_mc.run(ShellCommand.IN_BKG)
    run2a_ft_mc.run(not ShellCommand.IN_BKG)
    run3b_ft_mc.run(not ShellCommand.IN_BKG)
    run3c_ft_mc.run(not ShellCommand.IN_BKG)
    run4w_ft_mc.run(not ShellCommand.IN_BKG)
    run4a_ft_mc.run(not ShellCommand.IN_BKG)
    run5c_ft_mc.run(not ShellCommand.IN_BKG)
    run6b_ft_mc.run(not ShellCommand.IN_BKG)
    run6c_ft_mc.run(not ShellCommand.IN_BKG)
    run6d_ft_mc.run(not ShellCommand.IN_BKG)
    run6e_ft_mc.run(not ShellCommand.IN_BKG)
    run7b_ft_mc.run(not ShellCommand.IN_BKG)
    sand_fhc_ft_mc.run(not ShellCommand.IN_BKG)
    sand_rhc_ft_mc.run(not ShellCommand.IN_BKG)


def submit_ft_data():
    """submits data ft jobs"""
    run2w_ft_data = make_qsub_flattree_data(RN.RUN2WDATA, DATA_6M)
    run2a_ft_data = make_qsub_flattree_data(RN.RUN2ADATA, DATA_6M)
    run3b_ft_data = make_qsub_flattree_data(RN.RUN3BDATA, DATA_6M)
    run3c_ft_data = make_qsub_flattree_data(RN.RUN3CDATA, DATA_6M)
    run4w_ft_data = make_qsub_flattree_data(RN.RUN4WDATA, DATA_6M)
    run4a_ft_data = make_qsub_flattree_data(RN.RUN4ADATA, DATA_6M)
    run5c_ft_data = make_qsub_flattree_data(RN.RUN5CDATA, DATA_6M)
    run6b_ft_data = make_qsub_flattree_data(RN.RUN6BDATA, DATA_6M)
    run6c_ft_data = make_qsub_flattree_data(RN.RUN6CDATA, DATA_6M)
    run6d_ft_data = make_qsub_flattree_data(RN.RUN6DDATA, DATA_6M)
    run6e_ft_data = make_qsub_flattree_data(RN.RUN6EDATA, DATA_6M)
    run7b_ft_data = make_qsub_flattree_data(RN.RUN7BDATA, DATA_6N)

    run2w_ft_data.run(ShellCommand.IN_BKG)
    run2a_ft_data.run(ShellCommand.IN_BKG)
    run3b_ft_data.run(ShellCommand.IN_BKG)
    run3c_ft_data.run(ShellCommand.IN_BKG)
    run4w_ft_data.run(ShellCommand.IN_BKG)
    run4a_ft_data.run(ShellCommand.IN_BKG)
    run5c_ft_data.run(ShellCommand.IN_BKG)
    run6b_ft_data.run(ShellCommand.IN_BKG)
    run6c_ft_data.run(ShellCommand.IN_BKG)
    run6d_ft_data.run(ShellCommand.IN_BKG)
    run6e_ft_data.run(ShellCommand.IN_BKG)
    run7b_ft_data.run(ShellCommand.IN_BKG)


if __name__ == "__main__":
    main(sys.argv[1:])
