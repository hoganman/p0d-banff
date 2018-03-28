#!/usr/bin/env python
"""submits the mc flattree files to run RunSyst_New.exe"""

import os
import sys
from Directory import Directory
from ShellCommand import ShellCommand
import RunName

P0DBANFF = os.getenv('P0DBANFFROOT')
QUEUE = '\"physics.q|short.q\"'
MEM = '1024'
NEUT = 'NEUT'
MCMIN = '60'
OUTPUTBASE = '/physics/home/mhogan'
FLATTREEBASE = OUTPUTBASE+'/flattrees'
RUNSYSTNEWOUTPUT = os.path.join(OUTPUTBASE, 'systematics/')
NEUTMC_6B = 'mcp6_Spin_B/neut'
NEUTMC_6L = 'mcp6_Spin_L/neut'

QSUBRUNSYSTNEW = 'python qsubmitter_RunSyst_New.py'


def main(argv):
    """submits all the qsub python scripts"""
    submit_runsyst_new_jobs()


def submit_runsyst_new_jobs():
    """submits the RunSyst_New jobs"""
    submit_runsyst_new()


def make_qsub_runsyst_new(run_name, production, is_mc=True):
    """makes a MC qsubmitter.py command class
    for a particular RunName class input"""
    flattree_dir = Directory('%s/%s/%s' % (FLATTREEBASE,
                                           production, run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flattree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory(RUNSYSTNEWOUTPUT)
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'

    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s' % (run_name.upper_case())

    run_type = '-r "MC"'
    if not is_mc:
        run_type = '-r "Data"'

    this_run = ShellCommand('nohup %s' % (QSUBRUNSYSTNEW))
    this_run.add(flattree_dir_list)
    this_run.add(memory)
    this_run.add(queue)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(output_name)
    this_run.add(run_type)
    return this_run


def submit_runsyst_new():
    """submits MC RunSyst_New.exe jobs"""
    run2w_ft_mc = make_qsub_runsyst_new(RunName.RUN2W, NEUTMC_6B)
    run2a_ft_mc = make_qsub_runsyst_new(RunName.RUN2A, NEUTMC_6B)
    run3b_ft_mc = make_qsub_runsyst_new(RunName.RUN3B, NEUTMC_6B)
    run3c_ft_mc = make_qsub_runsyst_new(RunName.RUN3C, NEUTMC_6B)
    run4w_ft_mc = make_qsub_runsyst_new(RunName.RUN4W, NEUTMC_6B)
    run4a_ft_mc = make_qsub_runsyst_new(RunName.RUN4A, NEUTMC_6B)
    run5c_ft_mc = make_qsub_runsyst_new(RunName.RUN5C, NEUTMC_6B)
    run6b_ft_mc = make_qsub_runsyst_new(RunName.RUN6B, NEUTMC_6B)
    run6c_ft_mc = make_qsub_runsyst_new(RunName.RUN6C, NEUTMC_6B)
    run6d_ft_mc = make_qsub_runsyst_new(RunName.RUN6D, NEUTMC_6B)
    run6e_ft_mc = make_qsub_runsyst_new(RunName.RUN6E, NEUTMC_6B)
    run7b_ft_mc = make_qsub_runsyst_new(RunName.RUN7B, NEUTMC_6L)

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


if __name__ == "__main__":
    main(sys.argv[1:])
