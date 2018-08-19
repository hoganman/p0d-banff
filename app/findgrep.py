#!/usr/bin/env python

"""
A simple utility to combine the execution of find and grep.
See the usage function for usage
"""

import sys
import os


def usage():
    """Tell the user what the inputs should be"""
    print "ERROR: Printing usage\n"
    print "findgrep [inputs]\n"
    print "  Three inputs is path specific find"
    print "     input 1: path (default = .)"
    print "           2: filenames"
    print "           3: grep string"

    print "  Two  inputs uses CWD as search path"
    print "     input 1: filenames"
    print "           2: grep string"


def findgrep(path, filename, grepstring):
    """The grep and find command"""
    command = 'find %s \( -name node_modules -prune \) -o -name \"%s\" -exec \
            grep --color -Hn \"%s\" {} 2>/dev/null \;'
    os.system(command % (path, filename, grepstring))


def main(argv):
    """Parses the user input"""
    if len(argv) != 3 and len(argv) != 2:
        usage()
        sys.exit(1)

    path = '.'
    if len(argv) == 3:
        path = argv[0]
        argv = argv[1:]

    filename = argv[0]
    grepstring = argv[1]

    findgrep(path, filename, grepstring)
    sys.exit(0)


if __name__ == "__main__":
    main(sys.argv[1:])
