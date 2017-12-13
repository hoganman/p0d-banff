#!/usr/bin/env python
"""Checks if the POT from flattree (pre-T2KRW) or spline (post-T2KRW) file
    matches the official POT and hardcoded cut-off in ND280AnalysisUtils.cxx
"""

import sys
import os
import ROOT


def main(argv):
    """main function"""
    p0dbanff = os.getenv('P0DBANFFROOT')
    ROOT.gSystem.Load(os.path.join(p0dbanff, 'lib/libP0DBANFF.so'))
    print argv[0]


if __name__ == '__main__':
    main(sys.argv)
