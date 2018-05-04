import os
import program
import batchq
import Directory
import TextFile


class qsubmitter(program.program):
    """a general program in $PATH to handle common qsub options"""

    def __init__(self):
        super(qsubmitter, self).__init__()

        self.qoptions = None
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
        self.check_program_options()
        if self.show_usage:
            return
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse before '
            error_msg += '\"check_qsubmitter_options\" first!'
            print error_msg
            self.show_usage = True

    def make_qsubDir(self):
        """make a directory to store the qsub scripts"""
        self.qsubDir.mkdir()

    def get_list_of_batchq_cmdoptions_for_qsub_script(self):
        """return a list of strings each as the batchq options"""
        out_list = list()
        for qopt in self.qoptions:
            line = str()
            if qopt.is_set():
                line = str(qopt)
            # string must have some length to write to file
            if len(line) > 0:
                out_list.append(qopt.cmd_descript)
                out_list.append(line)
        return out_list

    def set_qsubDir(self):
        """initialize the directory where jobs are submitted"""
        self.qsubDirName = self.name
        self.qsubDirName = os.path.join(self.options.output_path, self.qsubDirName)
        self.qsubDirName += '_%s%s%s_%s%s' % \
                            (str(self.call_time.year),
                             str(self.call_time.month).zfill(2),
                             str(self.call_time.day).zfill(2),
                             str(self.call_time.hour).zfill(2),
                             str(self.call_time.minute).zfill(2))
        self.qsubDir = Directory.Directory(self.qsubDirName)


class multiqsub(qsubmitter):
    """A batch queue program that submits multiple jobs"""

    def __init__(self):
        super(multiqsub, self).__init__()

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
        self.check_program_options()
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
    """Batch queue jobs involving many files listed in a file/directory"""

    def __init__(self):
        super(filelist_jobs, self).__init__()

        # a list of files
        self.parser.add_option('-L', '--list',
                               help='The input file directory list or directory',
                               default='')

        # a dictionary with key = filename and value = bool. If used in job,
        # value = true
        self.fileDict = dict()

    def check_filelist_jobs_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        self.check_program_options()
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

    def make_qsub_script(self, script_name):
        """parse the qsub options and user inputs to make a script"""
        # declare the qsub file
        qsub_script = TextFile.WriteTextFile(script_name)
        if qsub_script.open_status is not qsub_script.IS_OPEN:
            error_msg = 'Cannot write to file '
            error_msg += '%s since it is not open' % str(qsub_script)
            print error_msg
            self.show_usage = True
            return

        # write the batch options first
        batchq_cmds = self.get_list_of_batchq_cmdoptions_for_qsub_script()
        # now write the program lines
        qsub_script.write(batchq_cmds)
        qsub_script.write(self._program.command)
        qsub_script.close()

    def set_fileDict(self):
        """set the file list and sort by key"""
        listTextFile = TextFile.ReadTextFile(self.options.list)
        list_of_files = sorted(listTextFile.get_file_as_list())
        for afile in list_of_files:
            self.fileDict[afile] = False


class RunCreateFlattree_univa_jobs(batchq.univa, filelist_jobs,
                                   program.RunCreateFlattree_job):
    """A Univa Grid Engine based RunCreateFlatTree.exe batch queue submitter"""

    def __init__(self):
        super(RunCreateFlattree_univa_jobs, self).__init__()
        self.name = 'RunCreateFlatTree.exe'
        # merge std.out and std.err
        self.parser.add_option('-j', '--merge',
                               help='Combine .o and .e files (default=yes)',
                               default='yes')
        # designate a hostname
        self.parser.add_option('-N', '--hostname',
                               help='Nodes to run on')
        # designate a specific queue by name
        self.parser.add_option('-Q', '--qname',
                               help='The queue name to use (default: physics.q)',
                               default='\"physics.q\"')
        # set the flattree exe output directory path
        self.parser.add_option('-p', '--output_path',
                               help='The output directiory for each job output',
                               default='')

    def check_RunCreateFlattree_univa_jobs_options(self):
        """"""
        self.check_program_options()
        if self.show_usage:
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
        self.check_RunCreateFlattree_job_options()
