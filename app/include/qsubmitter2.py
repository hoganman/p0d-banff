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

        # set sleep time between qsub commands
        self.add_option('S', 'sleep',
                        'The sleep time in seconds between qsubs',
                        '30')

        # format PROGNAME_OUTPUTNAME_YYYYMMDD_HHMM
        self.qsubDirName = str()
        self.qsubDir = None

    def add_qoption(self, opt_search, batchq_cmdopt, ):
        """append the list of qoptions"""
        qopt = batchq.qoption(opt_search, batchq_cmdopt, self.cmd_arg)
        self.qoptions.append(qopt)

    def check_qoptions(self):
        """make sure the set qoptions are not invalid"""

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


class filelist_jobs(multiqsub):
    """jobs involving many files"""

    def __init__(self, prog_name, cmd_arg='#$'):
        super(filelist_jobs, self).__init__(prog_name, cmd_arg)

        self._program = None
        self.requires_arguments = True

        # a list of files
        self.add_option('L', 'list',
                        'The input file directory list or directory')

    def check_inputs(self):
        """makes sure no bad inputs were given, else, tell user"""
        error_msg = str()
        self.check_qoptions()
        if self.is_help_set():
            # error_msg = 'Help message set'
            # print error_msg
            return
        if len(str(self.get_option('walltime')).split(':')) != 3:
            error_msg = 'ERROR: walltime must be the format HH:MM:SS'
            print error_msg
            self.set_help_opt()
            return
        if len(str(self.get_option('num-jobs'))) <= 0:
            error_msg = 'ERROR: Must set the number of jobs'
            print error_msg
            self.set_help_opt()
            return
        if int(self.get_option('num-jobs')) <= 0:
            error_msg = 'ERROR: number of jobs must be positive'
            print error_msg
            self.set_help_opt()
            return
        if self.requires_arguments and len(str(self.arguments)) <= 0:
            error_msg = 'ERROR: There must be arguments'
            print error_msg
            self.set_help_opt()
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

        # set the flattree exe output directory path
        self.add_option('p', 'output-path',
                        'The output directiory for each job')

        # set the flattree output name after directory
        self.add_option('o', 'output-name',
                        'Example -o naME produces naME.root')
