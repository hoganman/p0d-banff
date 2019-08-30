#!/bin/env python2.7

import argparse
import sys
import p0dbanff
import os
from os.path import join
from batchq import univa
from TextFile import ReadTextFile, WriteTextFile


def main(argv):
    len(argv)
    parser = argparse.ArgumentParser(description='Submitting jobs to perform\
     cross-validation on the BANFF L2 fit')
    parser.add_argument('-r', '--regularization',
                        required=True,
                        type=float,
                        help='Tuning parameter in L2 regularization',
                        dest='regularization')
    parser.add_argument('-N', '--name',
                        help='Special name for the CV fits.\
    If none is given, will use the same name from the submission option',
                        dest='name', default='')
    parser.add_argument('-s', '--submission',
                        required=True,
                        help='Directory to store all scripts and\
    saves the outputs of the cross validation',
                        dest='submission')
    args = parser.parse_args()
    if len(args.name) == 0:
        args.name = args.submission
    print args
    CrossValidation(args)
    return


class TrainTestSet(p0dbanff.p0dbanff):
    """
    An individual instance of the train+test in cross validation.
    This class will handle creating the job script, parameter override,
    and submiting the job
    """

    def __init__(self, args, set_index):
        super(TrainTestSet, self).__init__()
        self.parameter_override = TrainTestSet.ParameterOverride(args, set_index)
        self.jobscript = TrainTestSet.JobScript(args,
                                                set_index,
                                                self.parameter_override.filename)

    class JobScript(univa):
        """A class to create job submission files for the Univa scheduler"""

        def __init__(self, args, set_index, parameter_override):
            super(TrainTestSet.JobScript, self).__init__()
            """
            Store the job options
            """
            self.add_qoption('wd', '-wd ',
                             usr_input=join(self.banffroot, 'inputs'))
            self.add_qoption('output', '-o ',
                             usr_input=join(os.getcwd(),
                                            args.submission,
                                            CrossValidation.STDOUT))
            self.add_qoption('OMP_NUM_THREADS', '-v OMP_NUM_THREADS=',
                             usr_input='16')
            self.add_qoption('pe', '-pe OpenMP ',
                             usr_input='16')
            self.filename = join(os.getcwd(),
                                 args.submission,
                                 CrossValidation.JOBSCRIPTS,
                                 'submit_%s_set_%d.sh' % (args.submission, set_index))

            joint_fit_app = join(self.banffroot, 'Linux-x86_64',
                                 'RunJointFit2018.exe')
            apply_fit_app = join(self.banffroot, 'Linux-x86_64',
                                 'ApplyPostfitToNomMC.exe')
            print 'Creating job script %s' % self.filename
            self.script = WriteTextFile(self.filename)
            self.script.write('#!/bin/env bash\n')
            for qopt in self.qoptions:
                self.script.write(str('%s\n' % qopt))
            self.script.write('\n')
            self.script.write('%s\n' % self.source_script)
            self.script.write('%s %s\n' % (joint_fit_app, parameter_override))
            self.script.write('%s %s\n' % (apply_fit_app, parameter_override))
            self.script.close()

    class ParameterOverride(WriteTextFile):
        """Create a parameter override file for BANFF operations"""

        def __init__(self, args, set_index):
            self.filename = join(os.getcwd(),
                                 args.submission,
                                 CrossValidation.OVERRIDE,
                                 '%s_set_%d.override' % (args.name, set_index))
            print 'Creatng file %s' % self.filename
            super(TrainTestSet.ParameterOverride, self).__init__(self.filename)
            self.regularization = args.regularization
            self.fitoutput = join(args.submission,
                                  CrossValidation.OUTPUTS,
                                  '%s_DataFit_set_%s.root' % (args.name, set_index))
            self.applypostfitinput = self.fitoutput
            self.applypostfitoutput = join(args.submission,
                                           CrossValidation.OUTPUTS,
                                           'ApplyPostfit_%s_DataFit_set_%d.root' % (args.name, set_index))
            self.minutia = open('%s/parameters/PODOnlyBANFF.override' % self.banffroot).read()
            self.write("""
========= CRITICAL PARAMETERS FOR CV ============
< BANFF.RunFit.Regularization = %f >
< BANFF.RunFit.OutputFile = %s >
< BANFF.ApplyPostfitToNomMC.InputFile = %s >
< BANFF.ApplyPostfitToNomMC.OutputFile = %s >
< BANFF.InputFileList = %s/inputs/CrossValidation/set%d/run_periods_data_mc_sand_CrossValidationSet_%d.txt >
""" % (self.regularization,
                       self.fitoutput,
                       self.applypostfitinput,
                       self.applypostfitoutput,
                       self.banffroot,
                       set_index, set_index))
            self.write(self.minutia)
            self.close()

    class TestResults(ReadTextFile):
        """A simple CVS file to store the results for each test set"""

        def __init__(self, args, set_index):
            super(TrainTestSet.TestResults, self).__init__()


class CrossValidation(p0dbanff.p0dbanff):
    """Creates the necessary directory structure and files for the jobs"""

    NSETS = 5
    INPUTS = 'inputs'
    OVERRIDE = 'override'
    RESULTS = 'results'
    STDOUT = 'stdout'
    JOBSCRIPTS = 'jobscripts'
    OUTPUTS = 'outputs'

    def __init__(self, args):
        super(CrossValidation, self).__init__()
        base_directory_name = args.submission
        if os.path.isdir(base_directory_name):
            print 'WARNING: directory of name \"%s\" already exists' % base_directory_name
        os.mkdir(base_directory_name)
        os.chdir(base_directory_name)
        os.mkdir(self.INPUTS)
        os.mkdir(self.OVERRIDE)
        os.mkdir(self.RESULTS)
        os.mkdir(self.STDOUT)
        os.mkdir(self.JOBSCRIPTS)
        os.system('ln -s -T %s %s' % (self.INPUTS, self.OUTPUTS))
        os.chdir('..')
        for set_index in range(1, CrossValidation.NSETS+1):
            train_test = TrainTestSet(args, set_index)
        os.chdir('..')


if __name__ == '__main__':
    main(sys.argv)
