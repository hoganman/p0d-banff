#!/usr/bin/env python
"""submits the mc and data files to queue"""

import os
import sys
from TextFile import TextFile
from Directory import Directory
from ShellCommand import ShellCommand
import RunName

P0DBANFF = os.getenv('P0DBANFFROOT')
RUNLISTS = P0DBANFF+'/run_lists'
QUEUE = '\"physics.q\"'
MEM = '1024'
NEUT = 'NEUT'
MCSPLINE = 'MC'
DATASPLINE = 'Data'
MCMIN = '60'
DATAMIN = '120'
OUTPUTBASE = '/physics/home/mhogan'
FLATTREEBASE = OUTPUTBASE+'/flattrees'
SPLINEBASE = OUTPUTBASE+'/splines'
NEUTMC_6B = 'mcp6_Spin_B/neut'
NEUTMC_6L = 'mcp6_Spin_L/neut'
DATA_6M = 'rdp6_Spin_M'
DATA_6N = 'rdp6_Spin_N'

QSUBFLAT = 'python qsubmitter.py'
QSUBSPLINE = 'python qsubmitter_splines.py'


def main(argv):
    """submits all the qsub python scripts"""
    submit_flattree_jobs()
    submit_spline_jobs()


def submit_flattree_jobs():
    """submits the MC and data flat tree jobs"""
    submit_ft_data()
    submit_ft_mc()


def submit_spline_jobs():
    """submits the MC and data spline jobs"""
    submit_spline_data()
    submit_spline_mc()


def make_qsub_flattree_mc(run_name, production):
    """makes a MC qsubmitter.py command for a particular RunName class input"""
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
    n_jobs = '-n 100'

    this_run = ShellCommand('nohup %s' % (QSUBFLAT))
    this_run.add(run_list)
    this_run.add(memory)
    this_run.add(queue)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(n_jobs)
    this_run.add(output_name)
    return this_run


def make_qsub_spline_mc(run_name, production):
    """make spline command for mc files"""
    flattree_dir = Directory('%s/%s/%s' % (FLATTREEBASE,
                                           production, run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flatttree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (SPLINEBASE,
                                               production, run_name.low()))
    run_type = '-r MC'
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
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def make_qsub_flattree_data(run_name, production):
    """makes a data qsubmitter.py command class for a
    particular RunName class input"""
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
    n_jobs = '-n 10'

    this_run = ShellCommand('nohup %s' % (QSUBFLAT))
    this_run.add(run_list)
    this_run.add(queue)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(n_jobs)
    this_run.add(output_name)
    return this_run


def submit_ft_mc():
    """submits MC flattree jobs"""
    run2w_ft_mc = make_qsub_flattree_mc(RunName.RUN2W, NEUTMC_6B)
    run2a_ft_mc = make_qsub_flattree_mc(RunName.RUN2A, NEUTMC_6B)
    run3b_ft_mc = make_qsub_flattree_mc(RunName.RUN3B, NEUTMC_6B)
    run3c_ft_mc = make_qsub_flattree_mc(RunName.RUN3C, NEUTMC_6B)
    run4w_ft_mc = make_qsub_flattree_mc(RunName.RUN4W, NEUTMC_6B)
    run4a_ft_mc = make_qsub_flattree_mc(RunName.RUN4A, NEUTMC_6B)
    run5c_ft_mc = make_qsub_flattree_mc(RunName.RUN5C, NEUTMC_6B)
    run6b_ft_mc = make_qsub_flattree_mc(RunName.RUN6B, NEUTMC_6B)
    run6c_ft_mc = make_qsub_flattree_mc(RunName.RUN6C, NEUTMC_6B)
    run6d_ft_mc = make_qsub_flattree_mc(RunName.RUN6D, NEUTMC_6B)
    run6e_ft_mc = make_qsub_flattree_mc(RunName.RUN6E, NEUTMC_6B)
    run7b_ft_mc = make_qsub_flattree_mc(RunName.RUN7B, NEUTMC_6L)

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


def submit_spline_mc():
    """submits MC spline jobs"""
    run2w_sp_mc = make_qsub_spline_mc(RunName.RUN2W, NEUTMC_6B)
    run2a_sp_mc = make_qsub_spline_mc(RunName.RUN2A, NEUTMC_6B)
    run3b_sp_mc = make_qsub_spline_mc(RunName.RUN3B, NEUTMC_6B)
    run3c_sp_mc = make_qsub_spline_mc(RunName.RUN3C, NEUTMC_6B)
    run4w_sp_mc = make_qsub_spline_mc(RunName.RUN4W, NEUTMC_6B)
    run4a_sp_mc = make_qsub_spline_mc(RunName.RUN4A, NEUTMC_6B)
    run5c_sp_mc = make_qsub_spline_mc(RunName.RUN5C, NEUTMC_6B)
    run6b_sp_mc = make_qsub_spline_mc(RunName.RUN6B, NEUTMC_6B)
    run6c_sp_mc = make_qsub_spline_mc(RunName.RUN6C, NEUTMC_6B)
    run6d_sp_mc = make_qsub_spline_mc(RunName.RUN6D, NEUTMC_6B)
    run6e_sp_mc = make_qsub_spline_mc(RunName.RUN6E, NEUTMC_6B)
    run7b_sp_mc = make_qsub_spline_mc(RunName.RUN7B, NEUTMC_6L)

    run2w_sp_mc.run(ShellCommand.IN_BKG)
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


def submit_spline_data():
    """submits data spline jobs"""
    run2w_sp_data = make_qsub_spline_data(RunName.RUN2WDATA, DATA_6M)
    run2a_sp_data = make_qsub_spline_data(RunName.RUN2ADATA, DATA_6M)
    run3b_sp_data = make_qsub_spline_data(RunName.RUN3BDATA, DATA_6M)
    run3c_sp_data = make_qsub_spline_data(RunName.RUN3CDATA, DATA_6M)
    run4w_sp_data = make_qsub_spline_data(RunName.RUN4WDATA, DATA_6M)
    run4a_sp_data = make_qsub_spline_data(RunName.RUN4ADATA, DATA_6M)
    run5c_sp_data = make_qsub_spline_data(RunName.RUN5CDATA, DATA_6M)
    run6b_sp_data = make_qsub_spline_data(RunName.RUN6BDATA, DATA_6M)
    run6c_sp_data = make_qsub_spline_data(RunName.RUN6CDATA, DATA_6M)
    run6d_sp_data = make_qsub_spline_data(RunName.RUN6DDATA, DATA_6M)
    run6e_sp_data = make_qsub_spline_data(RunName.RUN6EDATA, DATA_6M)
    run7b_sp_data = make_qsub_spline_data(RunName.RUN7BDATA, DATA_6N)

    run2w_sp_data.run(ShellCommand.IN_BKG)
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


def submit_ft_data():
    """submits data ft jobs"""
    run2w_ft_data = make_qsub_flattree_data(RunName.RUN2WDATA, DATA_6M)
    run2a_ft_data = make_qsub_flattree_data(RunName.RUN2ADATA, DATA_6M)
    run3b_ft_data = make_qsub_flattree_data(RunName.RUN3BDATA, DATA_6M)
    run3c_ft_data = make_qsub_flattree_data(RunName.RUN3CDATA, DATA_6M)
    run4w_ft_data = make_qsub_flattree_data(RunName.RUN4WDATA, DATA_6M)
    run4a_ft_data = make_qsub_flattree_data(RunName.RUN4ADATA, DATA_6M)
    run5c_ft_data = make_qsub_flattree_data(RunName.RUN5CDATA, DATA_6M)
    run6b_ft_data = make_qsub_flattree_data(RunName.RUN6BDATA, DATA_6M)
    run6c_ft_data = make_qsub_flattree_data(RunName.RUN6CDATA, DATA_6M)
    run6d_ft_data = make_qsub_flattree_data(RunName.RUN6DDATA, DATA_6M)
    run6e_ft_data = make_qsub_flattree_data(RunName.RUN6EDATA, DATA_6M)
    run7b_ft_data = make_qsub_flattree_data(RunName.RUN7BDATA, DATA_6N)

    run2w_ft_data.run(ShellCommand.IN_BKG)
    run2a_ft_data.run(not ShellCommand.IN_BKG)
    run3b_ft_data.run(not ShellCommand.IN_BKG)
    run3c_ft_data.run(not ShellCommand.IN_BKG)
    run4w_ft_data.run(not ShellCommand.IN_BKG)
    run4a_ft_data.run(not ShellCommand.IN_BKG)
    run5c_ft_data.run(not ShellCommand.IN_BKG)
    run6b_ft_data.run(not ShellCommand.IN_BKG)
    run6c_ft_data.run(not ShellCommand.IN_BKG)
    run6d_ft_data.run(not ShellCommand.IN_BKG)
    run6e_ft_data.run(not ShellCommand.IN_BKG)
    run7b_ft_data.run(not ShellCommand.IN_BKG)


if __name__ == "__main__":
    main(sys.argv[1:])
