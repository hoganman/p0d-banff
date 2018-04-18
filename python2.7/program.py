import datetime
import p0dbanff
import optparse
# import batchq


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


class program(p0dbanff.p0dbanff):
    """A generic program class from terminal"""

    def __init__(self, prog_name, descpt='This is the description string'):
        super(program, self).__init__()
        self.name = prog_name
        self.execut = which(prog_name)
        self.args = None
        self.options = None
        self.parser = optparse.OptionParser(descpt)
        self.requires_arguments = False
        self.descpt = descpt
        self.call_time = datetime.datetime.now()
        self.show_usage = False

    def __str__(self):
        return self.name

    def set_descpt(self, descpt):
        """set program description"""
        self.descpt = descpt

    # def check_arguments(self):
    #     """if arguments are required, make sure they are set"""
    #     if self.requires_arguments:
    #         if len(str(self.arguments)) > 0:
    #             return True
    #         else:
    #             return False
    #     return False

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

    # def add_option(self, charOpt, strOpt, descript, usr_input=''):
    #     """add an input option for the program"""
    #     if not (type(charOpt) is str
    #             and type(strOpt) is str
    #             and type(descript) is str
    #             and (type(usr_input) is str or type(usr_input) is bool)):
    #         print 'ERROR: inputs must be of 4 strings'
    #         return
    #     opt = option(charOpt, strOpt, descript, usr_input)
    #     self.options.append(opt)

    # def get_option(self, search_param=str()):
    #     """search for all available options using char/str opt"""
    #     if type(search_param) is not str:
    #         print "ERROR: must search for a program option using a string"
    #     search_param = search_param.lstrip('-')
    #     search_param = search_param.rstrip('=')
    #     for opt in self.options:
    #         if len(search_param) == 0:
    #             if len(opt.charOpt) == 0 and len(opt.stringOpt) == 0:
    #                 return opt
    #         elif len(search_param) == 1:
    #             if search_param == opt.charOpt:
    #                 return opt
    #         elif len(search_param) > 1:
    #             if search_param == opt.stringOpt:
    #                 return opt
    #     return None

    # def set_help_opt(self, help_opt=True):
    #     """set the help option as enabled or not"""
    #     self.get_help_opt().usr_input = help_opt

    # def is_help_set(self):
    #     """check if the help option is set"""
    #     if not (type(self.get_help_opt()) is option):
    #         return False
    #     return self.get_help_opt().usr_input

    # def get_set_options(self):
    #     """get the string of all options that are set
    #     (usr_input NOT False or '') in the format
    #     parsable by the actual program"""
    #     opt_str = str()
    #     # in case the opt flags are length=0, then put this at the end
    #     for opt in self.options:
    #         if not (type(opt.usr_input) is bool or type(opt.usr_input) is str):
    #             continue
    #         if type(opt.usr_input) is str and len(opt.usr_input) == 0:
    #             continue
    #         if type(opt.usr_input) is bool and not opt.usr_input:
    #             continue
    #         append = str()
    #         if len(opt.charOpt) == 1:
    #             append = '-%s ' % (opt.charOpt)
    #         elif len(opt.stringOpt) > 1:
    #             append = '--%s=' % (opt.stringOpt)
    #         else:
    #             continue
    #         if type(opt.usr_input) is str:
    #             append += opt.usr_input
    #         opt_str += ' %s' % (append)
    #     return opt_str

    # def help_msg(self):
    #     """the result of asking for the help msg"""
    #     out = str()
    #     synopsis = '\t%s' % (self.name)
    #     if len(self.options) > 0:
    #         synopsis = synopsis + ' [OPTIONS]'
    #     synopsis = synopsis + ' [ARGUMENTS]'
    #     out = out + 'NAME\n'
    #     out = out + synopsis + '\n'
    #     out = out + '\n'
    #     out = out + 'DESCRIPTION' + '\n'
    #     out = out + '\t %s\n' % (self.descpt)
    #     out = out + 'OPTIONS\n'
    #     for opt in self.options:
    #         out = out + '\t%s\n' % (opt.help_msg())
    #         out = out + '\n'
    #     return out

    # def make_getopt_options(self):
    #     """make the string used by getopt.getopt(argv, options)"""
    #     out = str()
    #     for opt in self.options:
    #         out = out + opt.charOpt
    #         if type(opt.usr_input) is str:
    #             out = out + ':'
    #     return out

    # def make_getopt_long_options(self):
    #     """ make the list used by getopt.getopt(argv, options, long_options)"""
    #     out = list()
    #     for opt in self.options:
    #         new_entry = opt.stringOpt
    #         if type(opt.usr_input) is str:
    #             new_entry = new_entry + '='
    #         out.append(new_entry)
    #     return out

    # def set_usr_inputs_from_getopt(self, opts, args):
    #     """from the getopt.getopt method, parse the list of tuples"""
    #     error_msg = 'ERROR: inputs must be lists getopt.getopt'
    #     if not (type(args) is list and type(opts) is list):
    #         print error_msg
    #         return
    #     # print str(args)
    #     # print str(opts)
    #     for each_tup in opts:
    #         if not (type(each_tup) is tuple):
    #             print error_msg
    #             return
    #         tup_opt, tup_arg = each_tup
    #         # print str(tup_opt)
    #         # print str(tup_arg)
    #         prog_opt = self.get_option(tup_opt)
    #         # print type(prog_opt)
    #         # print type(tup_arg)
    #         input_successfully_assigned = False
    #         if type(prog_opt) is option:
    #             if len(tup_arg) == 0:
    #                 assignment = True
    #             else:
    #                 assignment = tup_arg.strip()
    #             # print assignment
    #             self.get_option(tup_opt).usr_input = assignment
    #             input_successfully_assigned = True
    #         if input_successfully_assigned:
    #             continue
    #         prog_qopt = self.get_qoption(tup_opt)
    #         if type(prog_qopt) is batchq.qoption:
    #             if len(tup_arg) == 0:
    #                 assignment = True
    #             else:
    #                 assignment = tup_arg.strip()
    #             # print assignment
    #             self.get_qoption(tup_opt).usr_input = assignment
    #             input_successfully_assigned = True
    #         if input_successfully_assigned:
    #             continue
    #         else:
    #             print 'ERROR: input ' + tup_opt + ' is NOT found'
    #             self.get_help_opt().usr_input = True
    #     if len(args) > 0 and self.get_option():
    #         arg_string = ' '.join(args).strip()
    #         self.get_option().usr_input = arg_string


# class option(p0dbanff.p0dbanff):
#     """a generic option to a program using getopt format"""
#
#     def __init__(self, charOpt, stringOpt='', descpt='', usr_input=''):
#         super(option, self).__init__()
#         self.charOpt = charOpt
#         self.stringOpt = stringOpt
#         self.descpt = descpt
#         # can also be a bool if initially set false
#         self.usr_input = usr_input
#
#     def __str__(self):
#         return str(self.usr_input)
#
#     def __int__(self):
#         if type(self.usr_input) is str:
#             try:
#                 return int(self.usr_input)
#             except ValueError:
#                 return -1
#         elif type(self.usr_input) is bool:
#             return int(self.usr_input)
#         else:
#             return -1
#
#     def __bool__(self):
#         if type(self.usr_input) is bool:
#             return self.usr_input
#         elif type(self.usr_input) is str:
#             return len(self.usr_input) > 0
#         else:
#             return False
#
#     def get_char_opt(self):
#         """get'er for character option"""
#         return self.charOpt
#
#     def get_string_opt(self):
#         """get'er for string option"""
#         return self.stringOpt
#
#     def get_descpt(self):
#         """get'er for option description"""
#         return self.descpt
#
#     def get_input(self, usr_input):
#         """get'er for user input"""
#         return self.usr_input
#
#     def help_msg(self):
#         """get'er for how option works"""
#         msg = str()
#         if len(self.charOpt) == 1:
#             msg = msg + '-%s' % (self.charOpt)
#         if len(self.stringOpt) > 1:
#             if len(self.charOpt) == 1:
#                 if len(msg) > 0:
#                     msg = msg + ', --%s' % (self.stringOpt)
#                 else:
#                     msg = msg + '--%s' % (self.stringOpt)
#                 if type(self.usr_input) is str:
#                     msg = msg + '='
#             else:
#                 if len(msg) > 0:
#                     msg = msg + '--%s' % (self.stringOpt)
#                 else:
#                     msg = msg + '--%s' % (self.stringOpt)
#                 if type(self.usr_input) is str:
#                     msg = msg + '='
#         msg = msg + '\t\t%s' % (self.descpt)
#         return msg
#
#     def set_descpt(self, descpt):
#         """set'er for option description"""
#         self.descpt = descpt
#
#     def set_char_opt(self, charOpt):
#         """set'er for character option"""
#         self.charOpt = charOpt
#
#     def set_strong_opt(self, strongOp):
#         """set'er for string option"""
#         self.strongOp = strongOp
#
#     def set_input(self, usr_input):
#         """set'er for user input"""
#         self.usr_input = usr_input
#
#     def is_set(self):
#         """checks if the user has set input to this option"""
#         if type(self.usr_input) is bool:
#             return self.usr_input
#         if type(self.usr_input) is str:
#             return len(self.usr_input) > 0
#         return False
#
#
# class argument(option):
#     """the arugments to a program"""
#
#     def __init__(self):
#         super(argument, self).__init__('', '', 'The argument string')


class RunCreateFlatTree(program):
    """Create the flattree program"""

    def __init__(self):
        super(RunCreateFlatTree, self).__init__('RunCreateFlatTree.exe')

        self.parser.add_option('-v', '', default=False,
                               help='Do not check oaAnalysis version',
                               action='store_true')

        self.parser.add_option('-o', '', default='', help='Output file')
        self.requires_arguments = True


class ROOTprog(program):
    """The 'root' executibble"""

    def __init__(self):
        super(ROOTprog, self).__init__('root', 'Run ROOT')


class PYTHONprog(program):
    """The 'python' executibble"""

    def __init__(self):
        super(PYTHONprog, self).__init__('python', 'Run python')
