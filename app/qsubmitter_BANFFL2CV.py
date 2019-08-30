#!/bin/env python2.7

import argparse
import sys
from batchq import univa


def main(argv):
    len(argv)
    parser = argparse.ArgumentParser(description='Submitting jobs to perform\
     cross-validation on the BANFF L2 fit')
    parser.add_argument('-l',
                        '--lambda',
                        type=float,
                        help='Tuning parameter in L2 regularization',
                        dest='lambda')
    parser.add_argument('-s',
                        '--submission',
                        help='Directory to store scripts.\
                         Also submission/submission.txt \
    saves the outputs of the cross validation',
                        dest='submission')
    args = parser.parse_args()
    print args
    return


class JobScript(univa):
    """A multiple purpose class to create job submission files"""

    def __init__(self, filename):
        super(JobScript, self).__init__()
        """
        Store the job options
        """
        self.add_qoption('wd', '-wd',
                         usr_input='%s/inputs' % self.banffroot)
        self.add_qoption('output', '-o',
                         usr_input='%s/Linux-x86_64' % self.banffroot)
        self.add_qoption('OMP_NUM_THREADS',
                         '-v OMP_NUM_THREADS=',
                         usr_input='16')
        self.add_qoption('pe',
                         '-pe OpenMP',
                         usr_input='16')


if __name__ == '__main__':
    main(sys.argv)
