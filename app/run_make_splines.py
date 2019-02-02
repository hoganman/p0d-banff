#!/usr/bin/env python
"""submits the mc and data files to queue"""

import os
import sys
from Directory import Directory
from ShellCommand import ShellCommand
import RunName as RN

P0DBANFF = os.getenv('P0DBANFFROOT')
RUNLISTS = P0DBANFF+'/run_lists'
# QUEUE = '\"physics.q|short.q\"'
# HOSTS = '\"node40|node41|node42|node43|node44|node45|node27|node28|node30|node29\"'
# MCMIN = str(int(60*8))
# DATAMIN = str(int(60*8))
# EXCLUDEHOSTS = [7, 11, 19, 29, 45]
# HOSTS = '\"'
# for x in range(20) + range(27, 31) + range(40, 46):
#     if x in EXCLUDEHOSTS:
#         continue
#     HOSTS += 'node{}|'.format(x)
# HOSTS = HOSTS.rstrip("|")
# HOSTS += '\"'
QUEUE = '\"physics.q\"'
HOSTS = '\"node40|node41|node42|node43|node44|node45\"'
MCMIN = '9999'
DATAMIN = '9999'
MEM = '2048'
MCNJOBS = '100'
DATANJOBS = '10'
FLATTREEBASE = os.getenv('FLATTREEROOT')
SPLINEBASE = os.getenv('SPLINEROOT')
NEUT_6B = 'mcp6_Spin_B/neut'
NEUT_6L = 'mcp6_Spin_L/neut'
SAND = 'mcp6_Spin_B/sand'
DATA_6M = 'rdp6_Spin_M'
DATA_6N = 'rdp6_Spin_N'
DATA_6P = 'rdp6_Spin_P'

QSUBSPLINE = 'python qsubmitter_splines.py'


def main(argv):
    """submits all the qsub python scripts"""
    submit_spline_jobs()


def submit_spline_jobs():
    """submits the MC and data spline jobs"""
    submit_spline_data()
    submit_spline_mc()


def make_qsub_spline_mc(run_name, production, data_type):
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
    output_path_name = Directory('%s/%s/%s' % (SPLINEBASE,
                                               production, run_name.low()))
    run_type = '-r %s' % (data_type)
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s_spline' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBSPLINE))
    this_run.add(flatttree_dir_list)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def make_qsub_spline_data(run_name, production):
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
    output_path_name = Directory('%s/%s/%s' % (SPLINEBASE,
                                               production,
                                               run_name.low()))
    run_type = '-r data'
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s_spline' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBSPLINE))
    this_run.add(flatttree_dir_list)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def submit_spline_mc():
    """submits MC spline jobs"""
    run2w_sp_mc = make_qsub_spline_mc(RN.RUN2W, NEUT_6B, DATATYPES.MC)
    run2a_sp_mc = make_qsub_spline_mc(RN.RUN2A, NEUT_6B, DATATYPES.MC)
    run3b_sp_mc = make_qsub_spline_mc(RN.RUN3B, NEUT_6B, DATATYPES.MC)
    run3c_sp_mc = make_qsub_spline_mc(RN.RUN3C, NEUT_6B, DATATYPES.MC)
    run4w_sp_mc = make_qsub_spline_mc(RN.RUN4W, NEUT_6B, DATATYPES.MC)
    run4a_sp_mc = make_qsub_spline_mc(RN.RUN4A, NEUT_6B, DATATYPES.MC)
    run5c_sp_mc = make_qsub_spline_mc(RN.RUN5C, NEUT_6B, DATATYPES.MC)
    run6b_sp_mc = make_qsub_spline_mc(RN.RUN6B, NEUT_6B, DATATYPES.MC)
    run6c_sp_mc = make_qsub_spline_mc(RN.RUN6C, NEUT_6B, DATATYPES.MC)
    run6d_sp_mc = make_qsub_spline_mc(RN.RUN6D, NEUT_6B, DATATYPES.MC)
    run6e_sp_mc = make_qsub_spline_mc(RN.RUN6E, NEUT_6B, DATATYPES.MC)
    run7b_sp_mc = make_qsub_spline_mc(RN.RUN7B, NEUT_6L, DATATYPES.MC)
    run8w_sp_mc = make_qsub_spline_mc(RN.RUN8W, NEUT_6L, DATATYPES.MC)
    run8a_sp_mc = make_qsub_spline_mc(RN.RUN8A, NEUT_6L, DATATYPES.MC)
    sand_fhc_sp_mc = make_qsub_spline_mc(RN.SANDFHC, SAND, DATATYPES.SAND)
    sand_rhc_sp_mc = make_qsub_spline_mc(RN.SANDRHC, SAND, DATATYPES.SAND)

    run2w_sp_mc.run(not ShellCommand.IN_BKG)
    run2a_sp_mc.run(not ShellCommand.IN_BKG)
    run3b_sp_mc.run(not ShellCommand.IN_BKG)
    run3c_sp_mc.run(not ShellCommand.IN_BKG)
    run4w_sp_mc.run(not ShellCommand.IN_BKG)
    run4a_sp_mc.run(not ShellCommand.IN_BKG)
    run5c_sp_mc.run(not ShellCommand.IN_BKG)
    run6b_sp_mc.run(not ShellCommand.IN_BKG)
    run6c_sp_mc.run(not ShellCommand.IN_BKG)
    run6d_sp_mc.run(not ShellCommand.IN_BKG)
    run6e_sp_mc.run(not ShellCommand.IN_BKG)
    run7b_sp_mc.run(not ShellCommand.IN_BKG)
    run8w_sp_mc.run(not ShellCommand.IN_BKG)
    run8a_sp_mc.run(not ShellCommand.IN_BKG)
    sand_fhc_sp_mc.run(not ShellCommand.IN_BKG)
    sand_rhc_sp_mc.run(not ShellCommand.IN_BKG)


def submit_spline_data():
    """submits data spline jobs"""
    run2w_sp_data = make_qsub_spline_data(RN.RUN2WDATA, DATA_6M)
    run2a_sp_data = make_qsub_spline_data(RN.RUN2ADATA, DATA_6M)
    run3b_sp_data = make_qsub_spline_data(RN.RUN3BDATA, DATA_6M)
    run3c_sp_data = make_qsub_spline_data(RN.RUN3CDATA, DATA_6M)
    run4w_sp_data = make_qsub_spline_data(RN.RUN4WDATA, DATA_6M)
    run4a_sp_data = make_qsub_spline_data(RN.RUN4ADATA, DATA_6M)
    run5c_sp_data = make_qsub_spline_data(RN.RUN5CDATA, DATA_6M)
    run6b_sp_data = make_qsub_spline_data(RN.RUN6BDATA, DATA_6M)
    run6c_sp_data = make_qsub_spline_data(RN.RUN6CDATA, DATA_6M)
    run6d_sp_data = make_qsub_spline_data(RN.RUN6DDATA, DATA_6M)
    run6e_sp_data = make_qsub_spline_data(RN.RUN6EDATA, DATA_6M)
    run7b_sp_data = make_qsub_spline_data(RN.RUN7BDATA, DATA_6N)
    run8w_sp_data = make_qsub_spline_data(RN.RUN8WDATA, DATA_6P)
    run8a_sp_data = make_qsub_spline_data(RN.RUN8ADATA, DATA_6P)

    run2w_sp_data.run(not ShellCommand.IN_BKG)
    run2a_sp_data.run(not ShellCommand.IN_BKG)
    run3b_sp_data.run(not ShellCommand.IN_BKG)
    run3c_sp_data.run(not ShellCommand.IN_BKG)
    run4w_sp_data.run(not ShellCommand.IN_BKG)
    run4a_sp_data.run(not ShellCommand.IN_BKG)
    run5c_sp_data.run(not ShellCommand.IN_BKG)
    run6b_sp_data.run(not ShellCommand.IN_BKG)
    run6c_sp_data.run(not ShellCommand.IN_BKG)
    run6d_sp_data.run(not ShellCommand.IN_BKG)
    run6e_sp_data.run(not ShellCommand.IN_BKG)
    run7b_sp_data.run(not ShellCommand.IN_BKG)
    run8w_sp_data.run(not ShellCommand.IN_BKG)
    run8a_sp_data.run(not ShellCommand.IN_BKG)


class DATATYPES(object):
    """list of data types for generating splines"""
    SAND = 'sand'
    MC = 'MC'


if __name__ == "__main__":
    main(sys.argv[1:])
