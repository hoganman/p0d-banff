#!/usr/bin/env python
import sys
import getopt
import qsubmitter2


def usage(error_msg, help_msg, exit_status=1):
    print error_msg
    print help_msg
    sys.exit(1)


def main(argv):

    # the job management class
    ft_jobs = qsubmitter2.CreateFlattree_jobs()

    # no inputs given
    if len(argv) == 0:
        error_msg = 'No inputs given\nPrinting help message'
        usage(error_msg, ft_jobs.help_msg(), 0)

    # accepted inputs defined in flattree_jobs
    shortArgs = ft_jobs.make_getopt_options()
    longArgsList = ft_jobs.make_getopt_long_options()

    # try to parse arguments and options
    try:
        opts, args = getopt.getopt(argv, shortArgs, longArgsList)
    except getopt.GetoptError as optErr:
        error_msg = 'ERROR: %s\nPrinting help message' % str(optErr)
        usage(error_msg, ft_jobs.help_msg())

    # assign options and arguments to jobs
    ft_jobs.set_usr_inputs_from_getopt(opts, args)
    ft_jobs.check_inputs()

    # discontinue if something was not accepted
    if ft_jobs.is_help_set():
        error_msg = 'Printing help message'
        usage(error_msg, ft_jobs.help_msg())

    # move to a new directory to make submission scripts
    ft_jobs.set_qsubDir()
    if ft_jobs.qsubDir.exists():
        ft_jobs.qsubDir.cd()
    else:
        sys.exit(1)


if __name__ == "__main__":
    main(sys.argv[1:])
