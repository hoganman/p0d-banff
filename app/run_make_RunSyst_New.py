#!/usr/bin/env python
"""submits the mc flattree files to run RunSyst_New.exe"""

import os
from os.path import join
import sys
from Directory import Directory
from ShellCommand import ShellCommand
import RunName as RN

P0DBANFF = os.getenv('P0DBANFFROOT')
QUEUE = '\"physics.q|short.q\"'
HOSTS = '\"node42|node27\"'
# EXCLUDEHOSTS = [6, 7, 10, 19, 45]
# HOSTS = '\"'
# for x in range(1, 20) + range(27, 31):  # + range(40, 46):
#     if x in EXCLUDEHOSTS:
#         continue
#     HOSTS += 'node{}|'.format(x)
# HOSTS = HOSTS.rstrip("|")
# HOSTS += '\"'
MEM = '1024'
NEUT = 'NEUT'
MCMIN = '60'
DATAMIN = '12'
SAND = 'mcp6_Spin_B/sand'
FLATTREEBASE = os.getenv('FLATTREEROOT')
RUNSYSTNEWBASE = join(os.getenv('SYSTEMATICSROOT'), 'addFDG')
NEUT_6B = 'mcp6_Spin_B/neut'
NEUT_6L = 'mcp6_Spin_L/neut'
DATA_6M = 'rdp6_Spin_M'
DATA_6N = 'rdp6_Spin_N'
QSUBRUNSYSTNEW = 'python qsubmitter_RunSyst_New.py'


def main(argv):
    """submits all the qsub python scripts"""
    submit_runsyst_new_mc()
    submit_runsyst_new_data()


def make_qsub(run_name, production, is_mc):
    """makes a MC qsubmitter.py command class
    for a particular RN class input"""
    flattree_dir = Directory(join(FLATTREEBASE,
                             production, run_name.low()))
    if not flattree_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (flattree_dir.get())
        return None
    flattree_dir_list = '-L %s' % (flattree_dir.get())
    queue = '-q %s' % (QUEUE)
    hosts = '-N %s' % (HOSTS)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (RUNSYSTNEWBASE,
                                               production, run_name.low()))
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
    this_run.add(hosts)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(output_name)
    this_run.add(run_type)
    return this_run


def submit_runsyst_new_mc():
    """submits MC RunSyst_New.exe jobs"""

    is_mc = True
    run2w_mc = make_qsub(RN.RUN2W, NEUT_6B, is_mc)
    run2a_mc = make_qsub(RN.RUN2A, NEUT_6B, is_mc)
    run3b_mc = make_qsub(RN.RUN3B, NEUT_6B, is_mc)
    run3c_mc = make_qsub(RN.RUN3C, NEUT_6B, is_mc)
    run4w_mc = make_qsub(RN.RUN4W, NEUT_6B, is_mc)
    run4a_mc = make_qsub(RN.RUN4A, NEUT_6B, is_mc)
    run5c_mc = make_qsub(RN.RUN5C, NEUT_6B, is_mc)
    run6b_mc = make_qsub(RN.RUN6B, NEUT_6B, is_mc)
    run6c_mc = make_qsub(RN.RUN6C, NEUT_6B, is_mc)
    run6d_mc = make_qsub(RN.RUN6D, NEUT_6B, is_mc)
    run6e_mc = make_qsub(RN.RUN6E, NEUT_6B, is_mc)
    run7b_mc = make_qsub(RN.RUN7B, NEUT_6L, is_mc)
    sand_fhc_mc = make_qsub(RN.SANDFHC, SAND, is_mc)
    sand_rhc_mc = make_qsub(RN.SANDRHC, SAND, is_mc)

    run2w_mc.run(ShellCommand.IN_BKG)
    run2a_mc.run(ShellCommand.IN_BKG)
    run3b_mc.run(ShellCommand.IN_BKG)
    run3c_mc.run(ShellCommand.IN_BKG)
    run4w_mc.run(ShellCommand.IN_BKG)
    run4a_mc.run(ShellCommand.IN_BKG)
    run5c_mc.run(ShellCommand.IN_BKG)
    run6b_mc.run(ShellCommand.IN_BKG)
    run6c_mc.run(ShellCommand.IN_BKG)
    run6d_mc.run(ShellCommand.IN_BKG)
    run6e_mc.run(ShellCommand.IN_BKG)
    run7b_mc.run(ShellCommand.IN_BKG)
    sand_fhc_mc.run(ShellCommand.IN_BKG)
    sand_rhc_mc.run(ShellCommand.IN_BKG)


def submit_runsyst_new_data():
    """submits data RunSyst_New.exe jobs"""

    is_mc = True
    run2w_data = make_qsub(RN.RUN2WDATA, DATA_6M, not is_mc)
    run2a_data = make_qsub(RN.RUN2ADATA, DATA_6M, not is_mc)
    run3b_data = make_qsub(RN.RUN3BDATA, DATA_6M, not is_mc)
    run3c_data = make_qsub(RN.RUN3CDATA, DATA_6M, not is_mc)
    run4w_data = make_qsub(RN.RUN4WDATA, DATA_6M, not is_mc)
    run4a_data = make_qsub(RN.RUN4ADATA, DATA_6M, not is_mc)
    run5c_data = make_qsub(RN.RUN5CDATA, DATA_6M, not is_mc)
    run6b_data = make_qsub(RN.RUN6BDATA, DATA_6M, not is_mc)
    run6c_data = make_qsub(RN.RUN6CDATA, DATA_6M, not is_mc)
    run6d_data = make_qsub(RN.RUN6DDATA, DATA_6M, not is_mc)
    run6e_data = make_qsub(RN.RUN6EDATA, DATA_6M, not is_mc)
    run7b_data = make_qsub(RN.RUN7BDATA, DATA_6N, not is_mc)

    run2w_data.run(ShellCommand.IN_BKG)
    run2a_data.run(ShellCommand.IN_BKG)
    run3b_data.run(ShellCommand.IN_BKG)
    run3c_data.run(ShellCommand.IN_BKG)
    run4w_data.run(ShellCommand.IN_BKG)
    run4a_data.run(ShellCommand.IN_BKG)
    run5c_data.run(ShellCommand.IN_BKG)
    run6b_data.run(ShellCommand.IN_BKG)
    run6c_data.run(ShellCommand.IN_BKG)
    run6d_data.run(ShellCommand.IN_BKG)
    run6e_data.run(ShellCommand.IN_BKG)
    run7b_data.run(ShellCommand.IN_BKG)


if __name__ == "__main__":
    main(sys.argv[1:])
