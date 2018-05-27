#!/usr/bin/env python2.7
"""
This program uses the GANGLIA website for the CSU ENS-HPC
cluster to determine if the hpc-storage server (a proxy for
the cluster disk usage) is being over-burdened. By default,
if the normalized usage (n processes / n cpus) is over 1,
then the program sleeps for one minute to check again. Once
the load is down, the program returns

USAGE: ./gethpcstorage_usage.py --help
"""
import datetime
import optparse
import requests
import time
import sys


def gethpcstorage_usage(diff_time):
    """
    Returns the current CSU ENS-HPC 'hpc-storage' usage (nprocesses/cores)
    """
    # This is the ganglia URL to get CSV data
    url = 'http://ens-hpc.engr.colostate.edu/ganglia/graph.php\
?r=hour&h=hpc-storage&m=load_one&s=by+name&mc=2&g=load_report&c=ens-hpc&csv=1'
    csv_contents = list()
    try:
        urlrequest = requests.get(url, allow_redirects=True)
        csv_contents = urlrequest.content.split('\n')
        del urlrequest
    except requests.RequestException as req_exp:
        print str(req_exp)
        print 'Unable to complete process'
        return 0

    # this is the column where the current load is
    time_string = 'Timestamp'
    time_column = -1
    load_string = '1-min'
    load_column = -1
    ncpu_string = 'CPUs'
    ncpu_column = -1
    for entry in csv_contents:
        # we have the CSV header
        if time_string in entry:
            columns = entry.split(',')
            for index in range(len(columns)):
                column = columns[index]
                if time_string in column:
                    time_column = index
                    continue
                if load_string in column:
                    load_column = index
                    continue
                if ncpu_string in column:
                    ncpu_column = index
                    continue
            # we don't need the header anymore
            csv_contents.remove(entry)
            # break from looping over entries
            break
    if time_column < 0 or load_column < 0 or ncpu_column < 0:
        print 'ERROR: could NOT find CSV header'
        return 0
    del time_string, load_string, ncpu_string
    while '' in csv_contents:
        csv_contents.remove('')

    # should already be reverse sorted, but to be safe
    csv_contents = sorted(csv_contents)
    latest_entry = csv_contents[len(csv_contents)-1].split(',')
    first_entry = csv_contents[0].split(',')
    del csv_contents

    latest_load = float(latest_entry[load_column])
    latest_time = latest_entry[time_column]
    ncpus = int(first_entry[ncpu_column])

    timeformat = '%Y-%m-%dT%H:%M:%S'
    """
    Need to reformat time string to remove last terms
    2018-05-27T10:16:45-06:00 -> 2018-05-27T10:16:45
    """
    latest_time = latest_time[:latest_time.rindex('-')]
    then = datetime.datetime.strptime(latest_time, timeformat)
    now = datetime.datetime.now()
    delta_time = now - then

    if delta_time.total_seconds() > diff_time:
        print 'Warning: latest load is over', diff_time / 60., ' minutes ago.'

    normalized_load = latest_load / ncpus

    print 'The load at', str(now), 'is', normalized_load
    return normalized_load


def main(argv):

    parser = optparse.OptionParser()
    parser.add_option('-d', '--diff', dest='diff',
                      default=600,  # in seconds
                      help='The time difference in seconds between call\
and last recorded usage')
    parser.add_option('-s', '--sleep', dest='sleep',
                      default=60,  # in seconds
                      help='The time in seconds between checking usage')
    parser.add_option('-l', '--load', dest='load',
                      default=1.,  # in processes / cpus
                      help='The normalized load (N procs/CPUs) to wait for')
    options, args = parser.parse_args()
    if len(args) > 0:
        print 'ERROR: Unaccepted input'
        parser.print_help()
        return 0
    try:
        if int(options.sleep) < 0:
            options.sleep = 0
        if int(options.diff) < 0:
            options.diff = 0
    except ValueError as val_err:
        print str(val_err)
        parser.print_help()
        return 1

    load = gethpcstorage_usage(options.diff)
    while load >= 1.:
        print 'The load is too high, waiting for', options.sleep
        time.sleep(options.sleep)
        load = gethpcstorage_usage(options.diff)
    return 0


if __name__ == "__main__":
    main(sys.argv[1:])
