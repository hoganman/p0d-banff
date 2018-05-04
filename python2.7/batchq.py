import p0dbanff
import os


class qoption(p0dbanff.p0dbanff):
    """a batch queue set of command options like walltime"""

    def __init__(self, search_tag, cmd_option, cmd_descript='',
                 usr_input='', cmd_arg=''):
        super(qoption, self).__init__()
        # how the class searches for the qoption
        # should be a len > 2 ie merge with no '-' and '--'
        self.search_tag = search_tag
        # what format batchq option is preceded by cmd_arg
        self.cmd_option = cmd_option
        # description of qoption for user
        self.cmd_descript = cmd_descript
        # what the user gave as input
        self.usr_input = usr_input
        self.cmd_arg = cmd_arg

    def __str__(self):
        """"""
        if not self.is_set():
            return str()
        out_str = '%s %s' % (self.cmd_arg, self.cmd_option)
        if type(self.usr_input) is str and len(self.usr_input) > 0:
            out_str += self.usr_input
        elif type(self.usr_input) is bool:
            return out_str
        else:
            return str()
        return out_str

    def is_set(self):
        """returns true if the usr_input is set"""
        if type(self.usr_input) is str and len(self.usr_input) > 0:
            return True
        elif type(self.usr_input) is bool:
            return self.usr_input
        else:
            return False


class batchq(p0dbanff.p0dbanff):
    """info on the batch queue system"""

    def __init__(self, name='batchq', cmd_arg='#$'):
        """defines a batch queue with no users and no nodes"""
        super(batchq, self).__init__()
        self.name = name
        self.queues = dict()  # a dict of nodes with associated names
        self.qoptions = list()
        self.cmd_arg = cmd_arg
        # self.users = {}  # a dict of users
        # self.n_nodes = 0
        # self.n_users = 0

    def check_batchq_qoptions(self):
        """make sure the set qoptions are not invalid"""
        for qopt in self.qoptions:
            if qopt.cmd_arg is not '#':
                error_msg = 'ERROR: %s does NOT ' % (qopt.search_tag)
                error_msg += 'have a proper cmd_arg'
                print error_msg
                self.set_help_opt(True)
                return

    def add_qoption(self, opt_search, batchq_cmdopt):
        """append the list of qoptions"""
        qopt = qoption(opt_search, batchq_cmdopt, self.cmd_arg)
        self.qoptions.append(qopt)

    def get_qoption(self, search_param):
        """search for all available options using str opt"""
        if type(search_param) is not str or len(search_param) < 1:
            print "ERROR: must search for a program qoption using a string"
        search_param = search_param.strip('-')
        search_param = search_param.rstrip('=')
        for qopt in self.qoptions:
            if search_param is qopt.search_tag:
                return qopt
        return None

    # def addnodes(self, assigned_name, node_numbers=[]):
    #     """adds node names (numbers) with assigned group name"""
    #     if len(node_numbers) == 0:
    #         print 'ERROR: No node numbers assigned'
    #     elif assigned_name in self.queues.keys():
    #         self.queues[assigned_name].extend(node_numbers)
    #     else:
    #         self.queues[assigned_name] = node_numbers
    #     self.n_nodes += len(node_numbers)

    # def addusers(self, assigned_name, user_names=[]):
    #     """adds user names with assigned group name"""
    #     if len(user_names) == 0:
    #         print 'ERROR: No user names given'
    #     elif assigned_name in self.users.keys():
    #         self.users[assigned_name].extend(user_names)
    #     else:
    #         self.users[assigned_name] = user_names
    #     self.n_users += len(user_names)


class univa(batchq):
    """The Univa Grid Engine batch queue"""

    def __init__(self, prog_name='Univa Grid Engine', cmd_arg='#$'):
        super(univa, self).__init__(prog_name, cmd_arg)

        # merge .o and .e, yes or no, default yes, on
        self.add_qoption('merge', '-j ')
        self.get_qoption('merge').usr_input = 'yes'

        # set walltime
        self.add_qoption('walltime', '-l h_cpu=')

        # set memory
        self.add_qoption('memory', '-l h_data=')

        # set hostname
        self.add_qoption('hostname', '-l hostname=')

        # set qname
        self.add_qoption('qname', '-l qname=')
        self.get_qoption('qname').usr_input = '\"physics.q\"'

        # set job priority
        self.add_qoption('priority', '-p ')

        # export environment variables
        self.add_qoption('export', '-v ')
        self.get_qoption('export').usr_input = 'PATH,LD_LIBRARY_PATH,PYTHONPATH,P0DBANFFROOT'

    def check_univa_batchq_qoptions(self):
        """runs all checks on batchq options, including inherited ones"""
        if self.show_usage:
            return
        if len(self.get_qoption('walltime').usr_input.split(':')) != 3:
            error_msg = 'ERROR: walltime must be the format HH:MM:DD'
            print error_msg
            self.set_help_opt(True)
            return
        test_opt = self.get_option('merge')
        if not (type(test_opt.usr_input) is str and
                (test_opt.usr_input is 'yes' or
                 str(test_opt) is 'no' or len(test_opt.usr_input) == 0)):
            error_msg = 'ERROR: merge .o/.e files must '
            error_msg += 'be "yes", "no", or empty string'
            print error_msg
            self.set_help_opt(True)
            return
        self.check_qsubmitter_options()


class node(p0dbanff.p0dbanff):
    """a general computer """

    def __init__(self, name=''):
        super(node, self).__init__()
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
