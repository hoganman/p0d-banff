from program import program
import batchq


class qsubmitter(program):
    """a general program to handle common qsub options"""

    def __init__(self, prog_name='qsubmitter', cmd_arg='#$'):
        super(qsubmitter, self).__init__(prog_name)
        self.qoptions = list()

        self.cmd_arg = cmd_arg

        # help option, no inputs taken
        self.add_option('h', 'help', 'This help message')

        # merge .o and .e, yes or no, default yes
        self.add_option('j', 'merge',
                        'Combine .o and .e files (default=yes)', 'yes')
        self.add_qoption('merge', '-j ')

        # set walltime
        self.add_option('t', 'walltime', 'time in HH:MM:SS')
        self.add_qoption('walltime', '-l h_cpu=')

        # set memory
        self.add_option('m', 'memory', 'Memory of job (100M = 100mb)')
        self.add_qoption('memory', '-l h_data=')

        # set hostname
        self.add_option('N', 'hostname', 'Nodes to run on')
        self.add_qoption('hostname', '-l hostname=')

        # set qname
        self.add_option('Q', 'qname',
                        'The queue name to use (default: physics.q)',
                        '"physics.q"')
        self.add_qoption('qname', '-l qname=')

        # set job priority
        self.add_option('P', 'priority', 'The job set priority')
        self.add_qoption('priority', '-p ')

    def add_qoption(self, opt_search, batch_opt):
        """append the list of qoptions"""
        qopt = batchq.qoption(opt_search, batch_opt, self.cmd_arg)
        self.qoptions.append(qopt)

    def make_qsub_script(self):
        """"""
        out = ''
        for qopt in self.qoptions:
            opt = self.get_option(qopt.search_tag)
            if opt is None:
                continue
            if not opt.is_set():
                continue
            out = out + str(qopt) + opt.usr_input + '\n'
        return out


class flattree_jobs(qsubmitter):
    """"""

    def __init__(self, prog_name='flattree_jobs'):
        super(flattree_jobs, self).__init__(prog_name)

        # a list of files
        self.add_option('L', 'list', 'The input file list')

        # how many jobs to split the submissions
        self.add_option('n', 'num-job', 'The number of jobs to submit')

        # set the flattree output name after directory
        self.add_option('o', 'output-name-prefix',
                        'Example -o naME produces naME.root')

        # set the flattree exe output directory path
        self.add_option('p', 'output-path', 'The directiory of output')

        # set t2k run number
        self.add_option('r', 'run', 'The T2K run number')

        # set sleep time between qsub commands
        self.add_option('S', 'sleep', 'The sleep time between qsubs')
