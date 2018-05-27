"""
A set of classes to use the ROOT macros defined in $P0DBANFFROOT/macros
"""
import p0dbanff
import program


class macro(p0dbanff.p0dbanff):
    """An interface for ROOT and python macros"""

    def __init__(self, macro_path, _program):
        super(macro, self).__init__()
        self.macro_path = macro_path
        self._program = _program

    def get(self, input):
        """for a single/set of input, get the macro string"""
        command = str()
        if type(input) is str or type(input) is int or type(input) is float:
            command = '{0} {1}\"({2})\"'.format(self._program.command,
                                                self.macro_path,
                                                input)
        elif type(input) is list:
            command = '{0} {1}\"('.format(self._program.command, self.macro_path)
            for eachInput in input:
                command += '{0}, '.format(eachInput)
            command = command.rstrip()
            command = command.rstrip(',')
            command += ')\"'
        else:
            print 'ERROR: macro.get only accepts str and list'
        return command


class CheckFileROOT(macro):
    """CheckFileROOT.C interface"""

    def __init__(self):
        super(CheckFileROOT,
              self).__init__('$P0DBANFFROOT/macros/CheckFileROOT.C',
                             program.ROOTprog())


class CheckFileShell(macro):
    """CheckFileShell.C interface"""
    def __init__(self):
        super(CheckFileShell,
              self).__init__('$P0DBANFFROOT/macros/CheckFileShell.C',
                             program.ROOTprog())


class ROOTRandomSleep(macro):
    """ROOTRandomSleep.C interface"""
    def __init__(self):
        super(ROOTRandomSleep,
              self).__init__('$P0DBANFFROOT/macros/ROOTRandomSleep.C',
                             program.ROOTprog())
