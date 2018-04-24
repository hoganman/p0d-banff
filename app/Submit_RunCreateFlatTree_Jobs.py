#!/usr/bin/env python
import sys
import qsubmitter2


def main(argv):

    # the job management class
    ft_univa_jobs = qsubmitter2.CreateFlattree_univa_jobs()

    # no inputs given
    if len(argv) == 0:
        error_msg = 'No inputs given\nPrinting help message'
        print error_msg
        ft_univa_jobs.print_help()
        sys.exit(1)

    # try to parse arguments and options
    try:
        ft_univa_jobs.parse_args()
    except Exception as optErr:
        error_msg = 'ERROR: %s\nPrinting help message' % str(optErr)
        print error_msg
        ft_univa_jobs.print_help()
        sys.exit(1)

    # assign options and arguments to jobs
    ft_univa_jobs.check_CreateFlattree_univa_jobs_options()

    # discontinue if something was not accepted
    if ft_univa_jobs.show_usage:
        error_msg = 'Printing help message'
        print error_msg
        ft_univa_jobs.print_help()
        sys.exit(1)

    # move to a new directory to make submission scripts
    ft_univa_jobs.set_qsubDir()
    if ft_univa_jobs.qsubDir.exists():
        ft_univa_jobs.qsubDir.cd()
    else:
        sys.exit(1)


if __name__ == "__main__":
    main(sys.argv[1:])
