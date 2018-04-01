import program
import batchq
import Directory
import TextFile


class qsubmitter(program.program):
    """a general program in $PATH to handle common qsub options"""

    def __init__(self, prog_name, cmd_arg='#$'):
        super(qsubmitter, self).__init__(prog_name)

        self.cmd_arg = cmd_arg
        self.qoptions = list()
        self._program = None

        # help option, no inputs taken
        self.add_option('h', 'help', 'This help message', False)

        # merge .o and .e, yes or no, default yes, on
        self.add_option('j', 'merge',
                        'Combine .o and .e files (default=yes)',
                        'yes')
        self.add_qoption('merge', '-j ',
                         'Combine .o and .e files (default=yes)',
                         'yes',
                         self.cmd_arg)

        # set walltime
        self.add_option('t', 'walltime', 'Job walltime in HH:MM:SS', '')
        self.add_qoption('walltime', '-l h_cpu=', 'Job walltime in HH:MM:SS',
                         '', self.cmd_arg)

        # set memory
        self.add_option('m', 'memory', 'Memory of job (64M = 64 megabytes)')
        self.add_qoption('memory', '-l h_data=',
                         'Memory of job (64M = 64 megabytes)',
                         '', self.cmd_arg)

        # set hostname
        self.add_option('N', 'hostname', 'Nodes to run on', '')
        self.add_qoption('hostname', '-l hostname=', 'Nodes to run on',
                         '', self.cmd_arg)

        # set qname
        self.add_option('Q', 'qname',
                        'The queue name to use (default: physics.q)',
                        '"physics.q"')
        self.add_qoption('qname', '-l qname=',
                         'The queue name to use (default: physics.q)',
                         '"physics.q"', self.cmd_arg)

        # set job priority
        self.add_option('P', 'priority', 'The job set priority')
        self.add_qoption('priority', '-p ', 'The job set priority',
                         '', self.cmd_arg)

        # format PROGNAME_OUTPUTNAME_YYYYMMDD_HHMM
        self.qsubDirName = str()
        self.qsubDir = None

    def add_qoption(self, opt_search, batchq_cmdopt, ):
        """append the list of qoptions"""
        qopt = batchq.qoption(opt_search, batchq_cmdopt, self.cmd_arg)
        self.qoptions.append(qopt)

    def get_qoption(self, search_param=str()):
        """search for all available options using char/str opt"""
        if type(search_param) is not str or len(search_param) < 1:
            print "ERROR: must search for a program qoption using a string"
        search_param = search_param.lstrip('-')
        search_param = search_param.rstrip('=')
        for qopt in self.qoptions:
            if search_param is qopt.stringOpt:
                return qopt
        return None

    def check_qsubmitter_qoptions(self):
        """make sure the set qoptions are not invalid"""
        for qopt in self.qoptions:
            if qopt.cmd_arg is not '#':
                error_msg = '\
ERROR: %s does not have a proper cmd_arg' % (qopt.search_tag)
                print error_msg
                self.set_help_opt(True)
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
            error_msg = '\
ERROR: merge .o/.e files must be "yes", "no", or empty string'
            print error_msg
            self.set_help_opt(True)
            return

    def check_qsubmitter_options(self):
        """"""
        if self.is_help_set():
            # error_msg = 'Help message set'
            # print error_msg
            return
        if len(str(self.get_option('walltime')).split(':')) != 3:
            error_msg = 'error: walltime must be the format hh:mm:ss'
            print error_msg
            self.set_help_opt(True)
            return

    def make_qsubDir(self, qsubDirName):
        """make a directory to store the qsub scripts"""
        self.qsubDir = Directory.Directory(self.qsubDirName)
        if not self.qsubDir.exists():
            self.qsubDir.mkdir()

    def get_list_of_batchq_cmdoptions_for_qsub_script(self):
        """return a list of strings each as the batchq options"""
        out_list = list()
        for qopt in self.qoptions:
            line = str()
            if qopt.is_set():
                line = str(qopt)
            # is the qopt associated with other option
            else:
                opt = self.get_option(qopt.search_tag)
                if opt is None or not opt.is_set():
                    continue
                line = str(qopt) + opt.usr_input
            # string must have some length to write to file
            if len(line) > 0:
                out_list.append(qopt.cmd_descript)
                out_list.append(line)
        return out_list

    def set_qsubDir(self):
        """initialize the directory where jobs are submitted"""
        if not self.get_option('o').is_set():
            print 'ERROR: must first set output file name before a directory'
            print '       can be  made'
        qsubDirName = self.name.strip('.exe')
        qsubDirName = '%s_%s' % (qsubDirName,
                                 str(self.get_option('o')).strip('.root'))
        qsubDirName = '%s_%s%s%s_%s%s' % \
                      (qsubDirName, str(self.call_time.year),
                       str(self.call_time.month).zfill(2),
                       str(self.call_time.day).zfill(2),
                       str(self.call_time.hour).zfill(2),
                       str(self.call_time.minute).zfill(2))
        self.make_qsubDir(qsubDirName)


class multiqsub(qsubmitter):
    """"""

    def __init__(self, prog_name, cmd_arg='#$'):
        super(multiqsub, self).__init__(prog_name, cmd_arg)

        # how many jobs to split the submissions
        self.add_option('n', 'num-jobs',
                        'The number of jobs to submit')

        # set sleep time between qsub commands
        self.add_option('S', 'sleep',
                        'The sleep time in seconds between qsubs',
                        '30')

    def check_multiqsub_qoptions(self):
        """check input qoptions"""
        self.check_qsubmitter_qoptions()

    def check_multiqsub_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        self.check_qsubmitter_options()
        if len(str(self.get_option('num-jobs'))) <= 0:
            error_msg = 'ERROR: Must set the number of jobs'
            print error_msg
            self.set_help_opt(True)
            return
        if int(str(self.get_option('num-jobs'))) <= 0:
            error_msg = 'ERROR: number of jobs must be positive'
            print error_msg
            self.set_help_opt(True)
            return
        if len(str(self.get_option('sleep'))) <= 0:
            error_msg = 'ERROR: invalid input for sleep time'
            print error_msg
            self.set_help_opt(True)
            return
        if int(str(self.get_option('sleep'))) <= 0:
            error_msg = 'ERROR: sleep time must be positive'
            print error_msg
            self.set_help_opt(True)
            return


class filelist_jobs(multiqsub):
    """jobs involving many files listed in a file/directory"""

    def __init__(self, prog_name, cmd_arg='#$'):
        super(filelist_jobs, self).__init__(prog_name, cmd_arg)

        # a list of files
        self.add_option('L', 'list',
                        'The input file directory list or directory')

    def check_filelist_jobs_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        self.check_multiqsub_options()
        if self.is_help_set():
            # error_msg = 'Help message set'
            # print error_msg
            return
        if len(str(self.get_option('list'))) <= 0:
            error_msg = 'error: missing input list!'
            print error_msg
            self.set_help_opt(True)
            return
        if self.requires_arguments and len(str(self.arguments)) <= 0:
            error_msg = 'ERROR: There must be arguments'
            print error_msg
            self.set_help_opt(True)
            return

    def make_qsub_script(self, script_name, _program):
        """parse the qsub options and user inputs to make a script"""
        # declare the qsub file
        qsub_script = TextFile.WriteTextFile(script_name)
        if not (qsub_script.open_status is qsub_script.IS_OPEN):
            print 'Cannot write to file %s\
since it is not open' % str(qsub_script)
        # write the batch options first
        batchq_cmds = self.get_list_of_batchq_cmdoptions_for_qsub_script()
        _program_cmds = _program.get_list_commands_for_qsub_script()
        # now write the program lines
        qsub_script(batchq_cmds)
        qsub_script(_program_cmds)
        qsub_script.close()


class CreateFlattree_jobs(filelist_jobs):
    """jobs involving a file of oaAnalysis files"""

    def __init__(self, prog_name='RunCreateFlatTree.exe', cmd_arg='#$'):
        super(CreateFlattree_jobs, self).__init__(prog_name, cmd_arg)
        self._program = program.RunCreateFlatTree()
        self.requires_arguments = True

        # set the flattree exe output directory path
        self.add_option('p', 'output-path',
                        'The output directiory for each job')

        # set the flattree output name after directory
        self.add_option('o', 'output-name',
                        'Example -o naME produces naME.root')
