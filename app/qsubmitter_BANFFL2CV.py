#!/bin/env python2.7

import argparse
import p0dbanff
import os
from os.path import join
from batchq import univa
from TextFile import ReadTextFile, WriteTextFile


def main():
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

        # training override
        self.parameter_override_training = TrainTestSet.TrainingParameterOverride(args, set_index)
        self.parameter_override_training.filename += '.training'

        # Training prefit override
        self.parameter_override_save_prefit = TrainTestSet.TrainingParameterOverride(args, set_index)
        self.parameter_override_save_prefit.minutia = self.parameter_override_save_prefit.minutia.replace("BANFF.SavePrefitParamsAndExit = 0", "BANFF.SavePrefitParamsAndExit = 1")
        self.parameter_override_save_prefit.filename += '.saveprefit'

        # testing postfit override
        self.parameter_override_testing = TrainTestSet.TestingParameterOverride(args, set_index)
        self.parameter_override_testing.filename += '.testing'

        self.job_dict = dict(
                [('args', args),
                 ('set_index', set_index),
                 ('training_override', self.parameter_override_training),
                 ('testing_override', self.parameter_override_testing),
                 ('saveprefit_override', self.parameter_override_save_prefit)])

        self.jobscript = TrainTestSet.JobScript(self.job_dict)

        # now that we have created job script with updated names
        # write the parameter overrides
        self.parameter_override_training.write_and_close()
        self.parameter_override_save_prefit.write_and_close()
        self.parameter_override_testing.write_and_close()

    def submit_job(self):
        # os.system('qsub %s' % self.jobscript.filename)
        pass

    class JobScript(univa):
        """A class to create job submission files for the Univa scheduler"""

        def __init__(self, job_dict):
            super(TrainTestSet.JobScript, self).__init__()
            """
            Store the job options
            """
            self.add_qoption('wd', '-wd ',
                             usr_input=join(self.banffroot, 'inputs'))
            self.add_qoption('output', '-o ',
                             usr_input=join(os.getcwd(),
                                            job_dict['args'].submission,
                                            CrossValidation.STDOUT))
            self.add_qoption('OMP_NUM_THREADS', '-v OMP_NUM_THREADS=',
                             usr_input='16')
            self.add_qoption('pe', '-pe OpenMP ',
                             usr_input='16')
            self.filename = join(os.getcwd(),
                                 job_dict['args'].submission,
                                 CrossValidation.JOBSCRIPTS,
                                 'submit_%s_set_%d.sh' % (job_dict['args'].submission, job_dict['set_index']))

            joint_fit_app = join('$BANFFROOT/Linux-x86_64',
                                 'RunJointFit2018.exe')
            apply_fit_app = join('$BANFFROOT/Linux-x86_64',
                                 'ApplyPostfitToNomMC.exe')
            print 'Creating job script %s' % self.filename
            self.script = WriteTextFile(self.filename)
            self.script.write('#!/bin/env bash\n')
            for qopt in self.qoptions:
                self.script.write(str('%s\n' % qopt))
            self.script.write('\n')
            self.script.write('source %s\n' % self.source_script)
            self.script.write('%s %s\n' % (joint_fit_app, job_dict['training_override'].filename))
            self.script.write('%s %s\n' % (joint_fit_app, job_dict['saveprefit_override'].filename))
            postfitfile = job_dict['training_override'].applypostfitinput
            prefitfile = postfitfile.replace('MultiTrackP0D', 'prefit_MultiTrackP0D')
            postfit2018file = postfitfile.replace('MultiTrackP0D', 'Make2018Postfit_MultiTrackP0D')
            self.script.write('$BANFFROOT/Linux-x86_64/Make2018PostFitFileAllParams.exe %s FittedParameters0 FittedCovariance0 $BANFFROOT/inputs/flux_covariance_banff_13av2.root %s FittedParameters0 FittedCovariance0 %s\n' % (prefitfile, postfitfile, postfit2018file))
            job_dict['testing_override'].applypostfitinput = postfit2018file
            self.script.write('%s %s\n' % (apply_fit_app, job_dict['testing_override'].filename))
            self.script.write('$P0DBANFFROOT/app/CalculateAndStoreDeltaChi2.py -s %d -r %f -T %s -t %s \
-o %s/results.txt\n' % (job_dict['set_index'],
                        job_dict['args'].regularization,
                        job_dict['testing_override'].applypostfitoutput,
                        job_dict['training_override'].applypostfitinput,
                        join(os.getcwd(), job_dict['args'].submission, CrossValidation.RESULTS)))
            self.script.close()

    class ParameterOverride(p0dbanff.p0dbanff):
        """Create a parameter override file for BANFF operations"""

        def __init__(self, args, set_index):
            super(TrainTestSet.ParameterOverride, self).__init__()
            self.set_index = set_index
            self.filename = join(os.getcwd(),
                                 args.submission,
                                 CrossValidation.OVERRIDE,
                                 '%s_set_%d.override' % (args.name, set_index))
            self.regularization = args.regularization
            self.fitoutput = join(os.getcwd(),
                                  args.submission,
                                  CrossValidation.OUTPUTS,
                                  '%s_DataFit_set_%s.root' % (args.name, set_index))
            self.applypostfitinput = join(os.getcwd(),
                                          args.submission,
                                          CrossValidation.OUTPUTS,
                                          'MultiTrackP0D_P0DOnly_Fit_v3r32_%s_DataFit_set_%d.root' % (args.name, set_index))
            self.applypostfitoutput = join(os.getcwd(),
                                           args.submission,
                                           CrossValidation.OUTPUTS,
                                           'ApplyPostfit_%s_DataFit_set_%d.root' % (args.name, set_index))
            self.minutia = open('%s/parameters/PODOnlyBANFF.override' % self.banffroot).read()

    class TrainingParameterOverride(ParameterOverride):
        """Create a parameter override file for BANFF training"""
        def __init__(self, args, set_index):
            super(TrainTestSet.TrainingParameterOverride, self).__init__(args, set_index)

        def write_and_close(self):
            print 'Creating file %s' % self.filename
            override_file = WriteTextFile(self.filename)
            override_file.write("""
========= CRITICAL PARAMETERS FOR CV ============
< BANFF.RunFit.Regularization = %f >
< BANFF.RunFit.OutputFile = %s >
< BANFF.ApplyPostfitToNomMC.InputFile = %s >
< BANFF.ApplyPostfitToNomMC.OutputFile = %s >
< BANFF.InputFileList = %s/inputs/CrossValidation/set%d/run_periods_data_mc_sand_CrossValidationTrainingSet_%d.txt >

""" % (self.regularization, self.fitoutput, self.applypostfitinput, self.applypostfitoutput, self.banffroot, self.set_index, self.set_index))
            override_file.write(self.minutia)
            override_file.close()

    class TestingParameterOverride(ParameterOverride):
        """Create a parameter override file for BANFF training"""
        def __init__(self, args, set_index):
            super(TrainTestSet.TestingParameterOverride, self).__init__(args, set_index)

        def write_and_close(self):
            print 'Creating file %s' % self.filename
            override_file = WriteTextFile(self.filename)
            override_file.write("""
========= CRITICAL PARAMETERS FOR CV ============
< BANFF.RunFit.Regularization = %f >
< BANFF.RunFit.OutputFile = %s >
< BANFF.ApplyPostfitToNomMC.InputFile = %s >
< BANFF.ApplyPostfitToNomMC.OutputFile = %s >
< BANFF.InputFileList = %s/inputs/CrossValidation/set%d/run_periods_data_mc_sand_CrossValidationTestingSet_%d.txt >

""" % (self.regularization, self.fitoutput, self.applypostfitinput, self.applypostfitoutput, self.banffroot, self.set_index, self.set_index))
            override_file.write(self.minutia)
            override_file.close()

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
            TrainTestSet(args, set_index).submit_job()


if __name__ == '__main__':
    main()
