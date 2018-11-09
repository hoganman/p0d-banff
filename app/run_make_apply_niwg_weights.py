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
HOSTS = '\"node40|node41|node42|node43|node45|node44|node27|node28|node30|node29\"'
# EXCLUDEHOSTS = [7, 11, 19, 29, 45]
# HOSTS = '\"'
# for x in range(20) + range(27, 31) + range(40, 46):
#     if x in EXCLUDEHOSTS:
#         continue
#     HOSTS += 'node{}|'.format(x)
# HOSTS = HOSTS.rstrip("|")
# HOSTS += '\"'
MEM = '1024'
MCMIN = str(int(8*60))
BINCORRBASE = os.getenv('BINCORRROOT')
NIWGWEIGHTSBASE = os.getenv('NIWGWEIGHTSROOT')
NEUT_6B = 'mcp6_Spin_B/neut'
NEUT_6L = 'mcp6_Spin_L/neut'
SAND = 'mcp6_Spin_B/sand'

QSUBNIWGWEIGHTS = 'python qsubmitter_niwg_weights.py'


def main(argv):
    """submits all the qsub python scripts"""
    submit_niwg_weights_jobs()


def submit_niwg_weights_jobs():
    """submits the MC and data niwg_weights jobs"""
    submit_niwg_weights_mc()


def make_qsub_niwg_weights_mc(run_name, production, data_type):
    """make niwg_weights command for mc files"""
    bincorr_dir = Directory('%s/%s/%s' % (BINCORRBASE,
                                          production,
                                          run_name.low()))
    if not bincorr_dir.exists():
        print 'ERROR: there is no directory'
        print '\"%s\"' % (bincorr_dir.get())
        return None
    bincorr_dir_list = '-L %s' % (bincorr_dir.get())
    queue = '-q %s' % (QUEUE)
    hosts = '-N %s' % (HOSTS)
    minutes = '-M %s' % (MCMIN)
    memory = '-m %s' % (MEM)
    output_path_name = Directory('%s/%s/%s' % (NIWGWEIGHTSBASE,
                                               production, run_name.low()))
    run_type = '-r %s' % (data_type)
    if not output_path_name.exists():
        print 'WARNING: directory'
        print '\"%s\"' % (output_path_name.get())
        print 'does not exist. Will create it now'
        # output_path_name.mkdir()
    output_path = '-p %s' % (output_path_name.get())
    output_name = '-o %s_niwg_weights' % (run_name.upper_case())

    this_run = ShellCommand('nohup %s' % (QSUBNIWGWEIGHTS))
    this_run.add(bincorr_dir_list)
    this_run.add(queue)
    this_run.add(hosts)
    this_run.add(memory)
    this_run.add(minutes)
    this_run.add(run_type)
    this_run.add(output_path)
    this_run.add(output_name)
    return this_run


def submit_niwg_weights_mc():
    """submits MC niwg_weights jobs"""
    run2w_nw_mc = make_qsub_niwg_weights_mc(RN.RUN2W, NEUT_6B, RUNTYPE.MC)
    run2a_nw_mc = make_qsub_niwg_weights_mc(RN.RUN2A, NEUT_6B, RUNTYPE.MC)
    run3b_nw_mc = make_qsub_niwg_weights_mc(RN.RUN3B, NEUT_6B, RUNTYPE.MC)
    run3c_nw_mc = make_qsub_niwg_weights_mc(RN.RUN3C, NEUT_6B, RUNTYPE.MC)
    run4w_nw_mc = make_qsub_niwg_weights_mc(RN.RUN4W, NEUT_6B, RUNTYPE.MC)
    run4a_nw_mc = make_qsub_niwg_weights_mc(RN.RUN4A, NEUT_6B, RUNTYPE.MC)
    run5c_nw_mc = make_qsub_niwg_weights_mc(RN.RUN5C, NEUT_6B, RUNTYPE.MC)
    run6b_nw_mc = make_qsub_niwg_weights_mc(RN.RUN6B, NEUT_6B, RUNTYPE.MC)
    run6c_nw_mc = make_qsub_niwg_weights_mc(RN.RUN6C, NEUT_6B, RUNTYPE.MC)
    run6d_nw_mc = make_qsub_niwg_weights_mc(RN.RUN6D, NEUT_6B, RUNTYPE.MC)
    run6e_nw_mc = make_qsub_niwg_weights_mc(RN.RUN6E, NEUT_6B, RUNTYPE.MC)
    run7b_nw_mc = make_qsub_niwg_weights_mc(RN.RUN7B, NEUT_6L, RUNTYPE.MC)
    sand_fhc_nw_mc = make_qsub_niwg_weights_mc(RN.SANDFHC, SAND, RUNTYPE.SAND)
    sand_rhc_nw_mc = make_qsub_niwg_weights_mc(RN.SANDRHC, SAND, RUNTYPE.SAND)

    run2w_nw_mc.run(not ShellCommand.IN_BKG)
    run2a_nw_mc.run(not ShellCommand.IN_BKG)
    run3b_nw_mc.run(not ShellCommand.IN_BKG)
    run5c_nw_mc.run(not ShellCommand.IN_BKG)
    run6b_nw_mc.run(not ShellCommand.IN_BKG)
    run6c_nw_mc.run(not ShellCommand.IN_BKG)
    run6d_nw_mc.run(not ShellCommand.IN_BKG)
    run6e_nw_mc.run(not ShellCommand.IN_BKG)
    run7b_nw_mc.run(not ShellCommand.IN_BKG)
    sand_fhc_nw_mc.run(not ShellCommand.IN_BKG)
    sand_rhc_nw_mc.run(not ShellCommand.IN_BKG)
    run3c_nw_mc.run(not ShellCommand.IN_BKG)
    run4w_nw_mc.run(not ShellCommand.IN_BKG)
    run4a_nw_mc.run(not ShellCommand.IN_BKG)


class RUNTYPE(object):
    """run types for genWeightFromPsycheRunSyst_new.exe"""
    MC = 'MC'
    SAND = 'Sand'


if __name__ == "__main__":
    main(sys.argv[1:])
