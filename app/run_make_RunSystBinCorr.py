#!/usr/bin/env python
"""submits the mc flattree files to run RunSystBinCorr.exe"""

import os
from os.path import join
import sys
from Directory import Directory
from ShellCommand import ShellCommand
import RunName as RN

P0DBANFF = os.getenv('P0DBANFFROOT')
QUEUE = '\"physics.q|short.q\"'
HOSTS = '\"node40|node41|node42|node43|node44|node45|node27|node28|node29|node30\"'
# EXCLUDEHOSTS = [6, 7, 10, 19]
# HOSTS = '\"'
# for x in range(1, 20) + range(27, 31) + range(40, 46):
#     if x in EXCLUDEHOSTS:
#         continue
#     HOSTS += 'node{}|'.format(x)
# HOSTS = HOSTS.rstrip("|")
# HOSTS += '\"'

MEM = '1024'
NEUT = 'NEUT'
MCMIN = str(int(60*8))
SAND = 'mcp6_Spin_B/sand'
FLATTREEBASE = os.getenv('FLATTREEROOT')
RUNSYSTBINCORRBASE = os.getenv('BINCORRROOT')
NEUT_6B = 'mcp6_Spin_B/neut'
NEUT_6L = 'mcp6_Spin_L/neut'
QSUBRUNSYSTBINCORR = 'python qsubmitter_RunSystBinCorr.py'


def submit_runsyst_new_mc():
    """submits MC RunSyst_New.exe jobs"""

    run2w_mc = make_qsub(RN.RUN2W, NEUT_6B)
    run2a_mc = make_qsub(RN.RUN2A, NEUT_6B)
    run3b_mc = make_qsub(RN.RUN3B, NEUT_6B)
    run3c_mc = make_qsub(RN.RUN3C, NEUT_6B)
    run4w_mc = make_qsub(RN.RUN4W, NEUT_6B)
    run4a_mc = make_qsub(RN.RUN4A, NEUT_6B)
    run5c_mc = make_qsub(RN.RUN5C, NEUT_6B)
    run6b_mc = make_qsub(RN.RUN6B, NEUT_6B)
    run6c_mc = make_qsub(RN.RUN6C, NEUT_6B)
    run6d_mc = make_qsub(RN.RUN6D, NEUT_6B)
    run6e_mc = make_qsub(RN.RUN6E, NEUT_6B)
    run7b_mc = make_qsub(RN.RUN7B, NEUT_6L)
    run8w_mc = make_qsub(RN.RUN8W, NEUT_6L)
    run8a_mc = make_qsub(RN.RUN8A, NEUT_6L)
    sand_fhc_mc = make_qsub(RN.SANDFHC, SAND)
    sand_rhc_mc = make_qsub(RN.SANDRHC, SAND)

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
    run8w_mc.run(ShellCommand.IN_BKG)
    run8a_mc.run(ShellCommand.IN_BKG)
    sand_fhc_mc.run(ShellCommand.IN_BKG)
    sand_rhc_mc.run(ShellCommand.IN_BKG)


def main(argv):
    """submits all the qsub python scripts"""
    submit_runsyst_new_mc()


def make_qsub(run_name, production):
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
    output_path_name = Directory('%s/%s/%s' % (RUNSYSTBINCORRBASE,
                                               production, run_name.low()))
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'

    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBRUNSYSTBINCORR))
    this_run.add(flattree_dir_list)
    this_run.add(memory)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(minutes)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


class DATATYPES(object):
    DATA = 0
    MC = 1


if __name__ == "__main__":
    main(sys.argv[1:])
