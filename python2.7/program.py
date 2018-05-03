import datetime
import p0dbanff
import optparse
# import batchq


class program(p0dbanff.p0dbanff):
    """A generic program class from terminal"""

    def __init__(self, descpt='This is the description string'):
        super(program, self).__init__()
        self.descpt = descpt
        self.args = None
        self.options = None
        self.parser = optparse.OptionParser(descpt)
        self.requires_arguments = False
        self.call_time = datetime.datetime.now()
        self.show_usage = False

    def __str__(self):
        return self.name

    def set_descpt(self, descpt):
        """set program description"""
        self.descpt = descpt

    def check_program_options(self):
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse first'
            print error_msg
            self.show_usage = True
            return
        return self.show_usage

    def parse_args(self):
        """run the parser from sys.argv"""
        try:
            (self.options, self.args) = self.parser.parse_args()
        except Exception as exception:
            print str(exception)
            print 'ERROR: unable to parse arguments'

    def print_help(self):
        """the help option"""
        self.parser.print_help()

    def get_options(self):
        """a get'er for the options"""
        return self.options

    def get_args(self):
        """a simple get'er for the arguments member"""
        return self.args


class RunCreateFlatTree(program):
    """Create the flattree program"""

    def __init__(self):
        super(RunCreateFlatTree, self).__init__()

        self.parser.add_option('-v', '', default=False,
                               help='Do not check oaAnalysis version',
                               action='store_true')

        self.parser.add_option('-o', '', default='', help='Output file')
        self.requires_arguments = True

    def check_RunCreateFlatTree_options(self):
        """"""
        self.check_program_options()
        if self.show_usage:
            return
        if not self.options.o:
            error_msg = 'ERROR: Please set output file name prefix'
            print error_msg
            self.show_usage = True
            return


class ROOTprog(program):
    """The 'root' executibble"""

    def __init__(self):
        super(ROOTprog, self).__init__()


class PYTHONprog(program):
    """The 'python' executibble"""

    def __init__(self):
        super(PYTHONprog, self).__init__()


class RunCreateFlattree_job(RunCreateFlatTree):
    """jobs involving a file of oaAnalysis files"""

    def __init__(self):
        super(RunCreateFlattree_job, self).__init__()
        self._program = RunCreateFlatTree()
        self.requires_arguments = True

    def check_CreateFlattree_jobs_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        self.check_program_options()
        if self.show_usage:
            return
        self.check_RunCreateFlatTree_options()


def which(program_name):
    """uses which to get full exec path"""
    if type(program_name) is not str or len(program_name) == 0:
        # print 'WARNING: No program name given'
        return None
    from os import popen
    out = popen('which '+program_name).read().split('\n')
    if not (type(out) is list):
        print 'ERROR unable to get program of name "%s"' % (program_name)
    elif len(out) == 2:
        return out[0]
    else:
        print 'ERROR unable to get program of name "%s"' % (program_name)
    return None
