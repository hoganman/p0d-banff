#!/usr/bin/env python
"""submits the mc flattree files to run RunSyst_New.exe"""

import os
import sys
from Directory import Directory
from Command import ShellCommand

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
    # source_highland = ShellCommand(
    #         'source %s/nd280Highland2/v2r22/cmt/setup.sh' % (P0DBANFF))
    # submit(source_highland)
    submit_runsyst_new_jobs()


class RunName(object):
    """store upper and lower case spellings of runs"""

    def __init__(self, lowercase, uppercase):
        self.lower = lowercase
        self.upper = uppercase

    def upper_case(self):
        """get upper case seplling"""
        return self.upper

    def low(self):
        """get lower case seplling"""
        return self.lower


def submit_runsyst_new_jobs():
    """submits the RunSyst_New jobs"""
    submit_runsyst_new()


def make_qsub_runsyst_new(run_name, production):
    """makes a MC qsubmitter.py command for a particular RunName class input"""
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

    this_run = ShellCommand('nohup %s' % (QSUBRUNSYSTNEW))
    this_run.add(flattree_dir_list)
    this_run.add(memory)
    this_run.add(queue)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def submit(command, background=False):
    """sumits a command to shell"""
    if command is None:
        print 'ERROR: There is no command to be run'
        return
    print command
    if background:
        os.system('%s &' % (command.get()))
    else:
        os.system(command.get())


RUN2W = RunName('run2-water', 'Run2_Water')
RUN2A = RunName('run2-air', 'Run2_Air')
RUN3B = RunName('run3b-air', 'Run3b_Air')
RUN3C = RunName('run3c-air', 'Run3c_Air')
RUN4W = RunName('run4-water', 'Run4_Water')
RUN4A = RunName('run4-air', 'Run4_Air')
RUN5C = RunName('run5c-water', 'Run5c_Water')
RUN6B = RunName('run6b-air', 'Run6b_Air')
RUN6C = RunName('run6c-air', 'Run6c_Air')
RUN6D = RunName('run6d-air', 'Run6d_Air')
RUN6E = RunName('run6e-air', 'Run6e_Air')
RUN7B = RunName('run7b-water', 'Run7b_Water')


def submit_runsyst_new():
    """submits MC RunSyst_New.exe jobs"""
    run2w_ft_mc = make_qsub_runsyst_new(RUN2W, NEUTMC_6B)
    run2a_ft_mc = make_qsub_runsyst_new(RUN2A, NEUTMC_6B)
    run3b_ft_mc = make_qsub_runsyst_new(RUN3B, NEUTMC_6B)
    run3c_ft_mc = make_qsub_runsyst_new(RUN3C, NEUTMC_6B)
    run4w_ft_mc = make_qsub_runsyst_new(RUN4W, NEUTMC_6B)
    run4a_ft_mc = make_qsub_runsyst_new(RUN4A, NEUTMC_6B)
    run5c_ft_mc = make_qsub_runsyst_new(RUN5C, NEUTMC_6B)
    run6b_ft_mc = make_qsub_runsyst_new(RUN6B, NEUTMC_6B)
    run6c_ft_mc = make_qsub_runsyst_new(RUN6C, NEUTMC_6B)
    run6d_ft_mc = make_qsub_runsyst_new(RUN6D, NEUTMC_6B)
    run6e_ft_mc = make_qsub_runsyst_new(RUN6E, NEUTMC_6B)
    run7b_ft_mc = make_qsub_runsyst_new(RUN7B, NEUTMC_6L)

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
