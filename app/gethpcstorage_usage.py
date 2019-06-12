#!/usr/bin/env python2.7
"""
This program uses the GANGLIA website for the CSU ENS-HPC
cluster to determine if the hpc-storage server (a proxy for
the cluster disk usage) is being over-burdened. By default,
if the normalized usage (n processes / n cpus) is over USAGE_MAX,
then the program sleeps for a random time proporational to the usage
and check again. Once the load is down, the program returns

USAGE: ./gethpcstorage_usage.py --help
"""
import datetime
import optparse
import requests
import time
import random
import signal
import sys

USAGE_MAX = 1.00


def signal_handler(signal, frame):
    """If you wish to quit using Ctrl-c, this exits the program"""
    print '\nExiting...'
    sys.exit(0)


def gethpcstorage_usage(diff_time=600):
    """
    Returns the current CSU ENS-HPC 'hpc-storage' usage (nprocesses/cores)
    diff_time in units of seconds
    """
    # This is the ganglia URL to get CSV data
    url = 'http://ens-hpc.engr.colostate.edu/ganglia/graph.php\
?r=hour&h=hpc-storage&m=load_one&s=by+name&mc=2&g=load_report&c=ens-hpc&csv=1'
    csv_contents = list()
    iterations = 1
    while iterations < 4:
        try:
            urlrequest = requests.get(url, allow_redirects=True, timeout=10)
            csv_contents = urlrequest.content.split('\n')
            iterations += 1
            if type(csv_contents) is list and len(csv_contents) > 2:
                break
        except requests.RequestException as req_exp:
            print str(req_exp)
            print 'Unable to complete process, trying again...'
            time.sleep(5)
    if len(csv_contents) < 2:
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
    first_entry = csv_contents[0].split(',')
    ncpus = int(first_entry[ncpu_column])
    if ncpus <= 0:
        print 'ERROR: Unable to determine number of CPUs'
        return 0

    latest_entry = csv_contents[len(csv_contents)-1].split(',')
    while latest_entry[ncpu_column] == 0:
        del csv_contents[len(csv_contents)-1]
        latest_entry = csv_contents[len(csv_contents)-1].split(',')

    latest_time = latest_entry[time_column]

    # should already be sorted, but to be safe
    csv_contents = sorted(csv_contents)
    last_loads = list()
    n_entries = 20  # 5 minute averge, entries updated every 15 sec (4 times / min * 5 min)
    for index in range(len(csv_contents)-1,
                       min(len(csv_contents)-1-n_entries,
                           len(csv_contents)-1),
                       -1):
        entry = csv_contents[index].split(',')
        load = float(entry[load_column])
        last_loads.append(load)
    del csv_contents
    load_mean = sum(last_loads) / len(last_loads)
    del last_loads

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

    normalized_load = load_mean / ncpus

    print 'The load at', str(now), 'is', normalized_load
    return normalized_load


def main(argv):

    signal.signal(signal.SIGINT, signal_handler)
    parser = optparse.OptionParser()
    parser.add_option('-d', '--diff', dest='diff',
                      default=600,  # in seconds
                      help='The time difference in seconds between call\
and last recorded usage')
    parser.add_option('-s', '--sleep', dest='sleep',
                      default=60,  # in seconds
                      help='The time in seconds between checking usage')
    parser.add_option('-l', '--load', dest='load',
                      default=USAGE_MAX,  # in processes / cpus
                      help='The normalized load (N procs/CPUs) to wait for')
    parser.add_option('-q', '--quit', dest='quit',
                      action='store_true', default=False,
                      help='Quit after one check')
    options, args = parser.parse_args()
    if len(args) > 0:
        print 'ERROR: Unaccepted input'
        parser.print_help()
        return 0
    try:
        if int(options.sleep) < 0:
            options.sleep = 60
        if int(options.diff) < 0:
            options.diff = 600
        load_max = float(options.load)
    except ValueError as val_err:
        print str(val_err)
        parser.print_help()
        return 1

    load = gethpcstorage_usage(options.diff)
    if options.quit:
        return 0
    while load >= load_max:
        rand = random
        seed = int(time.time()*1e6)
        rand.seed(seed)
        sleeptime = options.sleep * load * rand.uniform(1.5, 2.0)
        print 'The load is too high, waiting for', sleeptime, 'seconds'
        # if the load is high, high sleep time, also inserts randomness
        time.sleep(sleeptime)
        load = gethpcstorage_usage(options.diff)
    print 'Proceeding...'
    return 0


if __name__ == "__main__":
    main(sys.argv[1:])
