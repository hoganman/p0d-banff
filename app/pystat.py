#!/bin/env python2


import sys
import optparse
import os

# globals
NNODES = 46
FREEUSENODES = range(1, 20) + range(27, 31)
PHYSICSNODES = range(40, 46)
ALLNODES = range(1, 46)
FREEUSEQUEUENAMES = ['defaultfaculty', 'physics', 'short']
"""
A dictionary with NODES in the range node1 - node43 excluding those NOT listed
in FREEUSE and PHYSICSNODES
"""
NODES = dict()
NON_BATCHQ_USERS_DIR = ['local', 'aspensys', 'Projects', 'test']


class computeNode(object):
    """A compute node for a batch queue"""

    def __init__(self, name, nCores=12):
        self.name = name
        self.nCores = nCores
        # state of the node, the empty string is good
        self.state = str()
        self.nJobs = 0

    def GetState(self):
        """return the state string, the empty string is good"""
        return self.state

    def SetState(self, state):
        """Set the state of the node, the empty string is good"""
        self.state = str(state)

    def SetNCores(self, nCores):
        """Set the number of cores this node has"""
        self.nCores = nCores

    def SetNJobs(self, nJobs):
        """set the number of jobs the node has currently"""
        self.nJobs = nJobs

    def GetNFreeCores(self):
        """Take the difference of cores and jobs"""
        return max(0, self.nCores - self.nJobs)


PHYSICSQUEUENODE = computeNode('physics.q', 16)
SHORTQUEUENODE = computeNode('short.q', 12)
LONGQUEUENODE = computeNode('long.q', 12)
DEFAULTFACULTYQUEUENODE = computeNode('defaultfaculty.q', 12)


def GetOutputFromCommand(command, splitBy=''):
    out = os.popen(command).read()
    if out.find('error') != -1:
        if len(splitBy) > 0:
            return []
        else:
            return ''
    if len(splitBy) > 0:
        out = out.split(splitBy)
    return out


def InitNodes():
    global NODES
    for i in range(1, NNODES+1):
        if i not in FREEUSENODES and i not in PHYSICSNODES:
            continue
        nodeName = 'node'+str(i)
        node = computeNode(nodeName)
        if i in PHYSICSNODES:
            node.SetNCores(16)
        NODES[nodeName] = node
    currentJobs = GetOutputFromCommand('qstat -f', '\n')
    for line in currentJobs:
        # print line
        if line.find('.q@') == -1:
            continue
        index = line.find('@')
        queueName = line[0:line.find('.q')].strip()
        if queueName not in FREEUSEQUEUENAMES:
            # print queueName+' is not a good queue'
            continue
        while line[index] != ' ':
                index += 1
        index -= 1
        hostName = line[line.find('@')+1:index+2].strip()
        if hostName not in NODES.keys():
            # print hostName+' not found. continuing'
            continue
        # jobsAndSlots is a list of three string integers representing
        # reserverd, jobs, slots
        jobsAndSlots = None
        tmpStr = line.split('BIP  ')[1]
        if '.' in tmpStr:
            tmpStr.split('.')[0]
            jobsAndSlots = tmpStr[0:len(tmpStr)-5].strip().split('/')
            jobsAndSlots[2] = jobsAndSlots[2][0:3].strip()
        elif '-NA-' in tmpStr:
            tmpStr.split('-NA-')[0]
            jobsAndSlots = tmpStr[0:len(tmpStr)-5].strip().split('/')
            jobsAndSlots[2] = jobsAndSlots[2][0:3].strip()
        else:
            jobsAndSlots = ['0', '0', '0']
        state = line.split('lx-amd64')[1].strip()
        # reserved = jobsAndSlots[0]
        jobs = int(jobsAndSlots[1])
        slots = int(jobsAndSlots[2])
        # print hostName
        # print 'slots = '+str(slots)
        # print 'jobs = '+str(jobs)
        node = NODES[hostName]
        if node.nCores != slots:
            node.SetNCores(slots)
        node.SetNJobs(jobs)
        # only append the node state if not set already
        if len(state) > 0:
            if len(node.GetState()) == 0:
                node.SetState(state)
            if state not in node.GetState():
                node.SetState(node.GetState()+','+state)


def GetJobsFromUser(username, qstatOutput=[]):
    if len(qstatOutput) == 0:
        qstatOutput = GetOutputFromCommand('qstat -u %s' % (username), '\n')
    userJobs = {'queued': 0, 'slots': 0, 'errors': 0,  'suspended': 0,
                'transferring': 0}
    for line in qstatOutput:
        if line.find(username) == -1:
            continue
        processLine = False
        # exclude non-accepted queues
        for free_use_queue in FREEUSEQUEUENAMES:
            if free_use_queue in line:
                processLine = True
        if not processLine:
            continue
        skipJob = True
        for node_num in ALLNODES:
            nodeName = 'node%d ' % (node_num)
            if nodeName in line:
                skipJob = False
                break
        if skipJob:
            continue
        if ' qw ' in line:
            userJobs['queued'] += 1
            continue
        if ' Eqw ' in line or ' Er ' in line or ' E ' in line:
            userJobs['errors'] += 1
            continue
        if ' t ' in line:
            userJobs['transferring'] += 1
        splitLine = None
        line = line.strip()
        if not (' r ' in line or ' S ' in line or ' s ' in line):
            continue
        splitLine = line[:].split()
        while '' in splitLine:
            splitLine.remove('')
        # print splitLine
        for char in splitLine:
            if char == 'r':
                userJobs['slots'] += int(splitLine[len(splitLine)-1])
                continue
            if char == 'S' or char == 's':
                userJobs['suspended'] += int(splitLine[len(splitLine)-1])
                continue
    return userJobs


def PrintNodeInfo(nodeName):
    """The verbose output which makes a table"""
    if nodeName not in NODES.keys():
        return
    if len(NODES[nodeName].GetState()) > 0:
        print '| %6s |  --   |' % (nodeName)
        return
    aNode = NODES[nodeName]
    print '| %6s |  %2d   |' % (nodeName, aNode.GetNFreeCores())


def main(argv):

    parser = optparse.OptionParser()

    parser.add_option('-v', '--verbose', dest="verbose", default=False,
                      action="store_true",
                      help="Shows all the free use nodes usage")
    parser.add_option('-q', '--quiet', dest="quiet", default=False,
                      action="store_true",
                      help="Only prints the number of free use nodes")
    parser.add_option('-u', '--users', dest="users", default=False,
                      action="store_true", help="Show all users jobs")
    parser.add_option('-U', '--User', dest="User",
                      help="Get the verbose output for <User>")

    runHost = os.popen('hostname').read().split('\n')[0]
    if runHost.find('node') != -1:
        print 'ERROR: %s is not a suitable batch submission node' % (runHost)
        sys.exit(1)

    try:
        options, args = parser.parse_args()
    except optparse.OptParseError:
        print 'ERROR: Unable to parse options and arguments'
        print 'Printing help statement'
        print parser.help_msg()
        sys.exit(1)

    # print 'init NODES'
    InitNodes()
    # print 'done'

    showHosts = False
    showUsers = False
    if not options.quiet:
        if options.verbose:
            showHosts = True
        if options.users:
            showUsers = True

    availableSlots = 0
    for aNode in NODES.values():
        if len(aNode.GetState()) > 0:
            continue
        availableSlots += aNode.GetNFreeCores()
    if showHosts:
        print '=================='
        print '|  Name  | Slots |'
        print '|        | Open  |'
        print '=================='
        for i in range(1, NNODES+1):
            if i not in FREEUSENODES and i not in PHYSICSNODES:
                continue
            nodeName = 'node'+str(i)
            PrintNodeInfo(nodeName)
    if showUsers:
        groups_users_dict = GetUsersFromHome()

        qstatOutput = GetOutputFromCommand('qstat -u \"*\"', '\n')
        print '================================================================='
        print '| Username  |  Slots  | Transfers | Queued | Errors | Suspended |'
        print '================================================================='
        for group in groups_users_dict:
            # print group
            for user in groups_users_dict[group]:
                userJobs = GetJobsFromUser(user, qstatOutput)
                if userJobs['slots'] == 0 \
                        and userJobs['queued'] == 0 \
                        and userJobs['transferring'] == 0 \
                        and userJobs['errors'] == 0 \
                        and userJobs['suspended'] == 0:
                    continue
                # print 'user=', user
                # print group
                # print user
                template = '| %9s | %7d | %9d | %6d | %6d | %9d |'
                print template % (user, userJobs['slots'], userJobs['queued'],
                                  userJobs['transferring'], userJobs['errors'],
                                  userJobs['suspended'])

    if options.quiet:
        print availableSlots
    else:
        print 'There are a total of %d job slots available' % availableSlots
    sys.exit(0)


def GetUsersFromHome():
    """
    Returns dict. value = list of users, key = user associated group
    Determines all the users with a home directory. Returns a dict where the
    key is the directory beneath home ie /home/<key>/. The value is a list of
    users like /home/<key>/<user>.
    """
    user_groups = os.listdir('/home')
    group_users_dict = dict()
    for user_group in user_groups:
        full_directory = os.path.join('/home', user_group)
        if not os.path.isdir(full_directory):
            continue
        is_banned_dir = False
        for banned_dir in NON_BATCHQ_USERS_DIR:
            if banned_dir in full_directory:
                is_banned_dir = True
                break
        if is_banned_dir:
            continue
        users = os.listdir(full_directory)
        for test_user in users:
            if not os.path.isdir(os.path.join(full_directory, test_user)):
                users.remove(test_user)
        group_users_dict[user_group] = users
    return group_users_dict


if __name__ == "__main__":
    main(sys.argv[1:])
