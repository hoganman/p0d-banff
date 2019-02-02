#!/usr/bin/env python

from os import walk
import SubmitJobsWithSleep

f = []
for (dirpath, dirnames, filenames) in walk('.'):
    f.extend(dirnames)
    break
for dirname in f:
    argv = 'SubJobsWithSleep.py %s' % dirname
    argv += ' 0 1 100'
    argv = argv.split(' ')
    SubmitJobsWithSleep.main(argv)
