import batchq
import Directory
import macro
import os
import program
import TextFile
import math


class qsubmitter(program.program):
    """a general program in $PATH to handle common qsub options"""

    def __init__(self):
        super(qsubmitter, self).__init__()

        self.qoptions = None
        self._program = None

        # help option, no inputs taken
        self.parser.add_option('-t', '--walltime', dest='walltime',
                               help='Job walltime in HH:MM:SS')
        self.parser.add_option('-m', '--memory', dest='memory',
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
        """
        Return a list of strings each as the batchq options
        Example
        [ '#cmd_arg OPTION1', '#cmd_arg OPTION2']
        """
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
        self.qsubDirName = os.path.join(os.getcwd(), self.qsubDirName)
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
        self.parser.add_option('-n', '--num_jobs', dest="num_jobs",
                               help='The number of jobs to submit',
                               default=0)

        # set sleep time between job running in sequence
        self.parser.add_option('-S', '--sleep', dest='sleep',
                               help='The sleep time between jobs running in\
sequence as not to bog down a filesystem',
                               default='1')

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
        except ValueError as valErr:
            print str(valErr)
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
            except ValueError as valErr:
                print str(valErr)
                error_msg = 'ERROR: Please set 0+ sleep time'
                print error_msg
                self.show_usage = True
                return
        self.check_qsubmitter_options()

    def submit_jobs(self):
        """qsub the scripts to submit jobs"""
        pass


class filelist_jobs(multiqsub):
    """Batch queue jobs involving many files listed in a file/directory"""

    def __init__(self):
        super(filelist_jobs, self).__init__()

        # a list of files
        self.parser.add_option('-L', '--list', dest='list',
                               help='The input file directory list or directory',
                               default='')

        """
        A dictionary with key = filename and value = bool. If used in job,
        value = true. Used to loop over files and prevent the same file
        reappearing in another job
        """
        self.fileDict = dict()
        self.n_files = 0
        """
        A dictionary with key = job_num and value = files_per_job
        Has only two keys, floor(n_files/num_jobs) and ceil()
        and value = number of jobs for that file list size
        ie num jobs = 100, num files = 201
        99  jobs will have 2 files
         1  job  will have 3 file
        --------------------------
        100 jobs -> 99*2+3*1=198+3=201 files
        """
        self.filesPerJobDict = dict()

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
        if self.show_usage:
            return
        self.set_fileDict()
        if self.n_files < self.options.num_jobs:
            error_msg = 'ERROR: The number of jobs is GREATER than the number'
            error_msg += ' of files, please change the number of jobs'
            print error_msg
            self.show_usage = True
            return
        self.set_filesPerJobDict()

    def set_fileDict(self):
        """set the file list dict key=name, value=useage (F/T)"""
        listTextFile = TextFile.ReadTextFile(self.options.list)
        list_of_files_raw = listTextFile.get_file_as_list()
        list_of_files_stripped = list()
        for entry in list_of_files_raw:
            clean_entry = entry.strip()
            if len(clean_entry) > 0:
                list_of_files_stripped.append(clean_entry)
        # list_of_files_stripped = sorted(list_of_files_stripped)
        for afile in list_of_files_stripped:
            self.fileDict[afile] = False
        self.n_files = len(self.fileDict)

    def set_filesPerJobDict(self):
        """
        Determines how to distribute the files to match user
        input for number of jobs specified.
        When float(n_files) / num_jobs != integer, special care
        must be taken to split files among jobs
        """
        result = float(self.n_files) / self.options.num_jobs
        ceil_result = int(math.ceil(result))
        floor_result = ceil_result - 1
        # this is zero if result is an integer
        num_jobs_file_list_size_larger = (self.n_files - floor_result
                                          * self.options.num_jobs)
        # make the first set of jobs have more files
        for iter_range in range(num_jobs_file_list_size_larger):
            job_num = iter_range + 1
            self.filesPerJobDict[job_num] = ceil_result
        for iter_range in range(num_jobs_file_list_size_larger, self.options.num_jobs):
            job_num = iter_range + 1
            self.filesPerJobDict[job_num] = floor_result


class RunCreateFlatTree_univa_jobs(batchq.univa, filelist_jobs,
                                   program.RunCreateFlatTree_job):
    """A Univa Grid Engine based RunCreateFlatTree.exe batch queue submitter"""

    def __init__(self):
        super(RunCreateFlatTree_univa_jobs, self).__init__()
        self.name = 'RunCreateFlatTree.exe'
        self._program = program.RunCreateFlatTree_job
        # merge std.out and std.err
        self.parser.add_option('-j', '--merge', dest='merge',
                               help='Combine .o and .e files (default=yes)',
                               default='yes')
        # designate a hostname
        self.parser.add_option('-N', '--hostname', dest='hostname',
                               help='Nodes to run on')
        # designate a specific queue by name
        self.parser.add_option('-Q', '--qname', dest='qname',
                               help='The queue name to use (default: physics.q)',
                               default='\"physics.q\"')
        # set the flattree exe output directory path
        self.parser.add_option('-p', '--output_path', dest='output_path',
                               help='The output directiory for each job output',
                               default='')

    def check_RunCreateFlatTree_univa_jobs_options(self):
        """Check the options for all inherited classes"""
        self.check_program_options()
        if self.show_usage:
            return
        self.name += '_{}'.format(self.options.output_name)
        if self.options.merge != 'yes' and self.options.merge != 'no':
            error_msg = 'ERROR: Invalid merge option: use \"yes\" or \"no\"'
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
        self.check_RunCreateFlatTree_job_options()

    def make_job_scripts(self):
        """creates the scripts to submit jobs"""
        for job_num in sorted(self.filesPerJobDict.keys()):
            job_script_name = '{0}/RunCreateFlatTree_job{}.sh'.format(os.getcwd(),
                                                                      job_num)
            self.make_qsub_script(job_num, job_script_name)

    def make_qsub_script(self, job_num, script_name):
        """
        Parse the qsub options and user inputs to make a script
        It produces a script file called script_name
        The contents are the batchq qoptions first then in the program
        lists in order as they appear
        """

        # declare the qsub file
        qsub_script = TextFile.WriteTextFile(script_name)
        if qsub_script.open_status is not qsub_script.IS_OPEN:
            error_msg = 'Cannot write to file '
            error_msg += '%s since it is not open' % str(qsub_script)
            print error_msg
            self.show_usage = True
            return

        outputFile = os.path.join(self.options.output_path,
                                  '{}_{}.root'.format(self.options.output_name,
                                                      job_num))
        # a string of the CheckFileROOT.C macro
        checkfile_macro = macro.CheckFileShell().get(outputFile)

        sleep_macro = macro.ROOTRandomSleep().get(self.options.num_jobs*self.options.sleep)
        programsList = list()
        programsList.append(sleep_macro)
        # write the batch options first
        batchq_cmds = self.get_list_of_batchq_cmdoptions_for_qsub_script()
        # now write the program lines
        qsub_script.write(batchq_cmds)
        for prog in programsList:
            qsub_script.write('%s %s' % (prog.command, prog.args))
        qsub_script.close()
