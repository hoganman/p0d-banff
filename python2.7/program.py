import datetime
import p0dbanff
import optparse
import os
# import batchq


class program(p0dbanff.p0dbanff):
    """
    A generic program class from terminal.
    Not intended to be used directly.
    Instead, define class that inherits from it
    """

    def __init__(self, descpt='This is the description string'):
        super(program, self).__init__()
        self.descpt = descpt
        self.args = None
        self.options = None
        self.parser = optparse.OptionParser(descpt)
        self.requires_arguments = False
        self.call_time = datetime.datetime.now()
        self.show_usage = False
        self.command = None

        self.parser.add_option('-d', '--DEBUG', default=False,
                               dest='DEBUG', help='enable debugging',
                               action='store_true')

    def __str__(self):
        return self.name

    def set_descpt(self, descpt):
        """set program description"""
        self.descpt = descpt

    def DEBUG(self):
        """checks the debug status"""
        if self.options:
            if self.options.DEBUG:
                return True
        return False

    def check_program_options(self):
        """runs the rudamentary checks"""
        if self.DEBUG():
            print 'class program fcn: check_program_options'
        if not self.options:
            error_msg = 'ERROR: You must run arg_parse first'
            print error_msg
            self.show_usage = True
            return
        if self.requires_arguments:
            if not (type(self.args) is str and len(self.args) > 0):
                error_msg = 'ERROR: This program requires arguments!'
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

    def run(self):
        """a virtual function"""
        return


class RunCreateFlatTree(program):
    """Create the flattree program"""

    def __init__(self):
        super(RunCreateFlatTree, self).__init__()

        self.parser.add_option('-v', '--skip_version_check', default=False,
                               dest='v', help='Do not check oaAnalysis version',
                               action='store_true')

        self.parser.add_option('-o', '--output_name', dest='output_name',
                               help='Output file name')
        self.requires_arguments = True

    def check_RunCreateFlatTree_options(self):
        """checks if the options are correct"""
        if self.DEBUG():
            print 'class RunCreateFlatTree fcn: check_RunCreateFlatTree_options'
        self.check_program_options()
        if self.show_usage:
            return
        if not self.options.output_path:
            error_msg = 'ERROR: Please set output file name prefix'
            print error_msg
            self.show_usage = True
            return
        # self.command = 'RunCreateFlatTree.exe'
        # if self.options.v:
        #     self.command += ' -v'
        # full_output = os.path.join(self.options.output_path, self.options.output_name)
        # self.command += ' -o %s' % (full_output)
        # self.command += ' %s' % (self.arguments)

    def run(self):
        """runs the program"""
        os.system(self.command)


class ROOTprog(program):
    """The 'root' executibble"""

    def __init__(self):
        super(ROOTprog, self).__init__()
        self.command = '{}/bin/root -l'.format(os.getenv('ROOTSYS'))


class PYTHONprog(program):
    """The 'python' executibble"""

    def __init__(self):
        super(PYTHONprog, self).__init__()
        self.command = 'python'


class RunCreateFlatTree_job(RunCreateFlatTree):
    """jobs involving a file of oaAnalysis files"""

    def __init__(self):
        super(RunCreateFlatTree_job, self).__init__()
        # let another job manager handle arguments
        self.requires_arguments = False

    def check_RunCreateFlatTree_job_options(self):
        """makes sure no bad inputs were given, else, tell user"""
        if self.DEBUG():
            print 'class RunCreateFlatTree_job fcn: check_RunCreateFlatTree_job_options'
        self.check_program_options()
        if self.show_usage:
            return
        # self.check_RunCreateFlatTree_options()


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
