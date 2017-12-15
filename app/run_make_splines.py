#!usr/bin/env python
"""submits the mc and data files to queue"""

import os
import sys
from TextFile import TextFile
from Directory import Directory

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
NEUTMC = 'mcp6_Spin_B/neut'
DATA = 'rdp6_Spin_M'

QSUBFLAT = 'python qsubmitter.py'
QSUBSPLINE = 'python qsubmitter_splines.py'


def main(argv):
    """submits all the qsub python scripts"""

    # Data 6M
    # submit_ft_data()
    submit_spline_data()

    # MC 6B
    # submit_ft_mc()
    submit_spline_mc()


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


class ShellCommand(object):
    """a shell command class"""

    def __init__(self, command):
        self.command = command

    def __str__(self):
        return self.get()

    def get(self):
        """returns command"""
        return self.command

    def add(self, addition):
        """adds more to the command"""
        self.command = self.command + ' ' + addition


def make_qsub_flattree_mc(run_name):
    """makes a MC qsubmitter.py command for a particular RunName class input"""
    run_list_file = TextFile('%s/%s/%s.list' % (RUNLISTS,
                                                NEUTMC, run_name.upper_case()))
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
                                               NEUTMC, run_name.low()))
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


def make_qsub_spline_mc(run_name):
    """make spline command for mc files"""
    flattree_dir = Directory('%s/%s/%s' % (FLATTREEBASE,
                                           NEUTMC, run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flatttree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (SPLINEBASE,
                                               NEUTMC, run_name.low()))
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


def make_qsub_spline_data(run_name):
    """make spline command for data files"""
    flattree_dir = Directory('%s/%s/%s' % (FLATTREEBASE,
                                           DATA, run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flatttree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    minutes = '-M %s' % (DATAMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (SPLINEBASE,
                                               DATA, run_name.low()))
    run_type = '-r data'
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s_data_spline' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBSPLINE))
    this_run.add(flatttree_dir_list)
    this_run.add(queue)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def make_qsub_flattree_data(run_name):
    """makes a data qsubmitter.py command for a
    particular RunName class input"""
    run_list_file = TextFile('%s/%s/%s.list' % (RUNLISTS,
                                                DATA, run_name.upper_case()))
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
                                               DATA, run_name.low()))
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s_data' % (run_name.upper_case())
    n_jobs = '-n 1'

    this_run = ShellCommand('nohup %s' % (QSUBFLAT))
    this_run.add(run_list)
    this_run.add(queue)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(n_jobs)
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
RUN5W = RunName('run5-water', 'Run5_Water')


def submit_ft_mc():
    """submits MC flattree jobs"""
    run2w_ft_mc = make_qsub_flattree_mc(RUN2W)
    run2a_ft_mc = make_qsub_flattree_mc(RUN2A)
    run3b_ft_mc = make_qsub_flattree_mc(RUN3B)
    run3c_ft_mc = make_qsub_flattree_mc(RUN3C)
    run4w_ft_mc = make_qsub_flattree_mc(RUN4W)
    run4a_ft_mc = make_qsub_flattree_mc(RUN4A)
    run5w_ft_mc = make_qsub_flattree_mc(RUN5W)

    submit(run2w_ft_mc, False)
    submit(run2a_ft_mc, False)
    submit(run3b_ft_mc, False)
    submit(run3c_ft_mc, False)
    submit(run4w_ft_mc, False)
    submit(run4a_ft_mc, False)
    submit(run5w_ft_mc, False)


def submit_spline_mc():
    """submits MC spline jobs"""
    run2w_sp_mc = make_qsub_spline_mc(RUN2W)
    run2a_sp_mc = make_qsub_spline_mc(RUN2A)
    run3b_sp_mc = make_qsub_spline_mc(RUN3B)
    run3c_sp_mc = make_qsub_spline_mc(RUN3C)
    run4w_sp_mc = make_qsub_spline_mc(RUN4W)
    run4a_sp_mc = make_qsub_spline_mc(RUN4A)
    run5w_sp_mc = make_qsub_spline_mc(RUN5W)

    submit(run2w_sp_mc, False)
    submit(run2a_sp_mc, False)
    submit(run3b_sp_mc, False)
    submit(run3c_sp_mc, False)
    submit(run4w_sp_mc, False)
    submit(run4a_sp_mc, False)
    submit(run5w_sp_mc, False)


def submit_spline_data():
    """submits data spline jobs"""
    run2w_sp_data = make_qsub_spline_data(RUN2W)
    run2a_sp_data = make_qsub_spline_data(RUN2A)
    run3b_sp_data = make_qsub_spline_data(RUN3B)
    run3c_sp_data = make_qsub_spline_data(RUN3C)
    run4w_sp_data = make_qsub_spline_data(RUN4W)
    run4a_sp_data = make_qsub_spline_data(RUN4A)
    run5w_sp_data = make_qsub_spline_data(RUN5W)

    submit(run2w_sp_data, True)
    submit(run2a_sp_data, True)
    submit(run3b_sp_data, True)
    submit(run3c_sp_data, True)
    submit(run4w_sp_data, True)
    submit(run4a_sp_data, True)
    submit(run5w_sp_data, False)


def submit_ft_data():
    """submits data ft jobs"""
    run2w_ft_data = make_qsub_flattree_data(RUN2W)
    run2a_ft_data = make_qsub_flattree_data(RUN2A)
    run3b_ft_data = make_qsub_flattree_data(RUN3B)
    run3c_ft_data = make_qsub_flattree_data(RUN3C)
    run4w_ft_data = make_qsub_flattree_data(RUN4W)
    run4a_ft_data = make_qsub_flattree_data(RUN4A)
    run5w_ft_data = make_qsub_flattree_data(RUN5W)

    submit(run2w_ft_data, True)
    submit(run2a_ft_data, True)
    submit(run3b_ft_data, True)
    submit(run3c_ft_data, True)
    submit(run4w_ft_data, True)
    submit(run4a_ft_data, True)
    submit(run5w_ft_data, False)


if __name__ == "__main__":
    main(sys.argv[1:])
