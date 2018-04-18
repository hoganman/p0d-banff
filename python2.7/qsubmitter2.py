import os
import program
import batchq
import Directory
import TextFile


class qsubmitter(program.program):
    """a general program in $PATH to handle common qsub options"""

    def __init__(self, prog_name, cmd_arg):
        super(qsubmitter, self).__init__(prog_name,
                                         'Submit jobs to a batch queue')

        self.cmd_arg = cmd_arg
        self.qoptions = list()
        self._program = None

        # help option, no inputs taken
        # self.parser.add_option('-h', '--help', help='This help message',
        #                        default=False, action='store_true')
        self.parser.add_option('-t', '--walltime',
                               help='Job walltime in HH:MM:SS')
        self.parser.add_option('-m', '--memory',
                               help='Memory of job (4G = 4 gigabytes)')

        # format PROGNAME_OUTPUTNAME_YYYYMMDD_HHMM
        self.qsubDirName = str()
        self.qsubDir = None

    def check_qsubmitter_options(self):
        """checks the qsubmitter options from args_parse"""
        if self.show_usage:
            return
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse before '
            error_msg += '\"check_qsubmitter_options\" first!'
            print error_msg
            self.show_usage = True

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
        self.parser.add_option('-n', '--num_jobs',
                               help='The number of jobs to submit',
                               default=0)

        # set sleep time between qsub commands
        self.parser.add_option('-S', '--sleep',
                               help='The sleep time in seconds between qsubs',
                               default='30')

    def check_multiqsub_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        if self.show_usage:
            return
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse before '
            error_msg += '\"check_multiqsub_options\" first!'
            print error_msg
            self.show_usage = True
            return
        if not self.options.num_jobs:
            error_msg = 'ERROR: Please set postive integer num of jobs'
            print error_msg
            self.show_usage = True
            return
        try:
            if int(self.options.num_jobs) <= 0:
                error_msg = 'ERROR: Please set postive integer num of jobs'
                print error_msg
                self.show_usage = True
                return
        except Exception as exception:
            print str(exception)
            error_msg = 'ERROR: Please set postive integer num of jobs'
            print error_msg
            self.show_usage = True
            return
        if self.options.sleep:
            try:
                if int(self.options.sleep) < 0:
                    error_msg = 'ERROR: Please set 0+ sleep time'
                    print error_msg
                    self.show_usage = True
                    return
            except Exception as exception:
                print str(exception)
                error_msg = 'ERROR: Please set 0+ sleep time'
                print error_msg
                self.show_usage = True
                return
        self.check_qsubmitter_options()


class filelist_jobs(multiqsub):
    """jobs involving many files listed in a file/directory"""

    def __init__(self, prog_name, cmd_arg='#$'):
        super(filelist_jobs, self).__init__(prog_name, cmd_arg)

        # a list of files
        self.parser.add_option('-L', '--list',
                               help='The input file directory list or directory',
                               default='')

    def check_filelist_jobs_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        if self.show_usage:
            return
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse before '
            error_msg += '\"check_filelist_job_options\" first!'
            print error_msg
            self.show_usage = True
            return
        if not self.options.list:
            error_msg = 'ERROR: Please set input file list'
            print error_msg
            self.show_usage = True
            return
        if not os.path.isfile(self.options.list):
            error_msg = 'ERROR: input file '
            error_msg += '\"%s\" does NOT exist' % self.options.list
            print error_msg
            self.show_usage = True
            return
        self.check_multiqsub_options()

    def make_qsub_script(self, script_name, _program):
        """parse the qsub options and user inputs to make a script"""
        # declare the qsub file
        qsub_script = TextFile.WriteTextFile(script_name)
        if not (qsub_script.open_status is qsub_script.IS_OPEN):
            error_msg = 'Cannot write to file '
            error_msg += '%s since it is not open' % str(qsub_script)
            print error_msg
            self.show_usage = True
            return

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
        self.parser.add_option('-p', '--output_path',
                               help='The output directiory for each job',
                               default='')

        # set the flattree output name after directory
        self.parser.add_option('-o', '--output_name',
                               help='Example -o naME produces naME.root',
                               default='')

    def check_CreateFlattree_jobs_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        if self.show_usage:
            return
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse before '
            error_msg += '\"check_CreateFlattree_jobs_options\" first!'
            print error_msg
            self.show_usage = True
            return
        if not self.options.output_path:
            error_msg = 'ERROR: Please set output path'
            print error_msg
            self.show_usage = True
            return
        if not os.path.isdir(self.options.output_path):
            error_msg = 'ERROR: %s is NOT a valid output path' % self.options.output_path
            print error_msg
            self.show_usage = True
            return
        if not self.options.output_name:
            error_msg = 'ERROR: Please set output file name prefix'
            print error_msg
            self.show_usage = True
            return
        self.check_filelist_jobs_options()

    # def check_CreateFlatTree_jobs_qoptions(self):
    #     """check the qoptions"""
    #     self.check_filelist_jobs_qoptions()


class CreateFlattree_univa_jobs(batchq.univa, CreateFlattree_jobs):

    def __init__(self):
        super(CreateFlattree_univa_jobs, self).__init__()
        # merge std.out and std.err
        self.parser.add_option('-j', '--merge',
                               help='Combine .o and .e files (default=yes)', default='yes')
        # designate a hostname
        self.parser.add_option('-N', '--hostname',
                               help='Nodes to run on')
        # designate a specific queue by name
        self.parser.add_option('-Q', '--qname',
                               help='The queue name to use (default: physics.q)', default='\"physics.q\"')

    def check_CreateFlattree_univa_jobs_options(self):
        """"""
        if self.show_usage:
            return
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse before '
            error_msg += '\"check_CreateFlattree_univa_jobs_options\" first!'
            print error_msg
            self.show_usage = True
            return
        self.check_CreateFlattree_jobs_options()
