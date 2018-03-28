import os


class batchq(object):
    """info on the batch queue system"""

    def __init__(self, name='batchq'):
        """defines a batch queue with no users and no nodes"""
        self.name = name
        self.queues = {}  # a dict of nodes with associated names
        self.users = {}  # a dict of users
        self.n_nodes = 0
        self.n_users = 0

    def addnodes(self, assigned_name, node_numbers=[]):
        """adds node names (numbers) with assigned group name"""
        if len(node_numbers) == 0:
            print 'ERROR: No node numbers assigned'
        elif assigned_name in self.queues.keys():
            self.queues[assigned_name].extend(node_numbers)
        else:
            self.queues[assigned_name] = node_numbers
        self.n_nodes += len(node_numbers)

    def addusers(self, assigned_name, user_names=[]):
        """adds user names with assigned group name"""
        if len(user_names) == 0:
            print 'ERROR: No user names given'
        elif assigned_name in self.users.keys():
            self.users[assigned_name].extend(user_names)
        else:
            self.users[assigned_name] = user_names
        self.n_users += len(user_names)


class node(object):
    """a general computer """

    def __init__(self, name=''):
        self.name = name
        self.cores = 0

    def GetNCores(self):
        return self.cores

    def GetName(self):
        return self.name

    def GetNodeName(self):
        return self.name

    def SetNodeName(self, new_name):
        self.name = new_name

    def SetNCores(self, cores):
        self.cores = int(cores)

    def IsHeadNode(self):
        os.system('qstat > /dev/null 2> /dev/null')
        status = not bool(os.getenv('?'))
        return status


class computeNode(node):
    """ A compute node for a batch queue"""

    def __init__(self, name, opportunistic):
        super(node, self).__init__(name)
        self.opportunistic = opportunistic
        self.jobs = 0
        self.state = ''
        self.qname = ''

    def IsOpportunistic(self):
        return self.opportunistic

    def GetNJobs(self):
        return self.jobs

    def GetNFreeCores(self):
        return self.cores - self.jobs

    def GetState(self):
        return self.state

    def GetQueueName(self):
        return self.qname

    def SubmitJob(self):
        self.jobs = int(self.jobs) + 1

    def JobRunningAlready(self):
        self.jobs = int(self.jobs) + 1

    def SetState(self, state):
        self.state = str(state)

    def SetNJobs(self, jobs):
        self.jobs = int(jobs)

    def SetQueueName(self, queue):
        self.qname = queue


class gpuNode(computeNode):
    """An extension of a compute node with no different yet"""

    def __init__(self, name, opportunistic):
        super(gpuNode, self).__init__(name, opportunistic)
