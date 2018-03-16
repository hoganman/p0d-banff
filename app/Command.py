""" A way to run simple commands  """

import os


class Command(object):
    """a command class"""

    def __init__(self, command):
        self.command = command

    def __str__(self):
        return self.get()

    def get(self):
        """returns command"""
        return self.command

    def add(self, addition, spacer=' '):
        """adds more to the command"""
        self.command = self.command + spacer + addition

    def run_test(self):
        """simulates running the command"""
        print self.command


class ShellCommand(Command):
    """a shell command class"""

    IN_BKG = True

    def __init__(self, command):
        super(ShellCommand, self).__init__(command)
        self.output = ''

    def run_test(self, background=(not IN_BKG)):
        """simulates running the command"""
        if self.command is None or len(self.command) == 0:
            print 'ERROR: There is no command to be run'
            return
        if not (type(background) is bool):
            print 'ERROR: Only able to accept bools to \
run in bkg or not'
            return
        cmd = Command(self.command)
        if background:
            cmd.add('&')
        print cmd.get()

    def run(self, background=(not IN_BKG)):
        """"Executes the command"""
        if self.command is None or len(self.command) == 0:
            print 'ERROR: There is no command to be run'
            return
        if not (type(background) is bool):
            print 'ERROR: Only able to accept bools to \
run in bkg or not'
            return
        cmd = Command(self.command)
        if background:
            cmd.add('&')
        print cmd.get()
        self.output = os.popen(cmd.get()).read()

    def output(self):
        """gets the output of the command"""
        return self.output
